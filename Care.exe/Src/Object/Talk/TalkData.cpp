#include "TalkData.h"

// トークデータの定義
// ===========================
// PL = Player、PT = Patient
TalkDatas::Speaker 看護師 = TalkDatas::Speaker::Player;
TalkDatas::Speaker 患者 = TalkDatas::Speaker::Patient;
TalkDatas::Speaker EOS = TalkDatas::Speaker::EOS;

static std::vector<TD> talkList_0 =
{
	{看護師, "こんにちは。"},
	{患者, "…こんにちは。"},
	{看護師, "調子は？"},
	{患者, "…元気です。"},
	{看護師, "…血圧、測るね"},
	{患者, "はい"},

};

static std::vector<TD> talkList_1 =
{
	{看護師, "こんにちは。"},
	{患者, "...あの。"},
	{看護師, "何か？"},
	{患者, "あなたって..."},
	{患者, "誰かに操られてません？"},
	{看護師, "何...急に。"},
	{看護師, "(...でもなんだろう...。\n{WAIT:50}たしかに、そんな気がしてくる。)"},
	{患者, "{PARAM:EYE_BLINK_R:0.7f}{PARAM:EYE_BLINK_L:0.6f}{PARAM:BROW_RY:0.3f}"
	"......。"},
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
