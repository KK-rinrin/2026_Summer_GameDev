#include "ProcessMove.h"
#include "../Common/2DTransform.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/KeyConfig.h"

ProcessMove::ProcessMove()
	:
	moveSpeedPercent_(DEFAULT_MOVE_SPEED_PERCENT),
	useDelay_(false),
	minDelayFrame_(DEFAULT_DELAY_FRAME),
	maxDelayFrame_(DEFAULT_DELAY_FRAME),
	delaySeed_(1u),
	delayedInputs_()
{
}

void ProcessMove::SetMoveSpeedPercent(float moveSpeedPercent)
{
	moveSpeedPercent_ = moveSpeedPercent;
}

float ProcessMove::GetMoveSpeedPercent(void) const
{
	return moveSpeedPercent_;
}

void ProcessMove::SetDelayFrameRange(int minDelayFrame, int maxDelayFrame)
{
	const int smallerDelayFrame = (minDelayFrame < maxDelayFrame) ? minDelayFrame : maxDelayFrame;
	const int largerDelayFrame = (minDelayFrame < maxDelayFrame) ? maxDelayFrame : minDelayFrame;
	minDelayFrame_ = (smallerDelayFrame < 0) ? 0 : smallerDelayFrame;
	maxDelayFrame_ = (largerDelayFrame < 0) ? 0 : largerDelayFrame;
	useDelay_ = maxDelayFrame_ > 0;
}

void ProcessMove::ClearDelay(void)
{
	useDelay_ = false;
	minDelayFrame_ = DEFAULT_DELAY_FRAME;
	maxDelayFrame_ = DEFAULT_DELAY_FRAME;
	delayedInputs_.clear();
}

void ProcessMove::Reset(void)
{
	delayedInputs_.clear();
	delaySeed_ = 1u;
}

void ProcessMove::Update(Transform2D& transform)
{
	const MoveInput input = ReadInput();
	if (useDelay_)
	{
		UpdateDelayed(transform, input);
		return;
	}

	UpdateImmediate(transform, input);
}

ProcessMove::MoveInput ProcessMove::ReadInput(void) const
{
	MoveInput input = { 0.0f, 0.0f, DEFAULT_DELAY_FRAME };
	const InputManager& inputManager = InputManager::GetInstance();
	const VECTOR stickInput = inputManager.GetLeftStickInput(InputManager::JOYPAD_NO::PAD1);

	input.x += stickInput.x * moveSpeedPercent_;
	input.y += stickInput.y * moveSpeedPercent_;

	if (KeyConfig::IsNew(KeyConfig::ACTION::MOVE_RIGHT, inputManager))
	{
		input.x += moveSpeedPercent_;
	}
	if (KeyConfig::IsNew(KeyConfig::ACTION::MOVE_LEFT, inputManager))
	{
		input.x -= moveSpeedPercent_;
	}
	if (KeyConfig::IsNew(KeyConfig::ACTION::MOVE_UP, inputManager))
	{
		input.y -= moveSpeedPercent_;
	}
	if (KeyConfig::IsNew(KeyConfig::ACTION::MOVE_DOWN, inputManager))
	{
		input.y += moveSpeedPercent_;
	}

	return input;
}

void ProcessMove::ApplyMove(Transform2D& transform, const MoveInput& input) const
{
	transform.pos.x += input.x;
	transform.pos.y += input.y;
}

void ProcessMove::UpdateImmediate(Transform2D& transform, const MoveInput& input) const
{
	ApplyMove(transform, input);
}

void ProcessMove::UpdateDelayed(Transform2D& transform, const MoveInput& input)
{
	if (input.x != 0.0f || input.y != 0.0f)
	{
		MoveInput delayedInput = input;
		delayedInput.waitFrame = CreateDelayFrame();
		delayedInputs_.push_back(delayedInput);
	}

	for (auto it = delayedInputs_.begin(); it != delayedInputs_.end();)
	{
		--it->waitFrame;
		if (it->waitFrame <= 0)
		{
			ApplyMove(transform, *it);
			it = delayedInputs_.erase(it);
			continue;
		}

		++it;
	}
}

int ProcessMove::CreateDelayFrame(void)
{
	if (maxDelayFrame_ <= minDelayFrame_)
	{
		return minDelayFrame_;
	}

	delaySeed_ = delaySeed_ * DELAY_RANDOM_A + DELAY_RANDOM_C;
	const int delayRange = maxDelayFrame_ - minDelayFrame_ + 1;
	return minDelayFrame_ + static_cast<int>(delaySeed_ % static_cast<unsigned int>(delayRange));
}
