#pragma once
class Vector2
{
public:

	int x = 0;
	int y = 0;

	// コンストラクタ
	constexpr Vector2(void) = default;

	// コンストラクタ
	constexpr Vector2(int vX, int vY)
		:
		x(vX),
		y(vY)
	{
	}

	// デストラクタ
	~Vector2(void) = default;

};
