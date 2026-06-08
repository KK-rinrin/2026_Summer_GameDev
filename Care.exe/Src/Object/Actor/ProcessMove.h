#pragma once
#include <deque>

class Transform2D;

class ProcessMove
{
public:
	ProcessMove();

	void SetMoveSpeedPercent(float moveSpeedPercent);
	float GetMoveSpeedPercent(void) const;

	void SetDelayFrameRange(int minDelayFrame, int maxDelayFrame);
	void ClearDelay(void);
	void Reset(void);

	void Update(Transform2D& transform);

private:
	static constexpr float DEFAULT_MOVE_SPEED_PERCENT = 0.0f;
	static constexpr int DEFAULT_DELAY_FRAME = 0;
	static constexpr unsigned int DELAY_RANDOM_A = 1103515245u;
	static constexpr unsigned int DELAY_RANDOM_C = 12345u;

	struct MoveInput
	{
		float x;
		float y;
		int waitFrame;
	};

	MoveInput ReadInput(void) const;
	void ApplyMove(Transform2D& transform, const MoveInput& input) const;
	void UpdateImmediate(Transform2D& transform, const MoveInput& input) const;
	void UpdateDelayed(Transform2D& transform, const MoveInput& input);
	int CreateDelayFrame(void);

	float moveSpeedPercent_;
	bool useDelay_;
	int minDelayFrame_;
	int maxDelayFrame_;
	unsigned int delaySeed_;
	std::deque<MoveInput> delayedInputs_;
};