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
	static constexpr int GAME_OVER_MAX_LINES = 24;
	static constexpr int DX_MAX = 255;

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

	// ゲッター
	bool IsCreditSkipTriggered() const;
	bool IsFinalCredit() const;
	int GetCreditHoldFrames() const;
	int GetCreditAlpha() const;

	// ゲームオーバー処理
	void AddGameOverLine(const char* line);
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
	const char* gameOverLines_[GAME_OVER_MAX_LINES];
	int gameOverLineCount_;
	char prevKeyState_[256];

	bool isFadeOut_ = false;

	SoundManager* sndMng_;
};
