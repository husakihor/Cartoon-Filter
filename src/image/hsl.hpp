#pragma once

#include <algorithm>
#include <cstdint>

struct Color;
struct HSL
{
	HSL(double h, double s, double l) :
		h(h),
		s(s),
		l(l)
	{
	}

	static HSL from(const Color& color);

	double h;
	double s;
	double l;
};

HSL* convert_to_hsl(const Color*, std::uint32_t size);

#include "hsl.hxx"