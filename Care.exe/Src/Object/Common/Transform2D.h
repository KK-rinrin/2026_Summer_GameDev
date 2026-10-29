#pragma once
#include "../../Common/Vector2.h"
#include <DxLib.h>

// 百分率座標と拡大率を保持する。座標変換は StageProjection2D に委譲する。
class Transform2D
{
public:

	// ステージ内百分率座標
	Vector2<float> pos;

	// 横方向の拡大率
	float scaleX;

	// 縦方向の拡大率
	float scaleY;

	Transform2D();

	// 座標を指定して初期化
	explicit Transform2D(const Vector2<float>& position);
	void Update();
	void Delete();

	// 自身の百分率座標に対応する画面座標の取得
	Vector2<float> GetWorldPos() const;

	// 指定した百分率座標に対応する画面座標の取得
	static Vector2<float> GetWorldPos(const Vector2<float>& localPos);

	// 画面座標からステージ内百分率座標への変換（未使用）
	static Vector2<float> WorldToLocalPercent(const Vector2<float>& worldPos);
};
