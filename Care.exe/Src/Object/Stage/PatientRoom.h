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

	static constexpr VECTOR TOILET_POS = { 80.0f,10.0f,0.0f };

	void InitTransform() override;
	void InitLoad() override;

	void UpdateProcess() override;

	int wallHandle_;
	int toiletHandle_;
};
