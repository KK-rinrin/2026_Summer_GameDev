#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "SettingScene.h"

SettingScene::SettingScene(void)
	:
	SceneBase(),
	font_(INVALID_FONT_HANDLE),
	selectItem_(INITIAL_SELECT_ITEM)
{
}

SettingScene::~SettingScene(void)
{
	Delete();
}

void SettingScene::Update(void)
{
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
	DrawStringToHandle(TITLE_POS_X, TITLE_POS_Y, TITLE_TEXT, TITLE_COLOR, font_);
	
	// キャンセルキーでタイトルに戻る旨の描画
	DrawStringToHandle(TITLE_POS_X, TITLE_POS_Y + ITEM_INTERVAL_Y,
		"キャンセルキーでタイトルに戻る", ITEM_COLOR, font_);

	// 項目の描画
	DrawItems();
}

void SettingScene::Delete(void)
{
}

void SettingScene::InitLoad(void)
{
	font_ = resMng_.Load(ResourceManager::SRC::SETTING_FONT).handleId_;
}

void SettingScene::MoveSelectItem(int move)
{
	int inum = static_cast<int>(Item::MAX);
	selectItem_ = (selectItem_ + move + inum) % inum;
}

void SettingScene::DecideSelectItem(void)
{
	switch (static_cast<Item>(selectItem_))
	{
	case Item::BGM_VOLUME:
		break;
	case Item::SE_VOLUME:
		break;
	case Item::KEY_CONFIG:

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
	for (int i = 0; i < static_cast<int>(Item::MAX); ++i)
	{
		const int drawY = ITEM_POS_Y + ITEM_INTERVAL_Y * i;
		const int color = (i == selectItem_) ? ITEM_SELECTED_COLOR : ITEM_COLOR;
		const char* mark = (i == selectItem_) ? SELECT_MARK : MENU_MARK_SPACE;
		DrawFormatStringToHandle(ITEM_POS_X, drawY, color, font_, "%s%s", mark, ITEM_TEXTS[i]);
	}
}