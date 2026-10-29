#pragma once
#include "../../Common/Vector2.h"
#include <DxLib.h>

// ステージの百分率座標と画面上の座標・遠近倍率を変換する。
class StageProjection2D
{
public:

	// ステージ内百分率座標から画面座標への変換
	static Vector2<float> ToWorld(const Vector2<float>& localPos);

	// 画面座標からステージ内百分率座標への変換
	static Vector2<float> ToLocalPercent(const Vector2<float>& worldPos);

	// ステージ内座標の百分率範囲への制限
	static Vector2<float> ClampLocalPercent(Vector2<float> pos);

	// 画面Y座標に応じた遠近拡大率の取得
	static float GetPerspectiveScale(float worldY);

private:
	static constexpr int MOVE_MAX_Y = 570;		// 移動可能Y座標の最大値
	static constexpr int MOVE_MIN_Y = 210;		// 移動可能Y座標の最小値
	static constexpr int MOVE_MINY_MAX_X = 665;	// 最小Yでの最大X座標
	static constexpr int MOVE_MINY_MIN_X = 135;	// 最小Yでの最小X座標
	static constexpr int MOVE_MAXY_MAX_X = 750;	// 最大Yでの最大X座標
	static constexpr int MOVE_MAXY_MIN_X = 50;	// 最大Yでの最小X座標

	// 遠近での最小スケール（遠方でこの値になる）
	static constexpr float MOVE_PERSPECTIVE_MIN_SCALE = 0.7f;

	// ステージ内百分率座標の上限
	static constexpr float LOCAL_PERCENT_MAX = 100.0f;

	// ステージ内百分率座標の中央値
	static constexpr float LOCAL_PERCENT_MID = 50.0f;

	// 百分率Y座標から画面Y座標への変換
	static float PercentToY(float percentY);

	// 画面Y座標から百分率Y座標への変換
	static float WorldYToPercentY(float worldY);

	// 画面Y座標に対応する移動可能X座標の下限の算出
	static float CalcXMinForY(float y);

	// 画面Y座標に対応する移動可能X座標の上限の算出
	static float CalcXMaxForY(float y);

	// 画面座標から百分率X座標への変換
	static float WorldXToPercentX(float worldX, float y);
};
