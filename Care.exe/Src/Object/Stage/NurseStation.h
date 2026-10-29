#pragma once
#include "../../Common/Vector2.h"
#include "StageBase.h"
#include <memory>

class PCScene;

class NurseStation : public StageBase
{
public:

	// PC画像のステージ内百分率座標
	static constexpr Vector2<float> PC_POS = { 35.0f,12.0f };

	// 椅子画像のステージ内百分率座標
	static constexpr Vector2<float> CHAIR_POS = { 35.0f,22.0f };

	// ロッカー画像のステージ内百分率座標
	static constexpr Vector2<float> LOCKER_POS = { 92.2f,99.9f };

	// ロッカー画像の描画順に使用するY基準値
	static constexpr float LOCKER_BUFFER_Y = 370.0f;

	// 配膳台画像の画面座標
	static constexpr Vector2<int> TABLE_POS = { 159, 365 };

	// 配膳台の通過防止矩形の左上百分率座標
	static constexpr Vector2<float> TABLE_LEFT_TOP = { 12.1f, 61.1f };

	// 配膳台の通過防止矩形の右下百分率座標
	static constexpr Vector2<float> TABLE_RIGHT_BOTTOM = { 59.8f, 100.0f };

	// 配膳プレート画像の画面座標
	static constexpr Vector2<int> PLATE_POS = { 217, 393 };

	// 配膳台の描画順基準Y値
	static constexpr float TABLE_SORT_Y = 600.0f;

	// 配膳プレートの描画順基準Y値
	static constexpr float PLATE_SORT_Y = 601.0f;

	// 配膳プレート取得範囲の左上百分率座標
	static constexpr Vector2<float> PLATE_LEFT_TOP = { 12.3f, 42.8f };

	// 配膳プレート取得範囲の右下百分率座標
	static constexpr Vector2<float> PLATE_RIGHT_BOTTOM = { 60.3f, 80.2f };

	// ナースステーションから患者部屋へ移動するドア判定
	static constexpr Vector2<float> TO_PATIENT_ROOM_AREA1_LEFT_TOP = { 0.0f, 55.0f };

	// 左側ドアの移動判定範囲の右下百分率座標
	static constexpr Vector2<float> TO_PATIENT_ROOM_AREA1_RIGHT_BOTTOM = { 5.0f, 90.0f };

	// 左側ドア経由で患者部屋へ入る百分率座標
	static constexpr Vector2<float> PATIENT_ROOM_MOVE_POS1 = { 98.0f, 90.0f };

	// 上側ドアの移動判定範囲の左上百分率座標
	static constexpr Vector2<float> TO_PATIENT_ROOM_AREA2_LEFT_TOP = { 80.0f, 0.0f };

	// 上側ドアの移動判定範囲の右下百分率座標
	static constexpr Vector2<float> TO_PATIENT_ROOM_AREA2_RIGHT_BOTTOM = { 95.0f, 5.0f };

	// 上側ドア経由で患者部屋へ入る百分率座標
	static constexpr Vector2<float> PATIENT_ROOM_MOVE_POS2 = { 15.0f, 0.0f };

	static constexpr Vector2<float> PC_LEFTUP = { 25.0f,15.0f };		// PC起動判定範囲左上
	static constexpr Vector2<float> PC_RIGHTDOWN = { 50.0f,30.0f };	// PC起動判定範囲右下

	// 患者部屋への上側移動ガイドの画面座標
	static constexpr Vector2<int> GUIDE_TO_PATIENT_ROOM_TOP_POS = { 590, 0 };

	// 患者部屋への左側移動ガイドの画面座標
	static constexpr Vector2<int> GUIDE_TO_PATIENT_ROOM_LEFT_POS = { 28, 168 };

	NurseStation();
	~NurseStation();

	void Update() override;
	void DrawForeground() const override;
	void RegisterObjects(Renderer2D& renderer) override;
	void Delete() override;
	bool IsInputBlocked() const override;
	void DrawGuide(const ActorBase& controlActor) const override;
	void Decide(DecideContext& context) const override;

private:

	// PC机の通過防止矩形の左上百分率座標
	static constexpr Vector2<float> PC_BLOCK_LEFT_TOP = { 0.0f, 0.0f };

	// PC机の通過防止矩形の右下百分率座標
	static constexpr Vector2<float> PC_BLOCK_RIGHT_BOTTOM = { 69.0f, 15.0f };

	// ロッカーの通過防止矩形の左上百分率座標
	static constexpr Vector2<float> LOCKER_BLOCK_LEFT_TOP = { 83.0f, 40.0f };

	// ロッカーの通過防止矩形の右下百分率座標
	static constexpr Vector2<float> LOCKER_BLOCK_RIGHT_BOTTOM = { 100.0f, 100.0f };

	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;

	// 進行度に対応する配膳プレートの描画
	void DrawPlate() const;

	// PC机画像のハンドル
	int pcHandle_;

	// 椅子画像のハンドル
	int chairHandle_;

	// ロッカー画像のハンドル
	int lockerHandle_;

	// 配膳台画像のハンドル
	int tableHandle_;

	// 昼食用配膳プレート画像のハンドル
	int plateHandle_;

	// 夕食用配膳プレート画像のハンドル
	int plateFolderHandle_;

	// ステーション内PC画面の所有ポインタ
	std::unique_ptr<PCScene> pcScene_;
};
