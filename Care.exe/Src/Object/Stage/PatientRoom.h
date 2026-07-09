#pragma once
#include "StageBase.h"

class PatientRoom : public StageBase
{
public:
	PatientRoom();
	~PatientRoom() override;

	void DrawGuide(const ActorBase& controlActor) const override;
	void Decide(DecideContext& context) const override;

private:
	static constexpr float WALL_SORT_Y = 400.0f;
	static constexpr VECTOR WALL_BLOCK_LEFT_TOP = { 0.0f,51.0f,0.0f };
	static constexpr VECTOR WALL_BLOCK_RIGHT_BOTTOM = { 100.0f,62.0f,0.0f };

	static constexpr VECTOR TOILET_POS = { 80.0f,10.0f,0.0f };

	// 患者部屋からナースステーションへ移動するドア判定。
	static constexpr VECTOR TO_NURSE_STATION_AREA1_LEFT_TOP = { 95.0f, 55.0f, 0.0f };
	static constexpr VECTOR TO_NURSE_STATION_AREA1_RIGHT_BOTTOM = { 100.0f, 90.0f, 0.0f };
	static constexpr VECTOR NURSE_STATION_MOVE_POS1 = { 1.0f, 90.0f, 0.0f };

	static constexpr VECTOR TO_NURSE_STATION_AREA2_LEFT_TOP = { 3.0f, 0.0f, 0.0f };
	static constexpr VECTOR TO_NURSE_STATION_AREA2_RIGHT_BOTTOM = { 30.0f, 5.0f, 0.0f };
	static constexpr VECTOR NURSE_STATION_MOVE_POS2 = { 85.0f, 0.0f, 0.0f };

	static constexpr Vector2 GUIDE_TO_NURSE_STATION_TOP_POS = { 285, 0 };
	static constexpr Vector2 GUIDE_TO_NURSE_STATION_RIGHT_POS = { 730, 180 };

	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;

	int wallHandle_;
	int toiletHandle_;
};
