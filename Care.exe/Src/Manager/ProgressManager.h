#pragma once

class ProgressManager
{

public:

	enum STORY_PROGRESS
	{
		START = 0,	// ゲーム開始
		START_MINIGAME0,	// ゲーム開始後のイベント1

		AFTER_MG,	// ミニゲーム後
		AFTER_MG_TALKED,	// ミニゲーム後の会話後
		AFTER_PC,	// PC作業後
		LUNCH,		// 昼食

		AFTER_PC2,	// PC作業再び
		START_MINIGAME1,	// 2回目のミニゲーム前

		AFTER_MG2,	// ミニゲーム２後

		END_PATIENT_LOST = 100,
		END_NURCE_LOST,
		END_BOTH_LOST,

	};

	static void CreateInstance(void);

	static ProgressManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 進行度増加
	void AddProgress(void);

	// 進行度キャッシュを削除
	bool ResetProgressCache(void);

	// 進行度取得
	int GetProgress(void) const { return progress_; }

	// 進行度enum取得
	STORY_PROGRESS GetProgressEnum(void) const { return static_cast<STORY_PROGRESS>(progress_); }

	// 患者charファイル存在取得
	bool IsPatientCharExists(void) const { return isPatientCharExists_; }

	// 看護師charファイル存在取得
	bool IsNurceCharExists(void) const { return isNurceCharExists_; }

	// 削除
	void Destroy(void);

private:

	static ProgressManager* instance_;

	int progress_;
	bool isPatientCharExists_;
	bool isNurceCharExists_;

	ProgressManager(void);

	ProgressManager(const ProgressManager& instance) = default;

	~ProgressManager(void) = default;

	void LoadProgress(void);

	void SaveProgress(void) const;

	void CheckCharaFiles(bool isFirstLaunch);

	void ApplyEndProgressByCharaFiles(void);
};