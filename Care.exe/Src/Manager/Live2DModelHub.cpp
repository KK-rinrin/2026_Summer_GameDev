#include "Live2DModelHub.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Talk/window/Live2DTalkController.h"
#include <Windows.h>

// シングルトン取得
Live2DModelHub& Live2DModelHub::Instance()
{
	static Live2DModelHub instance;
	return instance;
}

Live2DModelHub::~Live2DModelHub()
{
	ReleaseAll();
}

Live2D* Live2DModelHub::GetModel(ResourceManager::SRC src)
{
	auto it = models_.find(src);
	if (it != models_.end())
	{
		Live2D* existing = it->second.get();
		if (existing)
		{
			existing->Load(src);
		}
		return existing;
	}

	// 存在しない -> 新規作成してロード
	auto model = std::unique_ptr<Live2D>(new Live2D());
	model->Load(src);
	Live2D* ptr = model.get();

	// デバッグ出力
	{
		char buf[256];
		sprintf_s(buf, "Live2DModelHub: creating model for src=%d, handle=%d, ptr=%p\n",
			static_cast<int>(src), ptr->GetHandle(), static_cast<void*>(ptr));
		OutputDebugStringA(buf);
	}

	models_.emplace(src, std::move(model));
	return ptr;
}

std::shared_ptr<Live2DTalkController> Live2DModelHub::GetController(ResourceManager::SRC src)
{
	auto it = controllers_.find(src);
	if (it != controllers_.end())
	{
		// 共有コントローラを返す前にシーン初期化を行う
		auto ctrl = it->second;
		if (ctrl)
		{
			ctrl->Load(src);
			ctrl->ResetForScene();
		}
		return ctrl;
	}

	// 新規生成して Load（内部でモデルは Live2DModelHub 経由で取得する）
	auto ctrl = std::make_shared<Live2DTalkController>();
	ctrl->Load(src);
	// 新規でもシーン初期化
	ctrl->ResetForScene();

	// デバッグ出力
	{
		char buf[256];
		sprintf_s(buf, "Live2DModelHub: created controller for src=%d ptr=%p\n", static_cast<int>(src), static_cast<void*>(ctrl.get()));
		OutputDebugStringA(buf);
	}

	controllers_.emplace(src, ctrl);
	return ctrl;
}

void Live2DModelHub::ReleaseAll()
{
	// 1) コントローラを先に解放（モデル参照を切る）
	controllers_.clear();

	// 2) モデルを解放
	for (auto& kv : models_)
	{
		if (kv.second)
		{
			kv.second->DeleteModel();
		}
	}
	models_.clear();
}