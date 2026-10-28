#include <direct.h>
#include <fstream>

#include "ProgressManager.h"

namespace
{
	const char* DATA_DIR_PATH = "Data";
	const char* SYSTEM_DIR_PATH = "Data/System";
	const char* CHARA_DIR_PATH = "Chara";
	const char* PATIENT_CHAR_PATH = "Chara/nui.char";
	const char* NURSE_CHAR_PATH = "Chara/neit.char";
	const char* PROGRESS_SAVE_PATH = "Data/System/cache.bin";

	bool IsFileExists(const char* path)
	{
		std::ifstream file(path, std::ios::binary);
		return file.good();
	}

	void CreateEmptyFile(const char* path)
	{
		std::ofstream file(path, std::ios::binary);
	}
}

std::shared_ptr<ProgressManager> ProgressManager::instance_ = nullptr;
	
void ProgressManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = std::shared_ptr<ProgressManager>(new ProgressManager(), [](ProgressManager* instance) { delete instance; });
	}
	instance_->Init();
}

ProgressManager& ProgressManager::GetInstance()
{
	if (instance_ == nullptr)
	{
		ProgressManager::CreateInstance();
	}
	return *instance_;
}


void ProgressManager::Init()
{
	progress_ = START;
	resetCount_ = 0;

	// システムフォルダの生成
	if (_mkdir(SYSTEM_DIR_PATH) == -1) {
		if (errno != EEXIST) {
			// フォルダが既に存在する場合以外のエラー
			printf("フォルダが正常に作成できませんでした。MAKE_SYSTEM_DIR_FAILED");

		}
	}

	// Charaフォルダの生成
	if (_mkdir(CHARA_DIR_PATH) == -1) {
		if (errno != EEXIST) {
			// フォルダが既に存在する場合以外のエラー
			printf("フォルダが正常に作成できませんでした。MAKE_CHARA_DIR_FAILED");

		}
	}

	const bool isFirstLaunch = !IsFileExists(PROGRESS_SAVE_PATH);
	if (!isFirstLaunch)
	{
		LoadProgress();
	}

	CheckCharaFiles(isFirstLaunch);
	ApplyEndProgressByCharaFiles();
	SaveProgress();
}

void ProgressManager::AddProgress()
{
	progress_++;
	SaveProgress();
}
void ProgressManager::SetProgress(STORY_PROGRESS progress)
{
	progress_ = progress;
	SaveProgress();
}

/// <summary>
/// セーブデータの削除
/// </summary>
/// <returns>リセット成功したか？</returns>
bool ProgressManager::ResetProgressCache()
{
	isPatientCharExists_ = IsFileExists(PATIENT_CHAR_PATH);
	isNurseCharExists_ = IsFileExists(NURSE_CHAR_PATH);

	if (!isPatientCharExists_)
	{
		CreateEmptyFile(PATIENT_CHAR_PATH);
		isPatientCharExists_ = IsFileExists(PATIENT_CHAR_PATH);
	}
	if (!isNurseCharExists_)
	{
		CreateEmptyFile(NURSE_CHAR_PATH);
		isNurseCharExists_ = IsFileExists(NURSE_CHAR_PATH);
	}

	progress_ = START;

#ifndef _DEMO
	++resetCount_;
#else
	// 体験版ではリセット回数を記録しない（常に0）
	resetCount_ = 0;
#endif

	// リセットが成功したか判定（charデータの存在復元・進行度の保存成功か）を返す
	return isPatientCharExists_ && isNurseCharExists_ && SaveProgress();
}

bool ProgressManager::IsPatientCharExists() const
{
	return IsFileExists(PATIENT_CHAR_PATH);
}

bool ProgressManager::IsNurseCharExists() const
{
	return IsFileExists(NURSE_CHAR_PATH);
}

bool ProgressManager::IsCharaFileDeletedDuringRun() const
{
	return (isPatientCharExists_ && !IsPatientCharExists()) ||
		(isNurseCharExists_ && !IsNurseCharExists());
}


bool ProgressManager::IsEndTalkProgress() const
{
	switch (GetProgressEnum())
	{
	case END_PATIENT_LOST:
	case END_NURSE_LOST: case END_BOTH_LOST: return true;
	default:
		return false;
	}
}
bool ProgressManager::IsEndLockedProgress() const
{
	switch (GetProgressEnum())
	{
	case END_PATIENT_LOCKED:
	case END_NURSE_LOCKED: case END_BOTH_LOCKED: case END_RUINED_LOCKED: return true;
	default:
		return false;
	}
}

void ProgressManager::Destroy()
{
	instance_.reset();
	instance_ = nullptr;
}

ProgressManager::ProgressManager()
	: progress_(START)
	, resetCount_(0)
	, isPatientCharExists_(false)
	, isNurseCharExists_(false)
{
}

void ProgressManager::LoadProgress()
{
	std::ifstream file(PROGRESS_SAVE_PATH);
	if (!file)
	{
		progress_ = START;
		resetCount_ = 0;
		return;
	}

	file >> progress_;
	if (!file)
	{
		progress_ = START;
		resetCount_ = 0;
		return;
	}

#ifndef _DEMO
	// デモ版でなければ、リセットカウントを読み込む
	file >> resetCount_;
	if (!file || resetCount_ < 0)
	{
		// ファイルが存在しないか、カウントが0未満なら0とする
		resetCount_ = 0;
	}
#else
	// 体験版ではファイルからリセット回数を読み込まず常に0とする
	resetCount_ = 0;
#endif
}

bool ProgressManager::SaveProgress() const
{
	std::ofstream file(PROGRESS_SAVE_PATH);
	if (!file)
	{
		return false;
	}

#ifndef _DEMO
	file << progress_ << ' ' << resetCount_;
#else
	// 体験版ではリセット回数を常に0で保存する
	file << progress_ << ' ' << 0;
#endif
	return file.good();
}

void ProgressManager::CheckCharaFiles(bool isFirstLaunch)
{
	isPatientCharExists_ = IsFileExists(PATIENT_CHAR_PATH);
	isNurseCharExists_ = IsFileExists(NURSE_CHAR_PATH);

	if (isFirstLaunch)
	{
		if (!isPatientCharExists_)
		{
			CreateEmptyFile(PATIENT_CHAR_PATH);
			isPatientCharExists_ = IsFileExists(PATIENT_CHAR_PATH);
		}

		if (!isNurseCharExists_)
		{
			CreateEmptyFile(NURSE_CHAR_PATH);
			isNurseCharExists_ = IsFileExists(NURSE_CHAR_PATH);
		}
	}
}

void ProgressManager::ApplyEndProgressByCharaFiles()
{
	if (IsResetRequiredProgress())
	{
		return;
	}

	if (!IsCanDeleteProgress())
	{
		return;
	}

	if (IsEndLockedProgress())
	{
		return;
	}

	if (isPatientCharExists_ && isNurseCharExists_)
	{
		return;
	}

	if (!isPatientCharExists_ && !isNurseCharExists_)
	{
		progress_ = END_BOTH_LOST;
		return;
	}

	if (!isPatientCharExists_)
	{
		progress_ = END_PATIENT_LOST;
		return;
	}

	progress_ = END_NURSE_LOST;
}
