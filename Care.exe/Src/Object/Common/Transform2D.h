#pragma once
#include <DxLib.h>

// 百分率座標と拡大率を保持する。座標変換は StageProjection2D に委譲する。
class Transform2D
{
public:
	VECTOR pos;
	float scaleX;
	float scaleY;

	Transform2D();
	explicit Transform2D(const VECTOR& position);
	void Update();
	void Delete();

	VECTOR GetWorldPos() const;
	static VECTOR GetWorldPos(const VECTOR& localPos);
	static VECTOR WorldToLocalPercent(const VECTOR& worldPos);
};
