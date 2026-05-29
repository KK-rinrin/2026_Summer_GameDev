#include "ProgressTable.h"
#include "../Object/Actor/Player.h"

namespace
{
	using STORY_PROGRESS = ProgressManager::STORY_PROGRESS;

	const ProgressData PROGRESS_DATAS[] =
	{
		{
			STORY_PROGRESS::START,
			GameScene::Stage::NURSE_STATION,
			{ Player::INIT_PER_X, Player::INIT_PER_Y, 0.0f },
			TDI::TALK_INIT,
			TDI::TALK_0,
			{ TDI::TALK_0, true },
			false
		},
		{
			STORY_PROGRESS::START_MINIGAME0,
			GameScene::Stage::PAT_ROOM,
			{ 17.0f, 10.0f, 0.0f },
			TDI::NONE,
			TDI::NONE,
			{},
			true
		},
		{
			STORY_PROGRESS::AFTER_MG,
			GameScene::Stage::PAT_ROOM,
			{ 60.0f, 45.0f, 0.0f },
			TDI::TALK_AFTERMG ,
			TDI::NONE,
			{},
			false
		},
	};

	const ProgressData DEFAULT_PROGRESS_DATA =
	{
		STORY_PROGRESS::START,
		GameScene::Stage::NURSE_STATION,
		{ Player::INIT_PER_X, Player::INIT_PER_Y, 0.0f },
		{},
		{},
		{},
		false
	};
}

const ProgressData& ProgressTable::Get(STORY_PROGRESS progress)
{
	for (const ProgressData& data : PROGRESS_DATAS)
	{
		if (data.progress == progress)
		{
			return data;
		}
	}

	return DEFAULT_PROGRESS_DATA;
}

bool ProgressTable::ShouldAdvanceByTalkEnd(
	STORY_PROGRESS progress,
	TDI talkId)
{
	const ProgressData& data = Get(progress);
	if (!data.talkEnd.advancesProgress || data.talkEnd.talkId != talkId)
	{
		return false;
	}

	return true;
}

bool ProgressTable::ShouldAutoAdvance(STORY_PROGRESS progress)
{
	const ProgressData& data = Get(progress);
	return data.autoAdvanceProgress;
}
