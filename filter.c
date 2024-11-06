#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"

int main(int argc, char *argv[])
{
    // Define allowable filters
    char *filters = "abegpr";

    // Get filter flag and check validity
    char filter = getopt(argc, argv, filters);
    if (filter == '?')
    {
        printf("Invalid filter.\n");
        return 1;
    }

    // Ensure only one filter
    if (getopt(argc, argv, filters) != -1)
    {
        printf("Only one filter allowed.\n");
        return 2;
    }

    // Ensure proper usage
    if (argc != optind + 2)
    {
        printf("Usage: ./filter [flag] infile outfile\n");
        return 3;
    }

    // Remember filenames
    char *infile = argv[optind];
    char *outfile = argv[optind + 1];

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 4;
    }

    // Open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 5;
    }

    if ((filter != 'a' && strcmp(getFileExtension(outfile), "txt") == 0) || (filter == 'a' && strcmp(getFileExtension(outfile), "txt") != 0))
    {
        fclose(inptr);
        fclose(outptr);
        printf("%s\n", "Invalid output file extension for requested operation.\nUse .bmp extension for image filters or .txt extension\nfor ASCII text filter.");
    }
    else
    {
        // Read infile's BITMAPFILEHEADER
        BITMAPFILEHEADER bf;
        fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

        // Read infile's BITMAPINFOHEADER
        BITMAPINFOHEADER bi;
        fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

        // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
        if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
            bi.biBitCount != 24 || bi.biCompression != 0)
        {
            fclose(outptr);
            fclose(inptr);
            printf("Unsupported file format.\n");
            return 6;
        }

        // Get image and char array dimensions
        int height = abs(bi.biHeight);
        int width = bi.biWidth;
        float aRatio = (float)height / (float)width;

        float arDenominoator = aRatio * 10000;
        float arNumerator = aRatio * arDenominoator;

        printf("ASPECT RATIO: %d / %d", (int)arNumerator, (int)arDenominoator);

        // Allocate memory for image and char array.
        RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
        if (image == NULL)
        {
            printf("Not enough memory to store image.\n");
            fclose(outptr);
            fclose(inptr);
            return 7;
        }

        // Determine padding for scanlines
        int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

        // Iterate over infile's scanlines
        for (int i = 0; i < height; i++)
        {
            // Read row into pixel array
            fread(image[i], sizeof(RGBTRIPLE), width, inptr);

            // Skip over padding
            fseek(inptr, padding, SEEK_CUR);
        }

        switch (filter)
        {
            // ASCII
            case 'a':
                {
                    int charWidth = 2;
                    int charHeight = 5;
                    int cHeight = (height + charHeight - 1) / charHeight;
                    int cWidth = (width + charWidth - 1) / charWidth;

                    char (*charArr)[cWidth] = calloc(cHeight, sizeof(*charArr));
                    if (charArr == NULL)
                    {
                        printf("Not enough memory to store char array.\n");
                        fclose(outptr);
                        fclose(inptr);
                        return 7;
                    }

                    ascii(height, width, cHeight, cWidth, charHeight, charWidth, image, charArr);

                    for (int i = 0; i < cHeight; i++)
                    {
                        for (int j = 0; j < cWidth; j++)
                            fputc(charArr[i][j], outptr);
                        fprintf(outptr, "\n");
                    }

                    free(charArr);
                    break;
                }

            // Box Blur and Pixelize
            case 'b':
            case 'p':
                {
                    int bSize = 10;

                    if (filter == 'b')
                        blur(height, width, image, bSize);
                    else
                        pixelize(height, width, image, bSize);
                    break;
                }

            // Edges
            case 'e':
                edges(height, width, image);
                break;

            // Grayscale
            case 'g':
                grayscale(height, width, image);
                break;

            // Reflect
            case 'r':
                reflect(height, width, image);
                break;
        }

        if (filter != 'a')
        {
            // Write outfile's BITMAPFILEHEADER
            fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

            // Write outfile's BITMAPINFOHEADER
            fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

            for (int i = 0; i < height; i++)
            {
                // Write row to outfile
                fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);

                // Write padding at end of row
                for (int k = 0; k < padding; k++)
                    fputc(0x00, outptr);
            }
        }


        // Free memory for image and char array.
        free(image);

        // Close files
        fclose(inptr);
        fclose(outptr);
        return 0;
    }
}
