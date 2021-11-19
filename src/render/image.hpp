#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <cstdio>
#include <cstdlib>

#include "../defs.hpp"

struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

class Image
{
public:
    std::vector<std::vector<Color>> pixels;
	int width;
	int height;

	Image(int width, int height);
	void SetPixelValue(int col, int row, const Color& color);
	void SaveImage(const char *imageName) const;
};

#endif
