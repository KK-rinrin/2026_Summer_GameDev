#include "Live2D.h"
#include "../FpsControl/FpsControl.h"

// static 初期化
int Live2D::s_renderCount = 0;

Live2D::Live2D()
{
	modelHandle = -1;
}

Live2D::~Live2D()
{
	// 明示的に DeleteModel() は呼ばない（ハブ側で管理）
}

void Live2D::Load(ResourceManager::SRC src)
{
	auto& model = ResourceManager::GetInstance().Load(src);
	if (model.type_ == Resource::TYPE::LIVE2D)
		modelHandle = model.handleId_;
}

void Live2D::Update(const TCHAR* motion)
{
	if (modelHandle < 0) return;

	if (Live2D_Model_IsMotionFinished(modelHandle) == TRUE)
	{
		Live2D_Model_StartMotion(modelHandle, motion, 0);
	}

	Live2D_Model_Update(modelHandle, static_cast<float>(1 / 60.0f));
}

// 追加: 即時にモーションを開始する（強制切替やポーズ切替に使用）
void Live2D::StartMotion(const TCHAR* motion, int priority)
{
	if (modelHandle < 0 || motion == nullptr) return;
	Live2D_Model_StartMotion(modelHandle, motion, priority);
}

void Live2D::DrawBegin()
{
	// 参照カウント方式で Live2D_RenderBegin を一度だけ呼ぶ
	if (s_renderCount == 0)
	{
		Live2D_RenderBegin();
	}
	++s_renderCount;
}

void Live2D::DrawModel()
{
	if (modelHandle < 0) return;
	// モデル描画
	Live2D_Model_Draw(modelHandle);
}

void Live2D::DrawEnd()
{
	// デクリメントし、0 になったら実際に RenderEnd を呼ぶ
	if (s_renderCount > 0)
	{
		--s_renderCount;
		if (s_renderCount == 0)
		{
			Live2D_RenderEnd();
		}
	}
}

void Live2D::DeleteModel()
{
	// Live2D モデル破棄
	if (modelHandle >= 0)
	{
		Live2D_DeleteModel(modelHandle);
		modelHandle = -1;
	}
}

void Live2D::SetExtendRate(float x, float y)
{
	if (modelHandle < 0) return;
	Live2D_Model_SetExtendRate(modelHandle, x, y);
}

void Live2D::SetTranslate(float x, float y)
{
	if (modelHandle < 0) return;
	Live2D_Model_SetTranslate(modelHandle, x, y);
}

void Live2D::SetParamerterValue(const TCHAR* parameterId, float value)
{
	if (modelHandle < 0) return;
	Live2D_Model_SetParameterValue(modelHandle, parameterId, value);
}