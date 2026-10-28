#pragma once
#include <memory>
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

	// デバッググリッドの百分率座標の上限
	static constexpr float GRID_PERCENT_MAX = 100.0f;

	// デバッググリッド線の色
	static constexpr int GRID_LINE_COLOR = Application::DEFAULT_COLOR_0;

	// グリッドの横方向の初期分割数
	static constexpr int INITIAL_GRID_DIVISION_X = 5;

	// グリッドの縦方向の初期分割数
	static constexpr int INITIAL_GRID_DIVISION_Y = 3;

	

	void InitLoad() override;

	// 現在表示中の背景のインデックス
	int currentBG_ = 0;

	std::vector<int> BG_;	// 背景ハンドルたち

	// カーソル座標表示の所有ポインタ
	std::unique_ptr<DebugCursorPosition> cursorPos_;

	// ステージ百分率座標のデバッグ線描画の所有ポインタ
	std::unique_ptr<DebugPerspective> debugPers_;

	int gridDivisionX_ = INITIAL_GRID_DIVISION_X;	// グリッドX分割数
	int gridDivisionY_ = INITIAL_GRID_DIVISION_Y;	// グリッドY分割数

	// 色ずれ量の1操作当たりの変更ピクセル数
	static constexpr float COLOR_SHIFT_STEP_PIXELS = 1.0f;

	// 色ずれ量の最大ピクセル数
	static constexpr float COLOR_SHIFT_MAX_PIXELS = 10.0f;

	// 色ずれ演出の矩形領域数
	static constexpr int COLOR_SHIFT_RECT_COUNT = 6;

	// 色ずれ矩形の更新間隔の秒数
	static constexpr float COLOR_SHIFT_RECT_CHANGE_SECONDS = 0.15f;

	// 現在の色ずれ量のピクセル数
	float colorShiftPixels_ = 0.0f;
};
