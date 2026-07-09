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
	static constexpr float LUNCH_MOVE_DELAY_SECONDS = 0.2f;
	static constexpr float GAME_FPS = 60.0f;

	
	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Delete(void) override;

private:
	void InitLoad() override;
	void InitPost(void) override;
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

	bool isReturningFromSetting_;
};
