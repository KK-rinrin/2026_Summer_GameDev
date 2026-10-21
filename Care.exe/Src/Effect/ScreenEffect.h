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
private:
	static constexpr int RECT_MIN_WIDTH_DIVISOR = 8;
	static constexpr int RECT_MAX_WIDTH_DIVISOR = 2;
	static constexpr int RECT_MIN_HEIGHT_DIVISOR = 24;
	static constexpr int RECT_MAX_HEIGHT_DIVISOR = 6;
	static constexpr int MAX_BRIGHTNESS = 255;
	static constexpr float MIN_CHANGE_SECONDS = 0.001f;
	static constexpr float MILLISECONDS_PER_SECOND = 1000.0f;
	static void RefreshColorShiftRects(int handle, int width, int height,
		int rectCount, unsigned int changeIntervalMs, unsigned int now);

};
