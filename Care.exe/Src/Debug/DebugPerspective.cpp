#include "DebugPerspective.h"
#include "../Object/Stage/StageProjection2D.h"

DebugPerspective::DebugPerspective()
{
}

DebugPerspective::~DebugPerspective()
{
}

void DebugPerspective::DrawLineDebug(Vector2<float> StartLPos, Vector2<float> EndLPos, int color)
{
	auto s = StageProjection2D::ToWorld(StartLPos);
	auto e = StageProjection2D::ToWorld(EndLPos);

	int sX = static_cast<int>(s.x);
	int sY = static_cast<int>(s.y);
	int eX = static_cast<int>(e.x);
	int eY = static_cast<int>(e.y);

	DrawLine(sX, sY, eX, eY, color);
}
