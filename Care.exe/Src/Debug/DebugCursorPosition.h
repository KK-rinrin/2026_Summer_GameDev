#pragma once
#include <DxLib.h>

class DebugCursorPosition
{
public:
	static constexpr int DEBUG_TEXT_X = 0;
	static constexpr int DEBUG_TEXT_Y = 0;
	static constexpr int DEBUG_TEXT_COLOR = 0x000000;

	DebugCursorPosition();
	~DebugCursorPosition();

	void Update();
	void Draw() const;

private:
	VECTOR worldPos_;
	VECTOR localPercent_;
};
