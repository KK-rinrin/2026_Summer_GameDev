#pragma once
#include "StageBase.h"

class PatientRoom : public StageBase
{
public:
	PatientRoom();
	~PatientRoom() override;

private:
	static constexpr float WALL_SORT_Y = 400.0f;
	static constexpr VECTOR WALL_BLOCK_LEFT_TOP = { 0.0f,51.0f,0.0f };
	static constexpr VECTOR WALL_BLOCK_RIGHT_BOTTOM = { 100.0f,62.0f,0.0f };

	static constexpr VECTOR TOILET_POS = { 80.0f,10.0f,0.0f };

	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;

	int wallHandle_;
	int toiletHandle_;
};
