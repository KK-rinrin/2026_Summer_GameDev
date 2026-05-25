#pragma once
#include "SceneBase.h"
#include "../Manager/Live2D.h"

class Talk;
class Player;
class Patient;
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
	static constexpr VECTOR PR_TO_NS_AREA1_0 = { 95.0f, 55.0f, 0.0f };
	static constexpr VECTOR PR_TO_NS_AREA1_1 = { 100.0f, 90.0f, 0.0f };
	static constexpr VECTOR NS_MOVE_POS = { 1.0f,90.0f,0.0f };

	static constexpr VECTOR NS_TO_PR_AREA1_0 = { 0.0f, 55.0f, 0.0f };
	static constexpr VECTOR NS_TO_PR_AREA1_1 = { 5.0f, 90.0f, 0.0f };
	static constexpr VECTOR PR_MOVE_POS = { 98.0f,90.0f,0.0f };

	
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
	void ChangeStage(Stage nextStage);
	void UpdatePR();
	void UpdateNS();

	Talk* talk_;

	Stage currentStage_ = Stage::PAT_ROOM;

	bool canMove_ = false;

	StageBase* stage_;
	Renderer2D* render_;
	Player* player_;
	Patient* patient_;

	DebugCursorPosition* debugCursorPosition_;

	bool firstUpdate_;
};
