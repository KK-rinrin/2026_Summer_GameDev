#pragma once

class ProgressManager
{

public:

	enum STORY_PROGRESS
	{
		START = 0,	// ゲーム開始
		START_MINIGAME0,	// ゲーム開始後のイベント1
		MINIGAME_RETRY,	// ミニゲーム再挑戦待ち

		AFTER_MG,	// ミニゲーム後
		AFTER_MG_TALKED,	// ミニゲーム後の会話後
		AFTER_PC,	// PC作業後
		AFTER_GET_PLATE,	// 昼食の配膳プレート取得後
		LUNCH,		// 昼食
		AFTER_PC2,	// PC作業再び
		AFTER_LUNCH,	// 昼食後
		AFTER_PC3,		// カルテ記入
		AFTER_TALK3,	// バグり始める
		AFTER_PC4,		// Charaフォルダ確認後
		DINNER,			// 夕食？

		END_PATIENT_LOST = 100,	// 患者削除END
		END_PATIENT_LOCKED,
		END_NURCE_LOST = 110,	// 看護師削除END
		END_NURCE_LOCKED,
		END_BOTH_LOST = 120,	// 両方削除END
		END_BOTH_LOCKED,
		END_RUINED = 200,		// 崩壊END
		END_RUINED_LOCKED,

		CLEAR_COMPLETE = 500,	// スタッフロール後リセット要求
	};

	static void CreateInstance();

	static ProgressManager& GetInstance();

	// 初期化
	void Init();

	// 進行度増加
	void AddProgress();

	void SetProgress(STORY_PROGRESS progress);

	// 進行度を初期化してリセット回数を記録
	bool ResetProgressCache();

	// 進行度取得
	int GetProgress() const { return progress_; }

	// 進行度enum取得
	STORY_PROGRESS GetProgressEnum() const { return static_cast<STORY_PROGRESS>(progress_); }

	// 患者charファイル存在取得
	bool IsPatientCharExists() const;

	// 看護師charファイル存在取得
	bool IsNurceCharExists() const;

	bool IsCharaFileDeletedDuringRun() const;

	bool IsEndTalkProgress() const;

	bool IsEndLockedProgress() const;

	bool IsCanDeleteProgress() const { return progress_ >= AFTER_LUNCH;  }

	bool IsResetRequiredProgress() const { return progress_ == CLEAR_COMPLETE || progress_ >= 512; }

	bool HasResetHistory() const { return resetCount_ > 0; }

	// 削除
	void Destroy();

private:

	static ProgressManager* instance_;

	int progress_;
	int resetCount_;
	bool isPatientCharExists_;
	bool isNurceCharExists_;

	ProgressManager();

	ProgressManager(const ProgressManager& instance) = default;

	~ProgressManager() = default;

	void LoadProgress();

	bool SaveProgress() const;

	void CheckCharaFiles(bool isFirstLaunch);

	void ApplyEndProgressByCharaFiles();
};
