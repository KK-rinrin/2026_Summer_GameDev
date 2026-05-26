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

GameScene::GameScene(void)
	:
	SceneBase(),
	talk_(nullptr),
	canMove_(false),
	stage_(nullptr),
	render_(nullptr),
	player_(nullptr),
	patient_(nullptr),
	debugCursorPosition_(nullptr)
{
	firstUpdate_ = true;
}

GameScene::~GameScene(void)
{
}

void GameScene::Update(void)
{
	// Talk の更新結果を使いまわす
	const bool isTalking = talk_->Update();
	canMove_ = !isTalking;

	// シーン遷移
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

#ifdef _DEBUG
	if (debugCursorPosition_ != nullptr)
	{
		debugCursorPosition_->Update();
	}
#endif

	switch (currentStage_)
	{
	case Stage::PAT_ROOM:
		UpdatePR();
		break;
	case Stage::NURSE_STATION:
		UpdateNS();
		break;
	}

	if (canMove_ && KeyConfig::IsNew(KeyConfig::ACTION::DECIDE, iptMng_))
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
	ChangeStage(currentStage_);
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

void GameScene::UpdatePR()
{
	if (stage_ != nullptr)
	{
		stage_->Update();
	}

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
		player_->SetLocalPercent(NS_MOVE_POS.x, NS_MOVE_POS.y);
		return;
	}

	// プレイヤーが患者の近くにいるときに会話開始
	if (Collision::IsPointInCircle(pPos, patPos, Patient::TALK_RADIUS))
	{
		talk_->SetTalk(TalkDatas::TalkDataIndex::TALK_0);
	}
}

void GameScene::UpdateNS()
{
	if (stage_ != nullptr)
	{
		stage_->Update();
	}

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
		player_->SetLocalPercent(PR_MOVE_POS.x, PR_MOVE_POS.y);
		return;
	}
}
