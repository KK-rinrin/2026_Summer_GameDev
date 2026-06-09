#include "SoundManager.h"
#include "../Manager/ResourceManager.h"
#include <DxLib.h>

using namespace SoundManager;

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

bool SoundManager::Load(BGM bgm)
{
	return;
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
