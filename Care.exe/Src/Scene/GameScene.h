#pragma once
#include "SceneBase.h"
#include "../Manager/Live2D.h"

class Talk;
class Player;

class GameScene : public SceneBase
{

public:
	enum class Stage
	{
		PICU,
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
	Stage currentStage_ = Stage::PICU;

	bool canMove_ = false;

	Player* player_;

	bool firstUpdate_;
};
