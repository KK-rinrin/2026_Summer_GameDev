#include <direct.h>
#include <fstream>

#include "ProgressManager.h"

namespace
{
	const char* DATA_DIR_PATH = "Data";
	const char* SYSTEM_DIR_PATH = "Data/System";
	const char* CHARA_DIR_PATH = "Chara";
	const char* PATIENT_CHAR_PATH = "Chara/nui.char";
	const char* NURCE_CHAR_PATH = "Chara/neit.char";
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

ProgressManager* ProgressManager::instance_ = nullptr;
	
void ProgressManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ProgressManager();
	}
	instance_->Init();
}

ProgressManager& ProgressManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		ProgressManager::CreateInstance();
	}
	return *instance_;
}


void ProgressManager::Init(void)
{
	progress_ = START;
	resetCount_ = 0;

	_mkdir(SYSTEM_DIR_PATH);
	_mkdir(CHARA_DIR_PATH);

	const bool isFirstLaunch = !IsFileExists(PROGRESS_SAVE_PATH);
	if (!isFirstLaunch)
	{
		LoadProgress();
	}

	CheckCharaFiles(isFirstLaunch);
	ApplyEndProgressByCharaFiles();
	SaveProgress();
}

void ProgressManager::AddProgress(void)
{
	progress_++;
	SaveProgress();
}
void ProgressManager::SetProgress(STORY_PROGRESS progress)
{
	progress_ = progress;
	SaveProgress();
}


bool ProgressManager::ResetProgressCache(void)
{
	isPatientCharExists_ = IsFileExists(PATIENT_CHAR_PATH);
	isNurceCharExists_ = IsFileExists(NURCE_CHAR_PATH);

	if (!isPatientCharExists_)
	{
		CreateEmptyFile(PATIENT_CHAR_PATH);
		isPatientCharExists_ = IsFileExists(PATIENT_CHAR_PATH);
	}
	if (!isNurceCharExists_)
	{
		CreateEmptyFile(NURCE_CHAR_PATH);
		isNurceCharExists_ = IsFileExists(NURCE_CHAR_PATH);
	}

	progress_ = START;

#ifndef _DEMO
	++resetCount_;
#else
	// 体験版ではリセット回数を記録しない（常に0）
	resetCount_ = 0;
#endif

	return isPatientCharExists_ && isNurceCharExists_ && SaveProgress();
}

bool ProgressManager::IsPatientCharExists(void) const
{
	return IsFileExists(PATIENT_CHAR_PATH);
}

bool ProgressManager::IsNurceCharExists(void) const
{
	return IsFileExists(NURCE_CHAR_PATH);
}

bool ProgressManager::IsCharaFileDeletedDuringRun(void) const
{
	return (isPatientCharExists_ && !IsPatientCharExists()) ||
		(isNurceCharExists_ && !IsNurceCharExists());
}


bool ProgressManager::IsEndTalkProgress(void) const
{
	switch (GetProgressEnum())
	{
	case END_PATIENT_LOST:
	case END_NURCE_LOST:
	case END_BOTH_LOST:
		return true;
	default:
		return false;
	}
}
bool ProgressManager::IsEndLockedProgress(void) const
{
	switch (GetProgressEnum())
	{
	case END_PATIENT_LOCKED:
	case END_NURCE_LOCKED:
	case END_BOTH_LOCKED:
		return true;
	default:
		return false;
	}
}

void ProgressManager::Destroy(void)
{
	delete instance_;
	instance_ = nullptr;
}

ProgressManager::ProgressManager(void)
	:
	progress_(START),
	resetCount_(0),
	isPatientCharExists_(false),
	isNurceCharExists_(false)
{
}

void ProgressManager::LoadProgress(void)
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
	file >> resetCount_;
	if (!file || resetCount_ < 0)
	{
		resetCount_ = 0;
	}
#else
	// 体験版ではファイルからリセット回数を読み込まず常に0とする
	resetCount_ = 0;
#endif
}

bool ProgressManager::SaveProgress(void) const
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
	isNurceCharExists_ = IsFileExists(NURCE_CHAR_PATH);

	if (isFirstLaunch)
	{
		if (!isPatientCharExists_)
		{
			CreateEmptyFile(PATIENT_CHAR_PATH);
			isPatientCharExists_ = IsFileExists(PATIENT_CHAR_PATH);
		}

		if (!isNurceCharExists_)
		{
			CreateEmptyFile(NURCE_CHAR_PATH);
			isNurceCharExists_ = IsFileExists(NURCE_CHAR_PATH);
		}
	}
}

void ProgressManager::ApplyEndProgressByCharaFiles(void)
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

	if (isPatientCharExists_ && isNurceCharExists_)
	{
		return;
	}

	if (!isPatientCharExists_ && !isNurceCharExists_)
	{
		progress_ = END_BOTH_LOST;
		return;
	}

	if (!isPatientCharExists_)
	{
		progress_ = END_PATIENT_LOST;
		return;
	}

	progress_ = END_NURCE_LOST;
}
