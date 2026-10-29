#pragma once
#include <type_traits>

/// 2D座標（intもしくはfloat型）
/// @param 初期化時に決まる座標の型
template<typename T>
struct Vector2
{
	static_assert(std::is_same_v<T, int> || std::is_same_v<T, float>,
		"Vector2 supports int and float components. Vector2はintとfloatにのみ対応しています。");

	T x{};
	T y{};

	constexpr Vector2() = default;
	constexpr Vector2(T vX, T vY) : x(vX), y(vY) {}

	// 整数からfloatへの自動変換
	// @param 変換元の型
	template<typename U, std::enable_if_t<
		std::is_same_v<T, float> && std::is_same_v<U, int>, int> = 0>
	constexpr Vector2(const Vector2<U>& value)
		: x(static_cast<T>(value.x)), y(static_cast<T>(value.y)) {}

	constexpr Vector2 operator-(const Vector2& other) const
	{
		return { x - other.x, y - other.y };
	}
};

// 初期値に合わせた型の選択
template<typename X, typename Y>
Vector2(X, Y) -> Vector2<std::common_type_t<X, Y>>;
