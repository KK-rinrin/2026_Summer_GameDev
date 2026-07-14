#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
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

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;
	settingReturnSceneId_ = SCENE_ID::TITLE;
	hasSettingReturnGameState_ = false;
	settingReturnGameStage_ = 0;
	settingReturnActorPos_ = VGet(0.0f, 0.0f, 0.0f);

	// フェード機能の初期化
	fader_ = new Fader();
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
	if (ProgressManager::GetInstance().IsEndLockedProgress())
	{
		DoChangeScene(SCENE_ID::CLEAR);
	}
	else
	{
#ifdef _DEBUG
		DoChangeScene(SCENE_ID::DEBUG);
#else
		DoChangeScene(SCENE_ID::TITLE);
#endif
	}

}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
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

void SceneManager::Draw(void)
{
	
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	// 各シーンの描画処理
	scene_->Draw();

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
	
	// 暗転・明転
	fader_->Draw();

}

void SceneManager::Destroy(void)
{

	// シーンの解放
	if (scene_ != nullptr)
	{
		delete scene_;
	}

	// フェード機能の解放
	delete fader_;

	// インスタンスのメモリ解放
	delete instance_;

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

void SceneManager::SetSettingReturnGameState(int stage, const VECTOR& actorPos)
{
	// SettingSceneのBACKでGameSceneへ戻るために、開いた時点の状態を保存する。
	settingReturnSceneId_ = SCENE_ID::GAME;
	hasSettingReturnGameState_ = true;
	settingReturnGameStage_ = stage;
	settingReturnActorPos_ = actorPos;
}

SceneManager::SCENE_ID SceneManager::GetSettingReturnScene(void) const
{
	return settingReturnSceneId_;
}

bool SceneManager::HasSettingReturnGameState(void) const
{
	return hasSettingReturnGameState_;
}

int SceneManager::GetSettingReturnGameStage(void) const
{
	return settingReturnGameStage_;
}

VECTOR SceneManager::GetSettingReturnActorPos(void) const
{
	return settingReturnActorPos_;
}

void SceneManager::ClearSettingReturnGameState(void)
{
	hasSettingReturnGameState_ = false;
	settingReturnGameStage_ = 0;
	settingReturnActorPos_ = VGet(0.0f, 0.0f, 0.0f);
}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	return 1.0f / 60.0f;
	//return deltaTime_;
}

SceneManager::SceneManager(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;
	settingReturnSceneId_ = SCENE_ID::TITLE;
	hasSettingReturnGameState_ = false;
	settingReturnGameStage_ = 0;
	settingReturnActorPos_ = VGet(0.0f, 0.0f, 0.0f);

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;

}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	// リソースの解放
	ResourceManager::GetInstance().Release();

	// シーンを変更する
	sceneId_ = sceneId;

	// 現在のシーンを解放
	if (scene_ != nullptr)
	{
		delete scene_;
	}

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SCENE_ID::GAME:
		scene_ = new GameScene();
		break;
	case SCENE_ID::SETTING:
		scene_ = new SettingScene();
		break;
	case SCENE_ID::BP_MINIGAME:
		scene_ = new BPMiniGameScene();
		break;
	case SCENE_ID::CLEAR:
		scene_ = new ClearScene();
		break;
	case SCENE_ID::DEBUG:
		scene_ = new DebugScene();
		break;
	}

	// 各シーンの初期化
	scene_->Init();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
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
