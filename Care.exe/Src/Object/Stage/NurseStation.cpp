#include "NurseStation.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/ProgressManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Object/Talk/Talk.h"
#include "../Actor/ActorBase.h"
#include "../Collider/Collider.h"
#include "../Renderer2D.h"
#include "../../Scene/PCScene.h"

NurseStation::NurseStation()
	: pcHandle_(-1)
	, chairHandle_(-1)
	, lockerHandle_(-1)
	, tableHandle_(-1)
	, plateHandle_(-1)
	, plateFolderHandle_(-1)
	, pcScene_(nullptr)
{
}

NurseStation::~NurseStation()
{
}

void NurseStation::Update()
{
	StageBase::Update();
	if (pcScene_ != nullptr)
	{
		pcScene_->Update(InputManager::GetInstance());
	}
}

void NurseStation::DrawForeground() const
{
	if (pcScene_ != nullptr)
	{
		pcScene_->Draw();
	}
}

void NurseStation::RegisterObjects(Renderer2D& renderer)
{
	StageBase::RegisterObjects(renderer);

	renderer.Add(TABLE_SORT_Y, [this]()
		{
			if (tableHandle_ != -1)
			{
				DrawGraph(TABLE_POS.x, TABLE_POS.y, tableHandle_, true);
			}
		});
	renderer.Add(PLATE_SORT_Y, [this]() { DrawPlate(); });
}

void NurseStation::Delete()
{
	if (pcScene_ != nullptr)
	{
		pcScene_->Delete();
		pcScene_.reset();
	}
	StageBase::Delete();
}

bool NurseStation::IsInputBlocked() const
{
	return pcScene_ != nullptr && pcScene_->IsOpen();
}

void NurseStation::DrawGuide(const ActorBase& controlActor) const
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
void NurseStation::Decide(DecideContext& context) const
{
	const VECTOR actorPos = context.controlActor.GetTransform().pos;
	const ProgressManager::STORY_PROGRESS progress = context.progressManager.GetProgressEnum();

	if ((progress == ProgressManager::STORY_PROGRESS::AFTER_PC ||
		progress == ProgressManager::STORY_PROGRESS::AFTER_PC4) &&
		Collision::IsPointInRect(actorPos, PLATE_LEFT_TOP, PLATE_RIGHT_BOTTOM))
	{
		context.progressManager.AddProgress();
		if (progress == ProgressManager::STORY_PROGRESS::AFTER_PC)
		{
			context.talk.SetTalk(TDI::TALK_GET_PLATE);
		}
		else
		{
			context.talk.SetTalk(TDI::TALK_GET_PLATE_FOLDER);
		}
		return;
	}

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
		else if (context.progressManager.GetProgressEnum() == ProgressManager::STORY_PROGRESS::AFTER_LUNCH)
		{
			context.talk.SetTalk(TDI::TALK_PC3);
		}
		else if (context.progressManager.GetProgressEnum() == ProgressManager::STORY_PROGRESS::AFTER_TALK3)
		{
			context.talk.SetTalk(TDI::TALK_PC4);
		}
		else if (pcScene_ != nullptr)
		{
			pcScene_->Open();
		}
		return;
	}

}

void NurseStation::InitLoad()
{
	pcScene_ = std::make_unique<PCScene>();
	pcScene_->Load();

	BGhandle_ = resMng_.Load(ResourceManager::SRC::BG_2).handleId_;

	pcHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_PC).handleId_;

	chairHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_CHAIR).handleId_;

	lockerHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_LOCKER).handleId_;

	tableHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_TABLE).handleId_;
	plateHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_PLATE).handleId_;
	plateFolderHandle_ = resMng_.Load(ResourceManager::SRC::BG_2_PLATE_FOLDER).handleId_;
}

void NurseStation::InitTransform()
{
	AddTransformObject(Transform2D(PC_POS), Sprite2D(pcHandle_, false));

	AddTransformObject(Transform2D(CHAIR_POS), Sprite2D(chairHandle_, false));

	AddTransformObject(Transform2D(LOCKER_POS), Sprite2D(lockerHandle_, false), LOCKER_BUFFER_Y);

}

void NurseStation::DrawPlate() const
{
	int plateHandle = -1;
	const ProgressManager::STORY_PROGRESS progress = ProgressManager::GetInstance().GetProgressEnum();
	if (progress == ProgressManager::STORY_PROGRESS::AFTER_PC)
	{
		plateHandle = plateHandle_;
	}
	else if (progress == ProgressManager::STORY_PROGRESS::AFTER_PC4)
	{
		plateHandle = plateFolderHandle_;
	}

	if (plateHandle != -1)
	{
		DrawGraph(PLATE_POS.x, PLATE_POS.y, plateHandle, true);
	}
}

void NurseStation::InitCollider()
{
	AddMBRectPercent(PC_BLOCK_LEFT_TOP, PC_BLOCK_RIGHT_BOTTOM);	// PC周辺

	AddMBRectPercent(TABLE_LEFT_TOP, TABLE_RIGHT_BOTTOM);	// 配膳台周辺

	// ロッカー周辺
	AddMBRectPercent(LOCKER_BLOCK_LEFT_TOP, LOCKER_BLOCK_RIGHT_BOTTOM);
}
