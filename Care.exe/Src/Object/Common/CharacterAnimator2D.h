#pragma once
#include "AnimationController2D.h"
class Sprite2D;

class CharacterAnimator2D
{
public:
	void Configure(int framesPerDirection, int stillIndex, float frameSeconds = 0.0f);
	void SetPingPong(bool enabled) { animation_.SetPingPong(enabled); }
	void ShowStill(Sprite2D& sprite);
	void Update(Sprite2D& sprite, const VECTOR& movement, float worldX);

private:
	// 8ï˚å¸
	enum Direction : int
	{
		DOWN,
		DOWN_RIGHT,
		RIGHT,
		UP_RIGHT,
		UP, 
		UP_LEFT,
		LEFT,
		DOWN_LEFT
	};

	static constexpr float RAD_TO_DEG = 180.0f / 3.14159265f;
	static constexpr float MOVEMENT_EPSILON = 0.1f;
	static constexpr int DIRECTION_COUNT = 8;
	static constexpr float SECTOR_ANGLE_DEGREES = 45.0f;
	static constexpr float SECTOR_HALF_ANGLE_DEGREES = 22.5f;

	static constexpr int SECTOR_TO_ROT[DIRECTION_COUNT] =
	{ RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT };
	static constexpr int LEGACY_DIRECTION_SHEET_ROWS = 4;
	static constexpr int LEGACY_FRONT_DIRECTION = 0;
	static constexpr int LEGACY_FRONT_FLIPPED_DIRECTION = 3;
	static constexpr int LEGACY_BACK_FLIPPED_DIRECTION = 2;

	AnimationController2D animation_;
	int stillIndex_ = 0;
	int frameBaseIndex_ = 0; // ç≈å„Ç…ï\é¶ÇµÇΩå¸Ç´ÇÃêÊì™ÉtÉåÅ[ÉÄ
	int direction_ = 0;
};
