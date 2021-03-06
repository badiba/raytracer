#include "image.hpp"
#include <iostream>
#include <fstream>

Image::Image(int width, int height) : width(width), height(height)
{
    for (auto i = 0; i < height; i++)
    {
        pixels.push_back(std::vector<Color>(width));
    }
}

void Image::SetPixelValue(int col, int row, const Color& color)
{
    if (row < height && row >= 0 && col < width && col >= 0)
    {
        pixels[row][col] = color;
    }
    else
    {
        // TODO: warn
    }
}

void Image::SaveImage(const char *imageName) const
{
    std::ofstream myfile(imageName);

    myfile << "P3" << std::endl;
    myfile << width << " " << height << std::endl;
    myfile << "255" << std::endl;

	for (auto y = 0 ; y < height; y++)
	{
		for (auto x = 0 ; x < width; x++)
        {
            auto& pixel = pixels[y][x];
            myfile << (unsigned)pixel.r << " " << (unsigned)pixel.g << " " << (unsigned)pixel.b << " ";
        }

        myfile << std::endl;
	}

    myfile.close();
}
