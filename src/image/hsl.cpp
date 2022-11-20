#include "hsl.hpp"

HSL* convert_to_hsl(const Color* colors, std::uint32_t size)
{
	HSL* ret = (HSL*)malloc(sizeof(HSL) * size);

	for (std::uint32_t i = 0; i < size; i++)
	{
		ret[i] = HSL::from(colors[i]);
	}

	return ret;
}