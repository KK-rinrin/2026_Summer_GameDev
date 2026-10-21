#include "StageProjection2D.h"
#include "../../Utility/SchoolUtility.h"

float StageProjection2D::PercentToY(float percentY) {
	const float t = SchoolUtility::Clamp(percentY, 0.0f, LOCAL_PERCENT_MAX) / LOCAL_PERCENT_MAX;
	return MOVE_MIN_Y + t * (MOVE_MAX_Y - MOVE_MIN_Y);
}
float StageProjection2D::WorldYToPercentY(float worldY) {
	const float t = (worldY - MOVE_MIN_Y) / static_cast<float>(MOVE_MAX_Y - MOVE_MIN_Y);
	return SchoolUtility::Clamp(t * LOCAL_PERCENT_MAX, 0.0f, LOCAL_PERCENT_MAX);
}
// y実座標に応じた許容する x の最小値
float StageProjection2D::CalcXMinForY(float y) {
	const float t = (y - MOVE_MIN_Y) / static_cast<float>(MOVE_MAX_Y - MOVE_MIN_Y);
	return MOVE_MINY_MIN_X + t * (MOVE_MAXY_MIN_X - MOVE_MINY_MIN_X);
}
// y実座標に応じた許容する x の最大値
float StageProjection2D::CalcXMaxForY(float y) {
	const float t = (y - MOVE_MIN_Y) / static_cast<float>(MOVE_MAX_Y - MOVE_MIN_Y);
	return MOVE_MINY_MAX_X + t * (MOVE_MAXY_MAX_X - MOVE_MINY_MAX_X);
}
// 実座標 x を、y に応じた範囲に収めたときの pos.x(%) を返す
float StageProjection2D::WorldXToPercentX(float worldX, float y) {
	const float xmin = CalcXMinForY(y);
	const float xmax = CalcXMaxForY(y);
	if (xmax - xmin == 0.0f) return LOCAL_PERCENT_MID;
	const float clamped = SchoolUtility::Clamp(worldX, xmin, xmax);
	return (clamped - xmin) / (xmax - xmin) * LOCAL_PERCENT_MAX;
}

VECTOR StageProjection2D::ToWorld(const VECTOR& localPos)
{
	VECTOR v = SchoolUtility::VECTOR_ZERO;

	const float actualY = PercentToY(localPos.y);
	const float xmin = CalcXMinForY(actualY);
	const float xmax = CalcXMaxForY(actualY);
	const float worldX = xmin + (localPos.x / LOCAL_PERCENT_MAX) * (xmax - xmin);

	v.x = worldX;
	v.y = actualY;
	v.z = 0.0f;

	return v;
}

VECTOR StageProjection2D::ToLocalPercent(const VECTOR& worldPos)
{
	VECTOR local = SchoolUtility::VECTOR_ZERO;
	local.x = WorldXToPercentX(worldPos.x, worldPos.y);
	local.y = WorldYToPercentY(worldPos.y);
	local.z = 0.0f;
	return local;
}

VECTOR StageProjection2D::ClampLocalPercent(VECTOR pos)
{
	pos.x = SchoolUtility::Clamp(pos.x, 0.0f, LOCAL_PERCENT_MAX);
	pos.y = SchoolUtility::Clamp(pos.y, 0.0f, LOCAL_PERCENT_MAX);

	// pos.y に対応する実座標 y を算出
	const float actualY = PercentToY(pos.y);

	// その y に対応する x の可動範囲を算出し、pos.x を補正する
	const float xmin = CalcXMinForY(actualY);
	const float xmax = CalcXMaxForY(actualY);
	// 現在の pos.x -> worldX
	const float worldX = xmin + (pos.x / LOCAL_PERCENT_MAX) * (xmax - xmin);
	// worldX を範囲内にクランプし、pos.x を再算出
	const float clampedWorldX = SchoolUtility::Clamp(worldX, xmin, xmax);
	pos.x = WorldXToPercentX(clampedWorldX, actualY);
	// pos.y, pos.x はここで 0..100 の有効値に調整済み

	return pos;
}

float StageProjection2D::GetPerspectiveScale(float worldY)
{
	const float t = (worldY - MOVE_MIN_Y) / (MOVE_MAX_Y - MOVE_MIN_Y);
	return MOVE_PERSPECTIVE_MIN_SCALE + (1.0f - MOVE_PERSPECTIVE_MIN_SCALE) * t;
}
