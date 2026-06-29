#pragma once
#include "../SceneBase.h"
#include "../../Manager/Live2D.h"
#include "../../Object/Stage/StageBase.h"

class Player;
class Patient;
class ActorBase;
class Renderer2D;
class DebugCursorPosition;
class GameMenu;
class DebugPerspective;

class DebugScene : public SceneBase
{

public:
	// コンストラクタ
	DebugScene(void);

	// デストラクタ
	~DebugScene(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Delete(void) override;

private:
	void InitLoad() override;

	int currentBG_ = 0;

	std::vector<int> BG_;	// 背景ハンドルたち

	DebugCursorPosition* cursorPos_;
	DebugPerspective* debugPers_;

	int gridDivisionX_ = 5;	// グリッドX分割数
	int gridDivisionY_ = 3;	// グリッドY分割数
};