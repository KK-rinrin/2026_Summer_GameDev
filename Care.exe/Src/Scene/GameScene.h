#pragma once
#include "SceneBase.h"
#include "../Manager/Live2D.h"

class Talk;
class Player;
class Patient;
class ActorBase;
class Renderer2D;
class StageBase;
class DebugCursorPosition;

class GameScene : public SceneBase
{

public:
	enum class Stage
	{
		PAT_ROOM,
		NURSE_STATION,
	};

	static constexpr Stage INIT_STAGE = Stage::NURSE_STATION;

	// 患者部屋PR前ドア←→ナースステーションNS前ドアの遷移エリア
	static constexpr VECTOR PR_TO_NS_AREA1_0 = { 95.0f, 55.0f, 0.0f };
	static constexpr VECTOR PR_TO_NS_AREA1_1 = { 100.0f, 90.0f, 0.0f };
	static constexpr VECTOR NS_MOVE_POS1 = { 1.0f,90.0f,0.0f };

	static constexpr VECTOR NS_TO_PR_AREA1_0 = { 0.0f, 55.0f, 0.0f };
	static constexpr VECTOR NS_TO_PR_AREA1_1 = { 5.0f, 90.0f, 0.0f };
	static constexpr VECTOR PR_MOVE_POS1 = { 98.0f,90.0f,0.0f };

	// 患者部屋PR後ドア←→ナースステーションNS後ドアの遷移エリア
	static constexpr VECTOR PR_TO_NS_AREA2_0 = { 3.0f, 0.0f, 0.0f };
	static constexpr VECTOR PR_TO_NS_AREA2_1 = { 30.0f, 5.0f, 0.0f };
	static constexpr VECTOR NS_MOVE_POS2 = { 85.0f,0.0f,0.0f };

	static constexpr VECTOR NS_TO_PR_AREA2_0 = { 80.0f, 0.0f, 0.0f };
	static constexpr VECTOR NS_TO_PR_AREA2_1 = { 95.0f, 5.0f, 0.0f };
	static constexpr VECTOR PR_MOVE_POS2 = { 15.0f,0.0f,0.0f };
	
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

	// ステージごとの更新処理
	void UpdatePR();
	void DecidePR();

	void UpdateNS();
	void DecideNS();

	Talk* talk_;

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
