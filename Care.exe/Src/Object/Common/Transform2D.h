#pragma once
#include <DxLib.h>

// 百分率座標と拡大率を保持する。座標変換は StageProjection2D に委譲する。
class Transform2D
{
public:

	// ステージ内百分率座標
	VECTOR pos;

	// 横方向の拡大率
	float scaleX;

	// 縦方向の拡大率
	float scaleY;

	Transform2D();

	// 座標を指定して初期化
	explicit Transform2D(const VECTOR& position);
	void Update();
	void Delete();

	// 自身の百分率座標に対応する画面座標の取得
	VECTOR GetWorldPos() const;

	// 指定した百分率座標に対応する画面座標の取得
	static VECTOR GetWorldPos(const VECTOR& localPos);

	// 画面座標からステージ内百分率座標への変換（未使用）
	static VECTOR WorldToLocalPercent(const VECTOR& worldPos);
};
