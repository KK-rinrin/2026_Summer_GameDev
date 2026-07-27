#pragma once
#include "SceneBase.h"
#include "../Manager/ProgressManager.h"
#include "../Manager/ResourceManager.h"

class ClearScene : public SceneBase
{
public:
	ClearScene(void);
	~ClearScene(void) override;

	void Update(void) override;
	void Draw(void) override;
	void Delete(void) override;

private:
	struct EndInfo
	{
		ProgressManager::STORY_PROGRESS progress;
		ResourceManager::SRC stillSrc;
		const char* title;
	};

	void InitLoad(void) override;
	void InitGameOver(void);
	void UpdateGameOver(void);
	void DrawGameOver(void) const;
	void DrawEndTitle(void) const;
	void UpdateCredits(void);
	void DrawCredits(void) const;
	void UpdateHiddenReset(void);
	void UpdateResetComplete(void);
	void DrawResetComplete(void) const;
	bool IsCreditSkipTriggered(void) const;
	bool IsFinalCredit(void) const;
	int GetCreditHoldFrames(void) const;
	int GetCreditAlpha(void) const;
	void AddGameOverLine(const char* line);
	bool IsAnyKeyTrgDown(void);
	bool IsAnyPadButtonTrgDown(void) const;
	const EndInfo& GetEndInfo(void) const;
	ResourceManager::SRC GetStillSrc(void) const;
	std::string GetEndTitle(void) const;

	enum class GameOverState
	{
		INITIAL_CONFIRM,
		EXIT_CONFIRM,
		WAIT_EXIT
	};

	static constexpr int GAME_OVER_MAX_LINES = 24;
	static constexpr int GAME_OVER_LINE_INTERVAL_Y = 24;
	static constexpr int END_TITLE_POS_X = 24;
	static constexpr int END_TITLE_SLIDE_DISTANCE_X = 20;
	static constexpr int END_TITLE_BOTTOM_MARGIN = 32;
	static constexpr int END_TITLE_ANIMATION_FRAMES = 60;
	static constexpr int END_TITLE_MAX_ALPHA = 255;
	static constexpr int END_TITLE_COLOR = 0x000000;
	static constexpr int END_STILL_HOLD_FRAMES = 180;
	static constexpr int CREDIT_FADE_IN_FRAMES = 30;
	static constexpr int CREDIT_HOLD_FRAMES = 60;
	static constexpr int CREDIT_FINAL_HOLD_FRAMES = 600;
	static constexpr int CREDIT_FADE_OUT_FRAMES = 30;
	static constexpr int CREDIT_ROLE_POS_Y = 422;
	static constexpr int CREDIT_NAME_POS_Y = 482;
	static constexpr int CREDIT_RIGHT_MARGIN = 30;
	static constexpr int CREDIT_SHADOW_OFFSET = 2;
	static constexpr int CREDIT_ROLE_COLOR = 0xaaaaaa;
	static constexpr int CREDIT_NAME_COLOR = 0xffffff;
	static constexpr int HIDDEN_RESET_HOLD_FRAMES = 180;
	static constexpr float HIDDEN_RESET_STICK_UP = -0.8f;
	static constexpr int RESET_WINDOW_LEFT = 50;
	static constexpr int RESET_WINDOW_TOP = 150;
	static constexpr int RESET_WINDOW_RIGHT = 750;
	static constexpr int RESET_WINDOW_BOTTOM = 370;
	static constexpr int RESET_TEXT_POS_X = 100;
	static constexpr int RESET_TEXT_POS_Y = 195;
	static constexpr int RESET_TEXT_LINE_INTERVAL = 45;
	static constexpr int RESET_WINDOW_BG_COLOR = 0xffffff;
	static constexpr int RESET_WINDOW_FRAME_COLOR = 0x222222;
	static constexpr int RESET_TEXT_COLOR = 0x222222;
	static constexpr int RESET_SUB_TEXT_COLOR = 0x666666;

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
	bool isExitRequested_;
	bool isResetCompleteOpen_;
	GameOverState gameOverState_;
	const char* gameOverLines_[GAME_OVER_MAX_LINES];
	int gameOverLineCount_;
	char previousKeyState_[256];
};
