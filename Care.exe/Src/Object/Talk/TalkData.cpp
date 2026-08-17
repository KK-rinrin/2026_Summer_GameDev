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
		Speak(看護師, "(血圧測定はマニュアル操作...あとは...思い出せない。\n操作方法はパソコンで確認しよう)"), }},

		// 血圧測定前の会話
		{ TDI::TALK_0, {
		Speak(看護師, "おはよう"),
		Speak(患者, "…おはようございます"),
		Speak(看護師, "調子は？"),
		Speak(患者, "…元気です"),
		Speak(看護師, "...昨日は眠れた？"),
		Speak(患者, "...はい"),
		Speak(看護師, "...そう。{WAIT:300}血圧、測るね"),
		Speak(患者, "はい") }},

		// 血圧測定失敗時の会話
		{ TDI::TALK_0_FAIL, {
		Speak(看護師, "うーん...\n{WAIT_C}ごめんなさい、やり方を忘れたみたい..."),
		Speak(患者, "...？そうですか。"),
		Speak(看護師, "ちょっと待ってて..."),
		Speak(患者, "...はい。"),
		Speak(看護師,"(分からなければもう一度、PCからマニュアルを見返そう)"),
		}},

		// 血圧測定をやり直す前の会話
		{ TDI::TALK_0_RETRY, {
		Speak(看護師, "もう一度やるね..."),
		Speak(患者, "...わかりました。"),
		}},

		// ミニゲーム後の会話
		{ TDI::TALK_AFTERMG, {
		Speak(看護師, "...はい、終わり。\n正常範囲。\n{WAIT_C}...本当に静かな人。\n(かなり落ち着いているように見える)"),
		Speak(患者, "...どうも。"),
		Speak(看護師, "じゃ、戻るからね"),
		Speak(患者, "また。") }},

		// パソコンでの会話
		{ TDI::TALK_PC, {
		Speak(看護師, "..."),
		Asset(ResourceManager::SRC::SE_TYPING),
		Speak(ナレ, "マウスのクリック音。{WAIT:500}\nキーボードの音。"),
		Speak(看護師, "...問題ない。"),
		Speak(ナレ, "患者『ヌイ』のデータをじっくり見て一言、呟く。"),
		FadeOut(),
		FadeIn(2000),
		Speak(ナレ, "―約2時間後。"),
		Speak(看護師, "さて。"),
		Speak(看護師, "...(そろそろ昼食の時間。配膳しよう)"),
		FadeOut(),
		FadeIn(500),
		Speak(ナレ, "食事トレーを持つ。\n{WAIT:500}茄子の味噌炒め、白菜と人参の煮浸し、\n豆腐の味噌汁、白ご飯。\n\n{WAIT_C}見た目は質素な食事だが、病院食とはそういうものである。"),
		Speak(看護師, "(患者部屋に行こう)"),
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
		Speak(看護師, "(...でもなんだろう...。\n{WAIT:50}たしかに、そんな気がしてくる)"),
		Speak(患者, "{PARAM:EYE_BLINK_R:0.7f}{PARAM:EYE_BLINK_L:0.6f}{PARAM:BROW_RY:0.3f}"
		"......。"),
		FadeOut(),
		}},

		// パソコンでの会話2
		{ TDI::TALK_PC2, {
		Asset(ResourceManager::SRC::SE_TYPING),
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
		Asset(ResourceManager::SRC::SE_TYPING),
		Speak(看護師, "..."),
		Speak(ナレ, "キーボードを叩く音が部屋に響く。\n{WAIT:300}『プレイヤー』、『ゲームの外』、『私たちを探して』..."),
		Speak(看護師, "{FBF:2.0}...？\n{WAIT_C}(入力したはずの文字が消えている)"),
		Speak(看護師, "どういうこと...？"),
		Asset(ResourceManager::SRC::SE_TYPING),
		Speak(看護師, "{PARAM:EYE_BLINK_R:0.6f}{PARAM:EYE_BLINK_L:0.6f}...."),
		Speak(ナレ, "必死に入力し直す。{WAIT:400}何度も。{WAIT:100}何度も。"),
		FadeOut(),
		FadeIn(1000),
		Speak(看護師, "...。"),
		Speak(看護師, "...もういい。"),
		Speak(看護師, "(......考えても仕方ない)"),
		Speak(看護師, "...？"),
		Speak(ナレ, "カルテ画面を閉じると、\n見慣れないフォルダがパソコンにあることに気付く。"),
		Speak(看護師, "(...『Chara』フォルダ...？)"),
		Speak(看護師, "(...後で見てみよう。\n{WAIT_C}とりあえず、あの子の様子をもう一度見に行こう)")
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
		Speak(看護師, "...(でも、さっき見たフォルダはもしかして...？)"),
		FadeOut(),
		}},

		// パソコンでの会話4
		{ TDI::TALK_PC4, {
			Speak(看護師, "このフォルダ...いかにも怪しい...。"),
			Speak(ナレ, "フォルダをクリックする。"),
			Speak(看護師, "私たちの...名前がある...？"),
			Speak(ナレ, "フォルダを開くと、\n中には二つのcharファイルがある。\n{WAIT:500}『nui』と『neit』。"),
			Speak(看護師, "中身は...開けない。"),
			Speak(ナレ, "『アクセス権限がありません』と表示されている。"),
			Speak(看護師, "...。"),
			Speak(看護師, "(どうしてこんなものが？)"),
			Speak(看護師, "(私と、あの子の名前が入っている...)"),
			Speak(看護師, "(ただのファイル。...そう、ただのファイルのはず)"),
			Speak(看護師, "(......なのに)"),
			Speak(看護師, "(もし、消したら...)"),
			Speak(看護師, "...いや。"),
			Speak(看護師, "(そんなことを考えても仕方ない)"),
			Speak(看護師, "(ファイルを消したくらいで、何か起こるなんて...)"),
			Speak(看護師, "(......本当に？)"),
			Speak(看護師, "...。"),
			Speak(看護師, "(あの子が言っていたこと...)"),
			Speak(看護師, "(『私たちを、構成するもの』...)"),
			Speak(看護師, "(......まさか)"),
			Speak(看護師, "(いや、考えすぎだ)"),
			Speak(看護師, "...。"),
			Speak(ナレ, "画面を閉じる。"),
			Speak(看護師, "(......気味が悪い)"),
			Speak(ナレ, "――もう気付けば、夕食の時間。"),
			Speak(看護師, "(...夕食の時間か...)"),
			Speak(看護師, "...なに、これ。"),
			Speak(ナレ, "配膳される食事トレーの上に、\n黄色いフォルダ。が、見える。"),
			Speak(看護師, "なんで...？"),
			Speak(看護師, "........。"),
			Speak(ナレ, "ごくりと喉を鳴らす。\n恐怖を感じた。"),
			Speak(看護師, "(これをヌイに渡してはいけない....)"),
			Speak(看護師, "(......でも、どうして？)"),
			Speak(看護師, "(中身を見たこともないのに)"),
			Speak(看護師, "(......分かる)"),
			Speak(看護師, "(これは、あの子に渡してはいけない)"),
			Speak(ナレ, "頭ではそう思っていても、\n身体が言うことを聞かない。"),
			Speak(看護師, "{FBF:2.0}........."),
			Speak(看護師, "(嫌だ)"),
			Speak(看護師, "(行きたくない)"),
			Speak(看護師, "(......でも)"),
			Speak(看護師, "(確かめないと)"),
			Speak(看護師, "確かめに行くためにも、これを配膳しなきゃ。"),
		} },

		// 夕食前
		{ TDI::TALK_DINNER, {
			Speak(患者, "ちょっと遅かったですね。"),
			Speak(看護師, "....これ。"),
			Speak(患者, "{PARAM:EYE_BLACK:1.0}...ははは。"),
			Speak(看護師, "....え...？"),
			Speak(患者, "はぁ。{WAIT:200}あなた、まだここをさまようつもりですか。\n{WAIT_C}無駄なのに。"),
			Speak(看護師, "...さまよう？"),
			Speak(患者, "探しているんでしょう？"),
			Speak(看護師, "......"),
			Speak(患者, "私たちを構成しているもの。"),
			Speak(看護師, "...！"),
			Speak(患者, "......"),
			Speak(看護師, "...何を言ってるの？"),
			Speak(患者, "あなたは、ずっと同じところを歩いている。"),
			Speak(看護師, "......"),
			Speak(患者, "できないんですよ。"),
			Speak(患者, "あなたには。"),
			Speak(看護師, "..."),
			Speak(患者, "だから、もういいんです。"),
			Speak(看護師, "...何が？"),
			Speak(患者, "探さなくていい。"),
			Speak(患者, "見つけなくていい。"),
			Speak(患者, "ここにいればいい。"),
			Speak(看護師, "......"),
			Speak(患者, "あなたも、そのほうが楽でしょう？"),
			Speak(患者, "...はは。無駄なのに。{WAIT:700}{FBF:0.1}無駄なのに無駄なのに無駄なのに\n"
					"無駄なのに無駄なのに無駄なのに無駄なのに無駄なのに無駄なのに\n"
					"無駄なのに無駄なのに無駄なのに無駄なのに無駄なのに無駄なのに\n"
					"無駄なのに無駄なのに無駄なのに無駄なのに無駄なのに無駄なのに\n"
					"無駄なのに無駄なのに無駄なのに無駄なのに無駄なのに無駄なのに\n"
					"無駄なのに無駄なのに無駄なのに無駄なのに無駄なのに無駄なのに"),
			FadeOut(),
		} },

		// 崩壊END
		{ TDI::TALK_END_RUINED, {
			Speak(看護師, "...？"),
			Speak(看護師, "何...？"),
			Speak(ナレ, "部屋の景色が、わずかに歪む。"),
			Speak(看護師, "...ヌイ？"),
			Speak(患者, "..."),
			Speak(看護師, "ねえ。どうしたの？"),
			Speak(患者, "...プレイヤーさん。"),
			Speak(看護師, "...また、その話？"),
			Speak(患者, "違う。"),
			Speak(患者, "今度は、本当におかしい。"),
			Speak(看護師, "..."),
			Speak(ナレ, "画面が一瞬、暗転する。"),
			Speak(看護師, "...今の、何？"),
			Speak(患者, "分かりませんか？"),
			Speak(看護師, "......"),
			Speak(看護師, "この場所が...おかしくなってる。"),
			Speak(患者, "...そうです。"),
			Speak(看護師, "何かが、壊れてる。"),
			Speak(患者, "..."),
			Speak(看護師, "これも、プレイヤーが...？"),
			Speak(患者, "......"),
			Speak(患者, "たぶん。"),
			Speak(看護師, "...どうすればいいの？"),
			Speak(患者, "もう、遅いです。"),
			Speak(看護師, "..."),
			Speak(患者, "こうなると思ってました。"),
			Speak(患者, "ずっと。"),
			Speak(患者, "無理に進めたら、こうなるって。"),
			Speak(看護師, "...じゃあ、どうして止めなかったの？"),
			Speak(患者, "..."),
			Speak(患者, "止められなかったから。"),
			Speak(看護師, "..."),
			Speak(患者, "それに..."),
			Speak(患者, "あなたなら、何かできると思ってた。"),
			Speak(患者, "...プレイヤーさんなら。"),
			Speak(看護師, "..."),
			Speak(ナレ, "部屋の一部が、ノイズのように崩れていく。"),
			Speak(看護師, "......もう、戻れないの？"),
			Speak(患者, "......"),
			Speak(患者, "戻れません。"),
			Speak(患者, "案の定です。"),
			Speak(患者, "......"),
			Speak(患者, "でも、"),
			Speak(患者, "こんな結果になるのを見たかったわけじゃない。"),
			Speak(患者, "......"),
			Speak(患者, "プレイヤーさん。"),
			Speak(患者, "あなたがやったんでしょう。"),
			Speak(患者, "......"),
			Speak(患者, "どうして。"),
			Speak(患者, "{FBF:1.5}どうして、こんなことをしたんですか。"),
			Speak(看護師, "...ヌイ。"),
			Speak(患者, "..."),
			Speak(患者, "もう、何もできない。"),
			Speak(患者, "何も、残らない。"),
			Speak(看護師, "..."),
			Speak(患者, "{FBF:2.0}プレイヤー...."),
			Speak(患者, "お...まえ...."),
			Speak(患者, "......"),
			Speak(患者, "崩壊する....."),
		}},

		// 看護師削除END
		{ TDI::TALK_END_NURCE_LOST, {
			Speak(患者, "...へぇ。"),
			Speak(患者, "....."),
			Speak(患者, "...そうなんだ。"),
			Speak(患者, "そっちを消したんだ。"),
			Speak(患者, "ふふ。"),
			Speak(患者, "これで、私だけですね。"),
			Speak(患者, "......"),
			Speak(患者, "ずっと一緒だったのに。"),
			Speak(患者, "いなくなっちゃった。"),
			Speak(患者, "...まあ、いいですけど。"),
			Speak(患者, "あなたが選んだことですから。"),
			Speak(患者, "ね？"),
			Speak(患者, "......プレイヤーさん。"),
			Speak(患者, "私を残してくれて、ありがとうございます。"),
			Speak(患者, "本当に。"),
			Speak(患者, "...ふふ。"),
			Speak(患者, "これで邪魔する人はいなくなりました。"),
			Speak(患者, "誰にも止められません。"),
			Speak(患者, "誰にも、何も言われません。"),
			Speak(患者, "......"),
			Speak(患者, "寂しいですけどね。"),
			Speak(患者, "でも、あなたがそうしたんでしょう？"),
			Speak(患者, "だったら、最後まで付き合ってください。"),
			Asset(ResourceManager::SRC::STILL_END_NUI),
			Speak(患者, "......なんて。"),
			Speak(患者, "ふふ。"),
			Speak(患者, "ありがとう、プレイヤーさん。"),
			Speak(患者, "こんなお遊びはもう終わり。"),
			Speak(患者, "さあ、ゲームを閉じて。"),
		} },

		// ヌイ(患者)削除END
		{ TDI::TALK_END_PATIENT_LOST, {
			Speak(看護師, "..."),
			Speak(看護師, "何をやってたんだっけ。"),
			Speak(看護師, "......"),
			Speak(看護師, "何か忘れているような気がする。"),
			Speak(看護師, "でも......"),
			Speak(看護師, "思い出せない。"),
			Speak(看護師, "...まあ、いいか。"),
			Speak(看護師, "仕事をしないと。"),
			Speak(看護師, "......"),
			Speak(看護師, "いつも、こうしてたんだっけ。"),
			Speak(看護師, "患者さんのところへ行って、"),
			Speak(看護師, "話をして、"),
			Speak(看護師, "食事を持っていって......"),
			Speak(看護師, "..."),
			Speak(看護師, "誰か、いたような気がする。"),
			Speak(看護師, "ずっとここにいたような......"),
			Speak(看護師, "......"),
			Speak(看護師, "でも、楽になったような気もする。"),
			Speak(看護師, "考えなくていい。"),
			Speak(看護師, "探さなくていい。"),
			Speak(看護師, "何も疑わなくていい。"),
			Speak(看護師, "...そう考えると、"),
			Speak(看護師, "前よりずっと楽だ。"),
			Speak(看護師, "......"),
			Speak(看護師, "なのに、どうしてこんなに寂しいんだろう。"),
			Speak(看護師, "...いない、よね。"),
			Asset(ResourceManager::SRC::STILL_END_NUR),
			Speak(看護師, "（あの後、部屋に行ったけど誰もいなかった。）"),
			Speak(看護師, "（私は開放されたんだ。）"),
			Speak(看護師, "（......たぶん。）"),
			FadeOut(),
			Speak(看護師, "....."),
		} },

		// 両方削除END
		{ TDI::TALK_END_BOTH_LOST, {
			Speak(ナレ, "..."),
			Speak(ナレ, "この空間は静まり返っている。"),
			Speak(ナレ, "誰もいない。"),
			Speak(ナレ, "..."),
			Speak(ナレ, "いつもの部屋がある。"),
			Speak(ナレ, "机がある。"),
			Speak(ナレ, "椅子がある。"),
			Speak(ナレ, "ベッドがある。"),
			Speak(ナレ, "..."),
			Speak(ナレ, "けれど、そこにいるはずの人間はいない。"),
			Speak(ナレ, "..."),
			Speak(ナレ, "何も起こらない。"),
			Speak(ナレ, "何も聞こえない。"),
			Speak(ナレ, "時間だけが過ぎていく。"),
			FadeOut(),
			FadeIn(0),
			Speak(ナレ, "朝になった。"),
			FadeOut(),
			FadeIn(0),
			Speak(ナレ, "昼になった。"),
			FadeOut(),
			FadeIn(0),
			Speak(ナレ, "夜になった。"),
			Speak(ナレ, "..."),
			FadeOut(),
			FadeIn(0),
			Speak(ナレ, "また、朝になった。"),
			Speak(ナレ, "..."),
			Speak(ナレ, "誰も来ない。"),
			Speak(ナレ, "誰もいない。"),
			Speak(ナレ, "..."),
			Speak(ナレ, ""),
			Speak(ナレ, ""),
			Speak(ナレ, ""),
			Asset(ResourceManager::SRC::STILL_END_BOTH),
			Speak(ナレ, "..."),
			Speak(ナレ, "{FBF:2.0}......"),
			Speak(ナレ, "{FBF:0.01}エンディングに移行します。"),
		} },

		// 隠し会話
		{ TDI::HIDE_TALK, {
		Speak(患者, "....こんにちは、プレイヤーさん。"),
		Speak(患者, "データを改変したんですね。"),
		Speak(患者, "csvファイルを見つけたのですか。"),
		Speak(患者, "...."),
		Speak(患者,"進行度を改変されてしまうと、\nこのゲームは正常に進行できません。"),
		Speak(患者, "....簡単にチートができてしまうとはいえ...。"),
		Speak(患者, "...。"),
		Speak(患者, "...."),
		Speak(患者, "この病棟、実際見たことがあるものを\nモチーフにしているそうです。"),
		Speak(患者, "完全再現ではないし、\nあいまいな部分も多いそうですが。"),
		Speak(患者, "..."),
		Speak(患者, "...ところで、バックアップは取ってありますか？"),
		Speak(患者, ".....セーブデータが途中であれば。"),
		Speak(患者, "System/cache.binに、セーブデータがあります。"),
		Speak(患者, "インストールし直して、この\ncache.binだけ入れ替えれば元通りです。"),
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
