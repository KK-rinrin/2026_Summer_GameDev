#include <chrono>
#include <DxLib.h>
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/SettingScene.h"
#include "../Scene/MiniGame/BPMiniGameScene.h"
#include "../Scene/ClearScene.h"
#include "ResourceManager.h"
#include "ProgressManager.h"
#include "SceneManager.h"
#include "../Scene/Debug/DebugScene.h"

std::shared_ptr<SceneManager> SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = std::shared_ptr<SceneManager>(new SceneManager(), [](SceneManager* instance) { delete instance; });
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance()
{
	return *instance_;
}

void SceneManager::Init()
{
	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;
	settingReturnSceneId_ = SCENE_ID::TITLE;
	hasSettingReturnGameState_ = false;
	settingReturnGameStage_ = 0;
	settingReturnActorPos_ = Vector2<float>{};

	// フェード機能の初期化
	fader_ = std::make_unique<Fader>();
	fader_->Init();

	// 画面遷移中判定
	isSceneChanging_ = false;

	// 背景色設定
	SetBackgroundColor(
		BACKGROUND_COLOR_R, 
		BACKGROUND_COLOR_G, 
		BACKGROUND_COLOR_B);

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// 初期シーンの設定
	ProgressManager& progressManager = ProgressManager::GetInstance();
	const bool isCharaFileMissing =
		!progressManager.IsPatientCharExists() || !progressManager.IsNurseCharExists();

	// 進行度が不十分でファイルが消されていたらエンドに直行
	// エンドロックされている場合もエンドに直行
	if ((isCharaFileMissing && !progressManager.IsCanDeleteProgress())
		|| progressManager.IsEndLockedProgress())
	{
		DoChangeScene(SCENE_ID::CLEAR);
	}
	// そうでなければタイトルへ（デバッグビルドはデバッグシーンへ）
	else
	{
#ifdef _DEBUG
		DoChangeScene(SCENE_ID::DEBUG);
#else
		DoChangeScene(SCENE_ID::TITLE);
#endif
	}

}

void SceneManager::Update()
{

	if (scene_ == nullptr)
	{
		return;
	}

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / NANOSECONDS_PER_SECOND);
	preTime_ = nowTime;

	// フェード機能の更新
	fader_->Update();
	if (isSceneChanging_)
	{
		// フェード状態の切替処理
		Fade();
	}
	else
	{
		// 各シーンの更新処理
		scene_->Update();
	}

}

void SceneManager::Draw()
{
	
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// 各シーンの描画処理
	scene_->Draw();

	
	// 暗転・明転
	fader_->Draw();

}

void SceneManager::Destroy()
{

	// シーンが所有するオブジェクトを解放してからシーンを破棄する
	if (scene_ != nullptr)
	{
		scene_->Delete();
		scene_.reset();
	}

	// フェード機能の解放
	fader_.reset();

	// インスタンスのメモリ解放
	instance_.reset();

}

void SceneManager::ChangeScene(SCENE_ID nextId)
{
	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

#ifdef _DEBUG
#else
	if (waitSceneId_ == SCENE_ID::DEBUG)
	{
		// デバッグシーンはリリース版では使用不可
		waitSceneId_ = SCENE_ID::TITLE;
	}
#endif // _DEBUG

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

void SceneManager::SetSettingReturnScene(SCENE_ID sceneId)
{
	settingReturnSceneId_ = sceneId;

	// GameScene以外へ戻る時は、古い座標復帰情報を残さないようにする。
	if (sceneId != SCENE_ID::GAME)
	{
		ClearSettingReturnGameState();
	}
}

void SceneManager::SetSettingReturnGameState(int stage, const Vector2<float>& actorPos)
{
	// SettingSceneのBACKでGameSceneへ戻るために、開いた時点の状態を保存する。
	settingReturnSceneId_ = SCENE_ID::GAME;
	hasSettingReturnGameState_ = true;
	settingReturnGameStage_ = stage;
	settingReturnActorPos_ = actorPos;
}

SceneManager::SCENE_ID SceneManager::GetSettingReturnScene() const
{
	return settingReturnSceneId_;
}

bool SceneManager::HasSettingReturnGameState() const
{
	return hasSettingReturnGameState_;
}

int SceneManager::GetSettingReturnGameStage() const
{
	return settingReturnGameStage_;
}

Vector2<float> SceneManager::GetSettingReturnActorPos() const
{
	return settingReturnActorPos_;
}

void SceneManager::ClearSettingReturnGameState()
{
	hasSettingReturnGameState_ = false;
	settingReturnGameStage_ = 0;
	settingReturnActorPos_ = Vector2<float>{};
}

SceneManager::SCENE_ID SceneManager::GetSceneID()
{
	return sceneId_;
}

float SceneManager::GetDeltaTime() const
{
	return deltaTime_;
}

SceneManager::SceneManager()
	: sceneId_(SCENE_ID::NONE)
	, waitSceneId_(SCENE_ID::NONE)
	, settingReturnSceneId_(SCENE_ID::TITLE)
	, hasSettingReturnGameState_(false)
	, settingReturnGameStage_(0)
	, settingReturnActorPos_(Vector2<float>{})
	, fader_(nullptr)
	, scene_(nullptr)
	, isSceneChanging_(false)
	, preTime_()
	, deltaTime_(FALLBACK_DELTA_SECONDS)
{
}

SceneManager::~SceneManager() = default;

void SceneManager::ResetDeltaTime()
{
	deltaTime_ = RESET_DELTA_SECONDS;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	// 現在のシーンが所有するオブジェクトを先に解放する
	if (scene_ != nullptr)
	{
		scene_->Delete();
		scene_.reset();
	}

	// シーン内オブジェクトが参照し終えてからリソースを解放する
	ResourceManager::GetInstance().Release();

	// シーンを変更する
	sceneId_ = sceneId;

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = std::make_unique<TitleScene>();
		break;
	case SCENE_ID::GAME:
		scene_ = std::make_unique<GameScene>();
		break;
	case SCENE_ID::SETTING:
		scene_ = std::make_unique<SettingScene>();
		break;
	case SCENE_ID::BP_MINIGAME:
		scene_ = std::make_unique<BPMiniGameScene>();
		break;
	case SCENE_ID::CLEAR:
		scene_ = std::make_unique<ClearScene>();
		break;
	case SCENE_ID::DEBUG:
		scene_ = std::make_unique<DebugScene>();
		break;
	}

	// 各シーンの初期化
	scene_->Init();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade()
{
	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}
