#pragma once

#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"
#include <string>
#include <variant>
#include <vector>

class TalkDatas
{
public:
	enum class Speaker
	{
		NONE = -1,
		Player,
		Patient,
		Narator,
	};

	// 1行分の通常発話。文字送り・クリック待ち・Live2D口パクの対象になる。
	struct SpeakEvent
	{
		Speaker speaker = Speaker::Player;
		std::string talk;
		float advanceTime = 1.0f;
	};

	// 素材イベント。画像なら表示、音なら再生する
	struct AssetEvent
	{
		ResourceManager::SRC src;
	};

	// 表示中の一枚絵を消す。クリック待ちはせず、指定時間で軽くフェードアウトする。
	struct ClearImageEvent
	{
		int fadeMs = 300;
	};

	// シーン遷移はせず、Talk内だけで暗転する。入力待ちはしない。
	struct FadeOutEvent
	{
	};

	// 直前のFadeOut完了から waitMs 待ってから明転する。
	struct FadeInEvent
	{
		int waitMs = 0;
	};

	struct BgmEvent
	{
		SoundManager::BGM bgm = SoundManager::BGM::GAME0;
	};

	// 会話は発話・素材・フェードを同じ列に並べたイベント列として扱う。
	using TalkEvent = std::variant<SpeakEvent, AssetEvent, ClearImageEvent, FadeOutEvent, FadeInEvent, BgmEvent>;

	enum class TalkDataIndex
	{
		NONE = -1,
		TALK_INIT,		// 一番最初
		TALK_0,			// 血圧測定前の会話
		TALK_AFTERMG,	// ミニゲーム後の会話
		TALK_PC,		// パソコンでの会話
		TALK_1,			// 昼食前の会話
		TALK_PC2,		// パソコンでの会話2
		TALK_2,			// 昼食後の会話
		TALK_PC3,		// カルテ記入

		TALK_3,			// ヌイとの会話（変更するかも）

		TALK_END_NURCE_LOST,
		TALK_END_PATIENT_LOST,
		TALK_END_BOTH_LOST,


	};

	// 可読性のためのイベント生成ヘルパー
	// 会話
	static TalkEvent Speak(Speaker speaker, const std::string& talk, float advanceTime = 1.0f);
	// 素材を表示・再生
	static TalkEvent Asset(ResourceManager::SRC src);
	// BGMを再生
	static TalkEvent Bgm(SoundManager::BGM bgm);
	// 一枚絵を消す
	static TalkEvent ClearImage(int fadeMs = 300);
	// 暗転
	static TalkEvent FadeOut();
	// 明転
	static TalkEvent FadeIn(int waitMs = 0);

	static const std::vector<TalkEvent>& GetTalkData(TalkDataIndex dataIndex);
};

using TD = TalkDatas::TalkEvent;
using TDI = TalkDatas::TalkDataIndex;
