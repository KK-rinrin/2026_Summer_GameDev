#pragma once

#include <DxLib.h>
#include "../Manager/ProgressManager.h"
#include "../Object/Talk/TalkData.h"
#include "GameScene.h"

struct ProgressTalkEndData
{
	TDI talkId = TDI::NONE;			// 会話ID
	bool advancesProgress = false;	// この会話が終わったときに進行状況を進めるか
};

struct ProgressData
{
	ProgressManager::STORY_PROGRESS progress;	// 進行状況
	GameScene::Stage initStage;					// ロード時初期ステージ
	VECTOR playerInitPos;						// ロード時操作キャラ初期位置
	TDI firstTalk;								// 進行状況開始時の会話
	TDI patientTalk;							// その進行状況時患者に話しかけたときの会話
	ProgressTalkEndData talkEnd;				// 進行する条件となる会話
	bool autoAdvanceProgress = false;			// 進行状況開始時に自動で進めるか（デバッグ用）
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
