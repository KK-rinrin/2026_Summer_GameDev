#include <DxLib.h>
#include <algorithm>
#include <random>
#include <vector>
#include "ScreenEffect.h"

namespace
{
	struct ColorShiftRectState
	{
		int handle = -1;
		int width = 0;
		int height = 0;
		int rectCount = 0;
		unsigned int changeIntervalMs = 0;
		unsigned int lastChangeTime = 0;
		std::vector<RECT> rects;
		std::mt19937 randomEngine{ static_cast<unsigned int>(GetNowCount()) };
	};

	ColorShiftRectState colorShiftRectState_;

	int GetRandomRange(int minValue, int maxValue)
	{
		std::uniform_int_distribution<int> distribution(minValue, maxValue);
		return distribution(colorShiftRectState_.randomEngine);
	}

	void RefreshColorShiftRects(
		int handle,
		int width,
		int height,
		int rectCount,
		unsigned int changeIntervalMs,
		unsigned int now
	)
	{
		colorShiftRectState_.handle = handle;
		colorShiftRectState_.width = width;
		colorShiftRectState_.height = height;
		colorShiftRectState_.rectCount = rectCount;
		colorShiftRectState_.changeIntervalMs = changeIntervalMs;
		colorShiftRectState_.lastChangeTime = now;
		colorShiftRectState_.rects.clear();
		colorShiftRectState_.rects.reserve(rectCount);

		const int minRectWidth = (std::max)(1, width / 8);
		const int maxRectWidth = (std::max)(minRectWidth, width / 2);
		const int minRectHeight = (std::max)(1, height / 24);
		const int maxRectHeight = (std::max)(minRectHeight, height / 6);

		for (int i = 0; i < rectCount; ++i)
		{
			const int rectWidth = GetRandomRange(minRectWidth, maxRectWidth);
			const int rectHeight = GetRandomRange(minRectHeight, maxRectHeight);
			const int left = GetRandomRange(0, width - rectWidth);
			const int top = GetRandomRange(0, height - rectHeight);

			colorShiftRectState_.rects.push_back(
				RECT{ left, top, left + rectWidth, top + rectHeight }
			);
		}
	}
}

void ScreenEffect::DrawColorShift(
	int handle,
	float x,
	float y,
	float shiftX,
	float shiftY,
	int rectCount,
	float rectChangeSeconds
)
{
	if (handle == -1)
	{
		return;
	}

	int previousRed = 255;
	int previousGreen = 255;
	int previousBlue = 255;
	GetDrawBright(&previousRed, &previousGreen, &previousBlue);

	int previousBlendMode = DX_BLENDMODE_NOBLEND;
	int previousBlendParam = 0;
	GetDrawBlendMode(&previousBlendMode, &previousBlendParam);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBright(255, 255, 255);
	DrawGraphF(x, y, handle, TRUE);

	int width = 0;
	int height = 0;
	GetGraphSize(handle, &width, &height);
	if (width <= 0 || height <= 0 || rectCount <= 0)
	{
		SetDrawBright(previousRed, previousGreen, previousBlue);
		SetDrawBlendMode(previousBlendMode, previousBlendParam);
		return;
	}

	const unsigned int changeIntervalMs =
		static_cast<unsigned int>((std::max)(0.001f, rectChangeSeconds) * 1000.0f);
	const unsigned int now = static_cast<unsigned int>(GetNowCount());
	const bool settingsChanged =
		colorShiftRectState_.handle != handle ||
		colorShiftRectState_.width != width ||
		colorShiftRectState_.height != height ||
		colorShiftRectState_.rectCount != rectCount ||
		colorShiftRectState_.changeIntervalMs != changeIntervalMs;
	const bool changeTimeReached =
		now - colorShiftRectState_.lastChangeTime >= changeIntervalMs;

	if (colorShiftRectState_.rects.empty() || settingsChanged || changeTimeReached)
	{
		RefreshColorShiftRects(
			handle,
			width,
			height,
			rectCount,
			changeIntervalMs,
			now
		);
	}

	RECT previousDrawArea = {};
	GetDrawArea(&previousDrawArea);

	SetDrawCustomBlendMode(
		TRUE,
		DX_BLEND_ONE,
		DX_BLEND_ONE,
		DX_BLENDOP_MAX,
		DX_BLEND_ONE,
		DX_BLEND_ONE,
		DX_BLENDOP_MAX,
		255
	);

	for (const RECT& rect : colorShiftRectState_.rects)
	{
		const int left = max(static_cast<int>(previousDrawArea.left), static_cast<int>(x) + rect.left);
		const int top = max(static_cast<int>(previousDrawArea.top), static_cast<int>(y) + rect.top);
		const int right = min(static_cast<int>(previousDrawArea.right), static_cast<int>(x) + rect.right);
		const int bottom = min(static_cast<int>(previousDrawArea.bottom), static_cast<int>(y) + rect.bottom);
		if (left >= right || top >= bottom)
		{
			continue;
		}

		SetDrawArea(left, top, right, bottom);
		SetDrawBright(255, 0, 0);
		DrawGraphF(x - shiftX, y - shiftY, handle, TRUE);
		SetDrawBright(0, 0, 255);
		DrawGraphF(x + shiftX, y + shiftY, handle, TRUE);
	}

	SetDrawArea(
		previousDrawArea.left,
		previousDrawArea.top,
		previousDrawArea.right,
		previousDrawArea.bottom
	);
	SetDrawBright(previousRed, previousGreen, previousBlue);
	SetDrawCustomBlendMode(FALSE, 0, 0, 0, 0, 0, 0, 0);
	SetDrawBlendMode(previousBlendMode, previousBlendParam);
}

#pragma region LegacyColorShift
#if 0
void ScreenEffect::DrawColorShift(int handle, float x, float y, float shiftX, float shiftY)
{
	if (handle == -1)
	{
		return;
	}

	int previousRed = 255;
	int previousGreen = 255;
	int previousBlue = 255;
	GetDrawBright(&previousRed, &previousGreen, &previousBlue);

	int previousBlendMode = DX_BLENDMODE_NOBLEND;
	int previousBlendParam = 0;
	GetDrawBlendMode(&previousBlendMode, &previousBlendParam);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	DrawColorChannel(handle, x - shiftX, y - shiftY, 255, 0, 0);
	DrawColorChannel(handle, x, y, 0, 255, 0);
	DrawColorChannel(handle, x + shiftX, y + shiftY, 0, 0, 255);

	SetDrawBright(previousRed, previousGreen, previousBlue);
	SetDrawBlendMode(previousBlendMode, previousBlendParam);
}

void ScreenEffect::DrawColorChannel(int handle, float x, float y, int red, int green, int blue)
{
	SetDrawBright(red, green, blue);
	DrawGraphF(x, y, handle, TRUE);
}
#endif
#pragma endregion
