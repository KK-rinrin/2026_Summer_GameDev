#include "Live2DTalkController.h"

Live2DTalkController::Live2DTalkController()
	: model_(nullptr)
{
	InitDefaultParamMap();
}

Live2DTalkController::~Live2DTalkController()
{
	DeleteModel();
}

void Live2DTalkController::InitDefaultParamMap()
{
	// デフォルト：口は既存のパラメータ名
	paramMap_[static_cast<int>(Param::MOUTH)] = TEXT("ParamMouthOpenY");
	// 代表的なもの（モデルによっては未定義）
	paramMap_[static_cast<int>(Param::EYE_BLINK_R)] = TEXT("ParamEyeROpen");
	paramMap_[static_cast<int>(Param::EYE_BLINK_L)] = TEXT("ParamEyeLOpen");
	paramMap_[static_cast<int>(Param::BROW_RY)] = TEXT("ParamBrowRY");
	paramMap_[static_cast<int>(Param::BROW_LY)] = TEXT("ParamBrowLY");
	paramMap_[static_cast<int>(Param::ALPHA)] = TEXT("ParamAlpha");
	paramMap_[static_cast<int>(Param::PAT_EYE_BLACK)] = TEXT("Param");
}

void Live2DTalkController::Load(ResourceManager::SRC src)
{
	model_.reset(new Live2D());
	model_->Load(src);
}

void Live2DTalkController::Update(const TCHAR* motion)
{
	if (model_) model_->Update(motion);
}

void Live2DTalkController::DrawBegin()
{
	if (model_) model_->DrawBegin();
}

void Live2DTalkController::DrawModel()
{
	if (model_) model_->DrawModel();
}

void Live2DTalkController::DrawEnd()
{
	if (model_) model_->DrawEnd();
}

void Live2DTalkController::DeleteModel()
{
	if (model_)
	{
		model_->DeleteModel();
		model_.reset();
	}
}

void Live2DTalkController::SetExtendRate(float x, float y)
{
	if (model_) model_->SetExtendRate(x, y);
}

void Live2DTalkController::SetTranslate(float x, float y)
{
	if (model_) model_->SetTranslate(x, y);
}

void Live2DTalkController::RegisterParam(Param p, const TCHAR* parameterId)
{
	if (!parameterId) return;
	paramMap_[static_cast<int>(p)] = parameterId;
}

void Live2DTalkController::SetParamValue(Param p, float value)
{
	if (!model_) return;
	auto it = paramMap_.find(static_cast<int>(p));
	if (it == paramMap_.end()) return; // 未登録なら何もしない
	const std::basic_string<TCHAR>& id = it->second;
	model_->SetParamerterValue(id.c_str(), value);
}

void Live2DTalkController::ResetParam(Param p)
{
	SetParamValue(p, 0.0f);
}

void Live2DTalkController::ResetAllParams()
{
	if (!model_) return;
	for (auto& kv : paramMap_)
	{
		model_->SetParamerterValue(kv.second.c_str(), 0.0f);
	}
}

void Live2DTalkController::ResetMouth()
{
	ResetParam(Param::MOUTH);
}

void Live2DTalkController::SetMouthProgress(float t)
{
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;
	SetParamValue(Param::MOUTH, t);
}

void Live2DTalkController::CloseMouth()
{
	ResetMouth();
}

void Live2DTalkController::SetAlpha(int alpha)
{
	SetParamValue(Param::ALPHA, static_cast<float>(alpha));
}

Live2D* Live2DTalkController::GetModel() const
{
	return model_.get();
}