#include "SoundManager.h"
#include "../Manager/ResourceManager.h"
#include <DxLib.h>

SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager();
	}
}

SoundManager& SoundManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		SoundManager::CreateInstance();
	}
	return *instance_;
}

void SoundManager::Destroy(void)
{
	delete instance_;
	instance_ = nullptr;
}

SoundManager::SoundManager()
{
	resHandles_.fill(-1);
}

bool SoundManager::Load(BGM bgm)
{
	ResourceManager::SRC res;
	switch (bgm)
	{
	case BGM::GAME0:
		res = ResourceManager::SRC::BGM_GAME;
		break;

	default:
		return false;
	}

	resHandles_[static_cast<int>(bgm)] = resMng_->GetInstance().Load(res).handleId_;
	return true;
}

bool SoundManager::PlayBGM(BGM bgm)
{
	if (resHandles_[static_cast<int>(bgm)] == -1)
	{
		if (Load(bgm) == false) return false;
	}
	int success = PlaySoundMem(resHandles_[static_cast<int>(bgm)], DX_PLAYTYPE_LOOP, true);
	if (success == 0) return true;
	else return false;
}
