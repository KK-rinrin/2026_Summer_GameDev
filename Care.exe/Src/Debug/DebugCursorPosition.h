#pragma once
#include "../Common/Vector2.h"
#include "../Application.h"
#include <DxLib.h>

class DebugCursorPosition
{
public:

	// デバッグ座標表示のX位置
	static constexpr int DEBUG_TEXT_X = 0;

	// デバッグ座標表示のY位置
	static constexpr int DEBUG_TEXT_Y = 0;

	// デバッグ文字と十字線の色
	static constexpr int DEBUG_TEXT_COLOR = Application::DEFAULT_COLOR_0;

	DebugCursorPosition();
	~DebugCursorPosition();

	void Update();
	void Draw() const;

	// カーソル位置を通る水平線と垂直線の描画
	void DrawCrossLine() const;

private:

	// カーソルの画面座標
	Vector2<float> worldPos_;

	// カーソルのステージ内百分率座標
	Vector2<float> localPercent_;
};
