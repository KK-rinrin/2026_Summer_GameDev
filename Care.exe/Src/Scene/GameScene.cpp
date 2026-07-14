#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/KeyConfig.h"
#include "GameScene.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Talk/Talk.h"
#include "../Object/Actor/Player.h"
#include "../Object/Actor/Patient.h"
#include "../Object/Actor/ProcessMove.h"
#include "../Object/Renderer2D.h"
#include "../Object/Stage/StageBase.h"
#include "../Object/Stage/PatientRoom.h"
#include "../Object/Stage/NurceStation.h"
#include "../Object/Menu/GameMenu.h"
#include "../Debug/DebugCursorPosition.h"
#include "../Manager/ProgressManager.h"
#include "../Manager/SoundManager.h"
#include "ProgressTable.h"

GameScene::GameScene(void)
	:
	SceneBase(),
	talk_(nullptr),
	gameMenu_(nullptr),
	canMove_(false),
	stage_(nullptr),
	render_(nullptr),
	player_(nullptr),
	patient_(nullptr),
	controlActor_(nullptr),
	debugCursorPosition_(nullptr),
	currentStage_(INIT_STAGE)
{
	isReturningFromSetting_ = false;
}

GameScene::~GameScene(void)
{
}

void GameScene::Update(void)
{
	if (gameMenu_ != nullptr && gameMenu_->IsOpen())
	{
		UpdateGameMenu();
		return;
	}

	UpdateTalkProgress();

	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
	{
		OpenGameMenu();
		return;
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

	if (canMove_ && stage_ != nullptr && controlActor_ != nullptr && talk_ != nullptr &&
		KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		StageBase::DecideContext context =
		{
			*controlActor_,
			patient_,
			prgMng_,
			*talk_,
			sndMng_,
			[this](StageBase::StageId nextStage, const VECTOR& movePos)
			{
				const Stage stage = (nextStage == StageBase::StageId::PAT_ROOM) ?
					Stage::PAT_ROOM : Stage::NURSE_STATION;
				ChangeStage(stage);
				if (controlActor_ != nullptr)
				{
					controlActor_->SetLocalPercent(movePos.x, movePos.y);
				}
			}
		};
		stage_->Decide(context);
	}
#ifdef _DEBUG
	if (debugCursorPosition_ != nullptr)
	{
		debugCursorPosition_->Update();
	}

#endif

}

void GameScene::Draw(void)
{
	if (stage_ != nullptr)
	{
		stage_->DrawBackground();
	}

	render_->Render();

	if (canMove_ && stage_ != nullptr && controlActor_ != nullptr &&
		(gameMenu_ == nullptr || !gameMenu_->IsOpen()))
	{
		stage_->DrawGuide(*controlActor_);
	}

	talk_->Draw();

	if (gameMenu_ != nullptr)
	{
		gameMenu_->Draw();
	}

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

	if (gameMenu_ != nullptr)
	{
		gameMenu_->Delete();
		delete gameMenu_;
		gameMenu_ = nullptr;
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

	gameMenu_ = new GameMenu();
	gameMenu_->Load();

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
	ApplyControlMoveDelay();
	ChangeStage(currentStage_);
}

void GameScene::InitPost(void)
{
	sndMng_.PlayBGM(SoundManager::BGM::GAME0);
	if (!isReturningFromSetting_)
	{
		StartFirstTalkByProgress();
	}
}

void GameScene::ApplyInitialProgressState()
{
	if (sceMng_.HasSettingReturnGameState())
	{
		const VECTOR returnPos = sceMng_.GetSettingReturnActorPos();

		// SettingSceneから戻った時は、開いた時点のステージと操作キャラ座標を復元する。
		currentStage_ = static_cast<Stage>(sceMng_.GetSettingReturnGameStage());
		if (controlActor_ != nullptr)
		{
			controlActor_->SetLocalPercent(returnPos.x, returnPos.y);
		}
		isReturningFromSetting_ = true;
		sceMng_.ClearSettingReturnGameState();
		return;
	}

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
	const bool suppressProgressTalkThisUpdate = isReturningFromSetting_;
	isReturningFromSetting_ = false;

	// Talk の更新結果を使いまわす
	const bool isTalking = talk_->Update();
	canMove_ = !isTalking;

	const ProgressManager::STORY_PROGRESS progressBefore = prgMng_.GetProgressEnum();
	const ProgressData& progressData = ProgressTable::Get(progressBefore);
	if (progressData.talkEnd != TDI::NONE &&
		talk_->ConsumeTalkEnd(progressData.talkEnd) &&
		ProgressTable::ShouldAdvanceByTalkEnd(
			progressBefore, progressData.talkEnd))
	{
		const ProgressManager::STORY_PROGRESS endLockedProgress =
			ProgressTable::GetEndLockedProgress(progressBefore);
		if (endLockedProgress != progressBefore)
		{
			prgMng_.SetProgress(endLockedProgress);
			sceMng_.ChangeScene(SceneManager::SCENE_ID::CLEAR);
			return;
		}

		prgMng_.AddProgress();
	}

	if (prgMng_.GetProgressEnum() == ProgressManager::START_MINIGAME0)
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::BP_MINIGAME);
		return;
	}

	if (ProgressTable::ShouldAutoAdvance(prgMng_.GetProgressEnum()))
	{
		prgMng_.AddProgress();
	}

	if (progressBefore != prgMng_.GetProgressEnum())
	{
		ApplyControlMoveDelay();
	}

	if (progressBefore != prgMng_.GetProgressEnum() && !suppressProgressTalkThisUpdate)
	{
		StartFirstTalkByProgress();
	}
}


void GameScene::ApplyControlMoveDelay()
{
	if (player_ == nullptr || player_->GetProcessMove() == nullptr)
	{
		return;
	}

	ProcessMove* processMove = player_->GetProcessMove();
	if (static_cast<int>(prgMng_.GetProgressEnum()) >= static_cast<int>(ProgressManager::STORY_PROGRESS::AFTER_TALK3))
	{
		const int delayFrame = static_cast<int>(INPUT_MOVE_DELAY_SECONDS_1 * GAME_FPS);
		processMove->SetDelayFrameRange(delayFrame, delayFrame);
		processMove->Reset();
		return;
	}
	else if (static_cast<int>(prgMng_.GetProgressEnum()) >= static_cast<int>(ProgressManager::STORY_PROGRESS::LUNCH))
	{
		const int delayFrame = static_cast<int>(INPUT_MOVE_DELAY_SECONDS_0 * GAME_FPS);
		processMove->SetDelayFrameRange(delayFrame, delayFrame);
		processMove->Reset();
		return;
	}

	processMove->ClearDelay();
}
void GameScene::StartFirstTalkByProgress()
{
	const ProgressData& progressData = ProgressTable::Get(prgMng_.GetProgressEnum());
	if (progressData.firstTalk != TDI::NONE)
	{
		talk_->SetTalk(progressData.firstTalk);
	}
}

void GameScene::UpdateGameMenu()
{
	gameMenu_->Update(iptMng_);

	switch (gameMenu_->ConsumeResult())
	{
	case GameMenu::Result::OPEN_SETTING:
		OpenSettingFromGameMenu();
		break;
	case GameMenu::Result::BACK_TO_TITLE:
		sceMng_.SetSettingReturnScene(SceneManager::SCENE_ID::TITLE);
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		break;
	default:
		break;
	}
}

void GameScene::OpenGameMenu()
{
	if (gameMenu_ == nullptr)
	{
		return;
	}

	const bool isTalkActive = (talk_ != nullptr && talk_->IsActive());
	const GameMenu::Mode mode = isTalkActive ? GameMenu::Mode::PAUSE : GameMenu::Mode::NORMAL;
	gameMenu_->Open(mode);
	sndMng_.PlaySE(SE::CANCEL);
}

void GameScene::OpenSettingFromGameMenu()
{
	if (controlActor_ != nullptr)
	{
		// SettingSceneから戻った時に、今いるステージと操作キャラ座標を再現できるようにする。
		sceMng_.SetSettingReturnGameState(static_cast<int>(currentStage_), controlActor_->GetTransform().pos);
	}
	else
	{
		// 念のため、操作キャラがいない場合でもBACK先だけはGameSceneにしておく。
		sceMng_.SetSettingReturnScene(SceneManager::SCENE_ID::GAME);
	}

	sceMng_.ChangeScene(SceneManager::SCENE_ID::SETTING);
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
