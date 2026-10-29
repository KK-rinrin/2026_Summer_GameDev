#pragma once
#include "../Application.h"
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "SceneBase.h"
#include "../Object/Common/Transform2D.h"
#include <memory>
#include <vector>

class Live2DTalkController;

class TitleScene : public SceneBase
{

	// タイトル画面のプレイヤーモデルの表示倍率
	static constexpr Vector2<float> PLAYER_MODEL_EXTEND = { 2.5f, 2.5f };

	// タイトル画面のプレイヤーモデルの表示位置
	static constexpr Vector2<float> PLAYER_MODEL_POS = { -240.0f, -300.0f };

	// 先頭メニュー項目の画面座標
	static constexpr Vector2<int> MENU_POS = { 520, 285 };

	// タイトル画面のフォントサイズ
	static constexpr int TITLE_FONT_SIZE = 38;

	// メニュー項目間の縦間隔
	static constexpr int MENU_INTERVAL_Y = 54;

	// 前の項目へ移動する増分
	static constexpr int MENU_MOVE_PREV = -1;

	// 次の項目へ移動する増分
	static constexpr int MENU_MOVE_NEXT = 1;

	// 未選択項目の文字色
	static constexpr int MENU_COLOR = Application::DEFAULT_COLOR_5;

	// 選択中の項目の文字色
	static constexpr int MENU_SELECTED_COLOR = Application::DEFAULT_COLOR_2;

	// 開始案内の点滅間隔のフレーム数（未使用）
	static constexpr int PUSH_START_BLINK_INTERVAL = 30;
public:

	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 解放
	void Delete() override;

private:

	// タイトルメニューの操作種別
	enum class Menu
	{
		START,		// ゲーム開始の項目
		SETTING,	// 設定画面の項目
		RESET,		// 進行リセットの項目
		CLOSE,		// アプリケーション終了の項目
	};

	void InitLoad() override;

	// 状態に応じた選択可能メニューの構築
	void BuildSelectMenu();

	// タイトルメニューの入力状態の更新
	void UpdateSelectMenu();

	// プレイヤーLive2Dモデルの更新
	void UpdatePlayerModel();

	// プレイヤーLive2Dモデルの描画
	void DrawPlayerModel();

	// タイトルメニューの描画
	void DrawSelectMenu();

	// 選択メニュー位置の移動
	void MoveSelectMenu(int move);

	// 選択中のメニュー操作の確定
	void DecideSelectMenu();

	// 現在選択中のメニュー種別の取得
	Menu GetSelectedMenu() const;

	// メニュー種別に対応する表示文字列の取得
	const char* GetMenuText(Menu menu) const;

	// タイトル画像のハンドル
	int imgTitle_;

	// メニュー用フォントのハンドル
	int font_;

	// 現在のメニュー選択位置
	int selectMenu_;

	// 現在選択可能なメニューの一覧
	std::vector<Menu> menuItems_;

	// タイトル用Live2D制御の共有ポインタ
	std::shared_ptr<Live2DTalkController> liveTalkController_;
};
