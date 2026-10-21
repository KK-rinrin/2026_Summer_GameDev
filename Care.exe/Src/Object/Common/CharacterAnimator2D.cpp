#include "CharacterAnimator2D.h"
#include "Sprite2D.h"
#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/SchoolUtility.h"
#include <cmath>

void CharacterAnimator2D::Configure(int framesPerDirection, int stillIndex, float frameSeconds)
{
	stillIndex_ = stillIndex;
	frameBaseIndex_ = 0;

	animation_.SetFrames(framesPerDirection);

	animation_.SetFrameDuration(frameSeconds);

	animation_.SetStillIndex(stillIndex);

	animation_.SetLoop(true);

}

void CharacterAnimator2D::ShowStill(Sprite2D& sprite)
{
	animation_.Stop();

	// 最後に表示した向きの静止フレームを使い、左右反転も維持する。
	sprite.SetFrame(frameBaseIndex_ + stillIndex_);

}

void CharacterAnimator2D::Update(Sprite2D& sprite, const VECTOR& movement, float worldX)
{
	if (sprite.GetFrameCount() == 0 || sprite.GetColumns() <= 1)
	{
		ShowStill(sprite);
		return;
	}

	const float dx = movement.x;
	const float dy = movement.y;
	const float absdx = std::fabs(dx);
	const float absdy = std::fabs(dy);

	bool isMoving = !(absdx < MOVEMENT_EPSILON && absdy < MOVEMENT_EPSILON);

	// 静止
	if (!isMoving)
	{
		ShowStill(sprite);
		return;
	}

	// 動いている → 再生
	animation_.Play();
	animation_.Update();

	// 方向推定（8方向）
	int deducedRot = direction_;

	if (!(absdx < MOVEMENT_EPSILON && absdy < MOVEMENT_EPSILON))
	{

		float angleDeg = std::atan2(dy, dx) * RAD_TO_DEG;
		if (angleDeg < 0.0f) angleDeg += 360.0f;

		int sector = static_cast<int>((angleDeg + SECTOR_HALF_ANGLE_DEGREES) / SECTOR_ANGLE_DEGREES) % DIRECTION_COUNT;

		deducedRot = SECTOR_TO_ROT[sector];

	}
	direction_ = deducedRot;

	// スプライト基点
	int framesPerDir = (sprite.GetColumns() > 0) ? sprite.GetColumns() : 1;
	int total = sprite.GetFrameCount();

	int baseIndex = 0;
	bool flip = false;

	// 特殊ケース: 前後2セット
	if (framesPerDir == ResourceManager::CHARACTER_SHEET_COLUMNS && total == framesPerDir * ResourceManager::CHARACTER_SHEET_ROWS)
	{
		bool verticalStrict = (absdx < MOVEMENT_EPSILON && absdy >= MOVEMENT_EPSILON);

		bool useBack = false;
		if (absdy >= MOVEMENT_EPSILON)
		{
			useBack = (dy < 0.0f);
		}

		baseIndex = useBack ? framesPerDir : 0;

		if (verticalStrict)
		{
			const float screenCenterX = static_cast<float>(Application::SCREEN_SIZE_X) * 0.5f;
			flip = (worldX < screenCenterX);
		}
		else
		{
			flip = (dx < 0.0f);
		}

	}
	else if (sprite.GetRows() == LEGACY_DIRECTION_SHEET_ROWS && total == framesPerDir * ResourceManager::CHARACTER_SHEET_ROWS)
	{
		int half = total / 2;

		if (direction_ == LEGACY_FRONT_DIRECTION || direction_ == LEGACY_FRONT_FLIPPED_DIRECTION) baseIndex = 0;
		else baseIndex = half;

		flip = (direction_ == LEGACY_FRONT_FLIPPED_DIRECTION) || (direction_ == LEGACY_BACK_FLIPPED_DIRECTION);
	
	}
	else
	{
		baseIndex = direction_ * framesPerDir;

		if (baseIndex < 0 || baseIndex >= total) baseIndex = 0;

		flip = false;
	}

	frameBaseIndex_ = baseIndex;
	sprite.SetFlipX(flip);

	int frameOffset = animation_.GetFrame();

	int target = baseIndex + frameOffset;

	sprite.SetFrame(SchoolUtility::Clamp(target, 0, total - 1));

}
