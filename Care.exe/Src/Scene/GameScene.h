#pragma once
#include "SceneBase.h"
#include "../Manager/Live2D.h"

class Talk;
class Player;
class Patient;
class Renderer2D;

class GameScene : public SceneBase
{

public:
	enum class Stage
	{
		PAT_ROOM,
		NURSE_STATION,
	};
	
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

	Talk* talk_;

	int BGHandle_[2];
	Stage currentStage_ = Stage::PAT_ROOM;

	bool canMove_ = false;

	Renderer2D* render_;
	Player* player_;
	Patient* patient_;

	bool firstUpdate_;
};