#pragma once

#include <DxLib.h>
#include "../Manager/ProgressManager.h"
#include "../Object/Talk/TalkData.h"
#include "GameScene.h"

struct ProgressTalkEndData
{
	TDI talkId = TDI::NONE;
	bool advancesProgress = false;
};

struct ProgressData
{
	ProgressManager::STORY_PROGRESS progress;
	GameScene::Stage initStage;
	VECTOR playerInitPos;
	TDI firstTalk;
	TDI patientTalk;
	ProgressTalkEndData talkEnd;
	bool autoAdvanceProgress = false;
};

class ProgressTable
{
public:
	using STORY_PROGRESS = ProgressManager::STORY_PROGRESS;

	static const ProgressData& Get(STORY_PROGRESS progress);
	static bool ShouldAdvanceByTalkEnd(
		STORY_PROGRESS progress,
		TDI talkId);
	static bool ShouldAutoAdvance(STORY_PROGRESS progress);
};
