#include <DxLib.h>
#include <algorithm>
#include <string>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/ProgressManager.h"
#include "SettingScene.h"

namespace
{
	constexpr std::array<int, 10> HIDDEN_RESET_COMMAND =
	{
		KEY_INPUT_UP,
		KEY_INPUT_UP,
		KEY_INPUT_DOWN,
		KEY_INPUT_DOWN,
		KEY_INPUT_LEFT,
		KEY_INPUT_RIGHT,
		KEY_INPUT_LEFT,
		KEY_INPUT_RIGHT,
		KEY_INPUT_B,
		KEY_INPUT_A,
	};
}

constexpr Vector2 SettingScene::ITEM_POS;
constexpr Vector2 SettingScene::TITLE_POS;
constexpr Vector2 SettingScene::DETAIL_POS;
constexpr Vector2 SettingScene::RESET_CONFIRM_WINDOW_POS;
constexpr Vector2 SettingScene::RESET_CONFIRM_WINDOW_SIZE;
constexpr Vector2 SettingScene::RESET_CONFIRM_TEXT_OFFSET;

SettingScene::SettingScene(void)
	:
	SceneBase(),
	fontTitle_(INVALID_FONT_HANDLE),
	font_(INVALID_FONT_HANDLE),
	sndMng_(SoundManager::GetInstance()),
	selectItem_(INITIAL_SELECT_ITEM),
	bgmVolume_(sndMng_.GetVolumeBGM()),
	seVolume_(sndMng_.GetVolumeSE()),
	previousVolume_(VOLUME_MIN),
	selectAction_(0),
	isWaitingKeyInput_(false),
	isKeyInputReady_(false),
	hiddenResetCommandIndex_(0),
	isProgressResetConfirmOpen_(false),
	isProgressResetResultOpen_(false),
	isProgressResetYesSelected_(false),
	isProgressResetSucceeded_(false)
{
}

SettingScene::~SettingScene(void)
{
	Delete();
}

void SettingScene::Update(void)
{
	if (isProgressResetConfirmOpen_)
	{
		UpdateProgressResetConfirm();
		return;
	}

	if (!iptMng_.IsPadConnected() && static_cast<Item>(selectItem_) == Item::PAD_CONFIG)
	{
		MoveSelectItem(MENU_MOVE_NEXT);
	}

	if (ItemUpdate_)
	{
		ItemUpdate_();
		return;
	}

	if (UpdateHiddenResetCommand())
	{
		return;
	}

	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_UP, iptMng_))
	{
		MoveSelectItem(MENU_MOVE_PREV);

		sndMng_.PlaySE(SE::MOVE);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_DOWN, iptMng_))
	{
		MoveSelectItem(MENU_MOVE_NEXT);

		sndMng_.PlaySE(SE::MOVE);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		sndMng_.PlaySE(SE::DECIDE);

		DecideSelectItem();
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
	{
		sndMng_.PlaySE(SE::CANCEL);

		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void SettingScene::Draw(void)
{
	// タイトルの描画
	DrawStringToHandle(TITLE_POS.x, TITLE_POS.y, TITLE_TEXT, TITLE_COLOR, fontTitle_);
	
	// キャンセルキーでタイトルに戻る旨の描画
	DrawStringToHandle(TITLE_POS.x, TITLE_POS.y + ITEM_INTERVAL_Y,
		"キャンセルキーでタイトルに戻る", ITEM_COLOR, fontTitle_);

	// 項目の描画
	DrawItems();

	switch (static_cast<Item>(selectItem_))
	{
	case Item::BGM_VOLUME:
		DrawItemBGMVol();
		break;
	case Item::SE_VOLUME:
		DrawItemSEVol();
		break;
	case Item::KEY_CONFIG:
		DrawItemKeyCon();
		break;
	case Item::PAD_CONFIG:
		DrawItemPadCon();
		break;
	default:
		break;
	}

	if (isProgressResetConfirmOpen_)
	{
		DrawProgressResetConfirm();
	}
}

void SettingScene::Delete(void)
{
}

void SettingScene::InitLoad(void)
{
	fontTitle_ = resMng_.Load(ResourceManager::SRC::TITLE_FONT).handleId_;
	font_ = resMng_.Load(ResourceManager::SRC::SETTING_FONT).handleId_;
}

void SettingScene::MoveSelectItem(int move)
{
	int inum = static_cast<int>(Item::MAX);
	do
	{
		selectItem_ = (selectItem_ + move + inum) % inum;
	} while (!IsItemVisible(static_cast<Item>(selectItem_)));
}

bool SettingScene::IsItemVisible(Item item) const
{
	return item != Item::PAD_CONFIG || iptMng_.IsPadConnected();
}

void SettingScene::DecideSelectItem(void)
{
	switch (static_cast<Item>(selectItem_))
	{
	case Item::BGM_VOLUME:
		previousVolume_ = bgmVolume_;
		ItemUpdate_ = std::bind(&SettingScene::UpdateItemBGMVol, this);
		break;
	case Item::SE_VOLUME:
		previousVolume_ = seVolume_;
		ItemUpdate_ = std::bind(&SettingScene::UpdateItemSEVol, this);
		break;
	case Item::KEY_CONFIG:
		selectAction_ = 0;
		isWaitingKeyInput_ = false;
		isKeyInputReady_ = false;
		ItemUpdate_ = std::bind(&SettingScene::UpdateItemKeyCon, this);
		break;
	case Item::PAD_CONFIG:
		selectAction_ = 0;
		isWaitingKeyInput_ = false;
		isKeyInputReady_ = false;
		ItemUpdate_ = std::bind(&SettingScene::UpdateItemPadCon, this);
		break;
	case Item::TITLE:
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		break;
	default:
		break;
	}
}

void SettingScene::DrawItems(void)
{
	int drawIndex = 0;
	for (int i = 0; i < static_cast<int>(Item::MAX); ++i)
	{
		if (!IsItemVisible(static_cast<Item>(i)))
		{
			continue;
		}

		const int drawY = ITEM_POS.y + ITEM_INTERVAL_Y * drawIndex;
		const int color = (i == selectItem_) ? ITEM_SELECTED_COLOR : ITEM_COLOR;
		const char* mark = (i == selectItem_) ? Application::SELECT_MARK : Application::MENU_MARK_SPACE;
		DrawFormatStringToHandle(ITEM_POS.x, drawY, color, font_, "%s%s", mark, ITEM_TEXTS[i]);
		++drawIndex;
	}
}

bool SettingScene::UpdateHiddenResetCommand(void)
{
	for (const int keyCode : HIDDEN_RESET_COMMAND)
	{
		if (!iptMng_.IsTrgDown(keyCode))
		{
			continue;
		}

		return CheckHiddenResetCommandKey(keyCode);
	}

	return false;
}

bool SettingScene::CheckHiddenResetCommandKey(int keyCode)
{
	if (keyCode == HIDDEN_RESET_COMMAND[hiddenResetCommandIndex_])
	{
		++hiddenResetCommandIndex_;
		if (hiddenResetCommandIndex_ >= static_cast<int>(HIDDEN_RESET_COMMAND.size()))
		{
			OpenProgressResetConfirm();
			return true;
		}

		return false;
	}

	hiddenResetCommandIndex_ = (keyCode == HIDDEN_RESET_COMMAND[0]) ? 1 : 0;
	return false;
}

void SettingScene::OpenProgressResetConfirm(void)
{
	hiddenResetCommandIndex_ = 0;
	isProgressResetConfirmOpen_ = true;
	isProgressResetResultOpen_ = false;
	isProgressResetYesSelected_ = false;
	isProgressResetSucceeded_ = false;
	sndMng_.PlaySE(SE::DECIDE);
}

void SettingScene::UpdateProgressResetConfirm(void)
{
	if (isProgressResetResultOpen_)
	{
		if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_) ||
			KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
		{
			isProgressResetConfirmOpen_ = false;
			isProgressResetResultOpen_ = false;
			sndMng_.PlaySE(SE::DECIDE);
		}
		return;
	}

	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_LEFT, iptMng_) ||
		KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_RIGHT, iptMng_))
	{
		isProgressResetYesSelected_ = !isProgressResetYesSelected_;
		sndMng_.PlaySE(SE::MOVE);
	}

	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
	{
		isProgressResetConfirmOpen_ = false;
		sndMng_.PlaySE(SE::CANCEL);
		return;
	}

	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		if (isProgressResetYesSelected_)
		{
			isProgressResetSucceeded_ = prgMng_.ResetProgressCache();
			isProgressResetResultOpen_ = true;
		}
		else
		{
			isProgressResetConfirmOpen_ = false;
		}
		sndMng_.PlaySE(SE::DECIDE);
	}
}

void SettingScene::DrawProgressResetConfirm(void)
{
	const Vector2 windowEnd = {
		RESET_CONFIRM_WINDOW_POS.x + RESET_CONFIRM_WINDOW_SIZE.x,
		RESET_CONFIRM_WINDOW_POS.y + RESET_CONFIRM_WINDOW_SIZE.y
	};
	const Vector2 textPos = {
		RESET_CONFIRM_WINDOW_POS.x + RESET_CONFIRM_TEXT_OFFSET.x,
		RESET_CONFIRM_WINDOW_POS.y + RESET_CONFIRM_TEXT_OFFSET.y
	};

	DrawBox(RESET_CONFIRM_WINDOW_POS.x, RESET_CONFIRM_WINDOW_POS.y,
		windowEnd.x, windowEnd.y, RESET_CONFIRM_BG_COLOR, TRUE);
	DrawBox(RESET_CONFIRM_WINDOW_POS.x, RESET_CONFIRM_WINDOW_POS.y,
		windowEnd.x, windowEnd.y, RESET_CONFIRM_FRAME_COLOR, FALSE);

	if (isProgressResetResultOpen_)
	{
		const char* message = isProgressResetSucceeded_ ?
			"ゲーム進捗をリセットしました" :
			"ゲーム進捗のリセットに失敗しました";
		DrawStringToHandle(textPos.x, textPos.y,
			message, ITEM_SELECTED_COLOR, font_);
		DrawStringToHandle(textPos.x,
			textPos.y + RESET_CONFIRM_TEXT_LINE_INTERVAL_Y,
			"決定キーで閉じる", ITEM_COLOR, font_);
		return;
	}

	DrawStringToHandle(textPos.x, textPos.y,
		"ゲーム進捗をリセットしますか？", ITEM_SELECTED_COLOR, font_);
	DrawStringToHandle(textPos.x,
		textPos.y + RESET_CONFIRM_TEXT_LINE_INTERVAL_Y,
		"キャラクターファイルはそのまま再起動でリセットします", ITEM_COLOR, font_);
	DrawFormatStringToHandle(textPos.x,
		textPos.y + RESET_CONFIRM_TEXT_LINE_INTERVAL_Y * 2,
		ITEM_SELECTED_COLOR, font_, "%sはい    %sいいえ",
		isProgressResetYesSelected_ ? Application::SELECT_MARK : Application::MENU_MARK_SPACE,
		isProgressResetYesSelected_ ? Application::MENU_MARK_SPACE : Application::SELECT_MARK);
}
void SettingScene::UpdateItemBGMVol()
{
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_LEFT, iptMng_))
	{
		bgmVolume_ = (std::max)(VOLUME_MIN, bgmVolume_ - VOLUME_STEP);
		sndMng_.SetVolumeBGM(bgmVolume_);

		sndMng_.PlaySE(SE::MOVE);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_RIGHT, iptMng_))
	{
		bgmVolume_ = (std::min)(VOLUME_MAX, bgmVolume_ + VOLUME_STEP);
		sndMng_.SetVolumeBGM(bgmVolume_);

		sndMng_.PlaySE(SE::MOVE);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		FinishItemUpdate();

		sndMng_.PlaySE(SE::DECIDE);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
	{
		bgmVolume_ = previousVolume_;
		sndMng_.SetVolumeBGM(bgmVolume_);
		FinishItemUpdate();

		sndMng_.PlaySE(SE::CANCEL);
	}
}

void SettingScene::UpdateItemSEVol()
{
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_LEFT, iptMng_))
	{
		seVolume_ = (std::max)(VOLUME_MIN, seVolume_ - VOLUME_STEP);
		sndMng_.SetVolumeSE(seVolume_);

		sndMng_.PlaySE(SE::MOVE);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_RIGHT, iptMng_))
	{
		seVolume_ = (std::min)(VOLUME_MAX, seVolume_ + VOLUME_STEP);
		sndMng_.SetVolumeSE(seVolume_);

		sndMng_.PlaySE(SE::MOVE);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		FinishItemUpdate();

		sndMng_.PlaySE(SE::DECIDE);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
	{
		seVolume_ = previousVolume_;
		sndMng_.SetVolumeSE(seVolume_);
		FinishItemUpdate();

		sndMng_.PlaySE(SE::CANCEL);
	}
}

void SettingScene::UpdateItemKeyCon()
{
	if (!isWaitingKeyInput_)
	{
		if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_UP, iptMng_))
		{
			const int actionNum = static_cast<int>(KeyConfig::ACTION::MAX);
			selectAction_ = (selectAction_ + MENU_MOVE_PREV + actionNum) % actionNum;

			sndMng_.PlaySE(SE::MOVE);
		}
		if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_DOWN, iptMng_))
		{
			const int actionNum = static_cast<int>(KeyConfig::ACTION::MAX);
			selectAction_ = (selectAction_ + MENU_MOVE_NEXT) % actionNum;

			sndMng_.PlaySE(SE::MOVE);
		}
		if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
		{
			isWaitingKeyInput_ = true;
			isKeyInputReady_ = false;

			sndMng_.PlaySE(SE::DECIDE);
		}
		if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
		{
			FinishItemUpdate();

			sndMng_.PlaySE(SE::CANCEL);
		}
		return;
	}

	char keyStates[KEY_STATE_NUM] = {};
	GetHitKeyStateAll(keyStates);

	if (!isKeyInputReady_)
	{
		isKeyInputReady_ = std::none_of(std::begin(keyStates), std::end(keyStates),
			[](char state) { return state != 0; });
		return;
	}

	for (int key = 0; key < KEY_STATE_NUM; ++key)
	{
		if (keyStates[key] == 0)
		{
			continue;
		}

		if (KeyConfig::SetConfigurableKey(static_cast<KeyConfig::ACTION>(selectAction_), key, iptMng_))
		{
			isWaitingKeyInput_ = false;
			isKeyInputReady_ = false;

			sndMng_.PlaySE(SE::DECIDE);
		}
		else
		{
			sndMng_.PlaySE(SE::BEEP);
		}
		return;
	}
}

void SettingScene::UpdateItemPadCon()
{
	if (!iptMng_.IsPadConnected())
	{
		FinishItemUpdate();
		return;
	}

	if (!isWaitingKeyInput_)
	{
		if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_UP, iptMng_))
		{
			selectAction_ = (selectAction_ + MENU_MOVE_PREV + PAD_CONFIG_ACTION_NUM) % PAD_CONFIG_ACTION_NUM;
		}
		if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_DOWN, iptMng_))
		{
			selectAction_ = (selectAction_ + MENU_MOVE_NEXT) % PAD_CONFIG_ACTION_NUM;
		}
		if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
		{
			isWaitingKeyInput_ = true;
			isKeyInputReady_ = false;
		}
		if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
		{
			FinishItemUpdate();
		}
		return;
	}

	if (!isKeyInputReady_)
	{
		isKeyInputReady_ = true;
		for (int i = 0; i < static_cast<int>(InputManager::JOYPAD_BTN::MAX); ++i)
		{
			if (iptMng_.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, static_cast<InputManager::JOYPAD_BTN>(i)))
			{
				isKeyInputReady_ = false;
				break;
			}
		}
		return;
	}

	for (int i = 0; i < static_cast<int>(InputManager::JOYPAD_BTN::MAX); ++i)
	{
		const auto button = static_cast<InputManager::JOYPAD_BTN>(i);
		if (!iptMng_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, button))
		{
			continue;
		}

		if (KeyConfig::SetConfigurablePadButton(static_cast<KeyConfig::ACTION>(selectAction_), button))
		{
			isWaitingKeyInput_ = false;
			isKeyInputReady_ = false;
		}
		return;
	}
}

void SettingScene::DrawItemBGMVol()
{
	DrawVolume(bgmVolume_, "BGM");
}

void SettingScene::DrawItemSEVol()
{
	DrawVolume(seVolume_, "SE");
}

void SettingScene::DrawItemKeyCon()
{
	for (int i = 0; i < static_cast<int>(KeyConfig::ACTION::MAX); ++i)
	{
		const auto* binding = KeyConfig::FindBinding(static_cast<KeyConfig::ACTION>(i));
		const int key = (binding == nullptr) ? KeyConfig::INVALID_KEY : binding->configurableKey;
		const bool isSelected = ItemUpdate_ && i == selectAction_;
		const int color = isSelected ? ITEM_SELECTED_COLOR : ITEM_COLOR;
		const char* mark = isSelected ? Application::SELECT_MARK : Application::MENU_MARK_SPACE;

		DrawFormatStringToHandle(DETAIL_POS.x, DETAIL_POS.y + DETAIL_INTERVAL_Y * i,
			color, font_, "%s%s : %s", mark,
			KeyConfig::GetActionText(static_cast<KeyConfig::ACTION>(i)), KeyConfig::GetKeyText(key));
	}

	if (isWaitingKeyInput_)
	{
		DrawStringToHandle(DETAIL_POS.x, DETAIL_POS.y + DETAIL_INTERVAL_Y * static_cast<int>(KeyConfig::ACTION::MAX),
			"割り当てるキーを押してください", ITEM_SELECTED_COLOR, font_);
	}
}

void SettingScene::DrawItemPadCon()
{
	for (int i = 0; i < PAD_CONFIG_ACTION_NUM; ++i)
	{
		const auto action = static_cast<KeyConfig::ACTION>(i);
		const auto* binding = KeyConfig::FindPadBinding(action);
		const auto button = (binding == nullptr) ? InputManager::JOYPAD_BTN::MAX : binding->configurableButton;
		const bool isSelected = ItemUpdate_ && i == selectAction_;
		const int color = isSelected ? ITEM_SELECTED_COLOR : ITEM_COLOR;
		const char* mark = isSelected ? Application::SELECT_MARK : Application::MENU_MARK_SPACE;

		DrawFormatStringToHandle(DETAIL_POS.x, DETAIL_POS.y + DETAIL_INTERVAL_Y * i,
			color, font_, "%s%s : %s", mark, KeyConfig::GetActionText(action),
			KeyConfig::GetPadButtonText(button));
	}

	if (isWaitingKeyInput_)
	{
		DrawStringToHandle(DETAIL_POS.x, DETAIL_POS.y + DETAIL_INTERVAL_Y * PAD_CONFIG_ACTION_NUM,
			"割り当てるパッドボタンを押してください", ITEM_SELECTED_COLOR, font_);
	}
}

void SettingScene::FinishItemUpdate(void)
{
	ItemUpdate_ = nullptr;
	isWaitingKeyInput_ = false;
	isKeyInputReady_ = false;
}

void SettingScene::DrawVolume(int volume, const char* label)
{
	std::string bar(VOLUME_BAR_LENGTH, '-');
	const int filledLength = volume / VOLUME_STEP;
	std::fill_n(bar.begin(), filledLength, '#');

	DrawFormatStringToHandle(DETAIL_POS.x, DETAIL_POS.y, ITEM_SELECTED_COLOR, font_,
		"%s [%s] %d", label, bar.c_str(), volume);

	if (ItemUpdate_)
	{
		DrawStringToHandle(DETAIL_POS.x, DETAIL_POS.y + DETAIL_INTERVAL_Y,
			"左右:変更  決定:確定  キャンセル:戻す", ITEM_COLOR, font_);
	}
}
