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
		END_PATIENT_LOST = 100,
		END_NURCE_LOST,
		END_BOTH_LOST,

	};

	static void CreateInstance(void);

	static ProgressManager& GetInstance(void);

	void Init(void);

	void AddProgress(void);


	int GetProgress(void) const { return progress_; }

	STORY_PROGRESS GetProgressEnum(void) const { return static_cast<STORY_PROGRESS>(progress_); }

	bool IsPatientCharExists(void) const { return isPatientCharExists_; }

	bool IsNurceCharExists(void) const { return isNurceCharExists_; }

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