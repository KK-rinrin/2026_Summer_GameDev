#pragma once
#include <memory>
#include <array>
#include "ResourceManager.h"

class SoundManager
{
public:

	// BGMの識別子
	enum class BGM
	{

		// タイトル画面のBGM
		TITLE,

		// ゲーム進行前半のBGM
		GAME0,

		// ゲーム進行後半のBGM
		GAME1,

		// 悲しい曲の識別子（未使用）
		SAD_THEME,

		// 恐怖演出用BGM
		KOWAI,

		// エンディング用BGM
		ENDING,

		// BGMの件数と未選択値
		MAX
	};

	// 効果音の識別子
	enum class SE
	{

		// 決定の効果音
		DECIDE,

		// キャンセルの効果音
		CANCEL,

		// カーソル移動の効果音
		MOVE,

		// 操作エラーの効果音
		BEEP,

		// ドアの効果音
		DOOR,

		// 呼び出しの効果音
		RINGTONE,

		// タイピングの効果音
		TYPING,

		// PCマウス操作の効果音
		MOUSE,

		// 効果音の件数
		MAX
	};

	// 初期音量の百分率
	static constexpr int INITIAL_VOLUME = 100;

	// 音量百分率の最小値
	static constexpr int VOLUME_PERCENT_MIN = 0;

	// 音量百分率の最大値
	static constexpr int VOLUME_PERCENT_MAX = 100;

	// DxLibへ渡す音量の最小値
	static constexpr int DXLIB_VOLUME_MIN = 0;

	// DxLibへ渡す音量の最大値
	static constexpr int DXLIB_VOLUME_MAX = 255;

	// BGM切り替え時の既定フェード時間のミリ秒数
	static constexpr int BGM_CROSS_FADE_MS = 1000;

	// シングルトン
	static void CreateInstance();

	// サウンド管理インスタンスの取得
	static SoundManager& GetInstance();
	void Destroy();

	// サウンドを再生
	bool PlayBGM(BGM bgm);	// 返り値:再生できたかどうか
	bool PlaySE(SE se);		// 返り値:再生できたかどうか
	bool PlaySE(ResourceManager::SRC src);		// 返り値:再生できたかどうか

	// フェードを伴うBGMの切り替え
	void ChangeBGM(BGM bgm, int fadeMs = BGM_CROSS_FADE_MS);
	void Update();

	// サウンドを停止
	void StopBGM();

	// 音量を設定
	void SetVolumeBGM(int vol);

	// 効果音の音量百分率の設定
	void SetVolumeSE(int vol);

	// 音量を取得
	const int GetVolumeBGM() const { return BGMvolPercent_; }

	// 効果音の音量百分率の取得
	const int GetVolumeSE() const { return SEvolPercent_; }

private:
	// コピー代入防止
	SoundManager();
	SoundManager(const SoundManager& instance) = delete;

	// サウンド管理インスタンスのコピー代入の禁止
	SoundManager& operator=(const SoundManager& instance) = delete;
	~SoundManager() = default;

	// 指定BGMの読み込みと音量の適用
	bool Load(BGM bgm);

	// 指定効果音の読み込みと音量の適用
	bool Load(SE se);

	// 同期再生対象のゲームBGMの判定
	bool IsSynchronizedGameBGM(BGM bgm) const;

	// 二つのゲームBGMの同期再生の開始
	bool StartSynchronizedGameBGM(BGM activeBgm);

	// BGMの目標音量とフェード時間の設定
	void SetBGMTarget(BGM bgm, int targetVol, int fadeMs);

	// 個別BGM音量と全体音量を合成した音量の適用
	void ApplyBGMVolume(BGM bgm);

	// BGM識別子の配列インデックスへの変換
	int GetBGMIndex(BGM bgm) const { return static_cast<int>(bgm); }


	// インスタンス
	static std::shared_ptr<SoundManager> instance_;

	// リソース管理への参照
	ResourceManager& resMng_;

	// BGMごとの音声ハンドル
	std::array<int, static_cast<int>(BGM::MAX)> resHandlesBGM_;

	// BGMごとの現在のフェード音量
	std::array<int, static_cast<int>(BGM::MAX)> currentBGMVols_;

	// BGMごとの目標フェード音量
	std::array<int, static_cast<int>(BGM::MAX)> targetBGMVols_;

	// BGMごとのフェード時間のミリ秒数
	std::array<int, static_cast<int>(BGM::MAX)> fadeBGMms_;

	// 現在選択中のBGM
	BGM currentBGM_;

	// 前回のBGM更新時刻のミリ秒値
	unsigned int lastBGMUpdateMs_;

	// 効果音ごとの音声ハンドル
	std::array<int, static_cast<int>(SE::MAX)> resHandlesSE_;

	// DxLibへ渡すBGM全体音量
	int BGMvol_;

	// DxLibへ渡す効果音全体音量
	int SEvol_;

	// BGM全体音量の百分率
	int BGMvolPercent_;

	// 効果音全体音量の百分率
	int SEvolPercent_;
};

using SE = SoundManager::SE;
