#include "filters.hpp"

std::uint8_t* adaptiveThreshold(const std::uint8_t* grayScale, const std::int64_t width, std::int64_t height, int blur)
{
	std::uint8_t* gray = medianBlur(grayScale, width, height, blur);
	std::uint8_t* res = (std::uint8_t*)malloc(width * height * sizeof(std::uint8_t));

	int kernelSize = 30;
	int t = 50;
	int mid = kernelSize / 2;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int count = 0, sum = 0;
			for (int i = -mid; i <= mid; i++)
			{
				for (int j = -mid; j <= mid; j++)
				{
					if ((x + i) < 0 || (y + j) < 0 || (x + i) >= width || (y + j) >= height)
					{
						continue;
					}
					sum += gray[(y + j) * width + x + i];
					count++;
				}
			}
			auto mean = sum / (count + t);
			auto threshold = mean;
			res[y * width + x] = (gray[y * width + x] < threshold) ? 0 : 1;
		}
	}
	free(gray);
	return res;
}
