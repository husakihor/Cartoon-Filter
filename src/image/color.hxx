#pragma once

#include "color.hpp"
#include "hsl.hpp"

inline Color Color::from(const HSL& color, std::uint8_t alpha)
{
	double h = color.h;//std::fmod(color.h, 360.0);

	double c = (1.0 - std::abs(2.0 * color.l - 1.0)) * color.s;
	double x = c * (1.0 - std::abs(std::fmod(h / 60.0, 2.0) - 1.0));

	double m = color.l - c / 2.0;
	c += m;
	x += m;

	double rp = 0.0;
	double gp = 0.0;
	double bp = 0.0;

	if (color.s == 0.0)
	{
		rp = gp = bp = color.l;
	}
	else if (h < 60.0)
	{
		rp = c;
		gp = x;
		bp = m;
	}
	else if (60.0 <= h && h < 120.0)
	{
		rp = x;
		gp = c;
		bp = m;
	}
	else if (120.0 <= h && h < 180.0)
	{
		rp = m;
		gp = c;
		bp = x;
	}
	else if (180.0 <= h && h < 240.0)
	{
		rp = m;
		gp = x;
		bp = c;
	}
	else if (240.0 <= h && h < 300.0)
	{
		rp = x;
		gp = m;
		bp = c;
	}
	else if (300.0 <= h)
	{
		rp = c;
		gp = m;
		bp = x;
	}

	return Color(std::clamp<std::int16_t>(rp * 255, 0, 255),
	std::clamp<std::int16_t>(gp * 255.0, 0, 255),
	std::clamp<std::int16_t>(bp * 255.0, 0, 255),
	alpha);
}