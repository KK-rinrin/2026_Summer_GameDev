#pragma once

// コライダークラス群
enum class ColliderType {
    None,
    Circle,
    Rect
};

// コライダーベースクラス
struct Collider {
    ColliderType type = ColliderType::None;
    virtual ~Collider() {}
};

// 円コライダー
struct CircleCollider : public Collider {
    float radius;

    CircleCollider(float r) {
        type = ColliderType::Circle;
        radius = r;
    }
};

// 矩形コライダー
struct RectCollider : public Collider {
    float halfW;
    float halfH;

    RectCollider(float w, float h) {
        type = ColliderType::Rect;
        halfW = w;
        halfH = h;
    }
};

// 衝突判定ユーティリティ（点と円の判定などを集約）
struct Collision
{
    // 点(point)が中心(center), 半径(radius)の円の内側にいるか
    static bool IsPointInCircle(const VECTOR& point, const VECTOR& center, float radius)
    {
        const float dx = point.x - center.x;
        const float dy = point.y - center.y;
        return (dx * dx + dy * dy) <= (radius * radius);
    }

	// 点(point)が中心(center), 半幅(halfW), 半高さ(halfH)の矩形の内側にいるか
    static bool IsPointInRect(const VECTOR& point, const VECTOR& center, float halfW, float halfH)
    {
        return (point.x >= center.x - halfW && point.x <= center.x + halfW &&
                point.y >= center.y - halfH && point.y <= center.y + halfH);
	}
};