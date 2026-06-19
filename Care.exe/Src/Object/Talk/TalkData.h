#pragma once

#include <string>
#include <vector>

class TalkDatas
{
public:
	enum class Speaker
	{
		Player,
		Patient,
		Narator,
	};

	struct TalkData
	{
		// 話者（デフォルトは Player）
		Speaker speaker = Speaker::Player;

		// トーク内容
		std::string talk;
	};
	
	enum class TalkDataIndex
	{
		NONE = -1,
		TALK_INIT,
		TALK_0,
		TALK_AFTERMG,
		TALK_PC,
		TALK_1,

		TALK_END_NURCE_LOST,
		TALK_END_PATIENT_LOST,
		TALK_END_BOTH_LOST,
	};

	static const std::vector<TalkData>& GetTalkData(TalkDataIndex dataIndex);
};

using TD = TalkDatas::TalkData;
using TDI = TalkDatas::TalkDataIndex;