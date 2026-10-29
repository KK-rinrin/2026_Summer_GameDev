#include <cmath>
#include "PadInput.h"

Vector2<float> PadInput::GetMoveAxis(const InputManager& input, InputManager::JOYPAD_NO no)
{
	const InputManager::JOYPAD_IN_STATE& state = input.GetPadInputState(no);
	float axisX = static_cast<float>(state.AKeyLX) / AXIS_VALUE_MAX;
	float axisY = static_cast<float>(state.AKeyLY) / AXIS_VALUE_MAX;
	const float length = std::sqrt(axisX * axisX + axisY * axisY);

	if (length < DEAD_ZONE_THRESHOLD)
	{
		return { 0.0f, 0.0f };
	}

	const float scale = (length - DEAD_ZONE_THRESHOLD) / (1.0f - DEAD_ZONE_THRESHOLD);
	axisX = (axisX / length) * scale;
	axisY = (axisY / length) * scale;

	return { axisX, axisY };
}

