#pragma once
#include <memory>
#include "../Application.h"
#include "SceneBase.h"
#include "../Manager/Live2D.h"
#include "../Object/Stage/StageBase.h"

class Talk;
class Player;
class Patient;
class ActorBase;
class Renderer2D;
class DebugCursorPosition;
class GameMenu;

class GameScene : public SceneBase
{
public:

	// メインゲームの滞在ステージ
	enum class Stage
	{

		// 患者部屋のステージ
		PAT_ROOM,

		// ナースステーションのステージ
		NURSE_STATION,
	};

	// ゲーム開始時の既定ステージ
	static constexpr Stage INIT_STAGE = Stage::NURSE_STATION;

	// 昼食以降の移動入力遅延の秒数
	static constexpr float INPUT_MOVE_DELAY_SECONDS_0 = 0.2f;

	// 異常進行後の移動入力遅延の秒数
	static constexpr float INPUT_MOVE_DELAY_SECONDS_1 = 0.4f;

	// 入力遅延のフレーム換算に使用するFPS
	static constexpr float GAME_FPS = 60.0f;

	// 色ずれの横方向ピクセル数
	static constexpr float COLOR_SHIFT_X = 4.0f;

	// 色ずれの縦方向ピクセル数
	static constexpr float COLOR_SHIFT_Y = 0.0f;

	// 色ずれ演出の矩形領域数
	static constexpr int COLOR_SHIFT_RECT_COUNT = 4;

	// 色ずれ矩形の更新間隔の秒数
	static constexpr float COLOR_SHIFT_RECT_CHANGE_SEC = 0.05f;

	// 崩壊演出の黒点滅間隔のフレーム数
	static constexpr int RUINED_BLACK_BLINK_INTERVAL_FRAMES = 120;

	// コンストラクタ
	GameScene();

	// デストラクタ
	~GameScene() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 解放
	void Delete() override;

private:

	// 崩壊演出の点滅色
	static constexpr int RUINED_BLINK_COLOR = Application::DEFAULT_COLOR_0;

	// デバッグ進行度表示のX座標
	static constexpr int DEBUG_PROGRESS_POS_X = 0;

	// デバッグ進行度表示のY座標
	static constexpr int DEBUG_PROGRESS_POS_Y = 50;

	// デバッグ進行度表示の文字色
	static constexpr int DEBUG_PROGRESS_COLOR = Application::DEFAULT_COLOR_0;

	void InitLoad() override;
	void InitPost() override;

	// 進行テーブルに基づく初期ステージと初期位置の適用
	void ApplyInitialProgressState();

	// 指定ステージへの切り替え
	void ChangeStage(Stage nextStage);

	// 会話終了に伴うストーリー進行の更新
	void UpdateTalkProgress();

	// 現在の進行度に対応する最初の会話の開始
	void StartFirstTalkByProgress();

	// ストーリー進行に応じた操作キャラの移動遅延の適用
	void ApplyControlMoveDelay();

	// ゲームメニューの入力と確定結果の更新
	void UpdateGameMenu();

	// 現在の状態に応じたゲームメニューの表示開始
	void OpenGameMenu();

	// ゲーム復帰情報を保存した設定画面への移動
	void OpenSettingFromGameMenu();

	// ステージごとの更新処理
	void UpdatePR();

	// ナースステーションでの決定操作とイベントの更新
	void UpdateNS();

	// 会話イベント制御の所有ポインタ
	std::unique_ptr<Talk> talk_;

	// ゲームメニューの所有ポインタ
	std::unique_ptr<GameMenu> gameMenu_;

	// 現在滞在中のステージ
	Stage currentStage_;

	// 操作キャラの移動許可状態
	bool canMove_ = false;

	// 現在のステージ実体の所有ポインタ
	std::unique_ptr<StageBase> stage_;

	// 奥行き順描画制御の所有ポインタ
	std::unique_ptr<Renderer2D> render_;

	// プレイヤーの所有ポインタ
	std::unique_ptr<Player> player_;

	// 患者の所有ポインタ
	std::unique_ptr<Patient> patient_;

	// 現在の操作対象キャラへの非所有ポインタ
	ActorBase* controlActor_;

	// デバッグ座標表示の所有ポインタ
	std::unique_ptr<DebugCursorPosition> debugCursorPosition_;

	// 色ずれ合成用の描画先ハンドル
	int colorShiftScreenHandle_ = -1;

	// 設定画面からの復帰中の状態
	bool isReturningFromSetting_;
};
