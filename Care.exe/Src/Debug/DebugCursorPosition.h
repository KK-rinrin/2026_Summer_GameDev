#pragma once
#include "../Application.h"
#include <DxLib.h>

class DebugCursorPosition
{
public:
	static constexpr int DEBUG_TEXT_X = 0;
	static constexpr int DEBUG_TEXT_Y = 0;
	static constexpr int DEBUG_TEXT_COLOR = Application::DEFAULT_COLOR_0;

	DebugCursorPosition();
	~DebugCursorPosition();

	void Update();
	void Draw() const;
	void DrawCrossLine() const;

private:
	VECTOR worldPos_;
	VECTOR localPercent_;
};
