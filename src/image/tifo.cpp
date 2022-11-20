#include "tifo.hpp"
#include "filters/filters.hpp"
#include "filters/sobel.hpp"
#include "filters/kuwahara.hpp"

#include <cmath>
#include <cstring>
#include <iostream>

Color image_mean(Color* colors, std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height, double distance)
{
	std::uint32_t r = 0;
	std::uint32_t g = 0;
	std::uint32_t b = 0;
	std::uint32_t a = 0;
	std::uint32_t num = 0;
	for (int j = -distance; j <= distance; j++)
	{
		for (int i = -distance; i <= distance; i++)
		{
			std::int64_t nx = i + x;
			std::int64_t ny = i + y;
			if (nx >= 0 && nx < width && ny >= 0 && ny < height)
			{
				Color c = colors[ny * width + nx];
				r += c.r;
				b += c.b;
				g += c.g;
				a += c.a;
				num++;
			}
		}
	}
	return Color(r/num, g/num, b/num, a/num);
}

void tifo_create_dots(const HSL* colors, Color* ret, std::uint32_t width, std::uint32_t height)
{
	double minSize = std::min(width, height);
	double ratio = minSize / 350.0;
	if (ratio < 1.0)
	{
		ratio = 1.0;
	}
	double inc = ratio * 3.0;
	
	auto rgb = convert_to_color(colors, width * height);
	for (std::uint32_t i = ratio; i < (height + ratio); i += inc)
	{
		for (std::uint32_t j = ratio; j < (width + ratio); j += inc)
		{
			Color c = image_mean(rgb, j, i, width, height, ratio);
			const double RADIAN = 0.01745329251;
			const double TWOPI = RADIAN * 360.0;
			for (double rad = 0; rad < ratio; rad += 1.0)
			{
				for (double ang = 0.0; ang < TWOPI; ang += RADIAN)
				{
					std::int64_t newY = (i + std::sin(ang) * rad);
					std::int64_t newX = (j + std::cos(ang) * rad);
					if (newX >= 0 && newX < width && newY >= 0 && newY < height)
					{
						ret[width * newY + newX] = c;
					}
				}
			}
		}
	}
	free(rgb);
}

void tifo_add_comic_circles(HSL* colors, std::uint32_t width, std::uint32_t height)
{
	bool* affected = (bool*)calloc(width * height, sizeof(bool));

	double minSize = std::min(width, height);
	double ratio = minSize / 350.0;
	if (ratio < 1.0)
	{
		ratio = 1.0;
	}

	std::uint32_t distance = std::round(ratio * 5.0);
	std::uint32_t up = std::ceil(ratio) * 3.0;
	std::uint32_t radius = std::floor(ratio);

	std::cout << "Distance : " << distance << "| Radius : " << radius << "| Up : " << up << std::endl;

	bool odd = false;
	for (std::uint32_t i = 0; i < (height + up); i += up)
	{
		for (std::uint32_t j = (odd) ? 0 : (distance / 2); j < width; j += distance)
		{
			const double RADIAN = 0.01745329251;
			const double TWOPI = RADIAN * 360.0;
			for (double rad = 0; rad < radius; rad += 1.0)
			{
				for (double ang = 0.0; ang < TWOPI; ang += RADIAN)
				{
					std::int64_t newY = (i + std::sin(ang) * rad);
					std::int64_t newX = (j + std::cos(ang) * rad);
					if (newX >= 0 && newX < width && newY >= 0 && newY < height)
					{
						auto pos = width * newY + newX;
						if (!affected[pos])
						{
							affected[pos] = true;
							colors[pos].l = 1.0 - colors[pos].l;
							colors[pos].s = 1.0 - colors[pos].s;
						}
					}
				}
			}
		}
		odd = !odd;
	}

	free(affected);
}

Color* tifo(const Color* pixels, std::uint32_t width, std::uint32_t height, bool dots)
{
	double minSize = std::min(width, height) / 180.0;
	if (minSize < 3.0)
	{
		minSize = 3.0;
	}
	auto rgb = KuwaharaBlur(pixels, width, height, minSize);

	std::uint32_t size = width * height;
	std::uint8_t* lightNess = (std::uint8_t*)malloc(size * sizeof(std::uint8_t));
	auto colors = convert_to_hsl(rgb, size);
	for (std::uint32_t i = 0; i < size; i++)
	{
		double sats = std::round(colors[i].s / 0.5);
		colors[i].s = 0.5 * sats;

		double lums = std::round(colors[i].l / 0.22);
		colors[i].l = 0.22 * lums;

		double deg = std::round(colors[i].h / 24.0);
		colors[i].h = 24.0 * deg;

		lightNess[i] = rgb[i].Grayscale();
	}
	free(rgb);

	std::uint8_t* edges = adaptiveThreshold(lightNess, width, height, 3);

	Color* ret = (Color*)calloc(size, sizeof(Color));
	if (!dots)
	{
		tifo_add_comic_circles(colors, width, height);
		for (std::uint32_t i = 0; i < size; i++)
		{
			if (edges[i] == 0)
			{
				ret[i] = Color(0, 0, 0, 255);
			}
			else
			{
				ret[i] = Color::from(colors[i]);
			}
		}
	}
	else
	{
		tifo_create_dots(colors, ret, width, height);
		for (std::uint32_t i = 0; i < size; i++)
		{
			if (edges[i] == 0)
			{
				ret[i] = Color(0, 0, 0, 255);
			}
			else
			{
				ret[i].a = 255;
			}
		}
	}

	free(edges);
	free(colors);
	free(lightNess);

	return ret;
}