#pragma once
#include "InputManager.h"

class PadInput
{
public:

	// パッドの左スティックと方向キーによる移動軸の取得
	static VECTOR GetMoveAxis(
		const InputManager& input,
		InputManager::JOYPAD_NO no = InputManager::JOYPAD_NO::PAD1);

	// スティック生入力からデッドゾーン適用済みXZ方向の取得
	static VECTOR GetDirectionXZA(int axisX, int axisY);

private:

	// スティック入力の最大絶対値
	static constexpr float AXIS_VALUE_MAX = 1000.0f;

	// スティック入力を無効とするデッドゾーンの閾値
	static constexpr float DEAD_ZONE_THRESHOLD = 0.35f;
};
