#include "NurceStation.h"
#include "../../Manager/ResourceManager.h"
#include "../Actor/ActorBase.h"
#include "../Collider/Collider.h"

NurceStation::NurceStation()
	: pcHandle_(-1)
	, chairHandle_(-1)
	, lockerHandle_(-1)
{
}

NurceStation::~NurceStation()
{
}

StageBase::DecideResult NurceStation::Decide(const ActorBase& controlActor, const ActorBase* patientActor) const
{
	(void)patientActor;
	const VECTOR actorPos = controlActor.GetTransform().pos;

	// 前ドアの位置にいて左を向いている時は、患者部屋前ドアへ移動する
	if (Collision::IsPointInRect(actorPos, TO_PATIENT_ROOM_AREA1_LEFT_TOP, TO_PATIENT_ROOM_AREA1_RIGHT_BOTTOM) &&
		!controlActor.IsFacingRight())
	{
		return { DecideType::CHANGE_STAGE, StageId::PAT_ROOM, PATIENT_ROOM_MOVE_POS1 };
	}

	// 後ろ側のドアは向きに関係なく、患者部屋後ろ側へ移動する
	if (Collision::IsPointInRect(actorPos, TO_PATIENT_ROOM_AREA2_LEFT_TOP, TO_PATIENT_ROOM_AREA2_RIGHT_BOTTOM))
	{
		return { DecideType::CHANGE_STAGE, StageId::PAT_ROOM, PATIENT_ROOM_MOVE_POS2 };
	}

	// PC机の近くなら、進行度に応じたPCイベントをGameScene側で選んでもらう
	if (Collision::IsPointInRect(actorPos, PC_LEFTUP, PC_RIGHTDOWN))
	{
		return { DecideType::PC, StageId::NURSE_STATION, { 0.0f, 0.0f, 0.0f } };
	}

	return DecideResult{};
}
void NurceStation::InitLoad()
{
	BGhandle_ = resMng_.Load(ResourceManager::SRC::BG_2).handleId_;

	pcHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_PC).handleId_;

	chairHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_CHAIR).handleId_;

	lockerHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_LOCKER).handleId_;
}

void NurceStation::InitTransform()
{
	AddTransformObject(Transform2D(pcHandle_, PC_POS, false));

	AddTransformObject(Transform2D(chairHandle_, CHAIR_POS, false));

	AddTransformObject(Transform2D(lockerHandle_, LOCKER_POS, false), LOCKER_BUFFER_Y);

}

void NurceStation::InitCollider()
{
	AddMBRectPercent({ 0.0f, 0.0f, 0.0f }, { 69.0f, 15.0f, 0.0f });	// PC周辺

	// ロッカー周辺
	AddMBRectPercent({ 83.0f, 40.0f, 0.0f }, { 100.0f, 100.0f, 0.0f });
}
