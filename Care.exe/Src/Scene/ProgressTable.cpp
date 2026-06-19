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
			Player::INIT_PER,
			TDI::TALK_INIT, TDI::TALK_0, TDI::TALK_0,
			false
		},
		{
			STORY_PROGRESS::START_MINIGAME0,
			GameScene::Stage::PAT_ROOM,
			{ 17.0f, 10.0f, 0.0f },
			TDI::NONE, TDI::NONE, TDI::NONE,
			true
		},
		{
			STORY_PROGRESS::AFTER_MG,
			GameScene::Stage::PAT_ROOM,
			{ 48.0f, 33.0f, 0.0f },
			TDI::TALK_AFTERMG, TDI::NONE, TDI::TALK_AFTERMG,
			false
		},
		{
			STORY_PROGRESS::AFTER_MG_TALKED,
			GameScene::Stage::PAT_ROOM,
			{ 48.0f, 33.0f, 0.0f },
			TDI::NONE, TDI::NONE, TDI::TALK_PC,
			false
		},

		{
			STORY_PROGRESS::AFTER_PC,
			GameScene::Stage::NURSE_STATION,
			{ 36.0f, 21.0f, 0.0f },
			TDI::NONE, TDI::NONE, TDI::NONE,
			false
		},

		{
			STORY_PROGRESS::END_NURCE_LOST,
			GameScene::Stage::PAT_ROOM,
			{ 50.0f, 50.0f, 0.0f },
			TDI::TALK_END_NURCE_LOST, TDI::NONE, TDI::NONE,
			false
		},

		{
			STORY_PROGRESS::END_PATIENT_LOST,
			GameScene::Stage::NURSE_STATION,
			{ 50.0f, 50.0f, 0.0f },
			TDI::TALK_END_PATIENT_LOST, TDI::NONE, TDI::NONE,
			false
		},
		{
			STORY_PROGRESS::END_BOTH_LOST,
			GameScene::Stage::NURSE_STATION,
			{ 50.0f, 50.0f, 0.0f },
			TDI::TALK_END_BOTH_LOST, TDI::NONE, TDI::NONE,
			false
		},
	};

	const ProgressData DEFAULT_PROGRESS_DATA =
	{
		STORY_PROGRESS::START,
		GameScene::Stage::NURSE_STATION,
		Player::INIT_PER,
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
	if (data.talkEnd != talkId)
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

