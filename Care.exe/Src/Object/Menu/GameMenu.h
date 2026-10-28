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

	// 通常メニューと一時停止の表示モード
	enum class Mode
	{

		// 通常メニュー表示
		NORMAL,

		// 一時停止表示
		PAUSE,
	};

	// メニュー操作の確定結果
	enum class Result
	{

		// 確定結果なし
		NONE,

		// ゲームへの復帰要求
		RESUME,

		// 設定画面への移動要求
		OPEN_SETTING,

		// タイトル画面への移動要求
		BACK_TO_TITLE,
	};

	GameMenu();
	~GameMenu();

	void Load();
	void Update(const InputManager& input);
	void Draw();
	void Delete();

	// 指定モードでのメニュー表示の開始
	void Open(Mode mode);

	// メニュー表示中の判定
	bool IsOpen() const { return isOpen_; }

	// 確定結果の取得と結果状態の消費
	Result ConsumeResult();

private:

	// 通常メニューの選択項目
	enum class Item
	{

		// ゲームに戻る項目
		RESUME,

		// 設定画面を開く項目
		SETTING,

		// タイトル画面に戻る項目
		BACK_TO_TITLE,

		// メニュー項目数
		MAX
	};

	// 完全不透明時のアルファ値
	static constexpr int MAX_ALPHA = 255;

	// 一時停止時の暗幕色
	static constexpr int DIM_COLOR = Application::DEFAULT_COLOR_0;

	// メニュー見出しの画面座標
	static constexpr Vector2 TITLE_POS = { 35, 20 };

	// メニュー見出しのフォントサイズ
	static constexpr int TITLE_FONT_SIZE = 38;

	// メニュー項目のフォントサイズ
	static constexpr int MENU_FONT_SIZE = 30;

	// 先頭メニュー項目の画面座標
	static constexpr Vector2 MENU_TEXT_POS = { 70, 280 };

	// メニュー項目間の縦間隔
	static constexpr int MENU_TEXT_INTERVAL_Y = 70;

	// 一時停止文字列の画面座標
	static constexpr Vector2 PAUSE_TEXT_POS = { 340, 280 };

	// 全画面矩形の左端
	static constexpr int SCREEN_LEFT = 0;

	// 全画面矩形の上端
	static constexpr int SCREEN_TOP = 0;

	// 全画面矩形の右端
	static constexpr int SCREEN_RIGHT = Application::SCREEN_SIZE_X;

	// 全画面矩形の下端
	static constexpr int SCREEN_BOTTOM = Application::SCREEN_SIZE_Y;

	// 前の項目へ移動する増分
	static constexpr int MENU_MOVE_PREV = -1;

	// 次の項目へ移動する増分
	static constexpr int MENU_MOVE_NEXT = 1;

	// 通常メニューの背景色
	static constexpr int MENU_BG_COLOR = Application::DEFAULT_COLOR_7;

	// 通常メニューの文字色
	static constexpr int MENU_TEXT_COLOR = Application::DEFAULT_COLOR_1;

	// 通常メニューの補助文字色
	static constexpr int MENU_SUB_TEXT_COLOR = Application::DEFAULT_COLOR_3;

	// 一時停止時の暗幕のアルファ値
	static constexpr int PAUSE_BLEND_ALPHA = 150;

	// 一時停止文字列の色
	static constexpr int PAUSE_TEXT_COLOR = Application::DEFAULT_COLOR_8;

	// プレイヤーモデルの表示倍率
	static constexpr VECTOR PLAYER_MODEL_EXTEND = { 3.0f, 3.0f, 0.0f };

	// プレイヤーモデルの表示位置
	static constexpr VECTOR PLAYER_MODEL_POS = { 230.0f, -300.0f, 0.0f };

	// メニュー見出しの表示文字列
	static constexpr const char* TITLE_TEXT = "MENU";

	// 一時停止の表示文字列
	static constexpr const char* PAUSE_TEXT = "PAUSE";

	// フォントハンドルの無効値
	static constexpr int INVALID_FONT_HANDLE = -1;

	// 通常メニューの入力と選択状態の更新
	void UpdateNormal(const InputManager& input);

	// 一時停止モードの入力状態の更新
	void UpdatePause(const InputManager& input);

	// 通常メニューの描画
	void DrawNormal();

	// 一時停止画面の描画
	void DrawPause();

	// 選択状態に応じたメニュー項目の描画
	void DrawMenuItems();

	// プレイヤープレビュー領域の描画
	void DrawPlayerPreview();

	// プレイヤーLive2Dモデルの更新
	void UpdatePlayerModel();

	// プレイヤーLive2Dモデルの描画
	void DrawPlayerModel();

	// メニュー選択位置の移動
	void MoveSelectItem(int move);

	// 現在の選択項目の確定
	void DecideSelectItem();

	// メニュー解除対象の入力の判定
	bool IsAnyMenuInput(const InputManager& input) const;

	// 項目に対応する表示文字列の取得
	const char* GetItemText(Item item) const;

	// メニューの表示状態
	bool isOpen_;

	// 現在の表示モード
	Mode mode_;

	// 未消費の確定結果
	Result result_;

	// 見出し用フォントのハンドル
	int fontTitle_;

	// 項目用フォントのハンドル
	int font_;

	// 現在選択中の項目番号
	int selectItem_;

	// プレイヤーLive2D制御の共有ポインタ
	std::shared_ptr<Live2DTalkController> playerController_;
};
