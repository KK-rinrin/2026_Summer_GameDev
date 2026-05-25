#pragma once
#include <array>
#include "SceneBase.h"

class SettingScene : public SceneBase
{

public:
	enum class Item
	{
		BGM_VOLUME,
		SE_VOLUME,
		KEY_CONFIG,
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
	static constexpr const char* SELECT_MARK = "> ";
	static constexpr const char* MENU_MARK_SPACE = "  ";
	static constexpr const char* TITLE_TEXT = "Setting";

	// 項目テキスト(余裕あれば4番目に"テキストサイズ（倍率）"を追加)
	static constexpr std::array<const char*, static_cast<int>(Item::MAX)> 
		ITEM_TEXTS = { "BGM音量", "SE音量", "キーコンフィグ", "タイトルへ"};

	SettingScene(void);
	~SettingScene(void) override;

	void Update(void) override;
	void Draw(void) override;
	void Delete(void) override;

	

private:
	void InitLoad(void) override;
	void MoveSelectItem(int move);
	void DecideSelectItem(void);
	void DrawItems(void);

	int fontTitle_;
	int font_;
	int selectItem_;

};
