#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Obtener los valores originales de RGB
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            // Calcular el promedio
            int gray = round((red + green + blue) / 3.0);

            // Asignar el mismo valor a los tres canales
            image[i][j].rgbtRed = gray;
            image[i][j].rgbtGreen = gray;
            image[i][j].rgbtBlue = gray;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int a = 0; a < height; a++)
    {
        for (int i = 0, aux = width / 2; i < aux; i++)
        {
            RGBTRIPLE auxTriple = image[a][i];
            image[a][i] = image[a][width - 1 - i];
            image[a][width - 1 - i] = auxTriple;
        }
    }
    return;
}

void add(float *avR, float *avG, float *avB, int i, int j, int h, int w, int height, int width,
         RGBTRIPLE image[height][width])
{
    *avR += image[h + i][w + j].rgbtRed;
    *avG += image[h + i][w + j].rgbtGreen;
    *avB += image[h + i][w + j].rgbtBlue;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Crear una copia de la imagen
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int a = 0; a < width; a++)
        {
            float averageRed = 0, averageGreen = 0, averageBlue = 0;
            int cont = 0;

            // Adding same pixel
            add(&averageRed, &averageGreen, &averageBlue, 0, 0, i, a, height, width, copy);
            cont++;

            // Going up
            if (i - 1 >= 0)
            {
                // Up is allowed
                add(&averageRed, &averageGreen, &averageBlue, -1, 0, i, a, height, width, copy);
                cont++;

                if (a - 1 >= 0)
                {
                    // Up and left is allowed (corner)
                    add(&averageRed, &averageGreen, &averageBlue, -1, -1, i, a, height, width,
                        copy);
                    cont++;
                }

                if (a + 1 < width)
                {
                    // Up and right is allowed (corner)
                    add(&averageRed, &averageGreen, &averageBlue, -1, 1, i, a, height, width, copy);
                    cont++;
                }
            }

            if (a - 1 >= 0)
            {
                // Only left
                add(&averageRed, &averageGreen, &averageBlue, 0, -1, i, a, height, width, copy);
                cont++;
            }

            if (a + 1 < width)
            {
                // Only right
                add(&averageRed, &averageGreen, &averageBlue, 0, 1, i, a, height, width, copy);
                cont++;
            }

            // Going down
            if (i + 1 < height)
            {
                add(&averageRed, &averageGreen, &averageBlue, 1, 0, i, a, height, width, copy);
                cont++;

                if (a - 1 >= 0)
                {
                    // Down and left
                    add(&averageRed, &averageGreen, &averageBlue, 1, -1, i, a, height, width, copy);
                    cont++;
                }

                if (a + 1 < width)
                {
                    // Down and right
                    add(&averageRed, &averageGreen, &averageBlue, 1, 1, i, a, height, width, copy);
                    cont++;
                }
            }

            // Average
            averageRed /= cont;
            averageGreen /= cont;
            averageBlue /= cont;

            // Assigning the new values to the pixel
            image[i][a].rgbtRed = round(averageRed);
            image[i][a].rgbtGreen = round(averageGreen);
            image[i][a].rgbtBlue = round(averageBlue);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Crear una copia de la imagen
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int a = 0; a < width; a++)
        {
            int gxR = 0, gxG = 0, gxB = 0, gyR = 0, gyG = 0, gyB = 0;
            // Going up
            if (i - 1 >= 0)
            {
                // Up is allowed
                // Gx is 0
                // Gy is -2
                gyR += (copy[i - 1][a].rgbtRed * -2);
                gyG += (copy[i - 1][a].rgbtGreen * -2);
                gyB += (copy[i - 1][a].rgbtBlue * -2);

                if (a - 1 >= 0)
                {
                    // Up and left is allowed (corner)
                    // Gx is -1
                    gxR += (copy[i - 1][a - 1].rgbtRed * -1);
                    gxG += (copy[i - 1][a - 1].rgbtGreen * -1);
                    gxB += (copy[i - 1][a - 1].rgbtBlue * -1);
                    // Gy is -1
                    gyR += (copy[i - 1][a - 1].rgbtRed * -1);
                    gyG += (copy[i - 1][a - 1].rgbtGreen * -1);
                    gyB += (copy[i - 1][a - 1].rgbtBlue * -1);
                }

                if (a + 1 < width)
                {
                    // Up and right is allowed (corner)
                    // Gx is 1
                    gxR += (copy[i - 1][a + 1].rgbtRed * 1);
                    gxG += (copy[i - 1][a + 1].rgbtGreen * 1);
                    gxB += (copy[i - 1][a + 1].rgbtBlue * 1);
                    // Gy is -1
                    gyR += (copy[i - 1][a + 1].rgbtRed * -1);
                    gyG += (copy[i - 1][a + 1].rgbtGreen * -1);
                    gyB += (copy[i - 1][a + 1].rgbtBlue * -1);
                }
            }

            if (a - 1 >= 0)
            {
                // Only left
                // Gx is -2
                gxR += (copy[i][a - 1].rgbtRed * -2);
                gxG += (copy[i][a - 1].rgbtGreen * -2);
                gxB += (copy[i][a - 1].rgbtBlue * -2);
                // Gy is 0
            }

            if (a + 1 < width)
            {
                // Only right
                // Gx is 2
                gxR += (copy[i][a + 1].rgbtRed * 2);
                gxG += (copy[i][a + 1].rgbtGreen * 2);
                gxB += (copy[i][a + 1].rgbtBlue * 2);
                // Gy is 0
            }

            // Going down
            if (i + 1 < height)
            {
                // Down is allowed
                // Gx is 0
                // Gy is 2
                gyR += (copy[i + 1][a].rgbtRed * 2);
                gyG += (copy[i + 1][a].rgbtGreen * 2);
                gyB += (copy[i + 1][a].rgbtBlue * 2);

                if (a - 1 >= 0)
                {
                    // Down and left
                    // Gx is -1
                    gxR += (copy[i + 1][a - 1].rgbtRed * -1);
                    gxG += (copy[i + 1][a - 1].rgbtGreen * -1);
                    gxB += (copy[i + 1][a - 1].rgbtBlue * -1);
                    // Gy is 1
                    gyR += (copy[i + 1][a - 1].rgbtRed * 1);
                    gyG += (copy[i + 1][a - 1].rgbtGreen * 1);
                    gyB += (copy[i + 1][a - 1].rgbtBlue * 1);
                }

                if (a + 1 < width)
                {
                    // Down and right
                    // Gx is 1
                    gxR += (copy[i + 1][a + 1].rgbtRed * 1);
                    gxG += (copy[i + 1][a + 1].rgbtGreen * 1);
                    gxB += (copy[i + 1][a + 1].rgbtBlue * 1);
                    // Gy is 1
                    gyR += (copy[i + 1][a + 1].rgbtRed * 1);
                    gyG += (copy[i + 1][a + 1].rgbtGreen * 1);
                    gyB += (copy[i + 1][a + 1].rgbtBlue * 1);
                }
            }

            image[i][a].rgbtRed = fmin(255, fmax(0, round(sqrt(gxR * gxR + gyR * gyR))));
            image[i][a].rgbtGreen = fmin(255, fmax(0, round(sqrt(gxG * gxG + gyG * gyG))));
            image[i][a].rgbtBlue = fmin(255, fmax(0, round(sqrt(gxB * gxB + gyB * gyB))));
        }
    }

    return;
}
