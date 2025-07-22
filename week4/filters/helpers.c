#include "helpers.h"
#include <math.h>

#define PIXELS_BOX 9
#define MAX 255
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double average_shade;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average_shade =
                (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            image[i][j].rgbtBlue = (int) round(average_shade);
            image[i][j].rgbtGreen = (int) round(average_shade);
            image[i][j].rgbtRed = (int) round(average_shade);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // create buffer of size width
    RGBTRIPLE placeholder;

    // iterate though rows
    for (int i = 0; i < height; i++)
    {
        // populate buffer with current row
        for (int j = 0, k = width - 1; j < width / 2; j++, k--)
        {
            placeholder = image[i][j];
            image[i][j] = image[i][k];
            image[i][k] = placeholder;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create copy of image
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // create indexes for each of the 3 x 3 pixels
    int sumB;
    int sumG;
    int sumR;
    double numPixels;

    // iterate between rows
    for (int i = 0; i < height; i++)
    { // iterate within rows
        for (int j = 0; j < width; j++)
        {
            sumB = 0;
            sumG = 0;
            sumR = 0;
            numPixels = 0.0;
            for (int k = 0; k < PIXELS_BOX; k++)
            {
                int index_x[PIXELS_BOX] = {j - 1, j, j + 1, j - 1, j, j + 1, j - 1, j, j + 1};
                int index_y[PIXELS_BOX] = {i - 1, i - 1, i - 1, i, i, i, i + 1, i + 1, i + 1};
                if (index_x[k] >= 0 && index_y[k] >= 0 && index_y[k] < height && index_x[k] < width)
                {
                    sumB = sumB + copy[index_y[k]][index_x[k]].rgbtBlue;
                    sumG = sumG + copy[index_y[k]][index_x[k]].rgbtGreen;
                    sumR = sumR + copy[index_y[k]][index_x[k]].rgbtRed;
                    numPixels++;
                }
            }
            image[i][j].rgbtBlue = (int) round(sumB / numPixels);
            image[i][j].rgbtGreen = (int) round(sumG / numPixels);
            image[i][j].rgbtRed = (int) round(sumR / numPixels);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create copy of image
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // create indexes for each of the 3 x 3 pixels
    double sumBx, sumBy, sumGx, sumGy, sumRx, sumRy;
    int Gx[PIXELS_BOX] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Gy[PIXELS_BOX] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // real pixel colors to cap on 255
    int color_b = 0, color_g = 0, color_r = 0;

    // iterate between rows
    for (int i = 0; i < height; i++)

        // iterate within rows
        for (int j = 0; j < width; j++)
        {
            sumBx = 0, sumBy = 0, sumGx = 0, sumGy = 0, sumRx = 0, sumRy = 0;
            for (int k = 0; k < PIXELS_BOX; k++)
            {
                int index_x[PIXELS_BOX] = {j - 1, j, j + 1, j - 1, j, j + 1, j - 1, j, j + 1};
                int index_y[PIXELS_BOX] = {i - 1, i - 1, i - 1, i, i, i, i + 1, i + 1, i + 1};
                if (index_x[k] >= 0 && index_y[k] >= 0 && index_y[k] < height && index_x[k] < width)
                {
                    sumBx = sumBx + copy[index_y[k]][index_x[k]].rgbtBlue * Gx[k];
                    sumBy = sumBy + copy[index_y[k]][index_x[k]].rgbtBlue * Gy[k];
                    sumGx = sumGx + copy[index_y[k]][index_x[k]].rgbtGreen * Gx[k];
                    sumGy = sumGy + copy[index_y[k]][index_x[k]].rgbtGreen * Gy[k];
                    sumRx = sumRx + copy[index_y[k]][index_x[k]].rgbtRed * Gx[k];
                    sumRy = sumRy + copy[index_y[k]][index_x[k]].rgbtRed * Gy[k];
                }
            }
            color_b = (int) round(sqrt(pow(sumBx, 2) + pow(sumBy, 2)));
            color_g = (int) round(sqrt(pow(sumGx, 2) + pow(sumGy, 2)));
            color_r = (int) round(sqrt(pow(sumRx, 2) + pow(sumRy, 2)));

            if (color_b < MAX)
                image[i][j].rgbtBlue = color_b;
            else
                image[i][j].rgbtBlue = MAX;

            if (color_g < MAX)
                image[i][j].rgbtGreen = color_g;
            else
                image[i][j].rgbtGreen = MAX;

            if (color_r < MAX)
                image[i][j].rgbtRed = color_r;
            else
                image[i][j].rgbtRed = MAX;
        }
    return;
}
