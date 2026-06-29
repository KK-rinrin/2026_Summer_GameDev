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
	void ApplyInitialProgressState();
	void ChangeStage(Stage nextStage);

	void UpdateTalkProgress();
	void StartFirstTalkByProgress();
	void UpdateGameMenu();
	void OpenGameMenu();
	void OpenSettingFromGameMenu();
	void HandleStageDecideResult(const StageBase::DecideResult& result);

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

	bool firstUpdate_;
};
