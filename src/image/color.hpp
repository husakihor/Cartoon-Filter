#pragma once

#include <cstdint>
#include <cmath>
#include <string>
#include <ostream>

struct HSL;
struct Color
{
	Color() : r(0),
			g(0),
			b(0),
			a(0)
	{
	}

	Color(std::uint8_t r, std::uint8_t g, std::uint8_t b) : r(r),
			g(g),
			b(b),
			a(255)
	{
	}

	Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) : r(r),
			g(g),
			b(b),
			a(a)
	{
	}

	void ToGrayscale()
	{
		auto v = Grayscale();
		this->r = v;
		this->g = v;
		this->b = v;
	}

	std::uint8_t Grayscale() const
	{
		return std::floor<std::uint16_t>((this->r * 0.299) + (this->g * 0.587) + (this->b * 0.114));
	}

	static Color from(const HSL& color, std::uint8_t alpha = 255);

	std::uint8_t r;
	std::uint8_t g;
	std::uint8_t b;
	std::uint8_t a;

	friend inline std::ostream& operator<<(std::ostream& os, const Color& dt)
	{
		os << "(R:" << std::to_string(dt.r) << ", G:" << std::to_string(dt.g) << ", B:" << std::to_string(dt.b)
		   << ", A:" << std::to_string(dt.a) << ")";
		return os;
	}
};

Color* convert_to_color(const HSL* colors, std::uint32_t size);

#include "color.hxx"