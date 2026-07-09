#include "NurceStation.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/ProgressManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Object/Talk/Talk.h"
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

void NurceStation::DrawGuide(const ActorBase& controlActor) const
{
	const VECTOR actorPos = controlActor.GetTransform().pos;

	if (Collision::IsPointInRect(actorPos, TO_PATIENT_ROOM_AREA1_LEFT_TOP, TO_PATIENT_ROOM_AREA1_RIGHT_BOTTOM) &&
		!controlActor.IsFacingRight())
	{
		DrawMoveGuide(GUIDE_TO_PATIENT_ROOM_LEFT_POS);
		return;
	}

	if (Collision::IsPointInRect(actorPos, TO_PATIENT_ROOM_AREA2_LEFT_TOP, TO_PATIENT_ROOM_AREA2_RIGHT_BOTTOM))
	{
		DrawMoveGuide(GUIDE_TO_PATIENT_ROOM_TOP_POS);
		return;
	}
}
void NurceStation::Decide(DecideContext& context) const
{
	const VECTOR actorPos = context.controlActor.GetTransform().pos;

	// 前ドアの位置にいて左を向いている時は、患者部屋前ドアへ移動する
	if (Collision::IsPointInRect(actorPos, TO_PATIENT_ROOM_AREA1_LEFT_TOP, TO_PATIENT_ROOM_AREA1_RIGHT_BOTTOM) &&
		!context.controlActor.IsFacingRight())
	{
		context.soundManager.PlaySE(SE::DOOR);
		context.ChangeStage(StageId::PAT_ROOM, PATIENT_ROOM_MOVE_POS1);
		return;
	}

	// 後ろ側のドアは向きに関係なく、患者部屋後ろ側へ移動する
	if (Collision::IsPointInRect(actorPos, TO_PATIENT_ROOM_AREA2_LEFT_TOP, TO_PATIENT_ROOM_AREA2_RIGHT_BOTTOM))
	{
		context.soundManager.PlaySE(SE::DOOR);
		context.ChangeStage(StageId::PAT_ROOM, PATIENT_ROOM_MOVE_POS2);
		return;
	}

	// PC机の近くなら、進行度に応じたPCイベントを開始する
	if (Collision::IsPointInRect(actorPos, PC_LEFTUP, PC_RIGHTDOWN))
	{
		if (context.progressManager.GetProgressEnum() == ProgressManager::STORY_PROGRESS::AFTER_MG_TALKED)
		{
			context.talk.SetTalk(TDI::TALK_PC);
		}
		else if (context.progressManager.GetProgressEnum() == ProgressManager::STORY_PROGRESS::LUNCH)
		{
			context.talk.SetTalk(TDI::TALK_PC2);
		}
		else
		{
			context.talk.SetTemporaryTalk("PCだ。\n患者の電子カルテなどが確認できる。\n{WAIT:300}今は触る必要はない。");
		}
		return;
	}

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
