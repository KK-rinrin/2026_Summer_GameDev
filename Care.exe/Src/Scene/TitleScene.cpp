#include <array>
#include <DxLib.h>
#include <Windows.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "TitleScene.h"
#include "../Application.h"
#include "../Object/Talk/window/Live2DTalkController.h"

namespace
{
	static constexpr int TITLE_IMAGE_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int TITLE_IMAGE_BOTTOM_OFFSET = 500;
	static constexpr int TITLE_IMAGE_POS_Y = Application::SCREEN_SIZE_Y - TITLE_IMAGE_BOTTOM_OFFSET;

	static constexpr float PLAYER_MODEL_EXTEND_X = 2.5f;
	static constexpr float PLAYER_MODEL_EXTEND_Y = 2.5f;
	static constexpr float PLAYER_MODEL_POS_X = -240.0f;
	static constexpr float PLAYER_MODEL_POS_Y = -300.0f;

	static constexpr int MENU_POS_X = 520;
	static constexpr int MENU_POS_Y = 285;
	static constexpr int MENU_INTERVAL_Y = 54;
	static constexpr int MENU_COLOR = 0x555555;
	static constexpr int MENU_SELECTED_COLOR = 0x333333;
	static constexpr const char* SELECT_MARK = "\xE2\x96\xB6 ";
	static constexpr const char* MENU_MARK_SPACE = "   ";
	static constexpr std::array<const char*, 3> MENU_TEXTS = { "Start", "Setting", "Close" };
}

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
	if (iptMng_.IsTrgDown(KEY_INPUT_UP) || iptMng_.IsTrgDown(KEY_INPUT_W))
	{
		MoveSelectMenu(-1);
	}
	if (iptMng_.IsTrgDown(KEY_INPUT_DOWN) || iptMng_.IsTrgDown(KEY_INPUT_S))
	{
		MoveSelectMenu(1);
	}
	if (iptMng_.IsTrgDown(KEY_INPUT_RETURN) || iptMng_.IsTrgDown(KEY_INPUT_SPACE))
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
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	for (int i = 0; i < MENU_ITEM_NUM; ++i)
	{
		const int drawY = MENU_POS_Y + MENU_INTERVAL_Y * i;
		const int color = (i == selectMenu_) ? MENU_SELECTED_COLOR : MENU_COLOR;
		const char* mark = (i == selectMenu_) ? SELECT_MARK : MENU_MARK_SPACE;
		DrawFormatStringToHandle(MENU_POS_X, drawY, color, font_, "%s%s", mark, MENU_TEXTS[i]);
	}
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_SHIFTJIS);
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
		break;
	case Menu::CLOSE:
		PostQuitMessage(0);
		break;
	}
}