#pragma once
#include "ActorBase.h"
#include <DxLib.h>

class ProcessMove;

class Player : public ActorBase
{
public:

	// 初期座標
	static constexpr VECTOR INIT_PER = { 50.0f, 50.0f, 0.0f };

	// 当たり判定（半径）
	static constexpr float COL_RADIUS = 3.0f;

	Player();

	void Draw() override;
	void SetLocalPercent(float x, float y);	// 百分率で指定した座標に移動
	bool IsFacingRight() const;		// 右を向いているか？

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
