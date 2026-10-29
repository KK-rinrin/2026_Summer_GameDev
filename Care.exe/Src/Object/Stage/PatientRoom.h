#pragma once
#include "../../Common/Vector2.h"
#include "StageBase.h"

class PatientRoom : public StageBase
{
public:

	PatientRoom();
	~PatientRoom() override;

	void DrawGuide(const ActorBase& controlActor) const override;
	void Decide(DecideContext& context) const override;

private:

	// トイレの通過防止矩形の左上百分率座標
	static constexpr Vector2<float> TOILET_BLOCK_LEFT_TOP = { 75.0f, 5.0f };

	// トイレの通過防止矩形の右下百分率座標
	static constexpr Vector2<float> TOILET_BLOCK_RIGHT_BOTTOM = { 87.0f, 10.0f };

	// 壁画像の描画順基準Y値
	static constexpr float WALL_SORT_Y = 400.0f;

	// 壁の通過防止矩形の左上百分率座標
	static constexpr Vector2<float> WALL_BLOCK_LEFT_TOP = { 0.0f,51.0f };

	// 壁の通過防止矩形の右下百分率座標
	static constexpr Vector2<float> WALL_BLOCK_RIGHT_BOTTOM = { 100.0f,62.0f };

	// トイレ画像のステージ内百分率座標
	static constexpr Vector2<float> TOILET_POS = { 80.0f,10.0f };

	// 患者部屋からナースステーションへ移動するドア判定。
	static constexpr Vector2<float> TO_NURSE_STATION_AREA1_LEFT_TOP = { 95.0f, 55.0f };

	// 右側ドアの移動判定範囲の右下百分率座標
	static constexpr Vector2<float> TO_NURSE_STATION_AREA1_RIGHT_BOTTOM = { 100.0f, 90.0f };

	// 右側ドア経由でステーションへ入る百分率座標
	static constexpr Vector2<float> NURSE_STATION_MOVE_POS1 = { 1.0f, 90.0f };

	// 上側ドアの移動判定範囲の左上百分率座標
	static constexpr Vector2<float> TO_NURSE_STATION_AREA2_LEFT_TOP = { 3.0f, 0.0f };

	// 上側ドアの移動判定範囲の右下百分率座標
	static constexpr Vector2<float> TO_NURSE_STATION_AREA2_RIGHT_BOTTOM = { 30.0f, 5.0f };

	// 上側ドア経由でステーションへ入る百分率座標
	static constexpr Vector2<float> NURSE_STATION_MOVE_POS2 = { 85.0f, 0.0f };

	// ステーションへの上側移動ガイドの画面座標
	static constexpr Vector2<int> GUIDE_TO_NURSE_STATION_TOP_POS = { 285, 0 };

	// ステーションへの右側移動ガイドの画面座標
	static constexpr Vector2<int> GUIDE_TO_NURSE_STATION_RIGHT_POS = { 730, 180 };

	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;

	// 壁画像のハンドル
	int wallHandle_;

	// トイレ画像のハンドル
	int toiletHandle_;
};
