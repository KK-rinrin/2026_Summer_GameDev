#pragma once
#include "StageBase.h"

class NurceStation : public StageBase
{
public:
	static constexpr VECTOR PC_POS = { 35.0f,12.0f,0.0f };
	static constexpr VECTOR CHAIR_POS = { 35.0f,22.0f,0.0f };
	static constexpr VECTOR LOCKER_POS = { 92.2f,99.9f,0.0f };
	static constexpr float LOCKER_BUFFER_Y = 370.0f;

	NurceStation();
	~NurceStation();

private:
	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;

	int pcHandle_;
	int chairHandle_;
	int lockerHandle_;
};
