#pragma once
#include "../Application.h"
#include <string>
#include "SceneBase.h"
#include "../Manager/ProgressManager.h"
#include "../Manager/ResourceManager.h"

class SoundManager;

class ClearScene : public SceneBase
{
public:
	ClearScene();
	~ClearScene() override;

	void Update() override;
	void Draw() override;
	void Delete() override;

private:
	static constexpr int GAME_OVER_MAX_LINES = 24;
	static constexpr int DX_MAX = 255;
	static constexpr int KEY_STATE_BUFFER_SIZE = 256;
	static constexpr int GAME_OVER_POS_X = 16;
	static constexpr int GAME_OVER_POS_Y = 16;
	static constexpr int GAME_OVER_TEXT_RED = 200;
	static constexpr int GAME_OVER_TEXT_GREEN = 255;
	static constexpr int GAME_OVER_TEXT_BLUE = 200;
	static constexpr int GAME_OVER_LINE_INTERVAL_Y = 24;
	static constexpr int END_TITLE_POS_X = 24;

	static constexpr int TITLE_FONT_SIZE = 38;
	static constexpr int RESET_FONT_SIZE = 30;

	static constexpr int END_TITLE_SLIDE_DISTANCE_X = 20;
	static constexpr int END_TITLE_BOTTOM_MARGIN = 32;
	static constexpr int END_TITLE_ANIMATION_FRAMES = 60;
	static constexpr int END_TITLE_MAX_ALPHA = DX_MAX;
	static constexpr int END_TITLE_COLOR = Application::DEFAULT_COLOR_0;
	static constexpr int END_STILL_HOLD_FRAMES = 180;

	static constexpr int CREDIT_FADE_IN_FRAMES = 30;
	static constexpr int CREDIT_HOLD_FRAMES = 60;
	static constexpr int CREDIT_FINAL_HOLD_FRAMES = 600;
	static constexpr int CREDIT_FADE_OUT_FRAMES = 30;
	static constexpr int CREDIT_ROLE_POS_Y = 422;
	static constexpr int CREDIT_NAME_POS_Y = 482;
	static constexpr int CREDIT_RIGHT_MARGIN = 30;
	static constexpr int CREDIT_SHADOW_OFFSET = 2;
	static constexpr int CREDIT_ROLE_COLOR = Application::DEFAULT_COLOR_6;
	static constexpr int CREDIT_NAME_COLOR = Application::DEFAULT_COLOR_8;

	static constexpr int HIDDEN_RESET_HOLD_FRAMES = 180;
	static constexpr float HIDDEN_RESET_STICK_UP = -0.8f;

	static constexpr int RESET_WINDOW_LEFT = 50;
	static constexpr int RESET_WINDOW_TOP = 150;
	static constexpr int RESET_WINDOW_RIGHT = 750;
	static constexpr int RESET_WINDOW_BOTTOM = 370;
	static constexpr int RESET_TEXT_POS_X = 100;
	static constexpr int RESET_TEXT_POS_Y = 195;
	static constexpr int RESET_TEXT_LINE_INTERVAL = 45;
	static constexpr int RESET_WINDOW_BG_COLOR = Application::DEFAULT_COLOR_8;
	static constexpr int RESET_WINDOW_FRAME_COLOR = Application::DEFAULT_COLOR_1;
	static constexpr int RESET_TEXT_COLOR = Application::DEFAULT_COLOR_1;
	static constexpr int RESET_SUB_TEXT_COLOR = Application::DEFAULT_COLOR_4;

	struct EndInfo
	{
		ProgressManager::STORY_PROGRESS progress;
		ResourceManager::SRC stillSrc;
		std::string title;
	};

	void InitLoad() override;
	void InitPost() override;

	void InitGameOver();

	void UpdateGameOver();
	void UpdateRuinedEnd();
	void UpdateCredits();
	void UpdateHiddenReset();
	void UpdateResetComplete();

	void DrawGameOver() const;
	void DrawEndTitle() const;
	void DrawCredits() const;
	void DrawResetComplete() const;

	// ゲッター
	bool IsCreditSkipTriggered() const;
	bool IsFinalCredit() const;
	int GetCreditHoldFrames() const;
	int GetCreditCount() const;
	int GetCreditAlpha() const;

	// ゲームオーバー処理
	void AddLineGameOver(const std::string& line);
	bool IsRuinedEnd() const 
	{
		return prgMng_.GetProgressEnum() == ProgressManager::END_RUINED_LOCKED;
	}								// 崩壊ENDのみ特殊処理を行うための判定

	const EndInfo& GetEndInfo() const;
	ResourceManager::SRC GetStillSrc() const;
	std::string GetEndTitle() const;

	enum class GameOverState
	{
		INITIAL_CONFIRM,
		EXIT_CONFIRM,
		WAIT_EXIT
	};

	int stillHandle_;
	int titleFontHandle_;
	int resetFontHandle_;
	int endTitleAnimationFrame_;
	int endStillHoldFrame_;
	int creditIndex_;
	int creditFrame_;
	int hiddenResetHoldFrame_;
	std::string endTitle_;
	bool isGameOver_;
	bool isCreditsActive_;
	bool isExitConfirm_;
	bool isResetCompleteOpen_;
	GameOverState gameOverState_;
	std::string gameOverLines_[GAME_OVER_MAX_LINES];
	int gameOverLineCount_;
	char prevKeyState_[KEY_STATE_BUFFER_SIZE];

	bool isFadeOut_ = false;

	SoundManager* sndMng_;
};
