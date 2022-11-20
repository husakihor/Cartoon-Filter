#include "color.hpp"

Color* convert_to_color(const HSL* colors, std::uint32_t size)
{
	Color* ret = (Color*)malloc(sizeof(Color) * size);

	for (std::uint32_t i = 0; i < size; i++)
	{
		ret[i] = Color::from(colors[i]);
	}

	return ret;
}