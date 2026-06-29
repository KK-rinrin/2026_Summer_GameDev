#pragma once

#include "../../Manager/ResourceManager.h"
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

	// 一枚絵を表示する。画像はLive2Dより手前、会話ウィンドウより奥に描画する。
	struct ImageEvent
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

	// 会話は発話・一枚絵・フェードを同じ列に並べたイベント列として扱う。
	using TalkEvent = std::variant<SpeakEvent, ImageEvent, ClearImageEvent, FadeOutEvent, FadeInEvent>;
	
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

		CHECK_PC,	// イベント進行時以外でPCを調べたとき

	};

	// TalkData.cpp を読みやすくするためのイベント生成ヘルパー。
	static TalkEvent Speak(Speaker speaker, const std::string& talk, float advanceTime = 1.0f);
	static TalkEvent Image(ResourceManager::SRC src);
	static TalkEvent ClearImage(int fadeMs = 300);
	static TalkEvent FadeOut();
	static TalkEvent FadeIn(int waitMs = 0);

	static const std::vector<TalkEvent>& GetTalkData(TalkDataIndex dataIndex);
};

using TD = TalkDatas::TalkEvent;
using TDI = TalkDatas::TalkDataIndex;
