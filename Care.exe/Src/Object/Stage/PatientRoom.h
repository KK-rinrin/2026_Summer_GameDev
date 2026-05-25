#pragma once
#include "StageBase.h"

class PatientRoom : public StageBase
{
public:
	PatientRoom();
	~PatientRoom() override;

private:
	static constexpr float WALL_SORT_Y = 400.0f;
	static constexpr float WALL_THICKNESS = 40.0f;

	void InitTransform() override;
	void InitLoad() override;

	void UpdateProcess() override;

	int wallHandle_;
	int toiletHandle_;
};
