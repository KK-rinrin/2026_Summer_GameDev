#include <DxLib.h>
#include "ScreenEffect.h"

void ScreenEffect::DrawColorShift(int handle, float x, float y, float shiftX, float shiftY)
{
	if (handle == -1)
	{
		return;
	}

	int previousRed = 255;
	int previousGreen = 255;
	int previousBlue = 255;
	GetDrawBright(&previousRed, &previousGreen, &previousBlue);

	int previousBlendMode = DX_BLENDMODE_NOBLEND;
	int previousBlendParam = 0;
	GetDrawBlendMode(&previousBlendMode, &previousBlendParam);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	DrawColorChannel(handle, x - shiftX, y - shiftY, 255, 0, 0);
	DrawColorChannel(handle, x, y, 0, 255, 0);
	DrawColorChannel(handle, x + shiftX, y + shiftY, 0, 0, 255);

	SetDrawBright(previousRed, previousGreen, previousBlue);
	SetDrawBlendMode(previousBlendMode, previousBlendParam);
}

void ScreenEffect::DrawColorChannel(int handle, float x, float y, int red, int green, int blue)
{
	SetDrawBright(red, green, blue);
	DrawGraphF(x, y, handle, TRUE);
}
