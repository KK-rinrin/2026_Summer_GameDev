#include "Collider.h"
#include "../../Utility/SchoolUtility.h"
#include <algorithm>
#include <cmath>

Vector2<float> Collision::BlockCrossingLocalRect(const Vector2<float>& beforePos, const Vector2<float>& currentPos,
	const Vector2<float>& leftTopPercent, const Vector2<float>& rightBottomPercent)
{
	Vector2<float> pos = currentPos;
	float left = leftTopPercent.x;
	float top = leftTopPercent.y;
	float right = rightBottomPercent.x;
	float bottom = rightBottomPercent.y;

	if (right < left) std::swap(left, right);
	if (bottom < top) std::swap(top, bottom);

	left = SchoolUtility::Clamp(left, 0.0f, LOCAL_PERCENT_MAX);
	top = SchoolUtility::Clamp(top, 0.0f, LOCAL_PERCENT_MAX);
	right = SchoolUtility::Clamp(right, 0.0f, LOCAL_PERCENT_MAX);
	bottom = SchoolUtility::Clamp(bottom, 0.0f, LOCAL_PERCENT_MAX);

	const float bx = beforePos.x;
	const float by = beforePos.y;
	const float cx = pos.x;
	const float cy = pos.y;
	const float dx = cx - bx;
	const float dy = cy - by;

	auto isInRect = [&](float x, float y) {
		return x >= left && x <= right && y >= top && y <= bottom;
		};

	const bool beforeInside = isInRect(bx, by);
	const bool currentInside = isInRect(cx, cy);

	if (fabsf(dx) < EPS && fabsf(dy) < EPS)
	{
		if (currentInside)
		{
			pos = beforePos;
		}
		return pos;
	}

	if (beforeInside)
	{
		if (currentInside)
		{
			pos = beforePos;
		}
		return pos;
	}

	float hitT = 2.0f;
	bool hit = false;

	auto addHit = [&](float t, float x, float y) {
		if (t < -EPS || t > 1.0f + EPS) return;
		if (x < left - EPS || x > right + EPS) return;
		if (y < top - EPS || y > bottom + EPS) return;
		if (t < hitT)
		{
			hitT = t;
			hit = true;
		}
		};

	if (fabsf(dx) >= EPS)
	{
		float t = (left - bx) / dx;
		addHit(t, left, by + t * dy);

		t = (right - bx) / dx;
		addHit(t, right, by + t * dy);
	}

	if (fabsf(dy) >= EPS)
	{
		float t = (top - by) / dy;
		addHit(t, bx + t * dx, top);

		t = (bottom - by) / dy;
		addHit(t, bx + t * dx, bottom);
	}

	if (hit)
	{
		const float moveLen = fabsf(dx) + fabsf(dy);
		const float backT = (moveLen > EPS) ? (OUTSIDE_MARGIN / moveLen) : 1.0f;
		const float stopT = SchoolUtility::Clamp(hitT - backT, 0.0f, 1.0f);
		pos.x = bx + dx * stopT;
		pos.y = by + dy * stopT;
		return pos;
	}

	if (currentInside)
	{
		pos = beforePos;
	}
	return pos;
}

