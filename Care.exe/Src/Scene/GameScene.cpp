#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "GameScene.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Talk/Talk.h"
#include "../Object/Actor/Player.h"
#include "../Object/Actor/Patient.h"

GameScene::GameScene(void)
	:
	SceneBase(),
	talk_(nullptr),
	canMove_(false),
	player_(nullptr)
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
		// 起動してすぐのみでやりたいことを記述
		talk_->SetTalk(TalkDatas::TalkDataIndex::TALK_0);
	}

	if (talk_->Update())
	{
		// 会話中は移動できない
		canMove_ = false;
	}
	else
	{
		// 会話が終了している場合、移動できるようにする
		canMove_ = true;
	}

	// シーン遷移
	if (iptMng_.IsTrgDown(KEY_INPUT_ESCAPE))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	if (canMove_)
	player_->Update();

	patient_->Update();

	firstUpdate_ = false;
}

void GameScene::Draw(void)
{
	DrawGraph(0, 0, BGHandle_[static_cast<int>(currentStage_)], false);

	patient_->Draw();
	player_->Draw();

	talk_->Draw();
}

void GameScene::Delete(void)
{
	talk_->Delete();
	delete talk_;

	delete player_;
	delete patient_;
}

void GameScene::InitLoad()
{
	talk_ = new Talk();
	talk_->Load();

	player_ = new Player();
	player_->Init();

	patient_ = new Patient();
	patient_->Init();

	BGHandle_[0] = resMng_.Load(ResourceManager::SRC::BG_1).handleId_;
	BGHandle_[1] = resMng_.Load(ResourceManager::SRC::BG_2).handleId_;
}