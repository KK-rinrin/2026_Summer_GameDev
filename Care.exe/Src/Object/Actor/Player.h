#pragma once
#include "ActorBase.h"
#include <DxLib.h>

class Player : public ActorBase
{
public:
	static constexpr float INIT_MOVE_SPEED_PER = 0.6f;
	static constexpr float INIT_PER_X = 50.0f;
	static constexpr float INIT_PER_Y = 50.0f;

	static constexpr float COL_RADIUS = 3.0f;

	Player();
	~Player();

	void Update(void) override;
	void SetLocalPercent(float x, float y);
	bool IsFacingRight() const;

private:
	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;
	void InitAnimation() override;
	void InitPost() override;

	void UpdateAnimation() override;

	float moveSpeedPercent; // フレームあたりの移動量（%）
};
