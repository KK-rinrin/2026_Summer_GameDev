#pragma once
#include <array>
class ResourceManager;

class SoundManager
{

public:
	enum BGM
	{
		TITLE,
		GAME0,
		GAME1,
		SAD_THEME,
		ENDING,

		MAX
	};

	// シングルトン
	static void CreateInstance(void);
	static SoundManager& GetInstance(void);
	void Destroy(void);

	// 音楽を再生
	bool PlayBGM(BGM bgm);

private:
	// コピー代入防止
	SoundManager(void) = default;
	SoundManager(const SoundManager& instance) = delete;
	SoundManager& operator=(const SoundManager& instance) = delete;
	~SoundManager(void) = default;

	bool Load(BGM bgm);

	// インスタンス
	static SoundManager* instance_;

	ResourceManager* resMng_;

	std::array<int, static_cast<int>(BGM::MAX)> resHandles_;
};
