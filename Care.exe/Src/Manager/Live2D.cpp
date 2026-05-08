#include "Live2D.h"
#include "../FpsControl/FpsControl.h"

Live2D::Live2D()
{
	modelHandle = -1;
}

Live2D::~Live2D()
{

}

void Live2D::Load(ResourceManager::SRC src)
{
	auto& model = ResourceManager::GetInstance().Load(src);
	if (model.type_ == Resource::TYPE::LIVE2D)
		modelHandle = model.handleId_;
}

void Live2D::Update(const TCHAR* motion)
{
	if (Live2D_Model_IsMotionFinished(modelHandle) == TRUE)
	{
		Live2D_Model_StartMotion(modelHandle, motion, 0);
	}

	Live2D_Model_Update(modelHandle, static_cast<float>(1 / 60.0f));

}

void Live2D::DrawBegin()
{
	// Live2D描画の開始
	Live2D_RenderBegin();
}

void Live2D::DrawModel()
{
	// モデルの描画
	Live2D_Model_Draw(modelHandle);
}

void Live2D::DrawEnd()
{
	// Live2D描画の終了
	Live2D_RenderEnd();
}

void Live2D::DeleteModel()
{
	// Live2D モデルの削除
	//Live2D_DeleteModel(modelHandle);
}

void Live2D::SetExtendRate(float x, float y)
{
	Live2D_Model_SetExtendRate(modelHandle, x, y);
}

void Live2D::SetTranslate(float x, float y)
{
	Live2D_Model_SetTranslate(modelHandle, x, y);
}

void Live2D::SetParamerterValue(const TCHAR* parameterId, float value)
{
	Live2D_Model_SetParameterValue(modelHandle, parameterId, value);
}
