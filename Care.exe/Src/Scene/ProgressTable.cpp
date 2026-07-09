#include "ProgressTable.h"
#include "../Application.h"
#include "../Object/Actor/Player.h"
#include "../Utility/SchoolUtility.h"
#include <fstream>
#include <string>
#include <vector>

namespace
{
	using STORY_PROGRESS = ProgressManager::STORY_PROGRESS;

	std::vector<ProgressData> progressDatas;
	bool isCsvLoaded = false;

	const ProgressData DEFAULT_PROGRESS_DATA =
	{
		STORY_PROGRESS::START,
		GameScene::Stage::NURSE_STATION,
		Player::INIT_PER,
		TDI::NONE,
		TDI::NONE,
		TDI::NONE,
		STORY_PROGRESS::START,
		false
	};
}

const ProgressData& ProgressTable::Get(STORY_PROGRESS progress)
{
	LoadCsvData();
	for (const ProgressData& data : progressDatas)
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

ProgressTable::STORY_PROGRESS ProgressTable::GetEndLockedProgress(STORY_PROGRESS progress)
{
	const ProgressData& data = Get(progress);
	return data.endLockedProgress;
}

bool ProgressTable::ShouldAutoAdvance(STORY_PROGRESS progress)
{
	const ProgressData& data = Get(progress);
	return data.autoAdvanceProgress;
}

void ProgressTable::LoadCsvData()
{
	// 何度もCSVを読み直さないように初回だけ処理する
	if (isCsvLoaded)
	{
		return;
	}
	isCsvLoaded = true;

	// ProgressTable専用CSVを開く
	std::ifstream ifs(Application::PATH_CSV + "ProgressTable.csv");
	if (!ifs)
	{
		// ファイルが無い場合はDEFAULT_PROGRESS_DATAだけで進める
		return;
	}

	// CSVを1行ずつ読み込むための文字列
	std::string line;

	// 1行目は日本語ヘッダーなのでデータとして扱わない
	bool isHeader = true;
	while (std::getline(ifs, line))
	{
		// 空行はデータ不足として読み飛ばす
		if (line.empty())
		{
			continue;
		}

		// ヘッダー行は列名確認ではなく単純にスキップする
		if (isHeader)
		{
			isHeader = false;
			continue;
		}

		// カンマ区切りで各列に分割する
		std::vector<std::string> strSplit = SchoolUtility::Split(line, ',');
		if (strSplit.size() < 10)
		{
			// 必要な列が足りない行は無視する
			continue;
		}

		try
		{
			// 1行分のCSVをProgressDataへ詰め替える
			ProgressData data = ProgressData();
			int idx = 0;

			// 進行度ID
			data.progress = static_cast<STORY_PROGRESS>(std::stoi(strSplit[idx++]));

			// ロード時の初期ステージID
			data.initStage = static_cast<GameScene::Stage>(std::stoi(strSplit[idx++]));

			// ロード時の操作キャラ初期座標
			data.playerInitPos =
			{
				std::stof(strSplit[idx++]),
				std::stof(strSplit[idx++]),
				std::stof(strSplit[idx++])
			};

			// 進行開始時に自動再生する会話ID
			data.firstTalk = static_cast<TDI>(std::stoi(strSplit[idx++]));

			// 患者に話しかけた時の会話ID
			data.patientTalk = static_cast<TDI>(std::stoi(strSplit[idx++]));

			// この会話が終わったら進行するという条件ID
			data.talkEnd = static_cast<TDI>(std::stoi(strSplit[idx++]));

			// エンディング会話後に固定する進行度ID
			data.endLockedProgress = static_cast<STORY_PROGRESS>(std::stoi(strSplit[idx++]));

			// 進行開始時に即AddProgressするかどうか
			data.autoAdvanceProgress = (std::stoi(strSplit[idx++]) != 0);

			// 正常に読めた行だけテーブルに追加する
			progressDatas.emplace_back(data);
		}
		catch (...)
		{
			// 数値変換に失敗した行は読み飛ばして次の行へ進む
			continue;
		}
	}
}
