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
#include "../Object/Stage/NurseStation.h"
#include "../Object/Menu/GameMenu.h"
#include "../Debug/DebugCursorPosition.h"
#include "../Manager/ProgressManager.h"
#include "../Manager/SoundManager.h"
#include "../Effect/ScreenEffect.h"
#include "../Application.h"
#include "../Manager/Csv/ProgressTable.h"

GameScene::GameScene()
	: SceneBase()
	, talk_(nullptr)
	, gameMenu_(nullptr)
	, canMove_(false)
	, stage_(nullptr)
	, render_(nullptr)
	, player_(nullptr)
	, patient_(nullptr)
	, controlActor_(nullptr)
	, debugCursorPosition_(nullptr)
	, currentStage_(INIT_STAGE)
	, isReturningFromSetting_(false)
{
}

GameScene::~GameScene()
{
}

void GameScene::Update()
{
	if (stage_ != nullptr && stage_->IsInputBlocked())
	{
		stage_->Update();
		return;
	}

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

	if (canMove_ && stage_ != nullptr && IsControlActorAvailable() && talk_ != nullptr &&
		KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		StageBase::DecideContext context =
		{
			*controlActor_,
			(prgMng_.IsPatientCharExists() ? patient_.get() : nullptr),
			prgMng_,
			*talk_,
			sndMng_,
			[this](StageBase::StageId nextStage, const Vector2<float>& movePos)
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

void GameScene::Draw()
{
	const bool useAfterTalk3ColorShift =
		prgMng_.GetProgressEnum() >= ProgressManager::STORY_PROGRESS::AFTER_TALK3 &&
		!prgMng_.IsEndTalkProgress() &&
		colorShiftScreenHandle_ != -1;
	const int previousDrawScreen = GetDrawScreen();
	if (useAfterTalk3ColorShift)
	{
		SetDrawScreen(colorShiftScreenHandle_);
		ClearDrawScreen();
	}

	if (stage_ != nullptr)
	{
		stage_->DrawBG();
	}

	render_->Render();

	if (canMove_ && stage_ != nullptr && IsControlActorAvailable() &&
		!stage_->IsInputBlocked() &&
		(gameMenu_ == nullptr || !gameMenu_->IsOpen()))
	{
		stage_->DrawGuide(*controlActor_);
	}

	talk_->Draw();

	if (gameMenu_ != nullptr)
	{
		gameMenu_->Draw();
	}

	if (stage_ != nullptr)
	{
		stage_->DrawForeground();
	}

	if (useAfterTalk3ColorShift)
	{
		int addBugging = static_cast<int>(prgMng_.GetProgressEnum()) - static_cast<int>(ProgressManager::STORY_PROGRESS::AFTER_TALK3);
		if (prgMng_.GetProgressEnum() == ProgressManager::STORY_PROGRESS::DINNER)
		{
			--addBugging;
		}

		SetDrawScreen(previousDrawScreen);
		ScreenEffect::DrawColorShift(
			colorShiftScreenHandle_,
			0.0f,
			0.0f,
			COLOR_SHIFT_X,
			COLOR_SHIFT_Y,
			COLOR_SHIFT_RECT_COUNT + addBugging,
			COLOR_SHIFT_RECT_CHANGE_SEC
		);
	}

#ifdef _DEBUG
	if (debugCursorPosition_ != nullptr)
	{
		debugCursorPosition_->Draw();
	}

	int a = ProgressManager::GetInstance().GetProgress();
	DrawFormatString(DEBUG_PROGRESS_POS_X, DEBUG_PROGRESS_POS_Y, DEBUG_PROGRESS_COLOR, "Progress: %d", a);
#endif

	if (prgMng_.GetProgressEnum() == ProgressManager::END_RUINED_LOCKED ||	// RUINEDエンドがLOCKED状態なら黒点滅しない(真っ黒)
		prgMng_.GetProgressEnum() == ProgressManager::END_RUINED &&
		GetRand(RUINED_BLACK_BLINK_INTERVAL_FRAMES - 1) == 0)		// RUINEDエンドの時は黒点滅
	{
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, RUINED_BLINK_COLOR, true);
	}
}

void GameScene::Delete()
{
	if (talk_ != nullptr)
	{
		talk_->Delete();
		talk_.reset();
	}

	if (gameMenu_ != nullptr)
	{
		gameMenu_->Delete();
		gameMenu_.reset();
	}

	if (player_ != nullptr)
	{
		player_->Release();
		player_.reset();
	}
	if (patient_ != nullptr)
	{
		patient_->Release();
		patient_.reset();
	}
	controlActor_ = nullptr;
	if (stage_ != nullptr)
	{
		stage_->Delete();
		stage_.reset();
	}
	if (colorShiftScreenHandle_ != -1)
	{
		DeleteGraph(colorShiftScreenHandle_);
		colorShiftScreenHandle_ = -1;
	}

	debugCursorPosition_.reset();
	render_.reset();
}

void GameScene::InitLoad()
{
	colorShiftScreenHandle_ = MakeScreen(
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		FALSE
	);

	talk_ = std::make_unique<Talk>();
	talk_->Load();

	gameMenu_ = std::make_unique<GameMenu>();
	gameMenu_->Load();

	if (prgMng_.IsNurseCharExists())
	{
		player_ = std::make_unique<Player>();
		player_->Init();
	}

	if (prgMng_.IsPatientCharExists())
	{
		patient_ = std::make_unique<Patient>();
		patient_->Init();
	}

	controlActor_ = (player_ != nullptr) ? static_cast<ActorBase*>(player_.get()) : static_cast<ActorBase*>(patient_.get());

	render_ = std::make_unique<Renderer2D>();
	debugCursorPosition_ = std::make_unique<DebugCursorPosition>();

	ApplyInitialProgressState();
	ApplyControlMoveDelay();
	ChangeStage(currentStage_);
}

void GameScene::InitPost()
{

	if (prgMng_.GetProgressEnum() < ProgressManager::AFTER_LUNCH)
		sndMng_.PlayBGM(SoundManager::BGM::GAME0);
	else sndMng_.PlayBGM(SoundManager::BGM::GAME1);

	if (!isReturningFromSetting_)
	{
		StartFirstTalkByProgress();
	}
}

void GameScene::ApplyInitialProgressState()
{
	if (sceMng_.HasSettingReturnGameState())
	{
		const Vector2<float> returnPos = sceMng_.GetSettingReturnActorPos();

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
		stage_.reset();
	}

	switch (currentStage_)
	{
	case Stage::PAT_ROOM:
		stage_ = std::make_unique<PatientRoom>();
		break;

	case Stage::NURSE_STATION:
		stage_ = std::make_unique<NurseStation>();
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
			render_->AddActor(patient_.get());
		}
		if (player_ != nullptr)
		{
			render_->AddActor(player_.get());
		}
	}
}

void GameScene::UpdateTalkProgress()
{
	const bool suppressProgressTalkThisUpdate = isReturningFromSetting_;
	isReturningFromSetting_ = false;

	// 会話プログラムの更新結果を使いまわす
	const bool isTalking = talk_->Update();
	canMove_ = !isTalking;

	const ProgressManager::STORY_PROGRESS progressBefore = prgMng_.GetProgressEnum();
	const ProgressData& progressData = ProgressTable::Get(progressBefore);

	// 会話終了時に進行状況を進める設定なら進める
	if (ProgressTable::ShouldAdvanceByTalkEnd(
		progressBefore, progressData.talkEnd))
	{
		if (progressData.talkEnd != TDI::NONE &&			// 進行状況に対応する会話が設定されている場合
			talk_->ConsumeTalkEnd(progressData.talkEnd))	// ＋その会話が終了した場合
		{
			// 進行ロックがある場合、ロック済みの進行状況に強制的に変更する
			const ProgressManager::STORY_PROGRESS endLockedProgress =
				ProgressTable::GetEndLockedProgress(progressBefore);
				if (endLockedProgress != progressBefore)
				{
					prgMng_.SetProgress(endLockedProgress);
					sceMng_.ChangeScene(SceneManager::SCENE_ID::CLEAR);
					return;
				}

			// 進行状況を進める
			if (progressBefore == ProgressManager::DINNER)
			{
				prgMng_.SetProgress(ProgressManager::END_RUINED);
			}
			else if (progressBefore == ProgressManager::MINIGAME_RETRY)
			{
				prgMng_.SetProgress(ProgressManager::START_MINIGAME0);
			}
			else
			{
				prgMng_.AddProgress();
			}
		}
	}

	if (prgMng_.GetProgressEnum() == ProgressManager::START_MINIGAME0)
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::BP_MINIGAME);
		return;
	}

	// (デバッグ用) 進行状況開始時に自動で進める設定なら進める
	if (ProgressTable::ShouldAutoAdvance(prgMng_.GetProgressEnum()))
	{
		prgMng_.AddProgress();
	}

	// 進行状況が変化したときの処理
	if (progressBefore != prgMng_.GetProgressEnum())
	{
		// 操作キャラ移動遅延の適用判定を行う
		ApplyControlMoveDelay();

		// 進行状況開始時の会話を開始する
		if (!suppressProgressTalkThisUpdate)
		{
			StartFirstTalkByProgress();
		}
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
	if (canMove_ && IsControlActorAvailable())
	{
		controlActor_->Update();
		if (stage_ != nullptr)
		{
			stage_->ApplyCollision(*controlActor_);
		}

		if (patient_ != nullptr && prgMng_.IsPatientCharExists() &&
			controlActor_ != patient_.get() && controlActor_->IsHitCircle(*patient_))
		{
			controlActor_->MoveToBeforePos();
		}
	}

	if (patient_ != nullptr && prgMng_.IsPatientCharExists() && controlActor_ != patient_.get())
	{
		patient_->Update(false);
	}
}

void GameScene::UpdateNS()
{
	if (canMove_ && IsControlActorAvailable())
	{
		controlActor_->Update();
		if (stage_ != nullptr)
		{
			stage_->ApplyCollision(*controlActor_);
		}
	}
}

bool GameScene::IsControlActorAvailable() const
{
	if (controlActor_ == nullptr)
	{
		return false;
	}
	if (controlActor_ == player_.get())
	{
		return prgMng_.IsNurseCharExists();
	}
	return controlActor_ == patient_.get() && prgMng_.IsPatientCharExists();
}
