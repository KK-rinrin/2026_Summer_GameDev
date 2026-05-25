#include "TalkData.h"

// トークデータの定義
// ===========================
// PL = Player、PT = Patient
TalkDatas::Speaker PL = TalkDatas::Speaker::Player;
TalkDatas::Speaker PT = TalkDatas::Speaker::Patient;

static std::vector<TD> talkList_0 =
{
	{PL, "こんにちは。"},
	{PT, "…こんにちは。"},
	{PL, "調子は？"},
	{PT, "…元気です。"}
};

static std::vector<TD> talkList_1 =
{
	{PL, "こんにちは。"},
	{PT, "...あの。"},
	{PL, "何か？"},
	{PT, "あなたって..."},
	{PT, "誰かに操られてません？"},
	{PL, "何...急に。"},
	{PL, "(...でもなんだろう...。\n{WAIT:50}たしかに、そんな気がしてくる。)"},
	{PT, "{PARAM:EYE_BLINK_R:0.7f}{PARAM:EYE_BLINK_L:0.6f}{PARAM:BROW_RY:0.3f}"
	"......。"}
};


// 空のリスト（デフォルト返却用）
static const std::vector<TD> emptyTalkList;

// ===========================

const std::vector<TD>& TalkDatas::GetTalkData(TalkDataIndex dataIndex)
{
	switch (dataIndex)
	{
	case TalkDataIndex::TALK_0:
		return talkList_0;

	case TalkDataIndex::TALK_1:
		return talkList_1;

	case TalkDataIndex::TALK_2:
		//return talkList_2;

	case TalkDataIndex::TALK_3:
		//return talkList_3;

	case TalkDataIndex::TALK_4:
		//return talkList_4;

	case TalkDataIndex::TALK_5:
		//return talkList_5;

	default:
		return emptyTalkList;
	};
}
