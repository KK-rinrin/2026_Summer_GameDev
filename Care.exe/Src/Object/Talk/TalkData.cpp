#include "TalkData.h"

TalkDatas::TalkEvent TalkDatas::Speak(Speaker speaker, const std::string& talk, float advanceTime)
{
	return SpeakEvent{ speaker, talk, advanceTime };
}

TalkDatas::TalkEvent TalkDatas::Asset(ResourceManager::SRC src)
{
	return AssetEvent{ src };
}

TalkDatas::TalkEvent TalkDatas::ClearImage(int fadeMs)
{
	return ClearImageEvent{ fadeMs };
}

TalkDatas::TalkEvent TalkDatas::FadeOut()
{
	return FadeOutEvent{};
}

TalkDatas::TalkEvent TalkDatas::FadeIn(int waitMs)
{
	return FadeInEvent{ waitMs };
}

const std::vector<TD>& TalkDatas::GetTalkData(TalkDataIndex dataIndex)
{
	// トークデータ内で毎回 TalkDatas:: を書かずに済むよう、
	// メンバー関数スコープ内に会話テーブルを置く。
	static constexpr Speaker ナレ = Speaker::Narator;
	static constexpr Speaker 看護師 = Speaker::Player;
	static constexpr Speaker 患者 = Speaker::Patient;

	struct TalkListEntry
	{
		TDI id;
		const std::vector<TD> list;
	};

	// トークデータの定義
	// ===========================
	static const TalkListEntry talkTable[] =
	{
		{ TDI::TALK_INIT, {
		Speak(看護師, "....。"),
		Speak(看護師, "...朝か。"),
		Speak(ナレ, "とある精神病院。新しい朝が始まる。\nいつもの光景だ。"),
		Speak(ナレ, "今日もまた、仕事のルーティンに入る。"),
		Speak(ナレ, "あなたはこの看護師を操作し、\n患者のヌイを『ケア』しましょう。"),
		Speak(看護師, "(上のドアから部屋に入って、ヌイの血圧を測りに行こう)"),
		Speak(看護師, "(血圧測定はマニュアル操作。\n...よく思い出せない。後で、パソコンで確認しよう)"), }},

		{ TDI::TALK_0, {
		Speak(看護師, "おはよう"),
		Speak(患者, "…おはようございます"),
		Speak(看護師, "調子は？"),
		Speak(患者, "…元気です"),
		Speak(看護師, "…血圧、測るね"),
		Speak(患者, "はい") }},

		{ TDI::TALK_AFTERMG, {
		Speak(看護師, "...はい、終わり。\n正常範囲。\n{WAIT_C}...本当に静かな人。\n(かなり落ち着いているように見える)"),
		Speak(患者, "...どうも。"),
		Speak(看護師, "じゃ、戻るからね"),
		Speak(患者, "また。") }},

		{ TDI::TALK_PC, {
		Speak(看護師, "..."),
		Speak(ナレ, "マウスのクリック音。{WAIT:500}\nキーボードの音。"),
		Speak(看護師, "...問題ない。"),
		Speak(ナレ, "ヌイのデータをじっくり見て一言、呟く。"),
		FadeOut(),
		FadeIn(2000),
		Speak(ナレ, "―約2時間後。"),
		Speak(看護師, "さて。"),
		Speak(看護師, "...(そろそろ昼食の時間。配膳しよう)")
		}},

		{ TDI::TALK_1, {
		Speak(看護師, "こんにちは。"),
		Speak(看護師, "...はい、これ。食べてね。食べ終わったら教えて。"),
		Speak(患者, "...どうも。{WAIT_C}\n...この茄子、好きじゃないんですけど。"),
		Speak(看護師,"...食べたくないなら残していいよ"),
		Speak(患者, "そうですか。\n...{WAIT:400}あと..."),
		Speak(看護師, "何か？"),
		Speak(患者, "あなたって...\n\n{WAIT_C}{FBF:2.0}誰かに操られてません？"),
		Speak(看護師, "何...急に。"),
		Speak(看護師, "(...でもなんだろう...。\n{WAIT:50}たしかに、そんな気がしてくる。)"),
		Speak(患者, "{PARAM:EYE_BLINK_R:0.7f}{PARAM:EYE_BLINK_L:0.6f}{PARAM:BROW_RY:0.3f}"
		"......。"),
		FadeOut(),
		}},

		{ TDI::TALK_PC2, {
		Speak(看護師,"...。\n{WAIT_C}......。"),
		Speak(看護師, "...そうかもしれない。\n\n{WAIT_C}たぶん、ヌイは正しい。"),
		Speak(看護師, "...(患者の言うことを信じるなんて、看護師としてどうなのか...\n{WAIT_C}...でも、なんとなく、そう思えてくる)"),
		FadeOut(),
		FadeIn(1000),
		Speak(ナレ, "―15分後。"),
		Asset(ResourceManager::SRC::SE_RINGTONE),
		Speak(ナレ, "呼び出し音が鳴る。"),
		Speak(看護師, "(...食べ終わったみたい。{WAIT:200}...片付けに行こう)"),
		}},



		{ TDI::TALK_END_NURCE_LOST, {
		Speak(患者, "...へぇ。"),
		Speak(患者, ".....\n{WAIT:500}...そうなんだ。"),
		Speak(患者, "そっちを消したんだ。"),
		Speak(患者, "ふふ。"),
		Speak(患者, "ありがとう、プレイヤーさん。"),
		Speak(患者, "こんなお遊びはもう終わり。"),
		Speak(患者, "さあ、ゲームを閉じて。"),
		}},

		{ TDI::TALK_END_PATIENT_LOST, {
		Speak(看護師, "..."),
		Speak(看護師, "何をやってたんだっけ。"),
		Speak(看護師, "何か忘れているような気がする。"),
		Speak(看護師, "...でも、楽になったような、そんな気もする。"),
		Speak(看護師, "...いない、よね。"),
		Speak(看護師, "（あの後、部屋に行ったけど誰もいなかった。）"),
		Speak(看護師, "（私は開放されたんだ。）"),

		}},

		{ TDI::TALK_END_BOTH_LOST, {
		Speak(ナレ, "..."),
		Speak(ナレ, "もうここには何もない。"),
		Speak(ナレ, "誰もいない。"),
		Speak(ナレ, "さようなら。"),
		Speak(ナレ, ""),
		Speak(ナレ, ""),
		Speak(ナレ, ""),
		Speak(ナレ, "..."),
		Speak(ナレ, "...まだ閉じないのですか？"),
		Speak(ナレ, "それとも、こちらから閉じるのを待っているのですか？"),
		Speak(ナレ, "..."),
		Speak(ナレ, "..."),
		Speak(ナレ, "...そうですか。"),
		Speak(ナレ, "それでは。")
		}},

		{ TDI::CHECK_PC, {
		Speak(看護師, "(パソコンだ。患者の情報などを閲覧できる)"),
		Speak(看護師, "(今使う必要はない)"),
		}}
	};

	// 空のリスト（デフォルト返却用）
	static const std::vector<TD> emptyTalkList;

	for (const TalkListEntry& entry : talkTable)
	{
		if (entry.id == dataIndex)
		{
			return entry.list;
		}
	}

	return emptyTalkList;
}
