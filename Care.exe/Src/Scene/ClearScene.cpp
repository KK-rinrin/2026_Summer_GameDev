#include <DxLib.h>
#include <Windows.h>
#include <cstring>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/KeyConfig.h"
#include "ClearScene.h"

namespace
{
	struct CreditEntry
	{
		const char* role;
		const char* name;
	};

	const CreditEntry CREDIT_ENTRIES[] =
	{
		{ "", "Care.exe" },
		{ "企画・原案", "KK" },
		{ "ゲームデザイン", "KK" },
		{ "プログラム", "KK" },
		{ "シナリオ", "KK" },
		{ "キャラクターデザイン", "KK" },
		{ "グラフィック制作", "KK" },
		{ "Live2Dモデル制作", "KK" },
		{ "BGM・シナリオ協力", "神楽" },
		{ "効果音素材", "Oto Logic / 効果音ラボ" },
		{ "効果音素材", "On-Jin ～音人～ / 電脳プロダクション" },
		{ "効果音素材", "Sound Dino" },
		{ "使用ライブラリ", "DxLib / Live2D" },
		{ "制作", "KK" },
		{ "プレイしていただきありがとうございました", "Thank you for Playing" },
	};
	constexpr int CREDIT_ENTRY_COUNT = sizeof(CREDIT_ENTRIES) / sizeof(CREDIT_ENTRIES[0]);
}

ClearScene::ClearScene(void)
	:
	SceneBase(),
	stillHandle_(-1),
	titleFontHandle_(-1),
	resetFontHandle_(-1),
	endTitleAnimationFrame_(0),
	endStillHoldFrame_(0),
	creditIndex_(0),
	creditFrame_(0),
	hiddenResetHoldFrame_(0),
	isGameOver_(false),
	isCreditsActive_(false),
	isExitRequested_(false),
	isResetCompleteOpen_(false),
	gameOverState_(GameOverState::INITIAL_CONFIRM),
	gameOverLineCount_(0)
{
	std::memset(previousKeyState_, 0, sizeof(previousKeyState_));
}

ClearScene::~ClearScene(void)
{
}

void ClearScene::Update(void)
{
	if (isGameOver_)
	{
		UpdateGameOver();
		return;
	}

	if (isResetCompleteOpen_)
	{
		UpdateResetComplete();
		return;
	}

	if (!isCreditsActive_ || !IsFinalCredit())
	{
		UpdateHiddenReset();
	}
	if (isExitRequested_)
	{
		return;
	}

	if (isCreditsActive_)
	{
		UpdateCredits();
		return;
	}

	if (endTitleAnimationFrame_ < END_TITLE_ANIMATION_FRAMES)
	{
		++endTitleAnimationFrame_;
		return;
	}

	++endStillHoldFrame_;
	if (endStillHoldFrame_ >= END_STILL_HOLD_FRAMES)
	{
		isCreditsActive_ = true;
		creditIndex_ = 0;
		creditFrame_ = 0;
	}
}

void ClearScene::Draw(void)
{
	if (isGameOver_)
	{
		DrawGameOver();
		return;
	}

	if (isCreditsActive_)
	{
		DrawCredits();
	}
	else
	{
		if (stillHandle_ < 0)
		{
			DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
		}
		else
		{
			DrawExtendGraph(
				0, 0,
				Application::SCREEN_SIZE_X,
				Application::SCREEN_SIZE_Y,
				stillHandle_, true);
		}

		DrawEndTitle();
	}

	if (isResetCompleteOpen_)
	{
		DrawResetComplete();
	}
}

void ClearScene::Delete(void)
{
	stillHandle_ = -1;
	titleFontHandle_ = -1;
	resetFontHandle_ = -1;
	endTitleAnimationFrame_ = 0;
	endStillHoldFrame_ = 0;
	creditIndex_ = 0;
	creditFrame_ = 0;
	hiddenResetHoldFrame_ = 0;
	endTitle_.clear();
	isGameOver_ = false;
	isCreditsActive_ = false;
	isExitRequested_ = false;
	isResetCompleteOpen_ = false;
}

void ClearScene::InitLoad(void)
{
	endTitleAnimationFrame_ = 0;
	endStillHoldFrame_ = 0;
	creditIndex_ = 0;
	creditFrame_ = 0;
	hiddenResetHoldFrame_ = 0;
	isCreditsActive_ = false;
	isExitRequested_ = false;
	isResetCompleteOpen_ = false;
	isGameOver_ = !prgMng_.IsCanDeleteProgress();
	if (isGameOver_)
	{
		InitGameOver();
		return;
	}

	stillHandle_ = resMng_.Load(GetStillSrc()).handleId_;
	titleFontHandle_ = resMng_.Load(ResourceManager::SRC::TITLE_FONT).handleId_;
	resetFontHandle_ = resMng_.Load(ResourceManager::SRC::SETTING_FONT).handleId_;
	endTitle_ = GetEndTitle();
	endTitleAnimationFrame_ = 0;
}

void ClearScene::InitGameOver(void)
{
	gameOverState_ = GameOverState::INITIAL_CONFIRM;
	gameOverLineCount_ = 0;
	if (!prgMng_.IsNurceCharExists())
	{
		AddGameOverLine("Chara/nurce.charが見つかりません。");
	}
	if (!prgMng_.IsPatientCharExists())
	{
		AddGameOverLine("Chara/patient.charが見つかりません。");
	}
	AddGameOverLine("必要なデータを読み込めませんでした。");
	AddGameOverLine("データをリセットしますか？y/n");
	GetHitKeyStateAll(previousKeyState_);
}

void ClearScene::DrawEndTitle(void) const
{
	if (titleFontHandle_ < 0 || endTitle_.empty())
	{
		return;
	}

	const float animationRate = static_cast<float>(endTitleAnimationFrame_) / END_TITLE_ANIMATION_FRAMES;
	const int drawX = END_TITLE_POS_X - END_TITLE_SLIDE_DISTANCE_X +
		static_cast<int>(END_TITLE_SLIDE_DISTANCE_X * animationRate);
	const int drawY = Application::SCREEN_SIZE_Y -
		GetFontSizeToHandle(titleFontHandle_) - END_TITLE_BOTTOM_MARGIN;
	const int alpha = static_cast<int>(END_TITLE_MAX_ALPHA * animationRate);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawStringToHandle(drawX, drawY, endTitle_.c_str(), END_TITLE_COLOR, titleFontHandle_);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ClearScene::UpdateCredits(void)
{
	if (creditIndex_ < 0 || creditIndex_ >= CREDIT_ENTRY_COUNT)
	{
		return;
	}

	const int holdFrames = GetCreditHoldFrames();
	const int holdStartFrame = CREDIT_FADE_IN_FRAMES;
	const int holdEndFrame = holdStartFrame + holdFrames;
	if (!IsFinalCredit() &&
		creditFrame_ >= holdStartFrame && creditFrame_ < holdEndFrame &&
		IsCreditSkipTriggered())
	{
		creditFrame_ = holdEndFrame;
	}

	++creditFrame_;
	const int totalFrames = CREDIT_FADE_IN_FRAMES + holdFrames + CREDIT_FADE_OUT_FRAMES;
	if (creditFrame_ < totalFrames)
	{
		return;
	}

	++creditIndex_;
	creditFrame_ = 0;
	if (creditIndex_ >= CREDIT_ENTRY_COUNT)
	{
		isExitRequested_ = true;
		PostQuitMessage(0);
	}
}

void ClearScene::DrawCredits(void) const
{
	if (stillHandle_ < 0)
	{
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
			GetColor(0, 0, 0), true);
	}
	else
	{
		DrawExtendGraph(
			0, 0,
			Application::SCREEN_SIZE_X,
			Application::SCREEN_SIZE_Y,
			stillHandle_, true);
	}
	DrawEndTitle();

	if (titleFontHandle_ < 0 || creditIndex_ < 0 || creditIndex_ >= CREDIT_ENTRY_COUNT)
	{
		return;
	}

	const CreditEntry& entry = CREDIT_ENTRIES[creditIndex_];
	const int alpha = GetCreditAlpha();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	const auto drawRightAligned = [this](const char* text, int drawY, int color)
	{
		const int width = GetDrawStringWidthToHandle(
			text, static_cast<int>(std::strlen(text)), titleFontHandle_);
		const int drawX = Application::SCREEN_SIZE_X - CREDIT_RIGHT_MARGIN - width;
		DrawStringToHandle(drawX + CREDIT_SHADOW_OFFSET, drawY + CREDIT_SHADOW_OFFSET,
			text, END_TITLE_COLOR, titleFontHandle_);
		DrawStringToHandle(drawX, drawY, text, color, titleFontHandle_);
	};

	if (entry.role[0] == '\0')
	{
		drawRightAligned(entry.name, CREDIT_NAME_POS_Y, CREDIT_NAME_COLOR);
	}
	else
	{
		drawRightAligned(entry.role, CREDIT_ROLE_POS_Y, CREDIT_ROLE_COLOR);
		drawRightAligned(entry.name, CREDIT_NAME_POS_Y, CREDIT_NAME_COLOR);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ClearScene::UpdateHiddenReset(void)
{
	const bool isKeyOrDpadUp = KeyConfig::IsNew(KeyConfig::ACTION::MOVE_UP, iptMng_);
	const VECTOR stickInput = iptMng_.GetLeftStickInput(InputManager::JOYPAD_NO::PAD1);
	const bool isStickUp = stickInput.y <= HIDDEN_RESET_STICK_UP;
	if (!isKeyOrDpadUp && !isStickUp)
	{
		hiddenResetHoldFrame_ = 0;
		return;
	}

	++hiddenResetHoldFrame_;
	if (hiddenResetHoldFrame_ < HIDDEN_RESET_HOLD_FRAMES)
	{
		return;
	}

	hiddenResetHoldFrame_ = 0;
	if (prgMng_.ResetProgressCache())
	{
		isResetCompleteOpen_ = true;
		GetHitKeyStateAll(previousKeyState_);
	}
}

void ClearScene::UpdateResetComplete(void)
{
	if (!IsAnyKeyTrgDown() && !IsAnyPadButtonTrgDown())
	{
		return;
	}

	isExitRequested_ = true;
	PostQuitMessage(0);
}

void ClearScene::DrawResetComplete(void) const
{
	DrawBox(RESET_WINDOW_LEFT, RESET_WINDOW_TOP, RESET_WINDOW_RIGHT, RESET_WINDOW_BOTTOM,
		RESET_WINDOW_BG_COLOR, true);
	DrawBox(RESET_WINDOW_LEFT, RESET_WINDOW_TOP, RESET_WINDOW_RIGHT, RESET_WINDOW_BOTTOM,
		RESET_WINDOW_FRAME_COLOR, false);

	if (resetFontHandle_ < 0)
	{
		return;
	}

	DrawStringToHandle(RESET_TEXT_POS_X, RESET_TEXT_POS_Y,
		"リセットが完了しました。", RESET_TEXT_COLOR, resetFontHandle_);
	DrawStringToHandle(RESET_TEXT_POS_X, RESET_TEXT_POS_Y + RESET_TEXT_LINE_INTERVAL,
		"何らかのキーを押して終了...", RESET_SUB_TEXT_COLOR, resetFontHandle_);
}

bool ClearScene::IsCreditSkipTriggered(void) const
{
	return KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_) ||
		KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_);
}

bool ClearScene::IsFinalCredit(void) const
{
	return creditIndex_ == CREDIT_ENTRY_COUNT - 1;
}

int ClearScene::GetCreditHoldFrames(void) const
{
	return IsFinalCredit() ? CREDIT_FINAL_HOLD_FRAMES : CREDIT_HOLD_FRAMES;
}

int ClearScene::GetCreditAlpha(void) const
{
	if (creditFrame_ < CREDIT_FADE_IN_FRAMES)
	{
		return creditFrame_ * 255 / CREDIT_FADE_IN_FRAMES;
	}

	const int fadeOutStartFrame = CREDIT_FADE_IN_FRAMES + GetCreditHoldFrames();
	if (creditFrame_ < fadeOutStartFrame)
	{
		return 255;
	}

	const int fadeOutFrame = creditFrame_ - fadeOutStartFrame;
	const int alpha = 255 - fadeOutFrame * 255 / CREDIT_FADE_OUT_FRAMES;
	return (alpha > 0) ? alpha : 0;
}

void ClearScene::DrawGameOver(void) const
{
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
		GetColor(0, 0, 0), true);
	for (int i = 0; i < gameOverLineCount_; ++i)
	{
		DrawString(16, 16 + GAME_OVER_LINE_INTERVAL_Y * i,
			gameOverLines_[i], GetColor(200, 255, 200));
	}
}

void ClearScene::UpdateGameOver(void)
{
	if (gameOverState_ == GameOverState::WAIT_EXIT)
	{
		if (IsAnyKeyTrgDown())
		{
			PostQuitMessage(0);
		}
		return;
	}

	if (iptMng_.IsTrgDown(KEY_INPUT_Y))
	{
		if (gameOverState_ == GameOverState::INITIAL_CONFIRM)
		{
			const bool isResetSucceeded = prgMng_.ResetProgressCache();
			AddGameOverLine(isResetSucceeded ? "データの初期化が完了しました。ゲームを再起動してください。"
				: "データの初期化に失敗しました。");
			AddGameOverLine("どれかのキーを押して終了...");
			gameOverState_ = GameOverState::WAIT_EXIT;
			GetHitKeyStateAll(previousKeyState_);
		}
		else
		{
			PostQuitMessage(0);
		}
		return;
	}

	if (iptMng_.IsTrgDown(KEY_INPUT_N))
	{
		if (gameOverState_ == GameOverState::INITIAL_CONFIRM)
		{
			AddGameOverLine("リセットしなければゲームが起動できません。このまま終了してよろしいですか？y/n");
			gameOverState_ = GameOverState::EXIT_CONFIRM;
		}
		else
		{
			AddGameOverLine("データをリセットしますか？y/n");
			gameOverState_ = GameOverState::INITIAL_CONFIRM;
		}
	}
}

void ClearScene::AddGameOverLine(const char* line)
{
	if (gameOverLineCount_ >= GAME_OVER_MAX_LINES)
	{
		for (int i = 1; i < GAME_OVER_MAX_LINES; ++i)
		{
			gameOverLines_[i - 1] = gameOverLines_[i];
		}
		--gameOverLineCount_;
	}

	gameOverLines_[gameOverLineCount_] = line;
	++gameOverLineCount_;
}

bool ClearScene::IsAnyKeyTrgDown(void)
{
	char currentKeyState[256];
	GetHitKeyStateAll(currentKeyState);

	bool isTriggered = false;
	for (int i = 0; i < 256; ++i)
	{
		if (currentKeyState[i] && !previousKeyState_[i])
		{
			isTriggered = true;
		}
	}

	std::memcpy(previousKeyState_, currentKeyState, sizeof(previousKeyState_));
	return isTriggered;
}

bool ClearScene::IsAnyPadButtonTrgDown(void) const
{
	for (int i = 0; i < static_cast<int>(InputManager::JOYPAD_BTN::MAX); ++i)
	{
		const InputManager::JOYPAD_BTN button = static_cast<InputManager::JOYPAD_BTN>(i);
		if (iptMng_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, button))
		{
			return true;
		}
	}

	return false;
}

const ClearScene::EndInfo& ClearScene::GetEndInfo(void) const
{
	static constexpr EndInfo END_INFOS[] =
	{
		{
			ProgressManager::STORY_PROGRESS::END_PATIENT_LOCKED,
			ResourceManager::SRC::STILL_END_NUR,
			"End_T:存在しない入院患者"
		},
		{
			ProgressManager::STORY_PROGRESS::END_NURCE_LOCKED,
			ResourceManager::SRC::STILL_END_NUI,
			"End_D:思い通り"
		},
		{
			ProgressManager::STORY_PROGRESS::END_BOTH_LOCKED,
			ResourceManager::SRC::STILL_END_BOTH,
			"End_NULL:もぬけの殻"
		},
	};
	static constexpr EndInfo DEFAULT_END_INFO =
	{
		ProgressManager::STORY_PROGRESS::END_BOTH_LOCKED,
		ResourceManager::SRC::STILL_END_BOTH,
		"そして誰もいなくなった"
	};

	for (const EndInfo& endInfo : END_INFOS)
	{
		if (endInfo.progress == prgMng_.GetProgressEnum())
		{
			return endInfo;
		}
	}

	return DEFAULT_END_INFO;
}

ResourceManager::SRC ClearScene::GetStillSrc(void) const
{
	return GetEndInfo().stillSrc;
}

std::string ClearScene::GetEndTitle(void) const
{
	return GetEndInfo().title;
}
