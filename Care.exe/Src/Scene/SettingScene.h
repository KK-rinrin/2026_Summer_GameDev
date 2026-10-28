#pragma once
#include "../Application.h"
#include <array>
#include <functional>
#include "../Common/Vector2.h"
#include "SceneBase.h"

class SoundManager;

class SettingScene : public SceneBase
{

public:

	// 設定画面の選択項目
	enum class Item
	{

		// BGM音量の設定項目
		BGM_VOLUME,

		// 効果音音量の設定項目
		SE_VOLUME,

		// キーボード割り当ての設定項目
		KEY_CONFIG,

		// パッド割り当ての設定項目
		PAD_CONFIG,

		// 直前の画面へ戻る項目
		BACK,

		// 設定項目の総数
		MAX
	};

	// 先頭設定項目の画面座標
	static constexpr Vector2 ITEM_POS = { 100, 210 };

	// 見出しのフォントサイズ
	static constexpr int TITLE_FONT_SIZE = 38;

	// 設定項目のフォントサイズ
	static constexpr int MENU_FONT_SIZE = 30;

	// 設定項目間の縦間隔
	static constexpr int ITEM_INTERVAL_Y = 60;

	// 未選択項目の文字色
	static constexpr int ITEM_COLOR = Application::DEFAULT_COLOR_4;

	// 選択中の項目の文字色
	static constexpr int ITEM_SELECTED_COLOR = Application::DEFAULT_COLOR_1;

	// 設定画面の見出し位置
	static constexpr Vector2 TITLE_POS = { 50, 50 };

	// 設定画面の見出しの色
	static constexpr int TITLE_COLOR = Application::DEFAULT_COLOR_2;

	// フォントハンドルの無効値
	static constexpr int INVALID_FONT_HANDLE = -1;

	// 初期選択項目のインデックス
	static constexpr int INITIAL_SELECT_ITEM = 0;

	// 前の項目へ移動する増分
	static constexpr int MENU_MOVE_PREV = -1;

	// 次の項目へ移動する増分
	static constexpr int MENU_MOVE_NEXT = 1;

	// 設定可能な最小音量の百分率
	static constexpr int VOLUME_MIN = 0;

	// 設定可能な最大音量の百分率
	static constexpr int VOLUME_MAX = 100;

	// 1操作当たりの音量変更幅
	static constexpr int VOLUME_STEP = 10;

	// 設定詳細の表示位置
	static constexpr Vector2 DETAIL_POS = { 390, 210 };

	// 設定詳細の行間隔
	static constexpr int DETAIL_INTERVAL_Y = 45;

	// 音量表示バーの段階数
	static constexpr int VOLUME_BAR_LENGTH = 10;

	// キー状態配列の要素数
	static constexpr int KEY_STATE_NUM = 256;

	// パッド割り当ての変更対象操作数
	static constexpr int PAD_CONFIG_ACTION_NUM = 2;

	// 隠しウィンドウ座標
	static constexpr Vector2 RESET_CONFIRM_WINDOW_POS = { 50, 150 };

	// リセット確認ウィンドウの幅と高さ
	static constexpr Vector2 RESET_CONFIRM_WINDOW_SIZE = { 700, 220 };

	// リセット確認本文の左上からの相対座標
	static constexpr Vector2 RESET_CONFIRM_TEXT_OFFSET = { 50, 45 };

	// リセット確認本文の行間隔
	static constexpr int RESET_CONFIRM_TEXT_LINE_INTERVAL_Y = 45;

	// リセット確認ウィンドウの背景色
	static constexpr int RESET_CONFIRM_BG_COLOR = Application::DEFAULT_COLOR_8;

	// リセット確認ウィンドウの枠色
	static constexpr int RESET_CONFIRM_FRAME_COLOR = Application::DEFAULT_COLOR_1;

	// 設定画面の見出し文字列
	static constexpr const char* TITLE_TEXT = "Setting";

	// 項目テキスト(余裕あれば4番目に"テキストサイズ（倍率）"を追加)
	static constexpr std::array<const char*, static_cast<int>(Item::MAX)>
		ITEM_TEXTS = { "BGM音量", "SE音量", "キーコンフィグ", "パッドコンフィグ", "BACK" };

	SettingScene();
	~SettingScene() override;

	void Update() override;
	void Draw() override;
	void Delete() override;

private:
	void InitLoad() override;	// ロード
	void MoveSelectItem(int move);	// 選択移動
	bool IsVisible(Item item) const;	// 項目を表示するかどうか

	// 現在選択中の設定項目の編集開始
	void SelectItem();

	// 設定画面から指定復帰先への移動
	void Return();

	// 設定項目メニューの描画
	void DrawMenu();

	// 隠しリセットコマンドの入力進行の更新
	bool UpdateResetSequence();

	// 進行リセット確認ダイアログの表示開始
	void OpenResetDialog();

	// 進行リセット確認と結果ダイアログの更新
	void UpdateResetDialog();

	// 進行リセット確認と結果ダイアログの描画
	void DrawResetDialog();

	// 現在編集中の設定項目の更新関数
	std::function<void()> ItemUpdate_;

	// BGM音量の編集処理
	void UpdateItemBGMVol();

	// 効果音音量の編集処理
	void UpdateItemSEVol();

	// キーボード割り当ての編集処理
	void UpdateItemKeyCon();

	// パッド割り当ての編集処理
	void UpdateItemPadCon();

	// BGM音量詳細の描画
	void DrawItemBGMVol();

	// 効果音音量詳細の描画
	void DrawItemSEVol();

	// キーボード割り当て詳細の描画
	void DrawItemKeyCon();

	// パッド割り当て詳細の描画
	void DrawItemPadCon();

	// 設定項目の編集状態の終了
	void FinishItemUpdate();

	// 指定音量の表示バーの描画
	void DrawVolume(int volume, const char* label);

	// 見出し用フォントのハンドル
	int fontTitle_;

	// 設定項目用フォントのハンドル
	int font_;

	// 設定音量を反映するサウンド管理への参照
	SoundManager& sndMng_;

	// 選択中の設定項目の番号
	int selectItem_;

	// 編集中のBGM音量百分率
	int bgmVolume_;

	// 編集中の効果音音量百分率
	int seVolume_;

	// 変更前の音量百分率
	int previousVolume_;

	// 割り当て編集中の操作番号
	int selectAction_;

	// 割り当てるキーまたはパッドボタンの入力待ち状態
	bool isWaitingKeyInput_;

	// 既存の押下が解除されて新規入力を受け付けられる状態
	bool isKeyInputReady_;

	// 隠しリセットコマンドの入力済み位置
	int hiddenResetCommandIndex_;

	// 進行リセット確認ダイアログの表示状態
	bool isProgressResetConfirmOpen_;

	// 進行リセット結果ダイアログの表示状態
	bool isProgressResetResultOpen_;

	// 進行リセット確認の実行選択状態
	bool isProgressResetYesSelected_;

	// 進行リセットの成功状態
	bool isProgressResetSucceeded_;

};
