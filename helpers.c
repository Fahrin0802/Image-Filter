#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0 ; row < height ; row++)
    {
        for (int column = 0 ; column < width ; column++)
        {
            int B = image[row][column].rgbtBlue;
            int R = image[row][column].rgbtRed;
            int G = image[row][column].rgbtGreen;

            int avg = round((B + R + G) / (float)3);
 
            image[row][column].rgbtBlue = avg;
            image[row][column].rgbtRed = avg;
            image[row][column].rgbtGreen = avg;

        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int G[10];
    int B[10];
    int R[10];

    for (int row = 0 ; row < height ; row++)
    {

        int i = 0;
        for (int column = 0 ; column < width ; column++)
        {

            G[column] = image[row][column].rgbtGreen;
            B[column] = image[row][column].rgbtBlue;
            R[column] = image[row][column].rgbtRed;

        }


        for (int column = width - 1 ; column >= 0 ; column--)
        {
            image[row][column].rgbtBlue = B[i];
            image[row][column].rgbtRed = R[i];
            image[row][column].rgbtGreen = G[i];

            i++;
        }

    }
    return;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int sumB;
    int sumR;
    int sumG;
    int counter;
    RGBTRIPLE duplicate[height][width];

    for (int row = 0 ; row < height ;  row ++)
    {
        for (int column = 0 ; column < width ; column ++)
        {
            sumB = 0;
            sumR = 0;
            sumG = 0;
            counter = 0;

            for (int SE = -1 ; SE < 2 ; SE++)
            {
                // Skip sum step if the surrounding pixels is left or right of the image
                if (column + SE >= 0 && column + SE <= width - 1)
                {
                    for (int HE = -1 ; HE < 2 ; HE++)
                    {
                        // Skip sum step if the surrounding pixels is above or below the image
                        if (row + HE >= 0 && row + HE <= height - 1)
                        {
                            // calculating sum of the colour values in the 9 pixels
                            sumR = sumR + image[row + HE][column + SE].rgbtRed;
                            sumB = sumB + image[row + HE][column + SE].rgbtBlue;
                            sumG = sumG + image[row + HE][column + SE].rgbtGreen;
                            counter++;
                        }

                    }
                }
            }

            // Storing the calculated pixels in the duplicate struct
            duplicate[row][column].rgbtRed = round(sumR / (float)counter);
            duplicate[row][column].rgbtGreen = round(sumG / (float)counter);
            duplicate[row][column].rgbtBlue = round(sumB / (float)counter);

        }
    }

    // Transfering duplicate pixels to output image
    for (int row = 0 ; row < height ;  row ++)
    {
        for (int column = 0 ; column < width ; column ++)
        {
            image[row][column].rgbtRed = duplicate[row][column].rgbtRed;
            image[row][column].rgbtGreen = duplicate[row][column].rgbtGreen;
            image[row][column].rgbtBlue = duplicate[row][column].rgbtBlue;
        }
    }

}



int limit(int pixel)
{
    if (pixel > 255)
    {
        pixel = 255;
        return pixel; 
        // alternative syntax
        // return pixel > 255 ? 255 : pixel; 
        // learned this syntax from youtube
    }
    else
    {
        return pixel;
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
  

    typedef struct
    {
        int G;
        int B;
        int R;
    }
    G_table;
    
    G_table X[height][width];
    G_table Y[height][width];
    // Modified pixels (i.e after implementing the Sobel operator) will be stored here 
    
    for (int row = 0 ; row < height ;  row ++)
    {
        for (int column = 0 ; column < width ; column ++)
        {
            // Populating array and ensuring all initial values are zero
            X[row][column].G = 0; 
            X[row][column].B = 0;
            X[row][column].R = 0;
            Y[row][column].G = 0;
            Y[row][column].B = 0;
            Y[row][column].R = 0;
        }
    }

   


    for (int row = 0 ; row < height ;  row ++)
    {
        for (int column = 0 ; column < width ; column ++)
        {

            for (int HE = -1 ; HE <= 1 ; HE++)
            {
                for (int SE = -1 ; SE <= 1 ; SE++)
                {
                    // Skip sum step if the surrounding pixels is left or right of the image
                    // Skip sum step if the surrounding pixels is above or below of the image
                    if ((column + SE < 0 || column + SE > width - 1) || (row + HE < 0 || row + HE > height - 1))
                    {
                        continue;
                        // Skips step in loop if the condition is met
                    }
                        
                    X[row][column].G += image[row + HE][column + SE].rgbtGreen * Gx[HE + 1][SE + 1];
                    X[row][column].R += image[row + HE][column + SE].rgbtRed * Gx[HE + 1][SE + 1];
                    X[row][column].B += image[row + HE][column + SE].rgbtBlue * Gx[HE + 1][SE + 1];
                    
                    Y[row][column].G += image[row + HE][column + SE].rgbtGreen * Gy[HE + 1][SE + 1];
                    Y[row][column].R += image[row + HE][column + SE].rgbtRed * Gy[HE + 1][SE + 1];
                    Y[row][column].B += image[row + HE][column + SE].rgbtBlue * Gy[HE + 1][SE + 1];
                        
                    
                }
            }
        }
    }


    for (int row = 0 ; row < height ;  row ++)
    {
        for (int column = 0 ; column < width ; column ++)
        {
            // Plugging in the edged pixels into the image
            image[row][column].rgbtRed = limit(round(sqrt(X[row][column].R * X[row][column].R + Y[row][column].R * Y[row][column].R)));
            image[row][column].rgbtGreen = limit(round(sqrt(X[row][column].G * X[row][column].G + Y[row][column].G * Y[row][column].G)));
            image[row][column].rgbtBlue = limit(round(sqrt(X[row][column].B * X[row][column].B + Y[row][column].B * Y[row][column].B)));

        }
    }
}
