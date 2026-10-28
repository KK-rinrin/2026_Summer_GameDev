#pragma once
#include "AnimationController2D.h"
class Sprite2D;

class CharacterAnimator2D
{
public:

	// 向きごとのコマ数と静止コマと再生時間の設定
	void Configure(int framesPerDirection, int stillIndex, float frameSeconds = 0.0f);

	// 往復再生の有効状態の設定
	void SetPingPong(bool enabled) { animation_.SetPingPong(enabled); }

	// 現在の向きに対応する静止コマの表示
	void ShowStill(Sprite2D& sprite);
	void Update(Sprite2D& sprite, const VECTOR& movement, float worldX);

private:
	// 8方向
	enum Direction : int
	{

		// 下向きの方向番号
		DOWN,

		// 右下向きの方向番号
		DOWN_RIGHT,

		// 右向きの方向番号
		RIGHT,

		// 右上向きの方向番号
		UP_RIGHT,

		// 上向きの方向番号
		UP, 

		// 左上向きの方向番号
		UP_LEFT,

		// 左向きの方向番号
		LEFT,

		// 左下向きの方向番号
		DOWN_LEFT
	};

	// ラジアンから度への換算倍率
	static constexpr float RAD_TO_DEG = 180.0f / 3.14159265f;

	// 移動とみなす最小変位
	static constexpr float MOVEMENT_EPSILON = 0.1f;

	// 方向の総数
	static constexpr int DIRECTION_COUNT = 8;

	// 方向区分一つ分の角度
	static constexpr float SECTOR_ANGLE_DEGREES = 45.0f;

	// 方向区分の中心から境界までの角度
	static constexpr float SECTOR_HALF_ANGLE_DEGREES = 22.5f;

	// 角度区分からアニメーション方向番号への対応表
	static constexpr int SECTOR_TO_ROT[DIRECTION_COUNT] =
	{ RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT };

	// 旧形式画像シートの方向別行数
	static constexpr int LEGACY_DIRECTION_SHEET_ROWS = 4;

	// 旧形式画像シートの正面方向番号
	static constexpr int LEGACY_FRONT_DIRECTION = 0;

	// 旧形式画像シートの左右反転対象の正面方向番号
	static constexpr int LEGACY_FRONT_FLIPPED_DIRECTION = 3;

	// 旧形式画像シートの左右反転対象の背面方向番号
	static constexpr int LEGACY_BACK_FLIPPED_DIRECTION = 2;

	// コマ送りの制御情報
	AnimationController2D animation_;

	// 停止中に表示する相対コマ番号
	int stillIndex_ = 0;
	int frameBaseIndex_ = 0; // 最後に表示した向きの先頭フレーム

	// 現在の向きの方向番号
	int direction_ = 0;
};
