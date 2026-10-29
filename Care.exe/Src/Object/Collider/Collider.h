#pragma once
#include "../../Common/Vector2.h"
#include <DxLib.h>

// コライダークラス群
enum class ColliderType {
	None,	// コライダー種別なし
	Circle,	// 円形コライダーの種別
	Rect	// 矩形コライダーの種別
};

// コライダーベースクラス
struct Collider {

	// コライダーの形状種別
	ColliderType type = ColliderType::None;
	virtual ~Collider() {}
};

// 円コライダー
struct CircleCollider : public Collider {

	// 円形コライダーの半径
	float radius;

	CircleCollider(float r) {
		type = ColliderType::Circle;
		radius = r;
	}
};

// 矩形コライダー
struct RectCollider : public Collider {

	// 矩形コライダーの幅の半分
	float halfW;

	// 矩形コライダーの高さの半分
	float halfH;

	RectCollider(float w, float h) {
		type = ColliderType::Rect;
		halfW = w;
		halfH = h;
	}
};

// 衝突判定ユーティリティ（点と円の判定などを集約）
class Collision
{
public:

	// 移動線分が矩形障害物を横切らない座標への補正
	static Vector2<float> BlockCrossingLocalRect(const Vector2<float>& beforePos, const Vector2<float>& currentPos,
		const Vector2<float>& leftTopPercent, const Vector2<float>& rightBottomPercent);


	// 点(point)が中心(center), 半径(radius)の円の内側にいるか
	static bool IsPointInCircle(const Vector2<float>& point, const Vector2<float>& center, float radius)
	{

		// 点と円中心のX方向の差
		const float dx = point.x - center.x;

		// 点と円中心のY方向の差
		const float dy = point.y - center.y;
		return (dx * dx + dy * dy) <= (radius * radius);
	}

	// 中心座標と半幅と半高さによる点の矩形内判定（未使用）
	static bool IsPointInRect(const Vector2<float>& point, const Vector2<float>& center, float halfW, float halfH)
	{
		return (point.x >= center.x - halfW && point.x <= center.x + halfW &&
				point.y >= center.y - halfH && point.y <= center.y + halfH);
	}

	// 点(point)が左上(leftTop), 右下(rightBottom)の矩形の内側にいるか
	static bool IsPointInRect(const Vector2<float>& point, const Vector2<float>& leftTop, const Vector2<float>& rightBottom)
	{
		return (point.x >= leftTop.x && point.x <= rightBottom.x &&
			point.y >= leftTop.y && point.y <= rightBottom.y);
	}

	// 二つの円の接触と重なりの判定（未使用）
	static bool CollisionCircle(const Vector2<float>& centerA, float radiusA, const Vector2<float>& centerB, float radiusB)
	{

		// 円中心同士のX方向の差
		const float dx = centerA.x - centerB.x;

		// 円中心同士のY方向の差
		const float dy = centerA.y - centerB.y;

		// 二つの円の半径の合計
		const float combinedRadius = radiusA + radiusB;
		return (dx * dx + dy * dy) <= (combinedRadius * combinedRadius);
	}

private:

	// 百分率座標の上限
	static constexpr float LOCAL_PERCENT_MAX = 100.0f;

	// 移動差をゼロとみなす微小値
	static constexpr float EPS = 1e-5f;

	// 障害物の外側へ押し出す百分率座標の余白
	static constexpr float OUTSIDE_MARGIN = 0.01f;
};
