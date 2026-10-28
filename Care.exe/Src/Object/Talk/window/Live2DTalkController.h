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

	// Live2Dの操作対象パラメーター識別子
	enum class Param : int
	{

		// 口の開閉パラメーター
		MOUTH = 0,

		// 右目の開閉パラメーター
		EYE_BLINK_R = 1,

		// 左目の開閉パラメーター
		EYE_BLINK_L = 2,

		// 右眉の縦位置パラメーター
		BROW_RY = 3,

		// 左眉の縦位置パラメーター
		BROW_LY = 4,

		// 顔のX軸回転パラメーター
		FACE_X,

		// 顔のY軸回転パラメーター
		FACE_Y,

		// 顔のZ軸回転パラメーター
		FACE_Z,

		// モデル透明度のパラメーター
		ALPHA = 99,

		// 任意ID指定用のパラメーター種別
		CUSTOM = 100,

		// 患者の黒目演出用パラメーター
		PAT_EYE_BLACK,
	};

	Live2DTalkController();
	~Live2DTalkController();

	void Load(ResourceManager::SRC src);
	void Update(const TCHAR* motion);

	// 共有Live2D描画の開始
	void DrawBegin();

	// 操作対象Live2Dモデルの描画
	void DrawModel();

	// 共有Live2D描画の終了
	void DrawEnd();

	// 操作対象Live2Dモデルの解放
	void DeleteModel();

	// Live2Dモデルの縦横倍率の設定
	void SetExtendRate(float x, float y);

	// Live2Dモデルの表示位置の設定
	void SetTranslate(float x, float y);

	// メニュー番号に対応する登録ポーズの適用（未使用）
	void SetMenuIndex(int index);

	// メニュー番号とモーション名の対応付け（未使用）
	void RegisterPose(int menuIndex, const TCHAR* motionName);

	// 識別子に対応するモデルパラメーターIDの登録
	void RegisterParam(Param p, const TCHAR* parameterId);

	// 識別子を指定したパラメーター値の設定
	void SetParamValue(Param p, float value);

	// 識別子を指定した会話用パラメーターの設定と復元対象への記録
	void SetTalkParamValue(Param p, float value);

	// 文字列IDを指定した会話用パラメーターの設定と復元対象への記録
	void SetTalkParamValue(const TCHAR* parameterId, float value);

	// 会話で変更した全パラメーターの既定値への復元
	void ResetTalkParams();

	// 指定パラメーターの既定値への復元
	void ResetParam(Param p);

	// 口の開閉に使用するパラメーターIDの登録（未使用）
	void SetMouthParameterId(const TCHAR* id) { RegisterParam(Param::MOUTH, id); }

	// 口の開閉パラメーターの既定値への復元
	void ResetMouth();

	// 文字送り進行率に応じた口の開閉値の設定
	void SetMouthProgress(float t);

	// 口を閉じた状態の設定
	void CloseMouth();

	// モデル透明度の設定
	void SetAlpha(int alpha);

	// 操作対象Live2Dモデルへの参照ポインタの取得
	Live2D* GetModel() const;

	// シーン取得直後にコントローラ内部状態を初期化する（共有時の副作用防止）
	void ResetForScene();

private:

	// 完全不透明時のアルファ値
	static constexpr int MAX_ALPHA = 255;

	// デバッグ出力用バッファの文字数
	static constexpr int DEBUG_BUFFER_SIZE = 256;

	// ハブが所有するモデルへの参照（非所有）
	Live2D* model_;

	// パラメーター識別子とモデル内IDの対応表
	std::unordered_map<int, std::basic_string<TCHAR>> paramMap_;

	// 会話で変更したパラメーターIDの記録
	std::unordered_map<std::basic_string<TCHAR>, bool> talkParamIds_;

	// 標準パラメーターIDの対応表の初期化
	void InitDefaultParamMap();

	// 識別子に対応するパラメーター既定値の取得
	float GetDefaultParamValue(Param p) const;

	// 文字列IDに対応するパラメーター既定値の取得
	float GetDefaultParamValue(const TCHAR* parameterId) const;

	// ポーズ用マップ: menuIndex -> motion name
	std::unordered_map<int, std::basic_string<TCHAR>> poseMap_;

	// 前回適用したメニュー項目番号
	int lastMenuIndex_;
};
