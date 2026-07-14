#include "SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Utility/SchoolUtility.h"

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
	currentBGM_(BGM::MAX),
	lastBGMUpdateMs_(0),
	BGMvol_(DXLIB_VOLUME_MAX),
	SEvol_(DXLIB_VOLUME_MAX),
	BGMvolPercent_(INITIAL_VOLUME),
	SEvolPercent_(INITIAL_VOLUME)
{
	// -1で初期化
	resHandlesBGM_.fill(-1);
	currentBGMVols_.fill(0);
	targetBGMVols_.fill(0);
	fadeBGMms_.fill(0);
	resHandlesSE_.fill(-1);
}

bool SoundManager::Load(BGM bgm)
{
	// SRCと対応付け
	ResourceManager::SRC res;
	switch (bgm)
	{
	case BGM::TITLE: res = ResourceManager::SRC::BGM_TITLE; break;
	case BGM::GAME0: res = ResourceManager::SRC::BGM_GAME; break;
	case BGM::GAME1: res = ResourceManager::SRC::BGM_GAME_2; break;
	default: return false;
	}

	// ロードしてハンドルを格納
	resHandlesBGM_[static_cast<int>(bgm)] = resMng_.Load(res).handleId_;

	ApplyBGMVolume(bgm);

	return true;
}

bool SoundManager::Load(SE se)
{
	// SRCと対応付け
	ResourceManager::SRC res;
	switch (se)
	{
	case SE::DECIDE: res = ResourceManager::SRC::SE_DECIDE; break;
	case SE::CANCEL: res = ResourceManager::SRC::SE_CANCEL; break;
	case SE::MOVE: res = ResourceManager::SRC::SE_CURSOR_MOVE; break;
	case SE::BEEP: res = ResourceManager::SRC::SE_BEEP; break;
	case SE::DOOR: res = ResourceManager::SRC::SE_DOOR; break;
	case SE::RINGTONE: res = ResourceManager::SRC::SE_RINGTONE; break;
	default: return false;
	}

	// ロードしてハンドルを格納
	resHandlesSE_[static_cast<int>(se)] = resMng_.Load(res).handleId_;

	// 音量変更を適用
	ChangeVolumeSoundMem(SEvol_, resHandlesSE_[static_cast<int>(se)]);
	return true;
}

bool SoundManager::PlayBGM(BGM bgm)
{
	if (IsSynchronizedGameBGM(bgm))
	{
		return StartSynchronizedGameBGM(bgm);
	}

	StopBGM();
	int& handle = resHandlesBGM_[GetBGMIndex(bgm)];

	// ハンドルが有効かチェック
	if (handle == -1 || CheckSoundMem(handle) == -1)
	{
		// ロード試行
		if (Load(bgm) == false) return false;
	}

	currentBGMVols_[GetBGMIndex(bgm)] = DXLIB_VOLUME_MAX;
	targetBGMVols_[GetBGMIndex(bgm)] = DXLIB_VOLUME_MAX;
	fadeBGMms_[GetBGMIndex(bgm)] = 0;
	ApplyBGMVolume(bgm);

	// 再生
	int success = PlaySoundMem(handle, DX_PLAYTYPE_LOOP, true);
	currentBGM_ = (success == 0) ? bgm : BGM::MAX;
	lastBGMUpdateMs_ = GetNowCount();

	if (success == 0) return true;
	else return false;
}

bool SoundManager::PlaySE(SE se)
{
	int& handle = resHandlesSE_[static_cast<int>(se)];

	// ハンドルが有効かチェック
	if (handle == -1 || CheckSoundMem(handle) == -1)
	{
		// ロード試行
		if (Load(se) == false) return false;
	}

	// 再生
	int success = PlaySoundMem(handle, DX_PLAYTYPE_BACK, true);


	if (success == 0) return true;
	else return false;
}


bool SoundManager::PlaySE(ResourceManager::SRC src)
{
	const Resource& res = resMng_.Load(src);
	if (res.type_ != Resource::TYPE::SOUND || res.handleId_ == -1)
	{
		return false;
	}

	ChangeVolumeSoundMem(SEvol_, res.handleId_);
	int success = PlaySoundMem(res.handleId_, DX_PLAYTYPE_BACK, true);

	return success == 0;
}

void SoundManager::ChangeBGM(BGM bgm, int fadeMs)
{
	if (!IsSynchronizedGameBGM(bgm) || !IsSynchronizedGameBGM(currentBGM_))
	{
		PlayBGM(bgm);
		return;
	}

	if (!StartSynchronizedGameBGM(currentBGM_))
	{
		return;
	}

	SetBGMTarget(BGM::GAME0, (bgm == BGM::GAME0) ? DXLIB_VOLUME_MAX : DXLIB_VOLUME_MIN, fadeMs);
	SetBGMTarget(BGM::GAME1, (bgm == BGM::GAME1) ? DXLIB_VOLUME_MAX : DXLIB_VOLUME_MIN, fadeMs);
	currentBGM_ = bgm;
}

void SoundManager::Update()
{
	const unsigned int now = GetNowCount();
	unsigned int delta = 0;
	if (lastBGMUpdateMs_ != 0)
	{
		delta = now - lastBGMUpdateMs_;
	}
	lastBGMUpdateMs_ = now;

	for (int i = 0; i < static_cast<int>(BGM::MAX); ++i)
	{
		if (resHandlesBGM_[i] == -1 || CheckSoundMem(resHandlesBGM_[i]) != 1)
		{
			continue;
		}

		if (currentBGMVols_[i] == targetBGMVols_[i])
		{
			continue;
		}

		const int diff = targetBGMVols_[i] - currentBGMVols_[i];
		int change = (diff > 0) ? 1 : -1;
		if (delta > 0 && fadeBGMms_[i] > 0)
		{
			change = static_cast<int>((static_cast<long long>(diff) * static_cast<long long>(delta)) / fadeBGMms_[i]);
			if (change == 0)
			{
				change = (diff > 0) ? 1 : -1;
			}
		}
		else if (fadeBGMms_[i] <= 0)
		{
			change = diff;
		}

		if ((diff > 0 && change > diff) || (diff < 0 && change < diff))
		{
			change = diff;
		}

		currentBGMVols_[i] += change;
		if (currentBGMVols_[i] < DXLIB_VOLUME_MIN) currentBGMVols_[i] = DXLIB_VOLUME_MIN;
		if (currentBGMVols_[i] > DXLIB_VOLUME_MAX) currentBGMVols_[i] = DXLIB_VOLUME_MAX;
		if (currentBGMVols_[i] == targetBGMVols_[i])
		{
			fadeBGMms_[i] = 0;
		}

		ApplyBGMVolume(static_cast<BGM>(i));
	}
}

void SoundManager::StopBGM()
{
	// すべてのBGMを停止
	for (auto& bgm : resHandlesBGM_)
	{
		if (CheckSoundMem(bgm) == 1)
		{
			StopSoundMem(bgm);
		}
	}
	currentBGMVols_.fill(0);
	targetBGMVols_.fill(0);
	fadeBGMms_.fill(0);
	currentBGM_ = BGM::MAX;
}

void SoundManager::SetVolumeBGM(int vol)
{
	// 0～100の設定値を保存し、DxLib用の0～255へ変換
	BGMvolPercent_ = static_cast<int>(SchoolUtility::Clamp(
		static_cast<float>(vol), VOLUME_PERCENT_MIN, VOLUME_PERCENT_MAX));
	BGMvol_ = SchoolUtility::Lerp(
		DXLIB_VOLUME_MIN, DXLIB_VOLUME_MAX,
		static_cast<float>(BGMvolPercent_) / VOLUME_PERCENT_MAX);

	// すべてのBGMに適用
	for (int i = 0; i < static_cast<int>(BGM::MAX); ++i)
	{
		ApplyBGMVolume(static_cast<BGM>(i));
	}
}

void SoundManager::SetVolumeSE(int vol)
{
	// 0～100の設定値を保存し、DxLib用の0～255へ変換
	SEvolPercent_ = static_cast<int>(SchoolUtility::Clamp(
		static_cast<float>(vol), VOLUME_PERCENT_MIN, VOLUME_PERCENT_MAX));
	SEvol_ = SchoolUtility::Lerp(
		DXLIB_VOLUME_MIN, DXLIB_VOLUME_MAX,
		static_cast<float>(SEvolPercent_) / VOLUME_PERCENT_MAX);

	// すべてのSEに適用
	for (auto& se : resHandlesSE_)
	{
		if (se != -1)
		{
			ChangeVolumeSoundMem(SEvol_, se);
		}
	}
}
bool SoundManager::IsSynchronizedGameBGM(BGM bgm) const
{
	return bgm == BGM::GAME0 || bgm == BGM::GAME1;
}

bool SoundManager::StartSynchronizedGameBGM(BGM activeBgm)
{
	if (!IsSynchronizedGameBGM(activeBgm))
	{
		return false;
	}

	const BGM bgms[] = { BGM::GAME0, BGM::GAME1 };
	for (BGM bgm : bgms)
	{
		int& handle = resHandlesBGM_[GetBGMIndex(bgm)];
		if (handle == -1 || CheckSoundMem(handle) == -1)
		{
			if (Load(bgm) == false) return false;
		}
	}

	for (BGM bgm : bgms)
	{
		const int index = GetBGMIndex(bgm);
		int& handle = resHandlesBGM_[index];
		if (CheckSoundMem(handle) != 1)
		{
			PlaySoundMem(handle, DX_PLAYTYPE_LOOP, true);
		}
		currentBGMVols_[index] = (bgm == activeBgm) ? DXLIB_VOLUME_MAX : DXLIB_VOLUME_MIN;
		targetBGMVols_[index] = currentBGMVols_[index];
		fadeBGMms_[index] = 0;
		ApplyBGMVolume(bgm);
	}

	currentBGM_ = activeBgm;
	lastBGMUpdateMs_ = GetNowCount();
	return true;
}

void SoundManager::SetBGMTarget(BGM bgm, int targetVol, int fadeMs)
{
	const int index = GetBGMIndex(bgm);
	targetBGMVols_[index] = targetVol;
	if (targetBGMVols_[index] < DXLIB_VOLUME_MIN) targetBGMVols_[index] = DXLIB_VOLUME_MIN;
	if (targetBGMVols_[index] > DXLIB_VOLUME_MAX) targetBGMVols_[index] = DXLIB_VOLUME_MAX;
	fadeBGMms_[index] = (fadeMs > 0) ? fadeMs : 1;
}

void SoundManager::ApplyBGMVolume(BGM bgm)
{
	const int index = GetBGMIndex(bgm);
	const int handle = resHandlesBGM_[index];
	if (handle == -1 || CheckSoundMem(handle) == -1)
	{
		return;
	}

	const int volume = currentBGMVols_[index] * BGMvol_ / DXLIB_VOLUME_MAX;
	ChangeVolumeSoundMem(volume, handle);
}