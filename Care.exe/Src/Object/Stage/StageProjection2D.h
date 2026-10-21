#pragma once
#include <DxLib.h>

// ステージの百分率座標と画面上の座標・遠近倍率を変換する。
class StageProjection2D
{
public:
	static VECTOR ToWorld(const VECTOR& localPos);
	static VECTOR ToLocalPercent(const VECTOR& worldPos);
	static VECTOR ClampLocalPercent(VECTOR pos);
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
	static constexpr float LOCAL_PERCENT_MAX = 100.0f;
	static constexpr float LOCAL_PERCENT_MID = 50.0f;

	static float PercentToY(float percentY);
	static float WorldYToPercentY(float worldY);
	static float CalcXMinForY(float y);
	static float CalcXMaxForY(float y);
	static float WorldXToPercentX(float worldX, float y);
};
