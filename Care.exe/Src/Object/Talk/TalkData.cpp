#include "TalkData.h"

// トークデータの定義
// ===========================
namespace {
	TalkDatas::Speaker ナレ = TalkDatas::Speaker::Narator;
	TalkDatas::Speaker 看護師 = TalkDatas::Speaker::Player;
	TalkDatas::Speaker 患者 = TalkDatas::Speaker::Patient;

	struct TalkListEntry
	{
		TDI id;
		const std::vector<TD> list;
	};

	static const TalkListEntry talkTable[] =
	{
		{ TDI::TALK_INIT, {
		{看護師, "....。"},
		{看護師, "...朝の当直か。"},
		{看護師, "(上のドアから部屋に入って、患者の血圧を測りに行こう)"}}},

		{ TDI::TALK_0, {
		{看護師, "こんにちは。"},
		{患者, "…こんにちは。"},
		{看護師, "調子は？"},
		{患者, "…元気です"},
		{看護師, "…血圧、測るね"},
		{患者, "はい"}}},

		{ TDI::TALK_AFTERMG, {
		{看護師, "...よし。\n{WAIT:300}...本当に静かな人。\n(かなり落ち着いているように見える。)"},
		{患者, "...どうも。"},
		{看護師, "じゃ、戻るからね"},
		{患者, "また。"}}},

		{ TDI::TALK_1, {
		{看護師, "こんにちは。"},
		{患者, "...あの。"},
		{看護師, "何か？"},
		{患者, "あなたって..."},
		{患者, "誰かに操られてません？"},
		{看護師, "何...急に。"},
		{看護師, "(...でもなんだろう...。\n{WAIT:50}たしかに、そんな気がしてくる。)"},
		{患者, "{PARAM:EYE_BLINK_R:0.7f}{PARAM:EYE_BLINK_L:0.6f}{PARAM:BROW_RY:0.3f}"
		"......。"}}}


	};
}

// 空のリスト（デフォルト返却用）
static const std::vector<TD> emptyTalkList;

// ===========================

const std::vector<TD>& TalkDatas::GetTalkData(TalkDataIndex dataIndex)
{
	for (const TalkListEntry& entry : talkTable)
	{
		if (entry.id == dataIndex)
		{
			return entry.list;
		}
	}

	return emptyTalkList;
}
