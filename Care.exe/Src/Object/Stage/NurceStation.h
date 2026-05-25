#pragma once
#include "StageBase.h"

class NurceStation : public StageBase
{
public:
	static constexpr VECTOR PC_POS = { 30.0f,10.0f,0.0f };
	static constexpr VECTOR CHAIR_POS = { 30.0f,20.0f,0.0f };
	static constexpr VECTOR LOCKER_POS = { 90.0f,100.0f,0.0f };

	NurceStation();
	~NurceStation();

private:
	void InitLoad() override;

	int pcHandle_;
	int chairHandle_;
	int lockerHandle_;
};
