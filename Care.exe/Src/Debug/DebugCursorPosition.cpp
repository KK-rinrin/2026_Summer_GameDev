#include "DebugCursorPosition.h"
#include "../Object/Common/2DTransform.h"
#include "../Utility/SchoolUtility.h"

DebugCursorPosition::DebugCursorPosition()
	: worldPos_(SchoolUtility::VECTOR_ZERO)
	, localPercent_(SchoolUtility::VECTOR_ZERO)
{
}

DebugCursorPosition::~DebugCursorPosition()
{
}

void DebugCursorPosition::Update()
{
	int mouseX = 0;
	int mouseY = 0;
	GetMousePoint(&mouseX, &mouseY);

	worldPos_.x = static_cast<float>(mouseX);
	worldPos_.y = static_cast<float>(mouseY);
	worldPos_.z = 0.0f;

	localPercent_ = Transform2D::WorldToLocalPercent(worldPos_);
}

void DebugCursorPosition::Draw() const
{
	DrawFormatString(DEBUG_TEXT_X, DEBUG_TEXT_Y, DEBUG_TEXT_COLOR,
		"World : X %.1f  Y %.1f\nLocal%%: X %.2f  Y %.2f",
		worldPos_.x, worldPos_.y,
		localPercent_.x, localPercent_.y);
}

void DebugCursorPosition::DrawCrossLine() const
{
	int screenWidth = 0;
	int screenHeight = 0;
	GetDrawScreenSize(&screenWidth, &screenHeight);

	const int cursorX = static_cast<int>(worldPos_.x);
	const int cursorY = static_cast<int>(worldPos_.y);
	DrawLine(0, cursorY, screenWidth, cursorY, DEBUG_TEXT_COLOR);
	DrawLine(cursorX, 0, cursorX, screenHeight, DEBUG_TEXT_COLOR);
}
