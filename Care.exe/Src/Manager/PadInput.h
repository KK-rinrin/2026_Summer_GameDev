#pragma once
#include "InputManager.h"

class PadInput
{
public:
	static VECTOR GetMoveAxis(
		const InputManager& input,
		InputManager::JOYPAD_NO no = InputManager::JOYPAD_NO::PAD1);
	static VECTOR GetDirectionXZA(int axisX, int axisY);

private:
	static constexpr float AXIS_VALUE_MAX = 1000.0f;
	static constexpr float DEAD_ZONE_THRESHOLD = 0.35f;
};