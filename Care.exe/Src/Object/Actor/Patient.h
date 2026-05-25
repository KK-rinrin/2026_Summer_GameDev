#pragma once
#include "ActorBase.h"

class Patient : public ActorBase
{
public:
	static constexpr float INIT_PER_X = 40.0f;
	static constexpr float INIT_PER_Y = 40.0f;

	static constexpr float COL_RADIUS = 3.0f;

	// 会話反応範囲（Player の位置がこの円に入っていて決定キーを押せば会話開始）
	static constexpr float TALK_RADIUS = 30.0f;

	Patient();
	~Patient();

	void Update(void) override;

private:
	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;
	void InitAnimation() override;
	void InitPost() override;

	void UpdateAnimation() override;
};