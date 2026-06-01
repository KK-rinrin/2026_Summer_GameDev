#pragma once
#include "ActorBase.h"
class ProcessMove;

class Patient : public ActorBase
{
public:
	static constexpr float INIT_MOVE_SPEED_PER = 0.6f;
	static constexpr float INIT_PER_X = 60.0f;
	static constexpr float INIT_PER_Y = 32.0f;

	static constexpr float COL_RADIUS = 3.0f;

	// 会話反応範囲（Player の位置がこの円に入っていて決定キーを押せば会話開始）
	static constexpr float TALK_RADIUS = 15.0f;

	Patient();
	~Patient();

	void Update(void) override;

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