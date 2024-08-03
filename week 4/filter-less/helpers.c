#include "helpers.h"
#include <math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            // convert pixels to float
            float Red = image[row][column].rgbtRed;
            float Green = image[row][column].rgbtGreen;
            float Blue = image[row][column].rgbtBlue;
            // convert to avg value
            int avg = round((Red + Green + Blue) / 3);
            image[row][column].rgbtRed = image[row][column].rgbtGreen = image[row][column].rgbtBlue = avg;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            // convert pixels to float
            float Red = image[row][column].rgbtRed;
            float Green = image[row][column].rgbtGreen;
            float Blue = image[row][column].rgbtBlue;
            // algorithm to convert sepia
            int sepiaRed = round(.393 * Red + .769 * Green + .189 * Blue);
            int sepiaGreen = round(.349 * Red + .686 * Green + .168 * Blue);
            int sepiaBlue = round(.272 * Red + .534 * Green + .131 * Blue);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[row][column].rgbtRed = sepiaRed;
            image[row][column].rgbtGreen = sepiaGreen;
            image[row][column].rgbtBlue = sepiaBlue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create a temporary image
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int total_Red, total_Blue, total_Green;
            total_Red = total_Blue = total_Green = 0;
            float counter = 0.00;
            // get neighbouring pixels
            //create x-y axes (3x3)
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    int currentX = i + x;
                    int currentY = j + y;
                    //check for valid neighboring pixels
                    if (currentX < 0 || currentY <0 || currentX > (height - 1) || currentY > (width - 1))
                    {
                        continue;
                    }
                    total_Red += image[currentX][currentY].rgbtRed;
                    total_Green += image[currentX][currentY].rgbtGreen;
                    total_Blue += image[currentX][currentY].rgbtBlue;

                    counter++;
                }
            }
            // avg of neighboring pixels ( using temp to avoid damage the stock image)
            temp[i][j].rgbtRed = round(total_Red / counter);
            temp[i][j].rgbtGreen = round(total_Green / counter);
            temp[i][j].rgbtBlue = round(total_Blue / counter);
        }
    }

    //copy the blurr image to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}
