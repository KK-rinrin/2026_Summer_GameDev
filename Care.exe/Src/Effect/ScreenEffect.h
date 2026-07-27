#pragma once

class ScreenEffect
{
public:
	static constexpr float DEFAULT_COLOR_SHIFT_PIXELS = 1.0f;
	static constexpr int DEFAULT_COLOR_SHIFT_RECT_COUNT = 8;
	static constexpr float DEFAULT_COLOR_SHIFT_RECT_CHANGE_SECONDS = 0.08f;

	static void DrawColorShift(
		int handle,
		float x,
		float y,
		float shiftX = DEFAULT_COLOR_SHIFT_PIXELS,
		float shiftY = 0.0f,
		int rectCount = DEFAULT_COLOR_SHIFT_RECT_COUNT,
		float rectChangeSeconds = DEFAULT_COLOR_SHIFT_RECT_CHANGE_SECONDS
	);
};
