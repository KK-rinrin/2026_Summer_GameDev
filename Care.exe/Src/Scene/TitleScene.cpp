#include <array>
#include <DxLib.h>
#include <Windows.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "TitleScene.h"
#include "../Application.h"
#include "../Object/Talk/window/Live2DTalkController.h"

static constexpr int TITLE_IMAGE_POS_X = Application::SCREEN_SIZE_X / 2;
static constexpr int TITLE_IMAGE_BOTTOM_OFFSET = 500;
static constexpr int TITLE_IMAGE_POS_Y = Application::SCREEN_SIZE_Y - TITLE_IMAGE_BOTTOM_OFFSET;

TitleScene::TitleScene(void)
	:
	SceneBase(),
	imgTitle_(-1),
	font_(-1),
	selectMenu_(static_cast<int>(Menu::START)),
	liveTalkController_(nullptr)
{
}

TitleScene::~TitleScene(void)
{
	Delete();
}

void TitleScene::Update(void)
{
	UpdateSelectMenu();
	UpdatePlayerModel();
}

void TitleScene::Draw(void)
{
	DrawRotaGraph(TITLE_IMAGE_POS_X, TITLE_IMAGE_POS_Y, 1.0, 0.0, imgTitle_, TRUE);
	DrawPlayerModel();
	DrawSelectMenu();
}

void TitleScene::Delete(void)
{
	if (liveTalkController_)
	{
		liveTalkController_.reset();
	}
}

void TitleScene::InitLoad()
{
	// ハブからコントローラ共有を取得
	liveTalkController_ = Live2DModelHub::Instance().GetController(ResourceManager::SRC::PLAYER_MODEL);

	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE_IMG).handleId_;
	font_ = resMng_.Load(ResourceManager::SRC::TITLE_FONT).handleId_;
	liveTalkController_->SetExtendRate(PLAYER_MODEL_EXTEND_X, PLAYER_MODEL_EXTEND_Y);
	liveTalkController_->SetTranslate(PLAYER_MODEL_POS_X, PLAYER_MODEL_POS_Y);
}

void TitleScene::UpdateSelectMenu(void)
{
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_UP, iptMng_))
	{
		MoveSelectMenu(MENU_MOVE_PREV);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_DOWN, iptMng_))
	{
		MoveSelectMenu(MENU_MOVE_NEXT);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		DecideSelectMenu();
	}
}

void TitleScene::UpdatePlayerModel(void)
{
	liveTalkController_->SetExtendRate(PLAYER_MODEL_EXTEND_X, PLAYER_MODEL_EXTEND_Y);
	liveTalkController_->SetTranslate(PLAYER_MODEL_POS_X, PLAYER_MODEL_POS_Y);
	// モーションと表情の更新を Live2DTalkController に任せる
	// タイトル画面ではメニューに応じてモーションを変えて再生する
	if (selectMenu_ == static_cast<int>(Menu::START)) liveTalkController_->Update(TEXT("Pose_Start"));
	if (selectMenu_ == static_cast<int>(Menu::SETTING)) liveTalkController_->Update(TEXT("Pose_Setting"));
	if (selectMenu_ == static_cast<int>(Menu::CLOSE)) liveTalkController_->Update(TEXT("Pose_Close"));

}

void TitleScene::DrawPlayerModel(void)
{
	liveTalkController_->DrawBegin();
	liveTalkController_->DrawModel();
	liveTalkController_->DrawEnd();
}

void TitleScene::DrawSelectMenu(void)
{
	for (int i = 0; i < MENU_ITEM_NUM; ++i)
	{
		const int drawY = MENU_POS_Y + MENU_INTERVAL_Y * i;
		const int color = (i == selectMenu_) ? MENU_SELECTED_COLOR : MENU_COLOR;
		const char* mark = (i == selectMenu_) ? SELECT_MARK : MENU_MARK_SPACE;
		DrawFormatStringToHandle(MENU_POS_X, drawY, color, font_, "%s%s", mark, MENU_TEXTS[i]);
	}
}

void TitleScene::MoveSelectMenu(int move)
{
	selectMenu_ = (selectMenu_ + move + MENU_ITEM_NUM) % MENU_ITEM_NUM;
}

void TitleScene::DecideSelectMenu(void)
{
	switch (static_cast<Menu>(selectMenu_))
	{
	case Menu::START:
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		break;
	case Menu::SETTING:
		sceMng_.ChangeScene(SceneManager::SCENE_ID::SETTING);
		break;
	case Menu::CLOSE:
		PostQuitMessage(0);
		break;
	}
}