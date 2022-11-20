#include "png.hpp"

PNG::PNG(const std::string& path, const std::uint32_t width, const std::uint32_t height) : Image(),
		m_fd(nullptr, &fclose),
		m_png(nullptr),
		m_pngInfo(nullptr),
		m_rowpointers(nullptr)
{
	Init(width, height);
	SetupOutput(path);
}

PNG::PNG(const std::string& path, const std::string& output) : Image(),
	m_fd(nullptr, &fclose),
	m_png(nullptr),
	m_pngInfo(nullptr),
	m_rowpointers(nullptr)
{
	SetupInput(path);
	SetupOutput(output);
}

PNG::PNG(const std::string& path) : Image(),
	m_fd(nullptr, &fclose),
	m_png(nullptr),
	m_pngInfo(nullptr),
	m_rowpointers(nullptr)
{
	SetupInput(path);
}

void PNG::SetupInput(const std::string& input)
{
	FILE* fd = fopen(input.c_str(), "rb");
	if (fd == nullptr)
	{
		throw std::invalid_argument("Couldn't open file: " + input);
	}

	auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

	if (!png_ptr || setjmp(png_jmpbuf(png_ptr)))
	{
		fclose(fd);
		throw std::invalid_argument("Jump buf error");
	}

	auto info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		fclose(fd);
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		throw std::invalid_argument("Couldn't create info ptr");
	}
	png_init_io(png_ptr, fd);

	png_read_info(png_ptr, info_ptr);

	Init(png_get_image_width(png_ptr, info_ptr), png_get_image_height(png_ptr, info_ptr));

	auto color_type = png_get_color_type(png_ptr, info_ptr);
	auto bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	if (bit_depth == 16)
	{
		png_set_strip_16(png_ptr);
	}

	if (color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(png_ptr);
	}

	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	{
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	}

	if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_ptr);
	}

	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
	}

	if (color_type == PNG_COLOR_TYPE_GRAY ||
	   color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		png_set_gray_to_rgb(png_ptr);
	}

	png_read_update_info(png_ptr, info_ptr);
	png_read_image(png_ptr, m_rowpointers);

	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	fclose(fd);
}

void PNG::SetupOutput(const std::string& output)
{
	FILE* fd = fopen(output.c_str(), "wb");
	if (fd == nullptr)
	{
		throw std::invalid_argument("Couldn't open file: " + output);
	}
	m_fd.reset(fd);

	m_png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (m_png == nullptr)
	{
		throw std::invalid_argument("Couldn't create png write struct.");
	}

	m_pngInfo = png_create_info_struct(m_png);
	if (m_pngInfo == nullptr)
	{
		throw std::invalid_argument("Couldn't create png info struct.");
	}

	if (setjmp(png_jmpbuf(m_png)))
	{
		throw std::invalid_argument("Jump buf error");
	}

	png_init_io(m_png, m_fd.get());
	png_set_IHDR(m_png, m_pngInfo, GetWidth(), GetHeight(), 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(m_png, m_pngInfo);
}


PNG::~PNG() noexcept
{
	if (m_png != nullptr)
	{
		png_write_image(m_png, m_rowpointers);
		png_write_end(m_png, m_pngInfo);
		png_destroy_write_struct(&m_png, &m_pngInfo);
	}
	free(m_rowpointers);
}