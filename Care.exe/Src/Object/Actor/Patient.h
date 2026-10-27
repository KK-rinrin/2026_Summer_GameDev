#pragma once
#include "ActorBase.h"
#include <DxLib.h>

class ProcessMove;

class Patient : public ActorBase
{
public:

	static constexpr VECTOR INIT_PER = { 60.0f, 32.0f, 0.0f };

	static constexpr float COL_RADIUS = 3.0f;

	// 会話反応範囲（Player の位置がこの円に入っていて決定キーを押せば会話開始）
	static constexpr float TALK_RADIUS = 15.0f;

	Patient();

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