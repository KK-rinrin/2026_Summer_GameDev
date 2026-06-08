#include <DxLib.h>
#include <algorithm>
#include <string>
#include "../Manager/InputManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "SettingScene.h"

int SettingScene::savedBgmVolume_ = SettingScene::INITIAL_VOLUME;
int SettingScene::savedSeVolume_ = SettingScene::INITIAL_VOLUME;

SettingScene::SettingScene(void)
	:
	SceneBase(),
	fontTitle_(INVALID_FONT_HANDLE),
	font_(INVALID_FONT_HANDLE),
	selectItem_(INITIAL_SELECT_ITEM),
	bgmVolume_(savedBgmVolume_),
	seVolume_(savedSeVolume_),
	previousVolume_(INITIAL_VOLUME),
	selectAction_(0),
	isWaitingKeyInput_(false),
	isKeyInputReady_(false)
{
}

SettingScene::~SettingScene(void)
{
	Delete();
}

void SettingScene::Update(void)
{
	if (!iptMng_.IsPadConnected() && static_cast<Item>(selectItem_) == Item::PAD_CONFIG)
	{
		MoveSelectItem(MENU_MOVE_NEXT);
	}

	if (ItemUpdate_)
	{
		ItemUpdate_();
		return;
	}

	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_UP, iptMng_))
	{
		MoveSelectItem(MENU_MOVE_PREV);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_DOWN, iptMng_))
	{
		MoveSelectItem(MENU_MOVE_NEXT);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		DecideSelectItem();
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void SettingScene::Draw(void)
{
	// タイトルの描画
	DrawStringToHandle(TITLE_POS_X, TITLE_POS_Y, TITLE_TEXT, TITLE_COLOR, fontTitle_);
	
	// キャンセルキーでタイトルに戻る旨の描画
	DrawStringToHandle(TITLE_POS_X, TITLE_POS_Y + ITEM_INTERVAL_Y,
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

		const int drawY = ITEM_POS_Y + ITEM_INTERVAL_Y * drawIndex;
		const int color = (i == selectItem_) ? ITEM_SELECTED_COLOR : ITEM_COLOR;
		const char* mark = (i == selectItem_) ? SELECT_MARK : MENU_MARK_SPACE;
		DrawFormatStringToHandle(ITEM_POS_X, drawY, color, font_, "%s%s", mark, ITEM_TEXTS[i]);
		++drawIndex;
	}
}

void SettingScene::UpdateItemBGMVol()
{
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_LEFT, iptMng_))
	{
		bgmVolume_ = (std::max)(VOLUME_MIN, bgmVolume_ - VOLUME_STEP);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_RIGHT, iptMng_))
	{
		bgmVolume_ = (std::min)(VOLUME_MAX, bgmVolume_ + VOLUME_STEP);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		savedBgmVolume_ = bgmVolume_;
		FinishItemUpdate();
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
	{
		bgmVolume_ = previousVolume_;
		FinishItemUpdate();
	}
}

void SettingScene::UpdateItemSEVol()
{
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_LEFT, iptMng_))
	{
		seVolume_ = (std::max)(VOLUME_MIN, seVolume_ - VOLUME_STEP);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_RIGHT, iptMng_))
	{
		seVolume_ = (std::min)(VOLUME_MAX, seVolume_ + VOLUME_STEP);
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		savedSeVolume_ = seVolume_;
		FinishItemUpdate();
	}
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_))
	{
		seVolume_ = previousVolume_;
		FinishItemUpdate();
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
		}
		if (KeyConfig::IsTrgDown(KeyConfig::ACTION::MOVE_DOWN, iptMng_))
		{
			const int actionNum = static_cast<int>(KeyConfig::ACTION::MAX);
			selectAction_ = (selectAction_ + MENU_MOVE_NEXT) % actionNum;
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
		const char* mark = isSelected ? SELECT_MARK : MENU_MARK_SPACE;

		DrawFormatStringToHandle(DETAIL_POS_X, DETAIL_POS_Y + DETAIL_INTERVAL_Y * i,
			color, font_, "%s%s : %s", mark,
			KeyConfig::GetActionText(static_cast<KeyConfig::ACTION>(i)), KeyConfig::GetKeyText(key));
	}

	if (isWaitingKeyInput_)
	{
		DrawStringToHandle(DETAIL_POS_X, DETAIL_POS_Y + DETAIL_INTERVAL_Y * static_cast<int>(KeyConfig::ACTION::MAX),
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
		const char* mark = isSelected ? SELECT_MARK : MENU_MARK_SPACE;

		DrawFormatStringToHandle(DETAIL_POS_X, DETAIL_POS_Y + DETAIL_INTERVAL_Y * i,
			color, font_, "%s%s : %s", mark, KeyConfig::GetActionText(action),
			KeyConfig::GetPadButtonText(button));
	}

	if (isWaitingKeyInput_)
	{
		DrawStringToHandle(DETAIL_POS_X, DETAIL_POS_Y + DETAIL_INTERVAL_Y * PAD_CONFIG_ACTION_NUM,
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

	DrawFormatStringToHandle(DETAIL_POS_X, DETAIL_POS_Y, ITEM_SELECTED_COLOR, font_,
		"%s [%s] %d", label, bar.c_str(), volume);

	if (ItemUpdate_)
	{
		DrawStringToHandle(DETAIL_POS_X, DETAIL_POS_Y + DETAIL_INTERVAL_Y,
			"左右:変更  決定:確定  キャンセル:戻す", ITEM_COLOR, font_);
	}
}
