#include <direct.h>
#include <cstdio>
#include <fstream>

#include "ProgressManager.h"

namespace
{
	const char* DATA_DIR_PATH = "Data";
	const char* SYSTEM_DIR_PATH = "Data/System";
	const char* CHARA_DIR_PATH = "Chara";
	const char* PATIENT_CHAR_PATH = "Chara/patient.char";
	const char* NURCE_CHAR_PATH = "Chara/nurce.char";
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


bool ProgressManager::ResetProgressCache(void)
{
	progress_ = START;
	isPatientCharExists_ = IsFileExists(PATIENT_CHAR_PATH);
	isNurceCharExists_ = IsFileExists(NURCE_CHAR_PATH);

	if (!IsFileExists(PROGRESS_SAVE_PATH))
	{
		return true;
	}

	return std::remove(PROGRESS_SAVE_PATH) == 0;
}


void ProgressManager::Destroy(void)
{
	delete instance_;
	instance_ = nullptr;
}

ProgressManager::ProgressManager(void)
	:
	progress_(START),
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
		return;
	}

	file >> progress_;
	if (!file)
	{
		progress_ = START;
	}
}

void ProgressManager::SaveProgress(void) const
{
	std::ofstream file(PROGRESS_SAVE_PATH);
	if (!file)
	{
		return;
	}

	file << progress_;
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
