#pragma once
#include <array>
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "SceneBase.h"
#include "../Object/Common/2DTransform.h"
#include <memory>

class Live2DTalkController;

class TitleScene : public SceneBase
{


	static constexpr VECTOR PLAYER_MODEL_EXTEND = { 2.5f, 2.5f, 0.0f };
	static constexpr VECTOR PLAYER_MODEL_POS = { -240.0f, -300.0f, 0.0f };

	static constexpr Vector2 MENU_POS = { 520, 285 };
	static constexpr int MENU_INTERVAL_Y = 54;
	static constexpr int MENU_MOVE_PREV = -1;
	static constexpr int MENU_MOVE_NEXT = 1;
	static constexpr int MENU_COLOR = 0x7a7a7a;
	static constexpr int MENU_SELECTED_COLOR = 0x333333;
	static constexpr int MENU_ITEM_NUM = 3;
	static constexpr std::array<const char*, MENU_ITEM_NUM> MENU_TEXTS = { "Start", "Setting", "Close" };

	static constexpr int PUSH_START_BLINK_INTERVAL = 30;
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