#pragma once

void sobelFilter(const std::uint8_t* gray, std::uint8_t** sobel_h_res, std::uint8_t** sobel_v_res, std::uint8_t** contour_img, int width, int height);