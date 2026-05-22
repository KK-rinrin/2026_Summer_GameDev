#pragma once
#include "SceneBase.h"
#include "../Object/Common/2DTransform.h"
#include <memory>

class Live2DTalkController;

class TitleScene : public SceneBase
{


	static constexpr float PLAYER_MODEL_EXTEND_X = 2.5f;
	static constexpr float PLAYER_MODEL_EXTEND_Y = 2.5f;
	static constexpr float PLAYER_MODEL_POS_X = -240.0f;
	static constexpr float PLAYER_MODEL_POS_Y = -300.0f;

	static constexpr int MENU_POS_X = 520;
	static constexpr int MENU_POS_Y = 285;
	static constexpr int MENU_INTERVAL_Y = 54;
	static constexpr int MENU_COLOR = 0x7a7a7a;
	static constexpr int MENU_SELECTED_COLOR = 0x333333;
	static constexpr const char* SELECT_MARK = "\xE2\x96\xB6 ";
	static constexpr const char* MENU_MARK_SPACE = "   ";
	static constexpr std::array<const char*, 3> MENU_TEXTS = { "Start", "Setting", "Close" };

	static constexpr int PUSH_START_BLINK_INTERVAL = 30;
	static constexpr int MENU_ITEM_NUM = 3;
public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Delete(void) override;

private:
	enum class Menu
	{
		START,
		SETTING,
		CLOSE,
	};

	void InitLoad() override;

	void UpdateSelectMenu(void);

	void UpdatePlayerModel(void);

	void DrawPlayerModel(void);

	void DrawSelectMenu(void);

	void MoveSelectMenu(int move);

	void DecideSelectMenu(void);

	int imgTitle_;
	int font_;
	int selectMenu_;
	std::shared_ptr<Live2DTalkController> liveTalkController_;
};