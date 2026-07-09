#include <DxLib.h>
#include "../Application.h"
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

const ClearScene::EndInfo& ClearScene::GetEndInfo(void) const
{
	static constexpr EndInfo END_INFOS[] =
	{
		{
			ProgressManager::STORY_PROGRESS::END_PATIENT_LOCKED,
			ResourceManager::SRC::STILL_END_NUR,
			"End_T:Š³ŽÒ‚Í‚¢‚È‚©‚Á‚½"
		},
		{
			ProgressManager::STORY_PROGRESS::END_NURCE_LOCKED,
			ResourceManager::SRC::STILL_END_NUI,
			"End_D:Žv‚¢’Ê‚è"
		},
		{
			ProgressManager::STORY_PROGRESS::END_BOTH_LOCKED,
			ResourceManager::SRC::STILL_END_BOTH,
			"End_NULL:‚à‚Ê‚¯‚ÌŠk"
		},
	};
	static constexpr EndInfo DEFAULT_END_INFO =
	{
		ProgressManager::STORY_PROGRESS::END_BOTH_LOCKED,
		ResourceManager::SRC::STILL_END_BOTH,
		"‚»‚µ‚Ä’N‚à‚¢‚È‚­‚È‚Á‚½"
	};

	for (const EndInfo& endInfo : END_INFOS)
	{
		if (endInfo.progress == prgMng_.GetProgressEnum())
		{
			return endInfo;
		}
	}

	return DEFAULT_END_INFO;
}

ResourceManager::SRC ClearScene::GetStillSrc(void) const
{
	return GetEndInfo().stillSrc;
}

std::string ClearScene::GetEndTitle(void) const
{
	return GetEndInfo().title;
}
