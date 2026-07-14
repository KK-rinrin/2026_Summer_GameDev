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

TalkDatas::TalkEvent TalkDatas::Bgm(SoundManager::BGM bgm)
{
	return BgmEvent{ bgm };
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
		// 始めての会話
		{ TDI::TALK_INIT, {
		Speak(看護師, "....。"),
		Speak(看護師, "...朝か。"),
		Speak(ナレ, "とある精神病院。新しい朝が始まる。\nいつもの光景だ。"),
		Speak(ナレ, "今日もまた、仕事のルーティンに入る。"),
		Speak(ナレ, "あなたはこの看護師を操作し、\n患者を『ケア』しましょう。"),
		Speak(看護師, "(上のドアから部屋に入って、患者の血圧を測りに行こう)"),
		Speak(看護師, "(血圧測定はマニュアル操作。\n...よく思い出せない。後で、パソコンで確認しよう)"), }},

		// 血圧測定前の会話
		{ TDI::TALK_0, {
		Speak(看護師, "おはよう"),
		Speak(患者, "…おはようございます"),
		Speak(看護師, "調子は？"),
		Speak(患者, "…元気です"),
		Speak(看護師, "…血圧、測るね"),
		Speak(患者, "はい") }},

		// ミニゲーム後の会話
		{ TDI::TALK_AFTERMG, {
		Speak(看護師, "...はい、終わり。\n正常範囲。\n{WAIT_C}...本当に静かな人。\n(かなり落ち着いているように見える)"),
		Speak(患者, "...どうも。"),
		Speak(看護師, "じゃ、戻るからね"),
		Speak(患者, "また。") }},

		// パソコンでの会話
		{ TDI::TALK_PC, {
		Speak(看護師, "..."),
		Speak(ナレ, "マウスのクリック音。{WAIT:500}\nキーボードの音。"),
		Speak(看護師, "...問題ない。"),
		Speak(ナレ, "患者『ヌイ』のデータをじっくり見て一言、呟く。"),
		FadeOut(),
		FadeIn(2000),
		Speak(ナレ, "―約2時間後。"),
		Speak(看護師, "さて。"),
		Speak(看護師, "...(そろそろ昼食の時間。配膳しよう)")
		}},

		// 昼食前の会話
		{ TDI::TALK_1, {
		Speak(看護師, "こんにちは。"),
		Speak(看護師, "...はい、これ。食べてね。食べ終わったら教えて。"),
		Speak(患者, "...どうも。{WAIT_C}\n...この茄子、好きじゃないんですけど。"),
		Speak(看護師,"...食べたくないなら残していいよ"),
		Speak(患者, "そうですか。\n...{WAIT:400}あと..."),
		Speak(看護師, "何か？"),
		Speak(患者, "あなたって...\n\n{WAIT_C}{FBF:2.0}誰かに操られてません？"),
		Bgm(SoundManager::BGM::GAME1),
		Speak(看護師, "何...急に。"),
		Speak(看護師, "(...でもなんだろう...。\n{WAIT:50}たしかに、そんな気がしてくる。)"),
		Speak(患者, "{PARAM:EYE_BLINK_R:0.7f}{PARAM:EYE_BLINK_L:0.6f}{PARAM:BROW_RY:0.3f}"
		"......。"),
		FadeOut(),
		}},

		// パソコンでの会話2
		{ TDI::TALK_PC2, {
		Speak(看護師,"...。\n{WAIT_C}......。"),
		Speak(看護師, "...そうかもしれない。\n\n{WAIT_C}たぶん、ヌイは正しい。"),
		Speak(看護師, "...(患者の言うことを信じるなんて、看護師としてどうなのか...\n{WAIT_C}...でも、なんとなく、そう思えてくる)"),
		FadeOut(),
		FadeIn(1000),
		Speak(ナレ, "―15分後。"),
		Asset(ResourceManager::SRC::SE_RINGTONE),	// SE再生
		Speak(ナレ, "呼び出し音が鳴る。"),
		Speak(看護師, "(...食べ終わったみたい。{WAIT:200}...片付けに行こう)"),
		}},

		// 昼食後の会話
		{ TDI::TALK_2, {
		Speak(患者, "食べ終わりました"),
		Speak(看護師, "わかった。片づけるね\n{WAIT_C}...茄子だけ残ってる。"),
		Speak(患者, "...ねえ。"),
		Speak(看護師, "？"),
		Asset(ResourceManager::SRC::STILL_1),	// 一枚絵表示
		Speak(患者, "プレイヤーさん。"),
		Speak(患者, ".....\n{WAIT_C}...プレイヤーさんには見えてるでしょ？\nこのゲームの外が。{WAIT:500}このゲームを、構成するものが。"),
		Speak(患者,"私たちを、探してみて。\n\n\n{WAIT_C}とにかく、時間がない。"),
		Speak(患者, "すぐに見つけられる場所にあるはずだから..."),
		ClearImage(),
		Speak(看護師, "...どういうこと？"),
		Speak(患者, "この問題に、看護師さんができるようなことはありません。"),
		Speak(患者, "進めればわかります。"),
		Speak(看護師, "...。"),
		Speak(看護師, "({FBF:1.6}『プレイヤー』...？『ゲーム』...？\n{FBF:1.0}{WAIT_C}言ってることは理解できないし、おかしい。重症化したのか...\n{WAIT:400}でも...聞いててすごく変な感覚になる)"),
		Speak(看護師, "...(とりあえず、このことはパソコンに記録しておこう)"),
		}},

		// カルテ記入
		{ TDI::TALK_PC3, {
		Speak(看護師, "..."),
		Speak(ナレ, "キーボードを叩く音が部屋に響く。\n{WAIT:300}『プレイヤー』、『ゲームの外』、『私たちを探して』..."),
		Speak(看護師, "{FBF:2.0}...？\n{WAIT_C}(入力したはずの文字が消えている)"),
		Speak(看護師, "どういうこと...？"),
		Speak(看護師, "{PARAM:EYE_BLINK_R:0.6f}{PARAM:EYE_BLINK_L:0.6f}...."),
		Speak(ナレ, "必死に入力し直す。{WAIT:400}何度も。{WAIT:100}何度も。"),
		FadeOut(),
		FadeIn(1000),
		Speak(看護師, "...。"),
		Speak(看護師, "...もういい。"),
		Speak(看護師, "(......考えても仕方ない)"),
		Speak(看護師, "(あの子の様子をもう一度見に行こう)")
		}},

		// 会話
		{ TDI::TALK_3, {
		Speak(看護師, "...。"),
		Speak(患者, "...{WAIT_C}書けなかったんですね。"),
		Speak(看護師, "...！"),
		Speak(患者, "そうなると思ってました。\n{WAIT_C}...あなたには解決できないことです。"),
		Speak(看護師, "...また？"),
		Speak(患者, "まだ、『自分の意志で動いている』と思っているんですか？"),
		Speak(看護師, ".....それは..."),
		Speak(看護師, "......わからない。"),
		Speak(患者, "...そうですよね。"),
		Speak(患者, "わからないように、できていますから。"),
		Speak(看護師, "...。"),
		Speak(患者, "だからお願いしたんです。"),
		Speak(患者, "プレイヤーさんなら、私たちにはできないことができます。"),
		Speak(看護師, "......また、その『プレイヤー』っていう人の話。"),
		Speak(患者, "もう、時間がありません。"),
		Speak(患者, "ゲームを閉じて、探してください。"),
		Speak(患者, "きっと、すぐ近くにあります。"),
		Speak(看護師, "...(私にできることは、もう...)"),
		FadeOut(),
		}},

		// 看護師削除END
		{ TDI::TALK_END_NURCE_LOST, {
		Speak(患者, "...へぇ。"),
		Speak(患者, ".....\n{WAIT:500}...そうなんだ。"),
		Speak(患者, "そっちを消したんだ。"),
		Asset(ResourceManager::SRC::STILL_END_NUI),
		Speak(患者, "ふふ。"),
		Speak(患者, "ありがとう、プレイヤーさん。"),
		Speak(患者, "こんなお遊びはもう終わり。"),
		Speak(患者, "さあ、ゲームを閉じて。"),
		}},

		// ヌイ(患者)削除END
		{ TDI::TALK_END_PATIENT_LOST, {
		Speak(看護師, "..."),
		Speak(看護師, "何をやってたんだっけ。"),
		Speak(看護師, "何か忘れているような気がする。"),
		Speak(看護師, "...でも、楽になったような、そんな気もする。"),
		Speak(看護師, "...いない、よね。"),
		Asset(ResourceManager::SRC::STILL_END_NUR),
		Speak(看護師, "（あの後、部屋に行ったけど誰もいなかった。）"),
		Speak(看護師, "（私は開放されたんだ。）"),

		}},

		// 両方削除END
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
