#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

// Divide image pixels up into 10*10 blocks
void ascii(int iHeight, int iWidth, int oHeight, int oWidth, int charHeight, int charWidth, RGBTRIPLE image[iHeight][iWidth], char chars[oHeight][oWidth])
{
    int charRow = 0, charCol = 0;
    int c;
    long avg, rAvg, gAvg, bAvg;

    printf("Image width: %dpx, Image height: %dpx\n", iWidth, iHeight);
    printf("CharArr width: %dpx, CharArr height: %dpx\n", oWidth, oHeight);

    // Iterate image pixels in increments of 10*10.
    for (int row = 0; row < iHeight; row += charHeight)
    {
        for (int col = 0; col < iWidth; col+= charWidth)
        {
            avg = 0, rAvg = 0, gAvg = 0, bAvg = 0, c = 0;

            // Iterate each charWidth * charHeight block 1 at a time.
            // Iterate entire block, but only count pixels within image bounds towards average calculation.
            for (int i = row; i < row + charHeight; i++)
            {
                for (int j = col; j < col + charWidth; j++)
                {
                    if (i < iHeight && j < iWidth)
                    {
                        rAvg += image[i][j].rgbtRed;
                        gAvg += image[i][j].rgbtGreen;
                        bAvg += image[i][j].rgbtBlue;
                        c++;
                    }
                }
            }

            rAvg /= c;
            gAvg /= c;
            bAvg /= c;

            // Calculate avg value for entire block and add corresponding ASCII char to char array..
            avg = (rAvg + gAvg + bAvg) / 3;

            charRow = row / charHeight;
            charCol = col / charWidth;

            if (avg >= 0 && avg < 17)
                chars[charRow][charCol] = ' ';
            else if (avg >= 17 && avg < 34)
                chars[charRow][charCol] = '.';
            else if (avg >= 34 && avg < 51)
                chars[charRow][charCol] = ',';
            else if (avg >= 51 && avg < 68)
                chars[charRow][charCol] = '-';
            else if (avg >= 68 && avg < 85)
                chars[charRow][charCol] = '^';
            else if (avg >= 85 && avg < 102)
                chars[charRow][charCol] = '*';
            else if (avg >= 102 && avg < 119)
                chars[charRow][charCol] = '+';
            else if (avg >= 119 && avg < 136)
                chars[charRow][charCol] = ':';
            else if (avg >= 136 && avg < 153)
                chars[charRow][charCol] = ';';
            else if (avg >= 153 && avg < 170)
                chars[charRow][charCol] = '=';
            else if (avg >= 170 && avg < 187)
                chars[charRow][charCol] = '%';
            else if (avg >= 187 && avg < 204)
                chars[charRow][charCol] = '$';
            else if (avg >= 204 && avg < 221)
                chars[charRow][charCol] = '#';
            else if (avg >= 221 && avg < 238)
                chars[charRow][charCol] = '&';
            else
                chars[charRow][charCol] = '@';

            printf("%c", chars[charRow][charCol]);
        }
        printf("\n");
    }
}

// Convert image to grayscale by taking the average of each pixel's R, G, and B bytes and reassigning each to the average.
// This works because if all values of R, G, and B are equal, then the result is that the pixel in question is a shade of
// grey between white (0xFF, 0xFF, 0xFF) and black (0x00, 0x00, 0x00)
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE *a, *b;
    int8_t avg = 0;

    for (int i = 0; i < height; i++)
    {
        for (a = image[i], b = image[i] + width; a < b; a++)
        {
            avg = round((a -> rgbtRed + a -> rgbtGreen + a -> rgbtBlue) / 3);
            a -> rgbtRed = avg;
            a -> rgbtGreen = avg;
            a -> rgbtBlue = avg;
        }
    }
}

// Reflect image horizontally by reversing the pixels in each row of the image.
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp, *a, *b;

    // Iterate each row up to middle pixel, swapping values of a and b
    for (int i = 0; i < height; i++)
    {
        for (a = image[i], b = image[i] + width - 1; a < b; a++, b--)
        {
            temp = *a;
            *a = *b;
            *b = temp;
        }
    }
}

// Blur image
/*
    For each RGB value of each pixel, iterate the RGB values of all other
    pixels within 1 of and including the target pixel and calculate the
    average R,G, and B values of all of those.

*/

// away from current pixel.
// EXAMPLE: If current pixel is row 0, col 4, and box size is 5,
// then box should still be as if current pixel is in center of box.
// So box would be from row -2 to row 2 and col 2 to col 6
void blur(int height, int width, RGBTRIPLE image[height][width], int boxSize)
{
    int count = 0;
    int halfA = boxSize / 2, halfB = boxSize - halfA;
    long rAvg, gAvg, bAvg;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // Iterate each RGBTRIPLE in the image, creating a box of width and height 'boxSize'.
            // Average the R, G, and B values of all pixels in box.
            rAvg = 0, gAvg = 0, bAvg = 0, count = 0;


            // Iterate each charWidth * charHeight block 1 at a time.
            // Iterate entire block, but only count pixels within image bounds towards average calculation.
            for (int i = row - halfA; i < row + halfB; i++)
            {
                for (int j = col - halfA; j < col + halfB; j++)
                {
                    if (i >= 0 && i < height && j >= 0 && j < width)
                    {
                        rAvg += image[i][j].rgbtRed;
                        gAvg += image[i][j].rgbtGreen;
                        bAvg += image[i][j].rgbtBlue;
                        count++;
                    }
                }
            }

            rAvg /= count;
            gAvg /= count;
            bAvg /= count;
            image[row][col].rgbtRed = rAvg;
            image[row][col].rgbtGreen = gAvg;
            image[row][col].rgbtBlue = bAvg;
        }
    }
}

// away from current pixel.
// EXAMPLE: If current pixel is row 0, col 4, and box size is 5,
// then box should still be as if current pixel is in center of box.
// So box would be from row -2 to row 2 and col 2 to col 6
void pixelize(int height, int width, RGBTRIPLE image[height][width], int boxSize)
{
    int count = 0;
    int halfA = boxSize / 2, halfB = boxSize - halfA;
    long rAvg, gAvg, bAvg;

    for (int row = 0; row < height; row += boxSize)
    {
        for (int col = 0; col < width; col += boxSize)
        {
            rAvg = 0, gAvg = 0, bAvg = 0, count = 0;

            for (int i = row - halfA; i < row + halfB; i++)
            {
                for (int j = col - halfA; j < col + halfB; j++)
                {
                    if (i >= 0 && i < height && j >= 0 && j < width)
                    {
                        rAvg += image[i][j].rgbtRed;
                        gAvg += image[i][j].rgbtGreen;
                        bAvg += image[i][j].rgbtBlue;
                        count++;
                    }
                }
            }

            rAvg /= count;
            gAvg /= count;
            bAvg /= count;

            for (int i = row - halfA; i < row + halfB; i++)
            {
                for (int j = col - halfA; j < col + halfB; j++)
                {
                    if (i >= 0 && i < height && j >= 0 && j < width)
                    {
                        image[i][j].rgbtRed = rAvg;
                        image[i][j].rgbtGreen = gAvg;
                        image[i][j].rgbtBlue = bAvg;
                    }
                }
            }
        }
    }
}

void edges(int height, int width, RGBTRIPLE image[height][width])
{
    const int KERNAL_SIZE = 3;
    const int M_COUNT = 6;
    const long GX[KERNAL_SIZE][KERNAL_SIZE] = {{-1, 0, 1},
                                               {-2, 0, 2},
                                               {-1, 0, 1}};
    const long GY[KERNAL_SIZE][KERNAL_SIZE] = {{-1,-2,-1},
                                               { 0, 0, 0},
                                               { 1, 2, 1}};
    uint8_t results[KERNAL_SIZE] = {0, 0, 0};
    long matrices[M_COUNT][KERNAL_SIZE][KERNAL_SIZE];
    long sums[M_COUNT];

    RGBTRIPLE edgeImage[height][width];

    for (int i = 0; i < height; i++)
    {
        // j iterates from -1 through width.
        for (int j = 0; j < width; j++)
        {
            // Set all matrix values to 0.
            resetMatrices(M_COUNT, KERNAL_SIZE, matrices, sums);

            // 1st 3 matrices are RGB for GX direction, 2nd 3 are RGB for GY direction
            for (int m = 0; m < M_COUNT; m++)
            {
                for (int row = 0; row < KERNAL_SIZE; row++)
                {
                    for (int col = 0; col < KERNAL_SIZE; col++)
                    {
                        if (row + i - 1 < 0 || row + i - 1 > height - 1 || col + j - 1 < 0 || col + j - 1 > width - 1)
                            matrices[m][row][col] = 0;
                        else
                        {
                            switch (m)
                            {
                                case 0:
                                    matrices[m][row][col] = round(image[row + i - 1][col + j - 1].rgbtRed * GX[row][col]);
                                    break;
                                case 1:
                                    matrices[m][row][col] = round(image[row + i - 1][col + j - 1].rgbtGreen * GX[row][col]);
                                    break;
                                case 2:
                                    matrices[m][row][col] = round(image[row + i - 1][col + j - 1].rgbtBlue * GX[row][col]);
                                    break;
                                case 3:
                                    matrices[m][row][col] = round(image[row + i - 1][col + j - 1].rgbtRed * GY[row][col]);
                                    break;
                                case 4:
                                    matrices[m][row][col] = round(image[row + i - 1][col + j - 1].rgbtGreen * GY[row][col]);
                                    break;
                                case 5:
                                    matrices[m][row][col] = round(image[row + i - 1][col + j - 1].rgbtBlue * GY[row][col]);
                                    break;
                            }

                            // ! Sums are possibly being accumulated wrong.
                            // Sum values in each matrix.
                            sums[m] += matrices[m][row][col];
                        }
                    }
                }
            }

            for (int s = 0; s < KERNAL_SIZE; s++)
            {
                // Check if sqrt((GX^2) + (GY^2)) is within bounds. If not, scale up/down.
                // Assign final values of a.
                results[s] = calculateColor(sums[s], sums[s + 3]);
            }

            edgeImage[i][j].rgbtRed = results[0];
            edgeImage[i][j].rgbtGreen = results[1];
            edgeImage[i][j].rgbtBlue = results[2];
        }
    }

    // Finally, update original image to match edgeImage
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = edgeImage[i][j].rgbtRed;
            image[i][j].rgbtGreen = edgeImage[i][j].rgbtGreen;
            image[i][j].rgbtBlue = edgeImage[i][j].rgbtBlue;
        }
    }
}

uint8_t calculateColor(long s1, long s2)
{
    long result = sqrtf(pow(s1, 2) + pow(s2, 2));

    if (result < 0)
        result = 0;
    else if (result > 255)
        result = 255;

    return round(result);
}


// Reset all matrix values and sums to 0.
void resetMatrices(int mCount, int pCount, long m[mCount][pCount][pCount], long s[mCount])
{
    for (int i = 0; i < mCount; i++)
    {
        s[i] = 0;

        for (int j = 0; j < pCount; j++)
        {
            for (int k = 0; k < pCount; k++)
                m[i][j][k] = 0;
        }
    }
}

char *getFileExtension(char* filename)
{
    char* dot = strrchr(filename, '.');

    if (!dot || dot == filename)
        return NULL;

    return dot + 1;
}

// Method that finds least GCD of two integers


// Methods to read/write images


// Method to get read video frames and save as images in memory


// TODO: Method to stream video
// 0. Create timer to set the frequency to cycle through video frames.
// 1. Track video time with variable.
// 2. In loop, load 1 frame of the video into memory at a time and create a 2D array of RGBTRIPLES to store the RGB data of every pixel in the frame.
// 3. Once the array is filled, run the ASCII filter on it and output the resulting ascii array to the console.
// 4. Repeat for all frames.
void streamVideo(char* filename)
{
    
}