#pragma once
#include "ActorBase.h"
#include <DxLib.h>

class ProcessMove;

class Player : public ActorBase
{
public:

	static constexpr float INIT_MOVE_SPEED_PER = 0.6f;
	static constexpr VECTOR INIT_PER = { 50.0f, 50.0f, 0.0f };

	static constexpr float COL_RADIUS = 3.0f;

	Player();
	~Player();

	void Update() override;
	void Draw() override;
	void SetLocalPercent(float x, float y);
	bool IsFacingRight() const;
	ProcessMove* GetProcessMove();

	void Release() override;

private:

	static constexpr float MOVEMENT_EPSILON = 0.1f;
	static constexpr int DIRECTION_COUNT = 8;
	static constexpr float SECTOR_ANGLE_DEGREES = 45.0f;
	static constexpr float SECTOR_HALF_ANGLE_DEGREES = 22.5f;
	enum Direction : int { DOWN, DOWN_RIGHT, RIGHT, UP_RIGHT, UP, UP_LEFT, LEFT, DOWN_LEFT };
	static constexpr int SECTOR_TO_ROT[DIRECTION_COUNT] = { RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT };
	static constexpr int LEGACY_DIRECTION_SHEET_ROWS = 4;
	static constexpr int LEGACY_FRONT_DIRECTION = 0;
	static constexpr int LEGACY_FRONT_FLIPPED_DIRECTION = 3;
	static constexpr int LEGACY_BACK_FLIPPED_DIRECTION = 2;

	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;
	void InitAnimation() override;
	void InitPost() override;

	void UpdateAnimation() override;

	ProcessMove* processMove_;
};
