#include <cstdint>
#include <stdlib.h>
#include <cstring>
#include <cmath>

#define SOBEL_OP_SIZE 9

void makeOpMem(const std::uint8_t* buffer, std::int64_t buffer_size, std::int64_t width, std::int64_t index, std::uint8_t* op)
{
    int bottom = index - width < 0;
    int top = index + width >= buffer_size;
    int left = index % width == 0;
    int right = (index + 1) % width == 0;

    op[0] = !bottom && !left  ? buffer[index - width - 1] : 0;
    op[1] = !bottom           ? buffer[index - width]   : 0;
    op[2] = !bottom && !right ? buffer[index - width + 1] : 0;

    op[3] = !left             ? buffer[index - 1]       : 0;
    op[4] = buffer[index];
    op[5] = !right            ? buffer[index + 1]       : 0;

    op[6] = !top && !left     ? buffer[index + width - 1] : 0;
    op[7] = !top              ? buffer[index + width]   : 0;
    op[8] = !top && !right    ? buffer[index + width + 1] : 0;
}

int convolveSum(std::uint8_t* X, int* Y, int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
	{
        sum += X[i] * Y[size - i - 1];
    }
    return sum;
}

void iterativeConvolution(const std::uint8_t* buffer, int buffer_size, int width, int* op, std::uint8_t** res)
{
	*res = (std::uint8_t*)malloc(sizeof(std::uint8_t) * buffer_size);

	std::uint8_t op_mem[SOBEL_OP_SIZE];
	std::memset(op_mem, 0, SOBEL_OP_SIZE);

	for (int i = 0; i < buffer_size; i++)
	{
		makeOpMem(buffer, buffer_size, width, i, op_mem);
		(*res)[i] = std::abs(convolveSum(op_mem, op, SOBEL_OP_SIZE));
	}
}

void contour(std::uint8_t* sobel_h, std::uint8_t* sobel_v, int gray_size, std::uint8_t** contour_img)
{
	*contour_img = (std::uint8_t*)malloc(sizeof(std::uint8_t) * gray_size);
	for (int i = 0; i < gray_size; i++)
	{
		(*contour_img)[i] = (std::uint8_t)std::sqrt((sobel_h[i] * sobel_h[i]) + (sobel_v[i] * sobel_v[i]));
	}
}

void sobelFilter(const std::uint8_t* gray, std::uint8_t** sobel_h_res, std::uint8_t** sobel_v_res, std::uint8_t** contour_img, int width, int height)
{
	int sobel_h[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1},
		sobel_v[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
	iterativeConvolution(gray, width * height, width, sobel_h, sobel_h_res);
	iterativeConvolution(gray, width * height, width, sobel_v, sobel_v_res);
	contour(*sobel_h_res, *sobel_v_res, width * height, contour_img);
}