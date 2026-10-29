#pragma once
#include <string>
#include <DxLib.h>
#include "../Common/Vector2.h"

class InputManager;
class SoundManager;

class PCScene
{

	// PCカーソルの1更新当たりの移動量
	static constexpr float CURSOR_MOVE_SPEED = 12.0f;

	// PC画面の通常フォントサイズ
	static constexpr int PC_FONT_SIZE = 22;

	// PC画面の小型フォントサイズ
	static constexpr int PC_FONT_SIZE_S = 18;

	// PC画面の文字の太さ
	static constexpr int PC_FONT_THICK = 0;

	// PC画面の文字間隔
	static constexpr int PC_FONT_SPACE = -1;

	// カルテ本文の表示位置
	static constexpr Vector2<int> KARTE_TEXT_POS = { 40, 55 };

	// カルテ本文の行間隔
	static constexpr int KARTE_TEXT_LINE_INTERVAL_Y = 40;

	// キャラファイルアイコンの表示位置一覧
	static constexpr Vector2<int> FILE_POS[] = { { 60, 80 }, { 170, 80 } };

	// ファイルアイコンからラベルまでの縦ずらし量
	static constexpr int FILE_LABEL_Y_OFFSET = 96;

	// アクセス拒否ウィンドウの左上座標
	static constexpr Vector2<int> ACCESS_DENIED_BOX_POS = { 200, 180 };

	// アクセス拒否ウィンドウの幅と高さ
	static constexpr Vector2<int> ACCESS_DENIED_BOX_SIZE = { 400, 180 };

	// アクセス拒否ウィンドウのタイトルバーの高さ
	static constexpr int ACCESS_DENIED_TITLE_BAR_HEIGHT = 28;

	// アクセス拒否のOKボタンの左上座標
	static constexpr Vector2<int> ACCESS_DENIED_OK_POS = { 480, 310 };

	// アクセス拒否のOKボタンの幅と高さ
	static constexpr Vector2<int> ACCESS_DENIED_OK_SIZE = { 80, 28 };

	// PC画面の操作案内文字列
	static constexpr const char* PC_GUIDE_TEXT = "マウス操作可　キャンセルキーで閉じる　×";

	// PC画面の閉じるボタンの文字列
	static constexpr const char* PC_GUIDE_CLOSE_TEXT = "×";

	// 閉じるボタンのX座標
	static constexpr int PC_GUIDE_CLOSE_POS_X = 750;

	// 閉じるボタンのY座標
	static constexpr int PC_GUIDE_CLOSE_POS_Y = 31;

	// 閉じるボタンの判定領域の高さ
	static constexpr int PC_GUIDE_CLOSE_HIT_SIZE_Y = 24;

public:

	PCScene();
	~PCScene();

	void Load();
	void Update(const InputManager& input);
	void Draw();
	void Delete();

	// 進行度に対応するPC画面の表示開始
	void Open();

	// PC画面の表示状態の取得
	bool IsOpen() const;

private:

	// 描画色のRGB成分の組
	struct Rgb
	{

		// 赤と緑と青の各色成分
		int r, g, b;

		// RGB成分からDxLib描画色への変換
		int ToDxColor() const { return GetColor(r, g, b); }
	};

	// 通常文字の色
	static constexpr Rgb TEXT_COLOR = { 0, 0, 0 };

	// タイトルバーの文字色
	static constexpr Rgb TITLE_TEXT_COLOR = { 255, 255, 255 };

	// ウィンドウ枠線の色
	static constexpr Rgb FRAME_COLOR = { 80, 80, 80 };

	// ウィンドウ本体の色
	static constexpr Rgb WINDOW_COLOR = { 220, 220, 220 };

	// タイトルバーの背景色
	static constexpr Rgb TITLE_BAR_COLOR = { 100, 100, 100 };

	// ウィンドウ枠線の幅
	static constexpr int FRAME_WIDTH = 2;

	// ウィンドウ左上から見たタイトル文字の位置
	static constexpr Vector2<int> TITLE_TEXT_OFFSET = { 12, 6 };

	// ウィンドウ左上から見たメッセージ文字の位置
	static constexpr Vector2<int> MESSAGE_TEXT_OFFSET = { 24, 58 };

	// OKボタン左上から見た文字の位置
	static constexpr Vector2<int> OK_TEXT_OFFSET = { 30, 5 };

	// マニュアルとカルテとキャラのフォルダ番号
	enum FolderIndex : int { MANUAL_FOLDER, KARTE_FOLDER, CHARA_FOLDER };

	// 看護師と患者のキャラファイル番号
	enum FileIndex : int { NURSE_FILE, PATIENT_FILE };

	// キャラファイルアイコンの個数
	static constexpr int FILE_COUNT = sizeof(FILE_POS) / sizeof(FILE_POS[0]);

	// PC画面内のキャンセル操作の処理
	void HandleCancel();

	// 進行度に応じたカルテ本文の更新
	void UpdateKarteText();

	// カルテ画面の描画
	void DrawKarte() const;

	// キャラファイル一覧の描画
	void DrawFiles() const;

	// アクセス拒否ウィンドウの描画
	void DrawAccessDeniedMessage() const;

	// 進行度に応じたフォルダ表示状態の更新
	void UpdatePcFolderState();

	// 指定ファイルアイコン上のカーソル判定
	bool IsFileHit(int fileIndex) const;

	// アクセス拒否のOKボタン上のカーソル判定
	bool IsAccessDeniedOkHit() const;

	// 閉じるボタン上のカーソル判定
	bool IsCloseButtonHit() const;

	// PCフォルダアイコンの表示位置一覧
	static constexpr Vector2<int> FOLDER_POS[] = { { 70, 33 }, { 70, 155 }, { 70, 277 } };

	// PCフォルダの総数
	static constexpr int FOLDER_COUNT = sizeof(FOLDER_POS) / sizeof(FOLDER_POS[0]);

	// キャラフォルダ出現前の表示フォルダ数
	static constexpr int INITIAL_FOLDER_COUNT = FOLDER_COUNT - 1;

	// PC画面背景の画像ハンドル
	int backgroundHandle_;

	// PC画面外枠の画像ハンドル
	int frameHandle_;

	// フォルダアイコンの画像ハンドル
	int folderHandle_;

	// ファイルアイコンの画像ハンドル
	int fileHandle_;

	// 血圧測定マニュアルの画像ハンドル
	int bpManualHandle_;

	// PCカーソルの画像ハンドル
	int cursorHandle_;

	// PC画面の通常フォントハンドル
	int pcFontHandle_;

	// PC画面の小型フォントハンドル
	int pcFontHandleS_;

	// フォルダアイコンの幅と高さ
	Vector2<int> folderSize_;

	// ファイルアイコンの幅と高さ
	Vector2<int> fileSize_;

	// PCカーソルの画面座標
	Vector2<float> cursorPos_;

	// カルテに表示する看護師名
	std::string nurseName_;

	// カルテに表示する症状の本文
	std::string symptomText_;

	// PC画面の表示状態
	bool isOpen_;

	// 血圧測定マニュアルの表示状態
	bool isBPManualVisible_;

	// カルテの表示状態
	bool isKarteVisible_;

	// キャラフォルダの出現状態
	bool isCharaFolderVisible_;

	// キャラフォルダの展開状態
	bool isCharaFolderOpen_;

	// アクセス拒否ウィンドウの表示状態
	bool isAccessDeniedVisible_;

	// PC操作音の再生に使用するサウンド管理への参照ポインタ
	SoundManager* soundMng_;
};
