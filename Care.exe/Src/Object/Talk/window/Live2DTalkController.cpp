#include "Live2DTalkController.h"
#include "../../../Manager/Live2DModelHub.h"
#include <Windows.h> // OutputDebugStringA

Live2DTalkController::Live2DTalkController()
	: model_(nullptr)
	, lastMenuIndex_(-1)
{
	InitDefaultParamMap();
}

Live2DTalkController::~Live2DTalkController()
{
	DeleteModel();
}

void Live2DTalkController::InitDefaultParamMap()
{
	paramMap_[static_cast<int>(Param::MOUTH)] = TEXT("ParamMouthOpenY");
	paramMap_[static_cast<int>(Param::EYE_BLINK_R)] = TEXT("ParamEyeROpen");
	paramMap_[static_cast<int>(Param::EYE_BLINK_L)] = TEXT("ParamEyeLOpen");
	paramMap_[static_cast<int>(Param::BROW_RY)] = TEXT("ParamBrowRY");
	paramMap_[static_cast<int>(Param::BROW_LY)] = TEXT("ParamBrowLY");
	paramMap_[static_cast<int>(Param::FACE_X)] = TEXT("ParamAngleX");
	paramMap_[static_cast<int>(Param::FACE_Y)] = TEXT("ParamAngleY");
	paramMap_[static_cast<int>(Param::FACE_Z)] = TEXT("ParamAngleZ");
	paramMap_[static_cast<int>(Param::ALPHA)] = TEXT("ParamAlpha");
	paramMap_[static_cast<int>(Param::PAT_EYE_BLACK)] = TEXT("Param");
}

float Live2DTalkController::GetDefaultParamValue(Param p) const
{
	switch (p)
	{
	case Param::EYE_BLINK_R:
	case Param::EYE_BLINK_L:
		return 1.0f;
	case Param::ALPHA:
		return 255.0f;
	default:
		return 0.0f;
	}
}

float Live2DTalkController::GetDefaultParamValue(const TCHAR* parameterId) const
{
	if (parameterId == nullptr) return 0.0f;

	const std::basic_string<TCHAR> id(parameterId);
	if (id == TEXT("ParamEyeROpen") || id == TEXT("ParamEyeLOpen")) return 1.0f;
	if (id == TEXT("ParamAlpha")) return 255.0f;
	return 0.0f;
}

void Live2DTalkController::Load(ResourceManager::SRC src)
{
	// ハブから共有モデルを取得（存在しなければハブが生成して Load する）
	model_ = Live2DModelHub::Instance().GetModel(src);
	lastMenuIndex_ = -1; // 再ロード時は強制再生可能にする

	// デバッグ: モデル取得状態を出力
	{
		char buf[256];
		sprintf_s(buf, "Live2DTalkController::Load src=%d model_ptr=%p handle=%d\n",
			static_cast<int>(src), static_cast<void*>(model_), model_ ? model_->GetHandle() : -1);
		OutputDebugStringA(buf);
	}
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
	// コントローラはモデルを所有しないので解放しない。参照をクリアするのみ。
	if (model_)
	{
		model_ = nullptr;
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
	if (it == paramMap_.end()) return;
	const std::basic_string<TCHAR>& id = it->second;
	model_->SetParamerterValue(id.c_str(), value);
}

void Live2DTalkController::SetTalkParamValue(Param p, float value)
{
	auto it = paramMap_.find(static_cast<int>(p));
	if (it == paramMap_.end()) return;
	SetTalkParamValue(it->second.c_str(), value);
}

void Live2DTalkController::SetTalkParamValue(const TCHAR* parameterId, float value)
{
	if (!model_ || parameterId == nullptr) return;
	const std::basic_string<TCHAR> id(parameterId);
	talkParamIds_[id] = true;
	model_->SetParamerterValue(id.c_str(), value);
}

void Live2DTalkController::ResetTalkParams()
{
	if (!model_)
	{
		talkParamIds_.clear();
		return;
	}

	for (const auto& kv : talkParamIds_)
	{
		model_->SetParamerterValue(kv.first.c_str(), GetDefaultParamValue(kv.first.c_str()));
	}
	talkParamIds_.clear();
}

void Live2DTalkController::ResetParam(Param p)
{
	SetParamValue(p, GetDefaultParamValue(p));
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
	return model_;
}

void Live2DTalkController::RegisterPose(int menuIndex, const TCHAR* motionName)
{
	if (!motionName) return;
	poseMap_[menuIndex] = motionName;
}

void Live2DTalkController::SetMenuIndex(int index)
{
	if (index == lastMenuIndex_) return;
	lastMenuIndex_ = index;

	auto it = poseMap_.find(index);
	if (it != poseMap_.end() && model_)
	{
		// 即時にポーズ（モーション）を再生
		model_->StartMotion(it->second.c_str(), 0);
	}
}

// 追加: シーン切替時にコントローラ内部状態を初期化する
void Live2DTalkController::ResetForScene()
{
	// 次のメニューは強制再生させる
	lastMenuIndex_ = -1;

	ResetTalkParams();

	// 口のパラメータだけリセット（目など自動制御されるパラメータは触らない）
	ResetMouth();

	// アルファは通常 255
	SetAlpha(255);

	// 必要なら他の一時状態（waiting flags など）もここでリセットする
}