#pragma once

#include <map>
#include <memory>
#include "Live2D.h"
#include "ResourceManager.h"

// 前方宣言（ヘッダ内で完結）
class Live2DTalkController;

class Live2DModelHub
{
public:
	// シングルトン取得
	static Live2DModelHub& Instance();

	// 指定リソースのモデルを取得（存在しなければ生成して Load する）
	// 戻り値はハブが所有する生ポインタ。呼び出し側は delete しないこと。
	Live2D* GetModel(ResourceManager::SRC src);

	// 指定リソースに対応する Live2DTalkController を取得（shared_ptr）
	std::shared_ptr<Live2DTalkController> GetController(ResourceManager::SRC src);

	// 全モデル・全コントローラを解放する（必要時に呼ぶ）
	void ReleaseAll();

private:
	Live2DModelHub() = default;
	~Live2DModelHub();

	// 非コピー
	Live2DModelHub(const Live2DModelHub&) = delete;
	Live2DModelHub& operator=(const Live2DModelHub&) = delete;

	// Live2D モデルの保管
	std::map<ResourceManager::SRC, std::unique_ptr<Live2D>> models_;

	// コントローラの保管（shared 所有）
	std::map<ResourceManager::SRC, std::shared_ptr<Live2DTalkController>> controllers_;
};