#pragma once

#include "image.hpp"

#include <png.h>
#include <memory>

class PNG : public Image
{
public:
	PNG(const std::string& path, const std::uint32_t width, const std::uint32_t height);
	PNG(const std::string& path, const std::string &output);
	PNG(const std::string& path);
	~PNG() noexcept;
private:
	void Init(std::uint32_t width, std::uint32_t height)
	{
		m_width = width;
		m_height = height;
		m_size = m_width * m_height;
		m_pixels = static_cast<Color*>(calloc(m_size, sizeof(Color)));
		m_rowpointers = static_cast<png_bytep*>(malloc(sizeof(png_bytep) * m_height));
		for (auto it = m_height; it;)
		{
			it--;
			m_rowpointers[it] = reinterpret_cast<png_bytep>(&m_pixels[m_width * it]);
		}
	}

	void SetupInput(const std::string& input);
	void SetupOutput(const std::string& output);

	std::unique_ptr<FILE, decltype(&fclose)> m_fd;
	png_structp m_png;
	png_infop m_pngInfo;
	png_bytep* m_rowpointers;
};