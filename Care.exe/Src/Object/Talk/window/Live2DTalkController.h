#pragma once

#include <unordered_map>
#include <string>
#include <tchar.h>
#include "../../../Manager/Live2D.h"
#include "../../../Manager/Live2DModelHub.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Utility/SchoolUtility.h"

class Live2DTalkController
{
public:
	enum class Param : int
	{
		MOUTH = 0,
		EYE_BLINK_R = 1,
		EYE_BLINK_L = 2,
		BROW_RY = 3,
		BROW_LY = 4,
		FACE_X,
		FACE_Y,
		FACE_Z,

		ALPHA = 99,
		CUSTOM = 100,
		PAT_EYE_BLACK,
	};

	Live2DTalkController();
	~Live2DTalkController();

	void Load(ResourceManager::SRC src);
	void Update(const TCHAR* motion);
	void DrawBegin();
	void DrawModel();
	void DrawEnd();
	void DeleteModel();

	void SetExtendRate(float x, float y);
	void SetTranslate(float x, float y);

	// menu index を通知する（TitleScene などから呼ぶ）
	void SetMenuIndex(int index);

	// ポーズ（モーション）登録
	void RegisterPose(int menuIndex, const TCHAR* motionName);

	// ----- パラメータ操作 -----
	void RegisterParam(Param p, const TCHAR* parameterId);
	void SetParamValue(Param p, float value);
	void ResetParam(Param p);
	void ResetAllParams();

	void SetMouthParameterId(const TCHAR* id) { RegisterParam(Param::MOUTH, id); }
	void ResetMouth();
	void SetMouthProgress(float t);
	void CloseMouth();

	void SetAlpha(int alpha);
	Live2D* GetModel() const;

	// シーン取得直後にコントローラ内部状態を初期化する（共有時の副作用防止）
	void ResetForScene();

private:
	// ハブが所有するモデルへの参照（非所有）
	Live2D* model_;
	std::unordered_map<int, std::basic_string<TCHAR>> paramMap_;
	void InitDefaultParamMap();

	// ポーズ用マップ: menuIndex -> motion name
	std::unordered_map<int, std::basic_string<TCHAR>> poseMap_;
	int lastMenuIndex_;
};