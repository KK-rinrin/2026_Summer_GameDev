#pragma once
#include <array>
#include <functional>
#include "SceneBase.h"

class SoundManager;

class SettingScene : public SceneBase
{

public:
	enum class Item
	{
		BGM_VOLUME,
		SE_VOLUME,
		KEY_CONFIG,
		PAD_CONFIG,
		// TEXT_SIZE,	// 余裕あれば実装
		TITLE,
		MAX
	};

	static constexpr int ITEM_POS_X = 100;
	static constexpr int ITEM_POS_Y = 210;
	static constexpr int ITEM_INTERVAL_Y = 60;
	static constexpr int ITEM_COLOR = 0x666666;
	static constexpr int ITEM_SELECTED_COLOR = 0x222222;
	static constexpr int TITLE_POS_X = 50;
	static constexpr int TITLE_POS_Y = 50;
	static constexpr int TITLE_COLOR = 0x333333;
	static constexpr int INVALID_FONT_HANDLE = -1;
	static constexpr int INITIAL_SELECT_ITEM = 0;
	static constexpr int MENU_MOVE_PREV = -1;
	static constexpr int MENU_MOVE_NEXT = 1;
	static constexpr int VOLUME_MIN = 0;
	static constexpr int VOLUME_MAX = 100;
	static constexpr int VOLUME_STEP = 10;
	static constexpr int DETAIL_POS_X = 390;
	static constexpr int DETAIL_POS_Y = 210;
	static constexpr int DETAIL_INTERVAL_Y = 45;
	static constexpr int VOLUME_BAR_LENGTH = 10;
	static constexpr int KEY_STATE_NUM = 256;
	static constexpr int PAD_CONFIG_ACTION_NUM = 2;
	static constexpr const char* SELECT_MARK = "> ";
	static constexpr const char* MENU_MARK_SPACE = "  ";
	static constexpr const char* TITLE_TEXT = "Setting";

	// 項目テキスト(余裕あれば4番目に"テキストサイズ（倍率）"を追加)
	static constexpr std::array<const char*, static_cast<int>(Item::MAX)> 
		ITEM_TEXTS = { "BGM音量", "SE音量", "キーコンフィグ", "パッドコンフィグ", "タイトルへ"};

	SettingScene(void);
	~SettingScene(void) override;

	void Update(void) override;
	void Draw(void) override;
	void Delete(void) override;

private:
	void InitLoad(void) override;
	void MoveSelectItem(int move);
	bool IsItemVisible(Item item) const;
	void DecideSelectItem(void);
	void DrawItems(void);

	std::function<void(void)> ItemUpdate_;

	void UpdateItemBGMVol();
	void UpdateItemSEVol();
	void UpdateItemKeyCon();
	void UpdateItemPadCon();

	void DrawItemBGMVol();
	void DrawItemSEVol();
	void DrawItemKeyCon();
	void DrawItemPadCon();

	void FinishItemUpdate(void);
	void DrawVolume(int volume, const char* label);

	int fontTitle_;
	int font_;
	SoundManager& sndMng_;
	int selectItem_;
	int bgmVolume_;
	int seVolume_;
	int previousVolume_;
	int selectAction_;
	bool isWaitingKeyInput_;
	bool isKeyInputReady_;

};
