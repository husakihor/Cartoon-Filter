#pragma once

#include "image/color.hpp"

std::uint8_t* medianBlur(const std::uint8_t* grayScale, std::uint32_t width, std::uint32_t height, int blurValue);
std::uint8_t* adaptiveThreshold(const std::uint8_t* grayScale, const std::int64_t width, std::int64_t height, int blur);