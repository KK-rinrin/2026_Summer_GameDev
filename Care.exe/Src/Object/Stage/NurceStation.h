#pragma once
#include "StageBase.h"

class PCScene;

class NurceStation : public StageBase
{
public:
	static constexpr VECTOR PC_POS = { 35.0f,12.0f,0.0f };
	static constexpr VECTOR CHAIR_POS = { 35.0f,22.0f,0.0f };
	static constexpr VECTOR LOCKER_POS = { 92.2f,99.9f,0.0f };
	static constexpr float LOCKER_BUFFER_Y = 370.0f;

	// ナースステーションから患者部屋へ移動するドア判定
	static constexpr VECTOR TO_PATIENT_ROOM_AREA1_LEFT_TOP = { 0.0f, 55.0f, 0.0f };
	static constexpr VECTOR TO_PATIENT_ROOM_AREA1_RIGHT_BOTTOM = { 5.0f, 90.0f, 0.0f };
	static constexpr VECTOR PATIENT_ROOM_MOVE_POS1 = { 98.0f, 90.0f, 0.0f };

	static constexpr VECTOR TO_PATIENT_ROOM_AREA2_LEFT_TOP = { 80.0f, 0.0f, 0.0f };
	static constexpr VECTOR TO_PATIENT_ROOM_AREA2_RIGHT_BOTTOM = { 95.0f, 5.0f, 0.0f };
	static constexpr VECTOR PATIENT_ROOM_MOVE_POS2 = { 15.0f, 0.0f, 0.0f };

	static constexpr VECTOR PC_LEFTUP = { 25.0f,15.0f,0.0f };		// PC起動判定範囲左上
	static constexpr VECTOR PC_RIGHTDOWN = { 50.0f,30.0f,0.0f };	// PC起動判定範囲右下

	static constexpr Vector2 GUIDE_TO_PATIENT_ROOM_TOP_POS = { 590, 0 };
	static constexpr Vector2 GUIDE_TO_PATIENT_ROOM_LEFT_POS = { 28, 168 };

	NurceStation();
	~NurceStation();

	void Update() override;
	void DrawForeground() const override;
	void Delete() override;
	bool IsInputBlocked() const override;
	void DrawGuide(const ActorBase& controlActor) const override;
	void Decide(DecideContext& context) const override;

private:
	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;

	int pcHandle_;
	int chairHandle_;
	int lockerHandle_;
	PCScene* pcScene_;
};
