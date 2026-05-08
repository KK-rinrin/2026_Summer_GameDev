#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_SOUND = Application::PATH_SOUND;
	static std::string PATH_TITLE = Application::PATH_TITLE;
	static std::string PATH_EFF = Application::PATH_EFFECT;
	static std::string PATH_MODEL = Application::PATH_MODEL;

	// タイトル画像
	CreateResource(SRC::TITLE_IMG, RES_T::IMG, PATH_TITLE + "Title.png");

	// 会話ウィンドウ画像
	CreateResource(SRC::TALK_WINDOW_IMG, RES_T::IMG, PATH_IMG + "window.png");
	CreateResource(SRC::TALK_WINDOW_NEXT_IMG, RES_T::IMG, PATH_IMG + "window_next.png");

	// キャラクターグラフィック
	Resource* res = new Resource(RES_T::IMGS, PATH_IMG + "Game/Player.png", 3, 2, 120, 300);
	resourcesMap_.emplace(SRC::PLAYER_GRAPHICS, res);
	res = new Resource(RES_T::IMGS, PATH_IMG + "Game/Patient.png", 3, 2, 120, 300);
	resourcesMap_.emplace(SRC::PATIENT_GRAPHICS, res);
	CreateResource(SRC::BG_1, RES_T::IMG, PATH_IMG + "Game/BG/PICU_BG.png");
	CreateResource(SRC::BG_1_WALL, RES_T::IMG, PATH_IMG + "Game/BG/PICU_Wall.png");
	CreateResource(SRC::BG_2, RES_T::IMG, PATH_IMG + "Game/NS_BG.png");

	// Live2Dモデル
	CreateResource(SRC::PATIENT_MODEL, RES_T::LIVE2D, PATH_MODEL + "patient/patient.model3.json");
	CreateResource(SRC::PLAYER_MODEL, RES_T::LIVE2D, PATH_MODEL + "nurce/nurce.model3.json");

	// フォント
	res = new Resource(RES_T::FONT, "コーポレート明朝 ver3 Medium", 30);
	resourcesMap_.emplace(SRC::TALK_FONT, res);
	
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	for (auto& res : resourcesMap_)
	{
		res.second->Release();
		delete res.second;
	}
	resourcesMap_.clear();
	delete instance_;
}

char* ResourceManager::GetResourcePath(SRC src) const
{
	const auto& rPair = resourcesMap_.find(src);
	if (rPair != resourcesMap_.end())
	{
		return const_cast<char*>(rPair->second->path_.c_str());
	}
	return nullptr;
}

const Resource& ResourceManager::Load(SRC src)
{
	const Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

void ResourceManager::CreateResource(SRC src, Resource::TYPE type, const std::string& path)
{
	Resource* res = new Resource(type, path);
	resourcesMap_.emplace(src, res);
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
