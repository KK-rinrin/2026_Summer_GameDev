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
	// 即時に指定モーションを再生したいときに呼ぶ（controller から直接再生）
	void StartMotion(const TCHAR* motion, int priority = 0);
	void DrawBegin();
	void DrawModel();
	void DrawEnd();
	void DeleteModel();
	void SetExtendRate(float x, float y);
	void SetTranslate(float x, float y);
	void SetParamerterValue(const TCHAR* parameterId, float value);

	// デバッグ／状態チェック用アクセサ
	int GetHandle() const { return modelHandle; }

private:
	int modelHandle;

	// RenderBegin/End の参照カウント（全インスタンスで共有）
	static int s_renderCount;
};