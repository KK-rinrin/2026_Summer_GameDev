#pragma once
#include <array>
#include "ResourceManager.h"

class SoundManager
{

public:
	enum class BGM
	{
		TITLE,
		GAME0,
		GAME1,
		SAD_THEME,
		ENDING,

		MAX
	};

	enum class SE
	{
		DECIDE,
		CANCEL,
		MOVE,
		BEEP,
		DOOR,
		RINGTONE,
		MAX
	};

	static constexpr int INITIAL_VOLUME = 100;
	static constexpr int VOLUME_PERCENT_MIN = 0;
	static constexpr int VOLUME_PERCENT_MAX = 100;
	static constexpr int DXLIB_VOLUME_MIN = 0;
	static constexpr int DXLIB_VOLUME_MAX = 255;
	static constexpr int BGM_CROSS_FADE_MS = 1000;

	// シングルトン
	static void CreateInstance(void);
	static SoundManager& GetInstance(void);
	void Destroy(void);

	// サウンドを再生
	bool PlayBGM(BGM bgm);	// 返り値:再生できたかどうか
	bool PlaySE(SE se);		// 返り値:再生できたかどうか
	bool PlaySE(ResourceManager::SRC src);		// 返り値:再生できたかどうか

	void ChangeBGM(BGM bgm, int fadeMs = BGM_CROSS_FADE_MS);
	void Update();

	// サウンドを停止
	void StopBGM();

	// 音量を設定
	void SetVolumeBGM(int vol);
	void SetVolumeSE(int vol);

	// 音量を取得
	const int GetVolumeBGM() const { return BGMvolPercent_; }
	const int GetVolumeSE() const { return SEvolPercent_; }

private:
	// コピー代入防止
	SoundManager(void);
	SoundManager(const SoundManager& instance) = delete;
	SoundManager& operator=(const SoundManager& instance) = delete;
	~SoundManager(void) = default;

	bool Load(BGM bgm);
	bool Load(SE se);
	bool IsSynchronizedGameBGM(BGM bgm) const;
	bool StartSynchronizedGameBGM(BGM activeBgm);
	void SetBGMTarget(BGM bgm, int targetVol, int fadeMs);
	void ApplyBGMVolume(BGM bgm);
	int GetBGMIndex(BGM bgm) const { return static_cast<int>(bgm); }


	// インスタンス
	static SoundManager* instance_;

	ResourceManager& resMng_;

	std::array<int, static_cast<int>(BGM::MAX)> resHandlesBGM_;
	std::array<int, static_cast<int>(BGM::MAX)> currentBGMVols_;
	std::array<int, static_cast<int>(BGM::MAX)> targetBGMVols_;
	std::array<int, static_cast<int>(BGM::MAX)> fadeBGMms_;
	BGM currentBGM_;
	unsigned int lastBGMUpdateMs_;
	std::array<int, static_cast<int>(SE::MAX)> resHandlesSE_;

	int BGMvol_;
	int SEvol_;
	int BGMvolPercent_;
	int SEvolPercent_;
};

using SE = SoundManager::SE;
