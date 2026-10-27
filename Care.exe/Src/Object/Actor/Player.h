#pragma once
#include "ActorBase.h"
#include <DxLib.h>

class ProcessMove;

class Player : public ActorBase
{
public:

	static constexpr VECTOR INIT_PER = { 50.0f, 50.0f, 0.0f };

	static constexpr float COL_RADIUS = 3.0f;

	Player();

	void Draw() override;
	void SetLocalPercent(float x, float y);
	bool IsFacingRight() const;
	ProcessMove* GetProcessMove();

	void Release() override;

private:

	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;
	void InitAnimation() override;
	void InitPost() override;

	void UpdateProcessMove() override;
	void UpdateAnimation() override;
};
