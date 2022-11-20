#include "filters.hpp"

#include <stdexcept>
#include <algorithm>

int getMedianFromKernel(const std::uint8_t* colors, int* kernel, int blurValue, int x, int y, std::int64_t width, std::int64_t height)
{
	int mid = blurValue / 2;
	int k = 0;
	for(int i = -mid; i <= mid; i++)
	{
		for(int j = -mid; j <= mid; j++)
		{
			if(x + i < 0 || x + i >= width || y + j < 0 || y + j >= height)
			{
				kernel[k++] = colors[y * width + x];
			}
			else
			{
				kernel[k++] = colors[(y + j) * width + x + i];
			}
		}
	}
	std::sort(kernel, kernel + (blurValue * blurValue));
	return kernel[(blurValue * blurValue) / 2];
}

std::uint8_t* medianBlur(const std::uint8_t* grayScale, std::uint32_t width, std::uint32_t height, int blurValue)
{
	if (blurValue % 2 != 1)
	{
		throw std::invalid_argument("blurValue should be an odd number but was given :" + blurValue);
	}
	auto size = width * height;
	std::uint8_t* buffer = (std::uint8_t*)malloc(size * sizeof(std::uint8_t));
	int* kernel = (int*)malloc(blurValue * blurValue * sizeof(int));
	for (std::uint32_t x = 0; x < width; x++)
	{
		for (std::uint32_t y = 0; y < height; y++)
		{
			buffer[y * width + x] = getMedianFromKernel(grayScale, kernel, blurValue, x, y, width, height);
		}
	}
	free(kernel);
	return buffer;
}