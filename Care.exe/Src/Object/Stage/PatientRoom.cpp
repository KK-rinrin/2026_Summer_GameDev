#include "PatientRoom.h"
#include "../../Manager/ResourceManager.h"
#include "../Actor/ActorBase.h"
#include "../Actor/Patient.h"
#include "../Collider/Collider.h"

PatientRoom::PatientRoom()
	: wallHandle_(-1)
	, toiletHandle_(-1)
{

}

PatientRoom::~PatientRoom()
{

}

StageBase::DecideResult PatientRoom::Decide(const ActorBase& controlActor, const ActorBase* patientActor) const
{
	const VECTOR actorPos = controlActor.GetTransform().pos;

	// 前ドアの位置にいて右を向いている時は、ナースステーション前ドアへ移動する
	if (Collision::IsPointInRect(actorPos, TO_NURSE_STATION_AREA1_LEFT_TOP, TO_NURSE_STATION_AREA1_RIGHT_BOTTOM) &&
		controlActor.IsFacingRight())
	{
		return { DecideType::CHANGE_STAGE, StageId::NURSE_STATION, NURSE_STATION_MOVE_POS1 };
	}

	// 後ろ側のドアは向きに関係なく、ナースステーション後ろ側へ移動する
	if (Collision::IsPointInRect(actorPos, TO_NURSE_STATION_AREA2_LEFT_TOP, TO_NURSE_STATION_AREA2_RIGHT_BOTTOM))
	{
		return { DecideType::CHANGE_STAGE, StageId::NURSE_STATION, NURSE_STATION_MOVE_POS2 };
	}

	// 患者の近くなら、現在進行度に対応した患者会話をGameScene側で選んでもらう
	if (patientActor != nullptr && &controlActor != patientActor &&
		Collision::IsPointInCircle(actorPos, patientActor->GetTransform().pos, Patient::TALK_RADIUS))
	{
		return { DecideType::PATIENT_TALK, StageId::PAT_ROOM, { 0.0f, 0.0f, 0.0f } };
	}

	return DecideResult{};
}
void PatientRoom::InitLoad()
{
	BGhandle_ = resMng_.Load(ResourceManager::SRC::BG_1).handleId_;
	wallHandle_ = resMng_.Load(ResourceManager::SRC::BG_1_WALL).handleId_;
	toiletHandle_ = resMng_.Load(ResourceManager::SRC::BG_1_TOILET).handleId_;

}

void PatientRoom::InitTransform()
{
	AddScreenObject(wallHandle_, WALL_SORT_Y, true);
	AddTransformObject(Transform2D(toiletHandle_, TOILET_POS, false));
}

void PatientRoom::InitCollider()
{
	AddMBRectPercent(WALL_BLOCK_LEFT_TOP, WALL_BLOCK_RIGHT_BOTTOM);
	AddMBRectPercent({ 75.0f, 5.0f, 0.0f }, { 87.0f, 10.0f, 0.0f });
}
