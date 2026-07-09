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
	BGMvol_(DXLIB_VOLUME_MAX),
	SEvol_(DXLIB_VOLUME_MAX),
	BGMvolPercent_(INITIAL_VOLUME),
	SEvolPercent_(INITIAL_VOLUME)
{
	// -1で初期化
	resHandlesBGM_.fill(-1);
	resHandlesSE_.fill(-1);
}

bool SoundManager::Load(BGM bgm)
{
	// SRCと対応付け
	ResourceManager::SRC res;
	switch (bgm)
	{
	case BGM::GAME0: res = ResourceManager::SRC::BGM_GAME; break;
	default: return false;
	}

	// ロードしてハンドルを格納
	resHandlesBGM_[static_cast<int>(bgm)] = resMng_.Load(res).handleId_;

	// 音量変更を適用
	ChangeVolumeSoundMem(BGMvol_, resHandlesBGM_[static_cast<int>(bgm)]);

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
	int& handle = resHandlesBGM_[static_cast<int>(bgm)];

	// ハンドルが有効かチェック
	if (handle == -1 || CheckSoundMem(handle) == -1)
	{
		// ロード試行
		if (Load(bgm) == false) return false;
	}

	// 再生
	int success = PlaySoundMem(handle, DX_PLAYTYPE_LOOP, true);

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