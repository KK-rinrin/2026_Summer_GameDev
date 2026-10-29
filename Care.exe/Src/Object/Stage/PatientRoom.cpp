#include "PatientRoom.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/ProgressManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Object/Talk/Talk.h"
#include "../../Manager/Csv/ProgressTable.h"
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

void PatientRoom::DrawGuide(const ActorBase& controlActor) const
{
	const Vector2<float> actorPos = controlActor.GetTransform().pos;

	if (Collision::IsPointInRect(actorPos, TO_NURSE_STATION_AREA1_LEFT_TOP, TO_NURSE_STATION_AREA1_RIGHT_BOTTOM) &&
		controlActor.IsFacingRight())
	{
		DrawMoveGuide(GUIDE_TO_NURSE_STATION_RIGHT_POS);
		return;
	}

	if (Collision::IsPointInRect(actorPos, TO_NURSE_STATION_AREA2_LEFT_TOP, TO_NURSE_STATION_AREA2_RIGHT_BOTTOM))
	{
		DrawMoveGuide(GUIDE_TO_NURSE_STATION_TOP_POS);
		return;
	}
}
void PatientRoom::Decide(DecideContext& context) const
{
	const Vector2<float> actorPos = context.controlActor.GetTransform().pos;

	// 前ドアの位置にいて右を向いている時は、ナースステーション前ドアへ移動する
	if (Collision::IsPointInRect(actorPos, TO_NURSE_STATION_AREA1_LEFT_TOP, TO_NURSE_STATION_AREA1_RIGHT_BOTTOM) &&
		context.controlActor.IsFacingRight())
	{
		context.soundManager.PlaySE(SE::DOOR);
		context.ChangeStage(StageId::NURSE_STATION, NURSE_STATION_MOVE_POS1);
		return;
	}

	// 後ろ側のドアは向きに関係なく、ナースステーション後ろ側へ移動する
	if (Collision::IsPointInRect(actorPos, TO_NURSE_STATION_AREA2_LEFT_TOP, TO_NURSE_STATION_AREA2_RIGHT_BOTTOM))
	{
		context.soundManager.PlaySE(SE::DOOR);
		context.ChangeStage(StageId::NURSE_STATION, NURSE_STATION_MOVE_POS2);
		return;
	}

	// 患者の近くなら、現在進行度に対応した患者会話を開始する
	if (context.patientActor != nullptr && &context.controlActor != context.patientActor &&
		Collision::IsPointInCircle(actorPos, context.patientActor->GetTransform().pos, Patient::TALK_RADIUS))
	{
		const ProgressData& progressData = ProgressTable::Get(context.progressManager.GetProgressEnum());
		if (progressData.patientTalk != TDI::NONE)
		{
			context.talk.SetTalk(progressData.patientTalk);
			return;
		}
	}

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
	AddTransformObject(Transform2D(TOILET_POS), Sprite2D(toiletHandle_, false));
}

void PatientRoom::InitCollider()
{
	AddMBRectPercent(WALL_BLOCK_LEFT_TOP, WALL_BLOCK_RIGHT_BOTTOM);
	AddMBRectPercent(TOILET_BLOCK_LEFT_TOP, TOILET_BLOCK_RIGHT_BOTTOM);
}
