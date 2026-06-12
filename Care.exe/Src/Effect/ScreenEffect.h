#pragma once

class ScreenEffect
{
public:
	static constexpr float DEFAULT_COLOR_SHIFT_PIXELS = 1.0f;

	static void DrawColorShift(
		int handle,
		float x,
		float y,
		float shiftX = DEFAULT_COLOR_SHIFT_PIXELS,
		float shiftY = 0.0f
	);

private:
	static void DrawColorChannel(int handle, float x, float y, int red, int green, int blue);
};
