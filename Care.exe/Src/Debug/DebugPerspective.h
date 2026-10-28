#pragma once
#include <DxLib.h>
class Transform2D;

class DebugPerspective
{
public:
	DebugPerspective();
	~DebugPerspective();

	// ƒ[ƒJƒ‹•S•ª—¦‚Åü‚ğ•`‰æ
	void DrawLineDebug(VECTOR StartLPos, VECTOR EndLPos, int color);
	
};