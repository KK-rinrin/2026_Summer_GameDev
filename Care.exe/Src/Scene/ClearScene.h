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

	// ゲームオーバー表示の最大行数
	static constexpr int GAME_OVER_MAX_LINES = 24;

	// アルファ値と色成分の最大値
	static constexpr int DX_MAX = 255;

	// キー状態配列の要素数
	static constexpr int KEY_STATE_BUFFER_SIZE = 256;

	// ゲームオーバー本文の表示開始X座標
	static constexpr int GAME_OVER_POS_X = 16;

	// ゲームオーバー本文の表示開始Y座標
	static constexpr int GAME_OVER_POS_Y = 16;

	// ゲームオーバー文字色の赤成分
	static constexpr int GAME_OVER_TEXT_RED = 200;

	// ゲームオーバー文字色の緑成分
	static constexpr int GAME_OVER_TEXT_GREEN = 255;

	// ゲームオーバー文字色の青成分
	static constexpr int GAME_OVER_TEXT_BLUE = 200;

	// ゲームオーバー本文の行間隔
	static constexpr int GAME_OVER_LINE_INTERVAL_Y = 24;

	// エンディング見出しの基準X座標
	static constexpr int END_TITLE_POS_X = 24;

	// エンディング見出しのフォントサイズ
	static constexpr int TITLE_FONT_SIZE = 38;

	// リセット結果のフォントサイズ
	static constexpr int RESET_FONT_SIZE = 30;

	// エンディング見出しの横スライド距離
	static constexpr int END_TITLE_SLIDE_DISTANCE_X = 20;

	// エンディング見出しの画面下端からの余白
	static constexpr int END_TITLE_BOTTOM_MARGIN = 32;

	// エンディング見出しの演出フレーム数
	static constexpr int END_TITLE_ANIMATION_FRAMES = 60;

	// エンディング見出しの最大アルファ値
	static constexpr int END_TITLE_MAX_ALPHA = DX_MAX;

	// エンディング見出しの文字色
	static constexpr int END_TITLE_COLOR = Application::DEFAULT_COLOR_0;

	// エンディング一枚絵の保持フレーム数
	static constexpr int END_STILL_HOLD_FRAMES = 180;

	// クレジットの明転フレーム数
	static constexpr int CREDIT_FADE_IN_FRAMES = 30;

	// 通常クレジットの保持フレーム数
	static constexpr int CREDIT_HOLD_FRAMES = 60;

	// 最終クレジットの保持フレーム数
	static constexpr int CREDIT_FINAL_HOLD_FRAMES = 600;

	// クレジットの暗転フレーム数
	static constexpr int CREDIT_FADE_OUT_FRAMES = 30;

	// クレジット役職の表示Y座標
	static constexpr int CREDIT_ROLE_POS_Y = 422;

	// クレジット名の表示Y座標
	static constexpr int CREDIT_NAME_POS_Y = 482;

	// クレジット表示の画面右端からの余白
	static constexpr int CREDIT_RIGHT_MARGIN = 30;

	// クレジット文字の影のずらし量
	static constexpr int CREDIT_SHADOW_OFFSET = 2;

	// クレジット役職の文字色
	static constexpr int CREDIT_ROLE_COLOR = Application::DEFAULT_COLOR_6;

	// クレジット名の文字色
	static constexpr int CREDIT_NAME_COLOR = Application::DEFAULT_COLOR_8;

	// 隠しリセット入力の長押しフレーム数
	static constexpr int HIDDEN_RESET_HOLD_FRAMES = 180;

	// 隠しリセットのスティック上入力の閾値
	static constexpr float HIDDEN_RESET_STICK_UP = -0.8f;

	// リセット結果ウィンドウの左端
	static constexpr int RESET_WINDOW_LEFT = 50;

	// リセット結果ウィンドウの上端
	static constexpr int RESET_WINDOW_TOP = 150;

	// リセット結果ウィンドウの右端
	static constexpr int RESET_WINDOW_RIGHT = 750;

	// リセット結果ウィンドウの下端
	static constexpr int RESET_WINDOW_BOTTOM = 370;

	// リセット結果本文のX座標
	static constexpr int RESET_TEXT_POS_X = 100;

	// リセット結果本文のY座標
	static constexpr int RESET_TEXT_POS_Y = 195;

	// リセット結果本文の行間隔
	static constexpr int RESET_TEXT_LINE_INTERVAL = 45;

	// リセット結果ウィンドウの背景色
	static constexpr int RESET_WINDOW_BG_COLOR = Application::DEFAULT_COLOR_8;

	// リセット結果ウィンドウの枠色
	static constexpr int RESET_WINDOW_FRAME_COLOR = Application::DEFAULT_COLOR_1;

	// リセット結果本文の文字色
	static constexpr int RESET_TEXT_COLOR = Application::DEFAULT_COLOR_1;

	// リセット結果の補助文字色
	static constexpr int RESET_SUB_TEXT_COLOR = Application::DEFAULT_COLOR_4;

	// エンディング進行度と一枚絵と見出しの対応情報
	struct EndInfo
	{

		// 対応するエンディング進行度
		ProgressManager::STORY_PROGRESS progress;

		// エンディング一枚絵のリソースID
		ResourceManager::SRC stillSrc;

		// エンディングの見出し文字列
		std::string title;
	};

	void InitLoad() override;
	void InitPost() override;

	// 崩壊エンディングの対話ログの初期化
	void InitGameOver();

	// ゲームオーバー対話の選択状態の更新
	void UpdateGameOver();

	// 崩壊エンディング終了入力の更新
	void UpdateRuinedEnd();

	// スタッフクレジットの進行更新
	void UpdateCredits();

	// 隠しリセットの長押し入力の更新
	void UpdateHiddenReset();

	// リセット完了表示の入力状態の更新
	void UpdateResetComplete();

	// ゲームオーバー対話ログの描画
	void DrawGameOver() const;

	// エンディング見出しの描画
	void DrawEndTitle() const;

	// スタッフクレジットの描画
	void DrawCredits() const;

	// リセット完了ウィンドウの描画
	void DrawResetComplete() const;

	// ゲッター
	bool IsCreditSkipTriggered() const;

	// 最終クレジット表示中の判定
	bool IsFinalCredit() const;

	// 現在のクレジットの保持フレーム数の取得
	int GetCreditHoldFrames() const;

	// クレジットの総項目数の取得
	int GetCreditCount() const;

	// クレジットの現在のアルファ値の取得
	int GetCreditAlpha() const;

	// ゲームオーバー処理
	void AddLineGameOver(const std::string& line);

	// 崩壊エンディング固定状態の判定
	bool IsRuinedEnd() const 
	{
		return prgMng_.GetProgressEnum() == ProgressManager::END_RUINED_LOCKED;
	}								// 崩壊ENDのみ特殊処理を行うための判定

	// 現在のエンディングに対応する表示情報の取得
	const EndInfo& GetEndInfo() const;

	// 現在のエンディングの一枚絵リソースIDの取得
	ResourceManager::SRC GetStillSrc() const;

	// 現在のエンディングの見出し文字列の取得
	std::string GetEndTitle() const;

	// ゲームオーバー対話の進行状態
	enum class GameOverState
	{
		INITIAL_CONFIRM,	// 最初の確認入力待ち
		EXIT_CONFIRM,		// 終了確認の選択入力待ち
		WAIT_EXIT			// 終了待ち状態
	};

	// エンディング一枚絵の画像ハンドル
	int stillHandle_;

	// エンディング見出し用フォントのハンドル
	int titleFontHandle_;

	// リセット結果用フォントのハンドル
	int resetFontHandle_;

	// 見出し演出の経過フレーム数
	int endTitleAnimationFrame_;

	// 一枚絵保持の経過フレーム数
	int endStillHoldFrame_;

	// 表示中のクレジットのインデックス
	int creditIndex_;

	// 現在のクレジットの経過フレーム数
	int creditFrame_;

	// 隠しリセット入力の継続フレーム数
	int hiddenResetHoldFrame_;

	// 現在のエンディングの見出し本文
	std::string endTitle_;

	// ゲームオーバー対話の有効状態
	bool isGameOver_;

	// スタッフクレジットの実行状態
	bool isCreditsActive_;

	// 終了確認の状態
	bool isExitConfirm_;

	// リセット完了ウィンドウの表示状態
	bool isResetCompleteOpen_;

	// 現在のゲームオーバー対話の進行状態
	GameOverState gameOverState_;

	// 表示対象のゲームオーバー対話ログ
	std::string gameOverLines_[GAME_OVER_MAX_LINES];

	// ゲームオーバー対話ログの使用行数
	int gameOverLineCount_;

	// 読み取り処理のないキー状態の保存領域（未使用）
	char prevKeyState_[KEY_STATE_BUFFER_SIZE];

	// 読み取り処理のない終了時フェードの開始済み記録（未使用）
	bool isFadeOut_ = false;

	// エンディングBGM用サウンド管理への参照ポインタ
	SoundManager* sndMng_;
};
