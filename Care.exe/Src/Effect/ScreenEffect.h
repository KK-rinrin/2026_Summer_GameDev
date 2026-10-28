#pragma once

class ScreenEffect
{
public:

	// 色ずれ量の既定ピクセル数
	static constexpr float DEFAULT_COLOR_SHIFT_PIXELS = 1.0f;

	// 色ずれ領域の既定矩形数
	static constexpr int DEFAULT_COLOR_SHIFT_RECT_COUNT = 8;

	// 色ずれ矩形を更新する既定間隔の秒数
	static constexpr float DEFAULT_COLOR_SHIFT_RECT_CHANGE_SECONDS = 0.08f;

	// 画像の矩形領域ごとに赤と青をずらす描画
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

	// 矩形の最小幅を求める画像幅の除数
	static constexpr int RECT_MIN_WIDTH_DIVISOR = 8;

	// 矩形の最大幅を求める画像幅の除数
	static constexpr int RECT_MAX_WIDTH_DIVISOR = 2;

	// 矩形の最小高さを求める画像高さの除数
	static constexpr int RECT_MIN_HEIGHT_DIVISOR = 24;

	// 矩形の最大高さを求める画像高さの除数
	static constexpr int RECT_MAX_HEIGHT_DIVISOR = 6;

	// 色成分の最大輝度
	static constexpr int MAX_BRIGHTNESS = 255;

	// 矩形更新間隔の最小秒数
	static constexpr float MIN_CHANGE_SECONDS = 0.001f;

	// 秒からミリ秒への換算倍率
	static constexpr float MILLISECONDS_PER_SECOND = 1000.0f;

	// 色ずれ領域のランダムな矩形配置の更新
	static void RefreshColorShiftRects(int handle, int width, int height,
		int rectCount, unsigned int changeIntervalMs, unsigned int now);

};
