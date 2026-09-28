#pragma once
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
	enum class Stage
	{
		PAT_ROOM,
		NURSE_STATION,
	};

	static constexpr Stage INIT_STAGE = Stage::NURSE_STATION;
	static constexpr float INPUT_MOVE_DELAY_SECONDS_0 = 0.2f;
	static constexpr float INPUT_MOVE_DELAY_SECONDS_1 = 0.4f;
	static constexpr float GAME_FPS = 60.0f;
	static constexpr float COLOR_SHIFT_X = 4.0f;
	static constexpr float COLOR_SHIFT_Y = 0.0f;
	static constexpr int COLOR_SHIFT_RECT_COUNT = 4;
	static constexpr float COLOR_SHIFT_RECT_CHANGE_SEC = 0.05f;
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
	void InitLoad() override;
	void InitPost() override;
	void ApplyInitialProgressState();
	void ChangeStage(Stage nextStage);

	void UpdateTalkProgress();
	void StartFirstTalkByProgress();
	void ApplyControlMoveDelay();
	void UpdateGameMenu();
	void OpenGameMenu();
	void OpenSettingFromGameMenu();

	// ステージごとの更新処理
	void UpdatePR();

	void UpdateNS();

	Talk* talk_;
	GameMenu* gameMenu_;

	Stage currentStage_;

	bool canMove_ = false;

	StageBase* stage_;
	Renderer2D* render_;
	Player* player_;
	Patient* patient_;
	ActorBase* controlActor_;

	DebugCursorPosition* debugCursorPosition_;
	int colorShiftScreenHandle_ = -1;

	bool isReturningFromSetting_;
};
