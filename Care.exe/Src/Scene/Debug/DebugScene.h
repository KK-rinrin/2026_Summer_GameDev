#pragma once
#include "../../Application.h"
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
	DebugScene();

	// デストラクタ
	~DebugScene() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 解放
	void Delete() override;

private:
	static constexpr float GRID_PERCENT_MAX = 100.0f;
	static constexpr int GRID_LINE_COLOR = Application::DEFAULT_COLOR_0;

	static constexpr int INITIAL_GRID_DIVISION_X = 5;
	static constexpr int INITIAL_GRID_DIVISION_Y = 3;

	

	void InitLoad() override;

	int currentBG_ = 0;

	std::vector<int> BG_;	// 背景ハンドルたち

	DebugCursorPosition* cursorPos_;
	DebugPerspective* debugPers_;

	int gridDivisionX_ = INITIAL_GRID_DIVISION_X;	// グリッドX分割数
	int gridDivisionY_ = INITIAL_GRID_DIVISION_Y;	// グリッドY分割数

	static constexpr float COLOR_SHIFT_STEP_PIXELS = 1.0f;
	static constexpr float COLOR_SHIFT_MAX_PIXELS = 10.0f;
	static constexpr int COLOR_SHIFT_RECT_COUNT = 6;
	static constexpr float COLOR_SHIFT_RECT_CHANGE_SECONDS = 0.15f;
	float colorShiftPixels_ = 0.0f;
};