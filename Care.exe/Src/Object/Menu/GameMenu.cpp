#include <DxLib.h>
#include "GameMenu.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/KeyConfig.h"
#include "../../Manager/Live2DModelHub.h"
#include "../../Manager/ProgressManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"
#include "../Talk/window/Live2DTalkController.h"

constexpr Vector2 GameMenu::TITLE_POS;
constexpr Vector2 GameMenu::MENU_TEXT_POS;
constexpr Vector2 GameMenu::PAUSE_TEXT_POS;
constexpr VECTOR GameMenu::PLAYER_MODEL_EXTEND;
constexpr VECTOR GameMenu::PLAYER_MODEL_POS;

GameMenu::GameMenu()
	:
	isOpen_(false),
	mode_(Mode::NORMAL),
	result_(Result::NONE),
	fontTitle_(INVALID_FONT_HANDLE),
	font_(INVALID_FONT_HANDLE),
	selectItem_(static_cast<int>(Item::RESUME)),
	playerController_(nullptr)
{
}

GameMenu::~GameMenu()
{
	Delete();
}

void GameMenu::Load()
{
	ResourceManager& resMng = ResourceManager::GetInstance();
	fontTitle_ = resMng.Load(ResourceManager::SRC::TITLE_FONT).handleId_;
	font_ = resMng.Load(ResourceManager::SRC::SETTING_FONT).handleId_;

	if (ProgressManager::GetInstance().IsNurceCharExists())
	{
		playerController_ = Live2DModelHub::Instance().GetController(ResourceManager::SRC::PLAYER_MODEL);
		UpdatePlayerModel();
	}
}

void GameMenu::Update(const InputManager& input)
{
	result_ = Result::NONE;

	if (!isOpen_)
	{
		return;
	}

	switch (mode_)
	{
	case Mode::NORMAL:
		UpdatePlayerModel();
		UpdateNormal(input);
		break;
	case Mode::PAUSE:
		UpdatePause(input);
		break;
	}
}

void GameMenu::Draw()
{
	if (!isOpen_)
	{
		return;
	}

	switch (mode_)
	{
	case Mode::NORMAL:
		DrawNormal();
		break;
	case Mode::PAUSE:
		DrawPause();
		break;
	}
}

void GameMenu::Delete()
{
	fontTitle_ = INVALID_FONT_HANDLE;
	font_ = INVALID_FONT_HANDLE;
	if (playerController_)
	{
		playerController_.reset();
	}
}

void GameMenu::Open(Mode mode)
{
	isOpen_ = true;
	mode_ = mode;
	result_ = Result::NONE;
	selectItem_ = static_cast<int>(Item::RESUME);
}

GameMenu::Result GameMenu::ConsumeResult()
{
	const Result result = result_;
	result_ = Result::NONE;
	return result;
}

void GameMenu::UpdateNormal(const InputManager& input)
{
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_UP, input))
	{
		MoveSelectItem(MENU_MOVE_PREV);
		SoundManager::GetInstance().PlaySE(SE::MOVE);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_DOWN, input))
	{
		MoveSelectItem(MENU_MOVE_NEXT);
		SoundManager::GetInstance().PlaySE(SE::MOVE);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, input))
	{
		DecideSelectItem();
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, input))
	{
		SoundManager::GetInstance().PlaySE(SE::CANCEL);
		isOpen_ = false;
		result_ = Result::RESUME;
	}
}

void GameMenu::UpdatePause(const InputManager& input)
{
	if (IsAnyMenuInput(input))
	{
		SoundManager::GetInstance().PlaySE(SE::CANCEL);
		isOpen_ = false;
		result_ = Result::RESUME;
	}
}

void GameMenu::DrawNormal()
{
	DrawBox(SCREEN_LEFT, SCREEN_TOP, SCREEN_RIGHT, SCREEN_BOTTOM, MENU_BG_COLOR, TRUE);
	DrawStringToHandle(TITLE_POS.x, TITLE_POS.y, TITLE_TEXT, MENU_TEXT_COLOR, fontTitle_);
	DrawMenuItems();
	DrawPlayerPreview();
}

void GameMenu::DrawPause()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, PAUSE_BLEND_ALPHA);
	DrawBox(SCREEN_LEFT, SCREEN_TOP, SCREEN_RIGHT, SCREEN_BOTTOM, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawStringToHandle(PAUSE_TEXT_POS.x, PAUSE_TEXT_POS.y, PAUSE_TEXT, PAUSE_TEXT_COLOR, fontTitle_);
}

void GameMenu::DrawMenuItems()
{
	for (int i = 0; i < static_cast<int>(Item::MAX); ++i)
	{
		const int drawY = MENU_TEXT_POS.y + MENU_TEXT_INTERVAL_Y * i;
		const char* mark = (i == selectItem_) ? Application::SELECT_MARK : Application::MENU_MARK_SPACE;
		const int color = (i == selectItem_) ? MENU_TEXT_COLOR : MENU_SUB_TEXT_COLOR;
		DrawFormatStringToHandle(MENU_TEXT_POS.x, drawY, color, font_, "%s%s", mark, GetItemText(static_cast<Item>(i)));
	}
}

void GameMenu::DrawPlayerPreview()
{
	DrawPlayerModel();
}

void GameMenu::UpdatePlayerModel()
{
	if (playerController_ == nullptr)
	{
		return;
	}

	playerController_->SetExtendRate(PLAYER_MODEL_EXTEND.x, PLAYER_MODEL_EXTEND.y);
	playerController_->SetTranslate(PLAYER_MODEL_POS.x, PLAYER_MODEL_POS.y);
	playerController_->SetAlpha(255);
	playerController_->Update(TEXT("Pose_Menu"));
}

void GameMenu::DrawPlayerModel()
{
	if (playerController_ == nullptr)
	{
		return;
	}

	playerController_->DrawBegin();
	playerController_->DrawModel();
	playerController_->DrawEnd();
}

void GameMenu::MoveSelectItem(int move)
{
	selectItem_ = (selectItem_ + move + static_cast<int>(Item::MAX)) % static_cast<int>(Item::MAX);
}

void GameMenu::DecideSelectItem()
{
	SoundManager::GetInstance().PlaySE(SE::DECIDE);

	switch (static_cast<Item>(selectItem_))
	{
	case Item::RESUME:
		isOpen_ = false;
		result_ = Result::RESUME;
		break;
	case Item::BACK_TO_TITLE:
		isOpen_ = false;
		result_ = Result::BACK_TO_TITLE;
		break;
	}
}

bool GameMenu::IsAnyMenuInput(const InputManager& input) const
{
	for (int i = 0; i < static_cast<int>(KeyConfig::ACTION::MAX); ++i)
	{
		if (KeyConfig::IsTrgDown(static_cast<KeyConfig::ACTION>(i), input))
		{
			return true;
		}
	}

	return false;
}

const char* GameMenu::GetItemText(Item item) const
{
	switch (item)
	{
	case Item::RESUME:
		return "Resume";
	case Item::BACK_TO_TITLE:
		return "Back to Title";
	default:
		return "";
	}
}
