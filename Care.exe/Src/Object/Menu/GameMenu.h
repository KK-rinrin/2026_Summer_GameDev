#pragma once

#include <DxLib.h>
#include <memory>
#include "../../Application.h"
#include "../../Common/Vector2.h"

class InputManager;
class Live2DTalkController;

class GameMenu
{
public:
	enum class Mode
	{
		NORMAL,
		PAUSE,
	};

	enum class Result
	{
		NONE,
		RESUME,
		OPEN_SETTING,
		BACK_TO_TITLE,
	};

	GameMenu();
	~GameMenu();

	void Load();
	void Update(const InputManager& input);
	void Draw();
	void Delete();

	void Open(Mode mode);
	bool IsOpen() const { return isOpen_; }
	Result ConsumeResult();

private:
	enum class Item
	{
		RESUME,
		SETTING,
		BACK_TO_TITLE,
		MAX
	};

	static constexpr Vector2 TITLE_POS = { 35, 20 };
	static constexpr Vector2 MENU_TEXT_POS = { 70, 280 };
	static constexpr int MENU_TEXT_INTERVAL_Y = 70;
	static constexpr Vector2 PAUSE_TEXT_POS = { 340, 280 };
	static constexpr int SCREEN_LEFT = 0;
	static constexpr int SCREEN_TOP = 0;
	static constexpr int SCREEN_RIGHT = Application::SCREEN_SIZE_X;
	static constexpr int SCREEN_BOTTOM = Application::SCREEN_SIZE_Y;

	static constexpr int MENU_MOVE_PREV = -1;
	static constexpr int MENU_MOVE_NEXT = 1;
	static constexpr int MENU_BG_COLOR = 0xe6e6e6;
	static constexpr int MENU_TEXT_COLOR = 0x222222;
	static constexpr int MENU_SUB_TEXT_COLOR = 0x555555;

	static constexpr int PAUSE_BLEND_ALPHA = 150;
	static constexpr int PAUSE_TEXT_COLOR = 0xffffff;

	static constexpr VECTOR PLAYER_MODEL_EXTEND = { 3.0f, 3.0f, 0.0f };
	static constexpr VECTOR PLAYER_MODEL_POS = { 230.0f, -300.0f, 0.0f };

	static constexpr const char* TITLE_TEXT = "MENU";
	static constexpr const char* PAUSE_TEXT = "PAUSE";
	static constexpr int INVALID_FONT_HANDLE = -1;

	void UpdateNormal(const InputManager& input);
	void UpdatePause(const InputManager& input);
	void DrawNormal();
	void DrawPause();
	void DrawMenuItems();
	void DrawPlayerPreview();
	void UpdatePlayerModel();
	void DrawPlayerModel();
	void MoveSelectItem(int move);
	void DecideSelectItem();
	bool IsAnyMenuInput(const InputManager& input) const;
	const char* GetItemText(Item item) const;

	bool isOpen_;
	Mode mode_;
	Result result_;
	int fontTitle_;
	int font_;
	int selectItem_;
	std::shared_ptr<Live2DTalkController> playerController_;
};
