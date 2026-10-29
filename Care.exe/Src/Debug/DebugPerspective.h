#pragma once
#include "../Common/Vector2.h"
#include <DxLib.h>
class Transform2D;

class DebugPerspective
{
public:
	DebugPerspective();
	~DebugPerspective();

	// ƒ[ƒJƒ‹•S•ª—¦‚Åü‚ğ•`‰æ
	void DrawLineDebug(Vector2<float> StartLPos, Vector2<float> EndLPos, int color);
	
};