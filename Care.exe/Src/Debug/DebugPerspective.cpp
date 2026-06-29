#include "DebugPerspective.h"
#include "../Object/Common/2DTransform.h"

DebugPerspective::DebugPerspective()
{
}

DebugPerspective::~DebugPerspective()
{
}

void DebugPerspective::DrawLineDebug(VECTOR StartLPos, VECTOR EndLPos, int color)
{
#ifdef _DEBUG
	auto s = Transform2D::GetWorldPos(StartLPos);
	auto e = Transform2D::GetWorldPos(EndLPos);

	DrawLine(s.x, s.y, e.x, e.y, color);
#endif
}
