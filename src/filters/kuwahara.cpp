#include "filters/kuwahara.hpp"

Color* KuwaharaBlur(const Color* colors, int width, int height, int size)
{
	Color* ret = (Color*)malloc(width * height * sizeof(Color));	

	int ApetureMinX[] = { -(size / 2), 0, -(size / 2), 0 };
	int ApetureMaxX[] = { 0, (size / 2), 0, (size / 2) };
	int ApetureMinY[] = { -(size / 2), -(size / 2), 0, 0 };
	int ApetureMaxY[] = { 0, 0, (size / 2), (size / 2) };
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			std::uint32_t RValues[] = { 0, 0, 0, 0 };
			std::uint32_t GValues[] = { 0, 0, 0, 0 };
			std::uint32_t BValues[] = { 0, 0, 0, 0 };
			std::uint32_t NumPixels[] = { 0, 0, 0, 0 };
			std::uint8_t MaxRValue[] = { 0, 0, 0, 0 };
			std::uint8_t MaxGValue[] = { 0, 0, 0, 0 };
			std::uint8_t MaxBValue[] = { 0, 0, 0, 0 };
			std::uint8_t MinRValue[] = { 255, 255, 255, 255 };
			std::uint8_t MinGValue[] = { 255, 255, 255, 255 };
			std::uint8_t MinBValue[] = { 255, 255, 255, 255 };
			for (int i = 0; i < 4; ++i)
			{
				for (int x2 = ApetureMinX[i]; x2 < ApetureMaxX[i]; ++x2)
				{
					int TempX = x + x2;
					if (TempX >= 0 && TempX < width)
					{
						for (int y2 = ApetureMinY[i]; y2 < ApetureMaxY[i]; ++y2)
						{
							int TempY = y + y2;
							if (TempY >= 0 && TempY < height)
							{
								Color TempColor = colors[TempY * width + TempX];
								RValues[i] += TempColor.r;
								GValues[i] += TempColor.g;
								BValues[i] += TempColor.b;
								if (TempColor.r > MaxRValue[i])
								{
									MaxRValue[i] = TempColor.r;
								}
								else if (TempColor.r < MinRValue[i])
								{
									MinRValue[i] = TempColor.r;
								}

								if (TempColor.g > MaxGValue[i])
								{
									MaxGValue[i] = TempColor.g;
								}
								else if (TempColor.g < MinGValue[i])
								{
									MinGValue[i] = TempColor.g;
								}

								if (TempColor.b > MaxBValue[i])
								{
									MaxBValue[i] = TempColor.b;
								}
								else if (TempColor.b < MinBValue[i])
								{
									MinBValue[i] = TempColor.b;
								}
								++NumPixels[i];
							}
						}
					}
				}
			}
			int j = 0;
			int MinDifference = 10000;
			for (int i = 0; i < 4; ++i)
			{
				int CurrentDifference = (MaxRValue[i] - MinRValue[i]) + (MaxGValue[i] - MinGValue[i]) + (MaxBValue[i] - MinBValue[i]);
				if (CurrentDifference < MinDifference && NumPixels[i] > 0)
				{
					j = i;
					MinDifference = CurrentDifference;
				}
			}

			ret[y * width + x] = Color(RValues[j] / NumPixels[j], GValues[j] / NumPixels[j], BValues[j] / NumPixels[j]);
		}
	}
	return ret;
}