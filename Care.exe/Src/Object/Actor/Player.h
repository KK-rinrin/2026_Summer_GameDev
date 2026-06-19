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

	void Update(void) override;
	void SetLocalPercent(float x, float y);
	bool IsFacingRight() const;
	ProcessMove* GetProcessMove(void);

	void Release(void) override;

private:
	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;
	void InitAnimation() override;
	void InitPost() override;

	void UpdateAnimation() override;

	ProcessMove* processMove_;
};
