#pragma once
#include <array>
class ResourceManager;

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
		MAX
	};

	// シングルトン
	static void CreateInstance(void);
	static SoundManager& GetInstance(void);
	void Destroy(void);

	// サウンドを再生
	bool PlayBGM(BGM bgm);	// 返り値:再生できたかどうか
	bool PlaySE(SE se);		// 返り値:再生できたかどうか

	// サウンドを停止
	void StopBGM();

	// 音量を設定
	void SetVolumeBGM(int vol);
	void SetVolumeSE(int vol);

	// 音量を取得
	const int GetVolumeBGM() const { return BGMvolPercent_; }
	const int GetVolumeSE() const { return SEvolPercent_; }

	static constexpr int INITIAL_VOLUME = 100;
	static constexpr int VOLUME_PERCENT_MIN = 0;
	static constexpr int VOLUME_PERCENT_MAX = 100;
	static constexpr int DXLIB_VOLUME_MIN = 0;
	static constexpr int DXLIB_VOLUME_MAX = 255;

private:
	// コピー代入防止
	SoundManager(void);
	SoundManager(const SoundManager& instance) = delete;
	SoundManager& operator=(const SoundManager& instance) = delete;
	~SoundManager(void) = default;

	bool Load(BGM bgm);
	bool Load(SE se);

	// インスタンス
	static SoundManager* instance_;

	ResourceManager& resMng_;

	std::array<int, static_cast<int>(BGM::MAX)> resHandlesBGM_;
	std::array<int, static_cast<int>(SE::MAX)> resHandlesSE_;

	int BGMvol_;
	int SEvol_;
	int BGMvolPercent_;
	int SEvolPercent_;
};

using SE = SoundManager::SE;