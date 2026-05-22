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

GameScene::GameScene(void)
	:
	SceneBase(),
	talk_(nullptr),
	canMove_(false),
	wallHandle_(-1),
	render_(nullptr),
	player_(nullptr),
	patient_(nullptr)
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

	if (canMove_)
	{
		player_->Update();
		if (currentStage_ == Stage::PAT_ROOM)
		{
			//player_->BlockCrossingWorldY(PICU_WALL_Y, PICU_WALL_THICKNESS);
		}

		// 決定キーで会話開始判定：プレイヤー位置（点）と Patient の会話円で判定
		// 会話中でなければ開始できる
		if (!isTalking)
		{
			const VECTOR pPos = player_->GetTransform().pos;
			const VECTOR patPos = patient_->GetTransform().pos;

			// 決定キーで会話開始判定
			if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
			{
				if (Collision::IsPointInCircle(pPos, patPos, Patient::TALK_RADIUS))
				{
					talk_->SetTalk(TalkDatas::TalkDataIndex::TALK_0);
				}
			}
		}

		if (player_->IsHitCircle(*patient_))
		{
			player_->MoveToBeforePos();
		}
	}

	patient_->Update();
}

void GameScene::Draw(void)
{
	DrawGraph(0, 0, BGHandle_[static_cast<int>(currentStage_)], false);

	render_->Render();

	talk_->Draw();
}

void GameScene::Delete(void)
{
	talk_->Delete();
	delete talk_;

	delete player_;
	delete patient_;
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
	render_->AddActor(patient_);
	//render_->Add(PICU_WALL_Y, [this]() { if (currentStage_ == Stage::PAT_ROOM) DrawGraph(0, 0, wallHandle_, true); });
	render_->AddActor(player_);

	BGHandle_[0] = resMng_.Load(ResourceManager::SRC::BG_1).handleId_;
	wallHandle_ = resMng_.Load(ResourceManager::SRC::BG_1_WALL).handleId_;
	BGHandle_[1] = resMng_.Load(ResourceManager::SRC::BG_2).handleId_;
}