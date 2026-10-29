#include <DxLib.h>
#include <Windows.h>
#include <cstring>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/PadInput.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/SoundManager.h"
#include "ClearScene.h"

namespace
{
	struct CreditEntry { std::string role; std::string name; };

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
		{ "BGM協力", "神楽 / T.R"},
		{ "シナリオ協力", "神楽" },
		{ "効果音素材", "Oto Logic / 効果音ラボ" },
		{ "効果音素材", "On-Jin ～音人～ / 電脳プロダクション" },
		{ "効果音素材", "Sound Dino" },
		{ "使用ライブラリ", "DxLib / Live2D" },
		{ "制作", "KK" },
		{ "プレイしていただきありがとうございました", "Thank you for Playing" },
	};
	constexpr int CREDIT_ENTRY_COUNT = sizeof(CREDIT_ENTRIES) / sizeof(CREDIT_ENTRIES[0]);
}

ClearScene::ClearScene()
	: SceneBase()
	, stillHandle_(-1)
	, titleFontHandle_(-1)
	, resetFontHandle_(-1)
	, endTitleAnimationFrame_(0)
	, endStillHoldFrame_(0)
	, creditIndex_(0)
	, creditFrame_(0)
	, hiddenResetHoldFrame_(0)
	, endTitle_()
	, isGameOver_(false)
	, isCreditsActive_(false)
	, isExitConfirm_(false)
	, isResetCompleteOpen_(false)
	, gameOverState_(GameOverState::INITIAL_CONFIRM)
	, gameOverLines_{}
	, gameOverLineCount_(0)
	, prevKeyState_{}
	, sndMng_(nullptr)
{
	std::memset(prevKeyState_, 0, sizeof(prevKeyState_));
	sndMng_ = &SoundManager::GetInstance();
}

ClearScene::~ClearScene()
{
}

void ClearScene::Update()
{
	if (isGameOver_)
	{
		UpdateGameOver();
		return;
	}

	if (IsRuinedEnd())
	{
		UpdateRuinedEnd();
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
	if (isExitConfirm_)
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

void ClearScene::Draw()
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
			DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, END_TITLE_COLOR, true);
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

void ClearScene::Delete()
{
	// 進行度がクリア済みかどうかを確認し、クリア済みであれば進行度をリセット完了に設定する
	int prgres = prgMng_.GetProgress();
	if (prgres == ProgressManager::END_PATIENT_LOCKED ||
		prgres == ProgressManager::END_NURSE_LOCKED || prgres == ProgressManager::END_BOTH_LOCKED ||
		prgres == ProgressManager::END_RUINED_LOCKED)
	{
		prgMng_.SetProgress(ProgressManager::CLEAR_COMPLETE);
	}

	// 開放処理
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
	isResetCompleteOpen_ = false;
}

void ClearScene::InitLoad()
{
	endTitleAnimationFrame_ = 0;
	endStillHoldFrame_ = 0;
	creditIndex_ = 0;
	creditFrame_ = 0;
	hiddenResetHoldFrame_ = 0;
	isCreditsActive_ = false;
	isResetCompleteOpen_ = false;
	isGameOver_ = !prgMng_.IsCanDeleteProgress();

	// ゲームオーバーなら、ゲームオーバー処理を初期化して以降の処理を行わない
	if (isGameOver_)
	{
		InitGameOver();
		return;
	}

	stillHandle_ = resMng_.Load(GetStillSrc()).handleId_;
	titleFontHandle_ = resMng_.LoadFont(ResourceManager::SRC::MAIN_FONT, TITLE_FONT_SIZE);
	resetFontHandle_ = resMng_.LoadFont(ResourceManager::SRC::MAIN_FONT, RESET_FONT_SIZE);
	endTitle_ = GetEndTitle();
	endTitleAnimationFrame_ = 0;

	if (IsRuinedEnd())
	{
		GetHitKeyStateAll(prevKeyState_);
	}
}

void ClearScene::InitPost()
{
	if (isGameOver_ || IsRuinedEnd())
	{
		sndMng_->StopBGM();
		return;
	}

	sndMng_->PlayBGM(SoundManager::BGM::ENDING);
}

void ClearScene::InitGameOver()
{
	// ゲームオーバー時の初期化処理
	gameOverState_ = GameOverState::INITIAL_CONFIRM;
	gameOverLineCount_ = 0;
	if (!prgMng_.IsNurseCharExists())
	{
		AddLineGameOver("Chara/neit.charが見つかりません。");
	}
	if (!prgMng_.IsPatientCharExists())
	{
		AddLineGameOver("Chara/nui.charが見つかりません。");
	}
	AddLineGameOver("必要なデータを読み込めませんでした。");
	AddLineGameOver("データをリセットしますか？y/n");
	GetHitKeyStateAll(prevKeyState_);
}

void ClearScene::DrawEndTitle() const
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

void ClearScene::UpdateCredits()
{
	if (creditIndex_ < 0 || creditIndex_ >= GetCreditCount())
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
	if (creditIndex_ >= GetCreditCount())
	{
		prgMng_.SetProgress(ProgressManager::CLEAR_COMPLETE);
		sndMng_->StopBGM();
		isFadeOut_ = true;

		// 今はフェードアウトは無いので終了要求を出す　後でやる
		PostQuitMessage(0);
	}
}

void ClearScene::UpdateRuinedEnd()
{
	if (KeyConfig::IsNewAny(iptMng_))
	{
		return;
	}

	PostQuitMessage(0);
}

void ClearScene::DrawCredits() const
{
	if (stillHandle_ < 0)
	{
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
			END_TITLE_COLOR, true);
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

	if (titleFontHandle_ < 0 || creditIndex_ < 0 || creditIndex_ >= GetCreditCount())
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

	if (entry.role.empty())
	{
		drawRightAligned(entry.name.c_str(), CREDIT_NAME_POS_Y, CREDIT_NAME_COLOR);
	}
	else
	{
		drawRightAligned(entry.role.c_str(), CREDIT_ROLE_POS_Y, CREDIT_ROLE_COLOR);
		drawRightAligned(entry.name.c_str(), CREDIT_NAME_POS_Y, CREDIT_NAME_COLOR);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ClearScene::UpdateHiddenReset()
{
	// ↑キーまたはパッドの上方向入力が押されていたら
	const bool isKeyOrDpadUp = KeyConfig::IsNew(KeyConfig::ACTION::MOVE_UP, iptMng_);
	const Vector2<float> stickInput = PadInput::GetMoveAxis(iptMng_, InputManager::JOYPAD_NO::PAD1);
	const bool isStickUp = stickInput.y <= HIDDEN_RESET_STICK_UP;

	if (!isKeyOrDpadUp && !isStickUp)
	{
		// 入力がなければ長押し判定のリセット（長押しが必要）
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
		GetHitKeyStateAll(prevKeyState_);
	}
}

void ClearScene::UpdateResetComplete()
{
	if (!KeyConfig::IsNewAny(iptMng_))
	{
		return;
	}

	PostQuitMessage(0);
}

void ClearScene::DrawResetComplete() const
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

bool ClearScene::IsCreditSkipTriggered() const
{
	return KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_) ||
		KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, iptMng_);
}

bool ClearScene::IsFinalCredit() const
{
	return creditIndex_ == GetCreditCount() - 1;
}

int ClearScene::GetCreditHoldFrames() const
{
	return IsFinalCredit() ? CREDIT_FINAL_HOLD_FRAMES : CREDIT_HOLD_FRAMES;
}

int ClearScene::GetCreditAlpha() const
{
	if (creditFrame_ < CREDIT_FADE_IN_FRAMES)
	{
		return creditFrame_ * DX_MAX / CREDIT_FADE_IN_FRAMES;
	}

	const int fadeOutStartFrame = CREDIT_FADE_IN_FRAMES + GetCreditHoldFrames();
	if (creditFrame_ < fadeOutStartFrame)
	{
		return DX_MAX;
	}

	const int fadeOutFrame = creditFrame_ - fadeOutStartFrame;
	const int alpha = DX_MAX - fadeOutFrame * DX_MAX / CREDIT_FADE_OUT_FRAMES;
	return (alpha > 0) ? alpha : 0;
}

void ClearScene::DrawGameOver() const
{
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
		END_TITLE_COLOR, true);
	for (int i = 0; i < gameOverLineCount_; ++i)
	{
		DrawString(GAME_OVER_POS_X, GAME_OVER_POS_Y + GAME_OVER_LINE_INTERVAL_Y * i,
			gameOverLines_[i].c_str(), GetColor(GAME_OVER_TEXT_RED, GAME_OVER_TEXT_GREEN, GAME_OVER_TEXT_BLUE));
	}
}

void ClearScene::UpdateGameOver()
{
	if (gameOverState_ == GameOverState::WAIT_EXIT)
	{
		if (KeyConfig::IsNewAny(iptMng_))
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
			AddLineGameOver(isResetSucceeded ? "データの初期化が完了しました。ゲームを再起動してください。"
				: "データの初期化に失敗しました。");
			AddLineGameOver("どれかのキーを押して終了...");
			gameOverState_ = GameOverState::WAIT_EXIT;
			GetHitKeyStateAll(prevKeyState_);
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
			AddLineGameOver("リセットしなければゲームが起動できません。このまま終了してよろしいですか？y/n");
			gameOverState_ = GameOverState::EXIT_CONFIRM;
		}
		else
		{
			AddLineGameOver("データをリセットしますか？y/n");
			gameOverState_ = GameOverState::INITIAL_CONFIRM;
		}
	}
}

void ClearScene::AddLineGameOver(const std::string& line)
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

const ClearScene::EndInfo& ClearScene::GetEndInfo() const
{
	static const EndInfo END_INFOS[] =
	{
		{
			ProgressManager::STORY_PROGRESS::END_PATIENT_LOCKED,
			ResourceManager::SRC::STILL_END_NUR,
			"End_T:存在しない入院患者"
		},
		{
			ProgressManager::STORY_PROGRESS::END_NURSE_LOCKED,
			ResourceManager::SRC::STILL_END_NUI,
			"End_D:思い通り"
		},
		{
			ProgressManager::STORY_PROGRESS::END_BOTH_LOCKED,
			ResourceManager::SRC::STILL_END_BOTH,
			"End_NULL:もぬけの殻"
		},
		{
			ProgressManager::STORY_PROGRESS::END_RUINED_LOCKED,
			ResourceManager::SRC::STILL_END_RUIN,
			""
		},
	};
	static const EndInfo DEFAULT_END_INFO =
	{
		ProgressManager::STORY_PROGRESS::END_BOTH_LOCKED,
		ResourceManager::SRC::STILL_END_BOTH,
		"そしていなくなった"
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

ResourceManager::SRC ClearScene::GetStillSrc() const
{
	return GetEndInfo().stillSrc;
}

std::string ClearScene::GetEndTitle() const
{
	return GetEndInfo().title;
}

int ClearScene::GetCreditCount() const
{
	return CREDIT_ENTRY_COUNT;
}
