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

GameScene::GameScene(void)
	:
	SceneBase(),
	talk_(nullptr),
	canMove_(false),
	stage_(nullptr),
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

	if (stage_ != nullptr)
	{
		stage_->Update();
	}

	// シーン遷移
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	if (canMove_)
	{
		player_->Update();
		if (stage_ != nullptr)
		{
			stage_->ApplyMovementBlocks(*player_);
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
	if (stage_ != nullptr)
	{
		stage_->DrawBackground();
	}

	render_->Render();

	talk_->Draw();
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
	delete render_;
}

void GameScene::InitLoad()
{
	talk_ = new Talk();
	talk_->Load();
	// talk_->SetTalk(TalkDatas::TalkDataIndex::TALK_0);

	switch (currentStage_)
	{
	case Stage::PAT_ROOM:
		stage_ = new PatientRoom();
		break;
	case Stage::NURSE_STATION:
		stage_ = new NurceStation();
		break;
	}
	stage_->Init();

	player_ = new Player();
	player_->Init();

	patient_ = new Patient();
	patient_->Init();

	render_ = new Renderer2D();
	stage_->RegisterObjects(*render_);
	render_->AddActor(patient_);
	render_->AddActor(player_);
}