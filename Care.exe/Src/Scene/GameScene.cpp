#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/KeyConfig.h"
#include "GameScene.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Talk/Talk.h"
#include "../Object/Actor/Player.h"
#include "../Object/Actor/Patient.h"
#include "../Object/Renderer2D.h"
#include "../Object/Stage/StageBase.h"
#include "../Object/Stage/PatientRoom.h"
#include "../Object/Stage/NurceStation.h"
#include "../Debug/DebugCursorPosition.h"
#include "../Manager/ProgressManager.h"
#include "ProgressTable.h"

GameScene::GameScene(void)
	:
	SceneBase(),
	talk_(nullptr),
	canMove_(false),
	stage_(nullptr),
	render_(nullptr),
	player_(nullptr),
	patient_(nullptr),
	debugCursorPosition_(nullptr),
	currentStage_(INIT_STAGE)
{
	firstUpdate_ = true;
}

GameScene::~GameScene(void)
{
}

void GameScene::Update(void)
{
	UpdateTalkProgress();

	// シーン遷移
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	if (stage_ != nullptr)
	{
		stage_->Update();
	}

	switch (currentStage_)
	{
	case Stage::PAT_ROOM:
		UpdatePR();
		break;
	case Stage::NURSE_STATION:
		UpdateNS();
		break;
	}

	if (canMove_ && KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		switch (currentStage_)
		{
		case Stage::PAT_ROOM:
			DecidePR();
			break;
		case Stage::NURSE_STATION:
			DecideNS();
			break;
		}
	}

#ifdef _DEBUG
	if (debugCursorPosition_ != nullptr)
	{
		debugCursorPosition_->Update();
	}

#endif

	firstUpdate_ = false;
}

void GameScene::Draw(void)
{
	if (stage_ != nullptr)
	{
		stage_->DrawBackground();
	}

	render_->Render();

	talk_->Draw();

#ifdef _DEBUG
	if (debugCursorPosition_ != nullptr)
	{
		debugCursorPosition_->Draw();
	}

	int a = ProgressManager::GetInstance().GetProgress();
	DrawFormatString(0, 50, 0x000000, "Progress: %d", a);
#endif
}

void GameScene::Delete(void)
{
	talk_->Delete();
	delete talk_;

	delete player_;
	delete patient_;
	if (stage_ != nullptr)
	{
		stage_->Delete();
		delete stage_;
	}
	delete debugCursorPosition_;
	delete render_;
}

void GameScene::InitLoad()
{
	talk_ = new Talk();
	talk_->Load();
	// talk_->SetTalk(TalkDatas::TalkDataIndex::TALK_0);

	player_ = new Player();
	player_->Init();

	patient_ = new Patient();
	patient_->Init();

	render_ = new Renderer2D();
	debugCursorPosition_ = new DebugCursorPosition();

	ApplyInitialProgressState();
	ChangeStage(currentStage_);
}

void GameScene::ApplyInitialProgressState()
{
	const ProgressData& startData = ProgressTable::Get(prgMng_.GetProgressEnum());

	currentStage_ = startData.initStage;
	player_->SetLocalPercent(startData.playerInitPos.x, startData.playerInitPos.y);
}

void GameScene::ChangeStage(Stage nextStage)
{
	currentStage_ = nextStage;

	if (stage_ != nullptr)
	{
		stage_->Delete();
		delete stage_;
		stage_ = nullptr;
	}

	switch (currentStage_)
	{
	case Stage::PAT_ROOM:
		stage_ = new PatientRoom();
		break;
	case Stage::NURSE_STATION:
		stage_ = new NurceStation();
		break;
	}

	if (stage_ != nullptr)
	{
		stage_->Init();
	}

	if (render_ != nullptr)
	{
		render_->Clear();
		if (stage_ != nullptr)
		{
			stage_->RegisterObjects(*render_);
		}

		if (currentStage_ == Stage::PAT_ROOM)
		{
			render_->AddActor(patient_);
		}
		render_->AddActor(player_);
	}
}

void GameScene::UpdateTalkProgress()
{
	// Talk の更新結果を使いまわす
	const bool isTalking = talk_->Update();
	canMove_ = !isTalking;

	if (firstUpdate_)
	{
		const ProgressData& progressData = ProgressTable::Get(prgMng_.GetProgressEnum());
		if (progressData.firstTalk != TDI::NONE)
		{
			talk_->SetTalk(progressData.firstTalk);
		}
	}

	const ProgressData& progressData = ProgressTable::Get(prgMng_.GetProgressEnum());
	if (progressData.talkEnd.talkId != TDI::NONE &&
		talk_->ConsumeTalkEnd(progressData.talkEnd.talkId) &&
		ProgressTable::ShouldAdvanceByTalkEnd(
			prgMng_.GetProgressEnum(), progressData.talkEnd.talkId))
	{
		prgMng_.AddProgress();
	}

	if (ProgressTable::ShouldAutoAdvance(prgMng_.GetProgressEnum()))
	{
		prgMng_.AddProgress();
	}
}

void GameScene::UpdatePR()
{
	if (canMove_)
	{
		player_->Update();
		if (stage_ != nullptr)
		{
			stage_->ApplyMovementBlocks(*player_);
		}


		if (player_->IsHitCircle(*patient_))
		{
			player_->MoveToBeforePos();
		}
	}

	patient_->Update();
}

void GameScene::DecidePR()
{
	const VECTOR pPos = player_->GetTransform().pos;
	const VECTOR patPos = patient_->GetTransform().pos;

	// ドアの位置にプレイヤーがいて、かつ右を向いているときに遷移
	if (Collision::IsPointInRect(pPos, PR_TO_NS_AREA1_0, PR_TO_NS_AREA1_1) && player_->IsFacingRight())
	{
		ChangeStage(Stage::NURSE_STATION);
		player_->SetLocalPercent(NS_MOVE_POS1.x, NS_MOVE_POS1.y);
		return;
	}

	// ドアの位置にプレイヤーがいて、かつ後ろを向いているときに遷移
	if (Collision::IsPointInRect(pPos, PR_TO_NS_AREA2_0, PR_TO_NS_AREA2_1))
	{
		ChangeStage(Stage::NURSE_STATION);
		player_->SetLocalPercent(NS_MOVE_POS2.x, NS_MOVE_POS2.y);
		return;
	}

	// プレイヤーが患者の近くにいるときに会話開始
	if (Collision::IsPointInCircle(pPos, patPos, Patient::TALK_RADIUS))
	{
		const ProgressData& progressData = ProgressTable::Get(prgMng_.GetProgressEnum());
		if (progressData.patientTalk != TDI::NONE)
		{
			talk_->SetTalk(progressData.patientTalk);
		}
	}
}

void GameScene::UpdateNS()
{
	if (canMove_)
	{
		player_->Update();
		if (stage_ != nullptr)
		{
			stage_->ApplyMovementBlocks(*player_);
		}
	}
}

void GameScene::DecideNS()
{
	const VECTOR pPos = player_->GetTransform().pos;

	// ドアの位置にプレイヤーがいて、かつ左を向いているときに遷移
	if (Collision::IsPointInRect(pPos, NS_TO_PR_AREA1_0, NS_TO_PR_AREA1_1) && !player_->IsFacingRight())
	{
		ChangeStage(Stage::PAT_ROOM);
		player_->SetLocalPercent(PR_MOVE_POS1.x, PR_MOVE_POS1.y);
		return;
	}

	// ドアの位置にプレイヤーがいて、かつ後ろを向いているときに遷移
	if (Collision::IsPointInRect(pPos, NS_TO_PR_AREA2_0, NS_TO_PR_AREA2_1))
	{
		ChangeStage(Stage::PAT_ROOM);
		player_->SetLocalPercent(PR_MOVE_POS2.x, PR_MOVE_POS2.y);
		return;
	}
}
