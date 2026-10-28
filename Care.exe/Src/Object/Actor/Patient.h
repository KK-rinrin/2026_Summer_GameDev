#pragma once
#include "ActorBase.h"
#include <DxLib.h>

class ProcessMove;

class Patient : public ActorBase
{
public:

	// 初期座標
	static constexpr VECTOR INIT_PER = { 60.0f, 32.0f, 0.0f };

	// 当たり判定（半径）
	static constexpr float COL_RADIUS = 3.0f;

	// 会話反応範囲（Player の位置がこの円に入っていて決定キーを押せば会話開始）
	static constexpr float TALK_RADIUS = 15.0f;

	// コンストラクタ
	Patient();

	// 開放
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