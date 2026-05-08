#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <tchar.h>
#include "../../../Manager/Live2D.h"
#include "../../../Manager/ResourceManager.h"

class Live2DTalkController
{
public:
	// 汎用パラメータ識別子（必要に応じて増やす）
	enum class Param : int
	{
		MOUTH = 0,
		EYE_BLINK_R = 1,
		EYE_BLINK_L = 2,
		BROW_RY = 3,
		BROW_LY = 4,
		ALPHA = 10,
		// モデル固有や拡張は CUSTOM 以降に登録して使う
		CUSTOM = 100,
		PAT_EYE_BLACK,	// 患者モデルの目を黒くする

	};

	Live2DTalkController();
	~Live2DTalkController();

	// モデル読み込み（controller が内部で Live2D を生成・所有する）
	void Load(ResourceManager::SRC src);

	// ライフサイクル / 更新 / 描画ラッパー
	void Update(const TCHAR* motion);
	void DrawBegin();
	void DrawModel();
	void DrawEnd();
	void DeleteModel();

	// 位置・拡大
	void SetExtendRate(float x, float y);
	void SetTranslate(float x, float y);

	// ----- パラメータ操作（enum 管理） -----
	// デフォルトマッピング以外を登録する場合に使用する
	void RegisterParam(Param p, const TCHAR* parameterId);

	// パラメータに値を設定（0..1 等の範囲は呼び出し側で管理）
	void SetParamValue(Param p, float value);

	// 指定パラメータをリセット（0）
	void ResetParam(Param p);

	// 全登録パラメータをリセット
	void ResetAllParams();

	// 互換性のための既存メソッド
	void SetMouthParameterId(const TCHAR* id) { RegisterParam(Param::MOUTH, id); }
	void ResetMouth();
	void SetMouthProgress(float t);
	void CloseMouth();

	void SetAlpha(int alpha);

	// 生モデル取得（非所有、読み取り専用が必要な場合）
	Live2D* GetModel() const;

private:
	std::unique_ptr<Live2D> model_;
	std::unordered_map<int, std::basic_string<TCHAR>> paramMap_; // key = static_cast<int>(Param)
	// デフォルトマッピング初期化用
	void InitDefaultParamMap();
};