#pragma once

#include "color.hpp"

#include <cstdint>
#include <cstdlib>

class Image
{
public:
	std::uint32_t GetWidth() const
	{
		return m_width;
	}

	std::uint32_t GetHeight() const
	{
		return m_height;
	}

	std::uint32_t GetSize() const
	{
		return m_size;
	}

	Color* GetPixels() const
	{
		return m_pixels;
	}

	void ToGrayscale()
	{
		for (auto it = m_size; it; it--)
		{
			m_pixels[it - 1].ToGrayscale();
		}
	}

protected:
	Image(const std::uint32_t width, const std::uint32_t height) :
		m_width(width),
		m_height(height),
		m_size(width * height),
		m_pixels((Color*)calloc(width * height, sizeof(Color)))
		{
		}

	Image() :
		m_width(0),
		m_height(0),
		m_size(0),
		m_pixels(nullptr)
		{
		}

	~Image() { free(m_pixels); };

	std::uint32_t m_width;
	std::uint32_t m_height;
	std::uint32_t m_size;
	Color* m_pixels;
};