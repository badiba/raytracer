#include "image.hpp"

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
    // TODO
	FILE *output;

	output = fopen(imageName, "w");
	fprintf(output, "P3\n");
	fprintf(output, "%d %d\n", width, height);
	fprintf(output, "255\n");

	for (auto y = 0 ; y < height; y++)
	{
		for (auto x = 0 ; x < width; x++)
        {
            auto pixel = pixels[y][x];
            fprintf(output, "%d ", pixel.r);
            fprintf(output, "%d ", pixel.g);
            fprintf(output, "%d ", pixel.b);
        }

		fprintf(output, "\n");
	}

	fclose(output);
}
