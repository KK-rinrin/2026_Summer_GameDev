#pragma once
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
	struct EndInfo
	{
		ProgressManager::STORY_PROGRESS progress;
		ResourceManager::SRC stillSrc;
		const char* title;
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

	bool IsCreditSkipTriggered() const;
	bool IsFinalCredit() const;
	int GetCreditHoldFrames() const;
	int GetCreditAlpha() const;
	void AddGameOverLine(const char* line);
	bool IsAnyKeyTrgDown();
	bool IsAnyPadButtonTrgDown() const;
	bool IsRuinedEnd() const;
	const EndInfo& GetEndInfo() const;
	ResourceManager::SRC GetStillSrc() const;
	std::string GetEndTitle() const;

	enum class GameOverState
	{
		INITIAL_CONFIRM,
		EXIT_CONFIRM,
		WAIT_EXIT
	};

	static constexpr int GAME_OVER_MAX_LINES = 24;
	static constexpr int GAME_OVER_LINE_INTERVAL_Y = 24;
	static constexpr int END_TITLE_POS_X = 24;
	static constexpr int TITLE_FONT_SIZE = 38;
	static constexpr int RESET_FONT_SIZE = 30;
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

	bool isFadeOut_ = false;

	SoundManager* sndMng_;
};
