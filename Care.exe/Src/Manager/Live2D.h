#pragma once
#include <DxLib.h>
#include "ResourceManager.h"

class Live2D
{
public:
	Live2D();
	~Live2D();
	void Load(ResourceManager::SRC src);
	void Update(const TCHAR* motion);
	void DrawBegin();
	void DrawModel();
	void DrawEnd();
	void DeleteModel();
	void SetExtendRate(float x, float y);
	void SetTranslate(float x, float y);
	void SetParamerterValue(const TCHAR* parameterId, float value);
private:
	int modelHandle;
};
