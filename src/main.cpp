#include "image/png.hpp"
#include "filters/filters.hpp"
#include "filters/sobel.hpp"
#include "filters/kuwahara.hpp"
#include "image/tifo.hpp"

#include <cstring>
#include <cstdlib>
#include <iostream>

int main(int argc, const char** argv)
{
	PNG toProcess("jules.dorbeau.png", "jules.dorbeau.output.png");

	auto rgb = tifo(toProcess.GetPixels(), toProcess.GetWidth(), toProcess.GetHeight(), false);

	auto pixels = toProcess.GetPixels();
	std::memcpy(pixels, rgb, toProcess.GetSize() * sizeof(Color));

	return 0;
}