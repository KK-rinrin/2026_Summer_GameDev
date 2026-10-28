#pragma once
class Vector2
{
public:

	// 横方向の整数座標
	int x = 0;

	// 縦方向の整数座標
	int y = 0;

	// コンストラクタ
	constexpr Vector2() = default;

	// コンストラクタ
	constexpr Vector2(int vX, int vY)
		:
		x(vX),
		y(vY)
	{
	}

	// デストラクタ
	~Vector2() = default;

};
