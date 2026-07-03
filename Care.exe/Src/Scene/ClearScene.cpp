#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ProgressManager.h"
#include "ClearScene.h"

ClearScene::ClearScene(void)
	:
	SceneBase(),
	stillHandle_(-1)
{
}

ClearScene::~ClearScene(void)
{
}

void ClearScene::Update(void)
{
}

void ClearScene::Draw(void)
{
	if (stillHandle_ < 0)
	{
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
		return;
	}

	DrawExtendGraph(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		stillHandle_, true);
}

void ClearScene::Delete(void)
{
	stillHandle_ = -1;
}

void ClearScene::InitLoad(void)
{
	stillHandle_ = resMng_.Load(GetStillSrc()).handleId_;
}

ResourceManager::SRC ClearScene::GetStillSrc(void) const
{
	switch (prgMng_.GetProgressEnum())
	{
	case ProgressManager::STORY_PROGRESS::END_PATIENT_LOCKED:
		return ResourceManager::SRC::STILL_END_NUR;
	case ProgressManager::STORY_PROGRESS::END_NURCE_LOCKED:
		return ResourceManager::SRC::STILL_END_NUI;
	case ProgressManager::STORY_PROGRESS::END_BOTH_LOCKED:
		return ResourceManager::SRC::STILL_END_BOTH;
	default:
		return ResourceManager::SRC::STILL_END_BOTH;
	}
}
