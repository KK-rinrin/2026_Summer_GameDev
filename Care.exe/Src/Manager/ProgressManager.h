#pragma once
#include <memory>

class ProgressManager
{
public:

	// ストーリーの進行状態
	enum STORY_PROGRESS
	{
		START = 0,				// ゲーム開始
		START_MINIGAME0,		// ゲーム開始後のイベント1
		MINIGAME_RETRY,			// ミニゲーム再挑戦待ち
		AFTER_MG,				// ミニゲーム後
		AFTER_MG_TALKED,		// ミニゲーム後の会話後
		AFTER_PC,				// PC作業後
		AFTER_GET_PLATE,		// 昼食の配膳プレート取得後
		LUNCH,					// 昼食
		AFTER_PC2,				// PC作業再び
		AFTER_LUNCH,			// 昼食後
		AFTER_PC3,				// カルテ記入
		AFTER_TALK3,			// バグり始める
		AFTER_PC4,				// Charaフォルダ確認後
		DINNER,					// 夕食？
		END_PATIENT_LOST = 100,	// 患者削除END
		END_PATIENT_LOCKED,		// 患者削除エンディングの固定状態
		END_NURSE_LOST = 110,	// 看護師削除END
		END_NURSE_LOCKED,		// 看護師削除エンディングの固定状態
		END_BOTH_LOST = 120,	// 両方削除END
		END_BOTH_LOCKED,		// 両者削除エンディングの固定状態
		END_RUINED = 200,		// 崩壊END
		END_RUINED_LOCKED,		// 崩壊エンディングの固定状態
		CLEAR_COMPLETE = 500,	// スタッフロール後リセット要求
	};

	// 進行管理インスタンスの生成と初期化
	static void CreateInstance();

	// 進行管理インスタンスの取得
	static ProgressManager& GetInstance();

	// 初期化
	void Init();

	// 進行度増加
	void AddProgress();

	// 進行度の設定と保存
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
	bool IsNurseCharExists() const;

	// 起動時に存在したキャラファイルの実行中削除判定
	bool IsCharaFileDeletedDuringRun() const;

	// キャラ削除に伴うエンディング会話状態の判定
	bool IsEndTalkProgress() const;

	// エンディング進行の固定状態の判定
	bool IsEndLockedProgress() const;

	// キャラファイル削除が進行に反映される段階の判定
	bool IsCanDeleteProgress() const { return progress_ >= AFTER_LUNCH;  }

	// リセット要求状態の判定
	bool IsResetRequiredProgress() const { return progress_ == CLEAR_COMPLETE || progress_ >= RESET_REQUIRED_PROGRESS_MIN; }

	// リセット履歴の有無の判定
	bool HasResetHistory() const { return resetCount_ > 0; }

	// 削除
	void Destroy();

private:

	// リセット要求とみなす進行値の下限
	static constexpr int RESET_REQUIRED_PROGRESS_MIN = 512;

	// 進行管理の共有インスタンス
	static std::shared_ptr<ProgressManager> instance_;

	// 現在の進行度の整数値
	int progress_;

	// 保存対象のリセット回数
	int resetCount_;

	// 起動時またはリセット時の患者ファイルの存在状態
	bool isPatientCharExists_;

	// 起動時またはリセット時の看護師ファイルの存在状態
	bool isNurseCharExists_;

	// シングルトン
	ProgressManager();
	ProgressManager(const ProgressManager& instance) = default;
	~ProgressManager() = default;

	// 保存ファイルからの進行度とリセット回数の読み込み
	void LoadProgress();

	// 進行度とリセット回数のファイル保存と成功判定
	bool SaveProgress() const;

	// キャラファイルの存在確認と初回起動時の生成
	void CheckCharaFiles(bool isFirstLaunch);

	// キャラファイルの有無に応じたエンディング進行の適用
	void ApplyEndProgressByCharaFiles();
};
