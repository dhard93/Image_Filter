#include "bmp.h"

void ascii(int iHeight, int iWidth, int oHeight, int oWidth, int charHeight, int charWidth, RGBTRIPLE image[iHeight][iWidth], char chars[oHeight][oWidth]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width], int boxSize);

// Pixelize image
void pixelize(int height, int width, RGBTRIPLE image[height][width], int boxSize);

uint8_t calculateColor(long s1, long s2);

// Reset matrices
void resetMatrices(int mCount, int pCount, long m[mCount][pCount][pCount], long s[mCount]);

char *getFileExtension(char* filename);