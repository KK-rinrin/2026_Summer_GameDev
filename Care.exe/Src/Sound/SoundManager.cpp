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
	:
	resMng_(ResourceManager::GetInstance()),
	BGMvol_(INITIAL_VOLUME),
	SEvol_(INITIAL_VOLUME)
{
	resHandlesBGM_.fill(-1);
	resHandlesSE_.fill(-1);
}

bool SoundManager::Load(BGM bgm)
{
	ResourceManager::SRC res;
	switch (bgm)
	{
	default: return false;
	case BGM::GAME0: res = ResourceManager::SRC::BGM_GAME;
	}

	resHandlesBGM_[static_cast<int>(bgm)] = resMng_.Load(res).handleId_;
	ChangeVolumeSoundMem(BGMvol_, resHandlesBGM_[static_cast<int>(bgm)]);
	return true;
}

bool SoundManager::Load(SE se)
{
	ResourceManager::SRC res;
	switch (se)
	{
	default: return false;
	case SE::DECIDE: res = ResourceManager::SRC::SE_DECIDE; break;
	case SE::CANCEL: res = ResourceManager::SRC::SE_CANCEL; break;
	case SE::DOOR: res = ResourceManager::SRC::SE_DOOR; break;
	}

	resHandlesSE_[static_cast<int>(se)] = resMng_.Load(res).handleId_;
	ChangeVolumeSoundMem(SEvol_, resHandlesSE_[static_cast<int>(se)]);
	return true;
}

bool SoundManager::PlayBGM(BGM bgm)
{
	if (resHandlesBGM_[static_cast<int>(bgm)] == -1)
	{
		if (Load(bgm) == false) return false;
	}
	int success = PlaySoundMem(resHandlesBGM_[static_cast<int>(bgm)], DX_PLAYTYPE_LOOP, true);
	if (success == 0) return true;
	else return false;
}

bool SoundManager::PlaySE(SE se)
{
	if (resHandlesSE_[static_cast<int>(se)] == -1)
	{
		if (Load(se) == false) return false;
	}
	int success = PlaySoundMem(resHandlesSE_[static_cast<int>(se)], DX_PLAYTYPE_BACK, true);
	if (success == 0) return true;
	else return false;
}

void SoundManager::StopBGM()
{
	for (auto& bgm : resHandlesBGM_)
	{
		if (CheckSoundMem(bgm))
		{
			StopSoundMem(bgm);
		}
	}
}

void SoundManager::SetVolumeBGM(int vol)
{
	BGMvol_ = vol;
	for (auto& bgm : resHandlesBGM_)
	{
		if (bgm != -1)
		{
			ChangeVolumeSoundMem(BGMvol_, bgm);
		}
	}
}

void SoundManager::SetVolumeSE(int vol)
{
	SEvol_ = vol;
	for (auto& se : resHandlesSE_)
	{
		if (se != -1)
		{
			ChangeVolumeSoundMem(SEvol_, se);
		}
	}
}
