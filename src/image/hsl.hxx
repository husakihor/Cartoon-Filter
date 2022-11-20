#pragma once

#include "color.hpp"
#include "hsl.hpp"

#include <iostream>

inline HSL HSL::from(const Color& color)
{
	double rp = color.r / 255.0;
	double gp = color.g / 255.0;
	double bp = color.b / 255.0;

	double cmin = std::min(std::min(rp, gp), bp);
	double cmax = std::max(std::max(rp, gp), bp);

	double delta = cmax - cmin;

	double l = (cmax + cmin) / 2.0;
	double s = (delta == 0) ? 0.0 : delta / (1.0 - std::abs(2.0 * l - 1.0));
	/*std::cout << color << std::endl;
	std::cout << s << std::endl;
	std::cout << l << std::endl;*/
	double h = 0;

	if (delta == 0)
	{
	}
	else if (cmax == rp)
	{
		h = 60.0 * std::fmod(((gp - bp) / delta), 6.0);
	}
	else if (cmax == gp)
	{
		h = 60.0 * ((bp - rp) / delta + 2.0);
	}
	else
	{
		h = 60.0 * ((rp - gp) / delta + 4.0);
	}
	//std::cout << h << std::endl;

	return HSL{h, s, l};
}