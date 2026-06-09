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
#include "../Sound/SoundManager.h"
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
	controlActor_(nullptr),
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
	if (firstUpdate_)
	{
		SoundManager::GetInstance().PlayBGM(SoundManager::BGM::GAME0);
	}

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
	if (talk_ != nullptr)
	{
		talk_->Delete();
		delete talk_;
		talk_ = nullptr;
	}

	if (player_ != nullptr)
	{
		player_->Release();
		delete player_;
		player_ = nullptr;
	}
	if (patient_ != nullptr)
	{
		patient_->Release();
		delete patient_;
		patient_ = nullptr;
	}
	controlActor_ = nullptr;
	if (stage_ != nullptr)
	{
		stage_->Delete();
		delete stage_;
		stage_ = nullptr;
	}
	delete debugCursorPosition_;
	debugCursorPosition_ = nullptr;
	delete render_;
	render_ = nullptr;
}

void GameScene::InitLoad()
{
	talk_ = new Talk();
	talk_->Load();
	// talk_->SetTalk(TalkDatas::TalkDataIndex::TALK_0);

	if (prgMng_.IsNurceCharExists())
	{
		player_ = new Player();
		player_->Init();
	}

	if (prgMng_.IsPatientCharExists())
	{
		patient_ = new Patient();
		patient_->Init();
	}

	controlActor_ = (player_ != nullptr) ? static_cast<ActorBase*>(player_) : static_cast<ActorBase*>(patient_);

	render_ = new Renderer2D();
	debugCursorPosition_ = new DebugCursorPosition();

	ApplyInitialProgressState();
	ChangeStage(currentStage_);
}

void GameScene::ApplyInitialProgressState()
{
	const ProgressData& startData = ProgressTable::Get(prgMng_.GetProgressEnum());

	currentStage_ = startData.initStage;
	if (controlActor_ != nullptr)
	{
		controlActor_->SetLocalPercent(startData.playerInitPos.x, startData.playerInitPos.y);
	}
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

		if (currentStage_ == Stage::PAT_ROOM && patient_ != nullptr)
		{
			render_->AddActor(patient_);
		}
		if (player_ != nullptr)
		{
			render_->AddActor(player_);
		}
	}
}

void GameScene::UpdateTalkProgress()
{
	// Talk の更新結果を使いまわす
	const bool isTalking = talk_->Update();
	canMove_ = !isTalking;

	if (firstUpdate_)
	{
		StartFirstTalkByProgress();
	}

	const ProgressManager::STORY_PROGRESS progressBefore = prgMng_.GetProgressEnum();
	const ProgressData& progressData = ProgressTable::Get(progressBefore);
	if (progressData.talkEnd.talkId != TDI::NONE &&
		talk_->ConsumeTalkEnd(progressData.talkEnd.talkId) &&
		ProgressTable::ShouldAdvanceByTalkEnd(
			progressBefore, progressData.talkEnd.talkId))
	{
		prgMng_.AddProgress();
	}

	if (ProgressTable::ShouldAutoAdvance(prgMng_.GetProgressEnum()))
	{
		prgMng_.AddProgress();
	}

	if (progressBefore != prgMng_.GetProgressEnum())
	{
		StartFirstTalkByProgress();
	}
}

void GameScene::StartFirstTalkByProgress()
{
	const ProgressData& progressData = ProgressTable::Get(prgMng_.GetProgressEnum());
	if (progressData.firstTalk != TDI::NONE)
	{
		talk_->SetTalk(progressData.firstTalk);
	}
}

void GameScene::UpdatePR()
{
	if (canMove_ && controlActor_ != nullptr)
	{
		controlActor_->Update();
		if (stage_ != nullptr)
		{
			stage_->ApplyMovementBlocks(*controlActor_);
		}

		if (patient_ != nullptr && controlActor_ != patient_ && controlActor_->IsHitCircle(*patient_))
		{
			controlActor_->MoveToBeforePos();
		}
	}

	if (patient_ != nullptr && controlActor_ != patient_)
	{
		patient_->Update();
	}
}

void GameScene::DecidePR()
{
	if (controlActor_ == nullptr)
	{
		return;
	}

	const VECTOR pPos = controlActor_->GetTransform().pos;

	// ドアの位置にプレイヤーがいて、かつ右を向いているときに遷移
	if (Collision::IsPointInRect(pPos, PR_TO_NS_AREA1_0, PR_TO_NS_AREA1_1) && controlActor_->IsFacingRight())
	{
		ChangeStage(Stage::NURSE_STATION);
		controlActor_->SetLocalPercent(NS_MOVE_POS1.x, NS_MOVE_POS1.y);
		return;
	}

	// ドアの位置にプレイヤーがいて、かつ後ろを向いているときに遷移
	if (Collision::IsPointInRect(pPos, PR_TO_NS_AREA2_0, PR_TO_NS_AREA2_1))
	{
		ChangeStage(Stage::NURSE_STATION);
		controlActor_->SetLocalPercent(NS_MOVE_POS2.x, NS_MOVE_POS2.y);
		return;
	}

	// プレイヤーが患者の近くにいるときに会話開始
	if (patient_ != nullptr && controlActor_ != patient_)
	{
		const VECTOR patPos = patient_->GetTransform().pos;
		if (Collision::IsPointInCircle(pPos, patPos, Patient::TALK_RADIUS))
		{
			const ProgressData& progressData = ProgressTable::Get(prgMng_.GetProgressEnum());
			if (progressData.patientTalk != TDI::NONE)
			{
				talk_->SetTalk(progressData.patientTalk);
			}
		}
	}
}

void GameScene::UpdateNS()
{
	if (canMove_ && controlActor_ != nullptr)
	{
		controlActor_->Update();
		if (stage_ != nullptr)
		{
			stage_->ApplyMovementBlocks(*controlActor_);
		}
	}
}

void GameScene::DecideNS()
{
	if (controlActor_ == nullptr)
	{
		return;
	}

	const VECTOR pPos = controlActor_->GetTransform().pos;

	// ドアの位置にプレイヤーがいて、かつ左を向いているときに遷移
	if (Collision::IsPointInRect(pPos, NS_TO_PR_AREA1_0, NS_TO_PR_AREA1_1) && !controlActor_->IsFacingRight())
	{
		ChangeStage(Stage::PAT_ROOM);
		controlActor_->SetLocalPercent(PR_MOVE_POS1.x, PR_MOVE_POS1.y);
		return;
	}

	// ドアの位置にプレイヤーがいて、かつ後ろを向いているときに遷移
	if (Collision::IsPointInRect(pPos, NS_TO_PR_AREA2_0, NS_TO_PR_AREA2_1))
	{
		ChangeStage(Stage::PAT_ROOM);
		controlActor_->SetLocalPercent(PR_MOVE_POS2.x, PR_MOVE_POS2.y);
		return;
	}
}
