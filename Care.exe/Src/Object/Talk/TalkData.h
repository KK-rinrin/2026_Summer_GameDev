#pragma once

#include <string>
#include <vector>

class TalkDatas
{
public:
	enum class Speaker
	{
		EOS, // 会話終了（話者なし）
		Player,
		Patient,
	};

	struct TalkData
	{
		// 話者（デフォルトは Patient）
		Speaker speaker = Speaker::Player;

		// トーク内容
		std::string talk;
	};
	
	enum class TalkDataIndex
	{
		NONE = -1,
		TALK_0,
		TALK_1,
		TALK_2,
		TALK_3,
		TALK_4,
		TALK_5,
	};

	static const std::vector<TalkData>& GetTalkData(TalkDataIndex dataIndex);
};

using TD = TalkDatas::TalkData;
using TDI = TalkDatas::TalkDataIndex;