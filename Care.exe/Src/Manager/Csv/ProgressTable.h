#pragma once
#include "../../Common/Vector2.h"
#include <cstddef>
#include <DxLib.h>
#include "../ProgressManager.h"
#include "../../Object/Talk/TalkData.h"
#include "../../Scene/GameScene.h"

struct ProgressData
{
	ProgressManager::STORY_PROGRESS progress;	// 進行状況
	GameScene::Stage initStage;					// ロード時初期ステージ
	Vector2<float> playerInitPos;						// ロード時操作キャラ初期位置
	TDI firstTalk;								// 進行状況開始時の会話
	TDI patientTalk;							// その進行状況時患者に話しかけたときの会話
	TDI talkEnd;								// 進行する条件となる会話

	// 会話終了後に固定するエンディング進行度
	ProgressManager::STORY_PROGRESS endLockedProgress;
	bool autoAdvanceProgress = false;			// 進行状況開始時に自動で進めるか（デバッグ用）
};

class ProgressTable
{
public:
	using STORY_PROGRESS = ProgressManager::STORY_PROGRESS;

	// 進行度に対応する進行テーブル行の取得
	static const ProgressData& Get(STORY_PROGRESS progress);

	// 会話終了による進行条件の一致判定
	static bool ShouldAdvanceByTalkEnd(
		STORY_PROGRESS progress,
		TDI talkId);

	// 指定進行度に対応するエンディング固定状態の取得
	static STORY_PROGRESS GetEndLockedProgress(STORY_PROGRESS progress);

	// 開始時の自動進行の有効判定
	static bool ShouldAutoAdvance(STORY_PROGRESS progress);

private:

	// 進行CSVの必須列数
	static constexpr std::size_t REQUIRED_COLUMN_COUNT = 10;

	// 進行テーブルのCSV読み込み
	static void LoadCsvData();
};
