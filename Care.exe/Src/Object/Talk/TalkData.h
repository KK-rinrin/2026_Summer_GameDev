#pragma once

#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"
#include <string>
#include <variant>
#include <vector>

class TalkDatas
{
public:

	// 既定の文字送り時間倍率
	static constexpr float DEFAULT_ADVANCE_TIME = 1.0f;

	// 一枚絵消去時の既定フェード時間のミリ秒数
	static constexpr int DEFAULT_CLEAR_IMAGE_FADE_MS = 300;

	// 会話の話者識別子
	enum class Speaker
	{
		NONE = -1,	// 話者の指定なし
		Player,		// 操作キャラクターの話者
		Patient,	// 患者キャラクターの話者
		Narator,	// 地の文の話者
	};

	// 1行分の通常発話。文字送り・クリック待ち・Live2D口パクの対象になる。
	struct SpeakEvent
	{

		// 発話する話者
		Speaker speaker = Speaker::Player;

		// 発話する本文
		std::string talk;

		// 1文字当たりの表示時間に掛ける倍率
		float advanceTime = DEFAULT_ADVANCE_TIME;
	};

	// 素材イベント。画像なら表示、音なら再生する
	struct AssetEvent
	{

		// 表示または再生するリソースID
		ResourceManager::SRC src;
	};

	// 表示中の一枚絵を消す。クリック待ちはせず、指定時間で軽くフェードアウトする。
	struct ClearImageEvent
	{

		// 一枚絵を消去するフェード時間のミリ秒数
		int fadeMs = DEFAULT_CLEAR_IMAGE_FADE_MS;
	};

	// シーン遷移はせず、Talk内だけで暗転する。入力待ちはしない。
	struct FadeOutEvent
	{
	};

	// 直前のFadeOut完了から waitMs 待ってから明転する。
	struct FadeInEvent
	{

		// 暗転完了から明転開始までの待機時間のミリ秒数
		int waitMs = 0;
	};

	// BGM切り替えイベント
	struct BgmEvent
	{

		// 切り替え先のBGM
		SoundManager::BGM bgm = SoundManager::BGM::GAME0;
	};

	// 会話は発話・素材・フェードを同じ列に並べたイベント列として扱う。
	using TalkEvent = std::variant<SpeakEvent, AssetEvent, ClearImageEvent, FadeOutEvent, FadeInEvent, BgmEvent>;

	// 会話イベント列の識別子
	enum class TalkDataIndex
	{
		NONE = -1,						// 指定なし

		TALK_INIT,						// 一番最初
		TALK_0,							// 血圧測定前の会話
		TALK_0_RETRY,					// 血圧測定しなおし
		TALK_AFTERMG,					// ミニゲーム後の会話
		TALK_PC,						// パソコンでの会話
		TALK_GET_PLATE = 12,			// 昼食の配膳プレート取得
		TALK_1 = 5,						// 昼食前の会話
		TALK_PC2,						// パソコンでの会話2
		TALK_2,							// 昼食後の会話
		TALK_PC3,						// カルテ記入
		TALK_3,							// ヌイとの会話
		TALK_PC4,						// パソコンでの会話4
		TALK_DINNER,					// 夕食前の会話
		TALK_GET_PLATE_FOLDER,			// 夕食の配膳プレート取得
		TALK_END_NURSE_LOST = 901,		// 看護師削除時の患者生存
		TALK_END_PATIENT_LOST = 902,	// 患者削除時の看護師生存
		TALK_END_BOTH_LOST = 903,		// 両方削除
		TALK_END_RUINED = 904,			// 崩壊END

		TALK_0_FAIL = 102,				// 血圧測定失敗時の会話

		HIDE_TALK = 666,				// 隠し会話イベント
	};

	// 可読性のためのイベント生成ヘルパー
	// 会話
	static TalkEvent Speak(Speaker speaker, const std::string& talk, float advanceTime = DEFAULT_ADVANCE_TIME);
	// 素材を表示・再生
	static TalkEvent Asset(ResourceManager::SRC src);
	// BGMを再生
	static TalkEvent Bgm(SoundManager::BGM bgm);
	// 一枚絵を消す
	static TalkEvent ClearImage(int fadeMs = DEFAULT_CLEAR_IMAGE_FADE_MS);
	// 暗転
	static TalkEvent FadeOut();
	// 明転
	static TalkEvent FadeIn(int waitMs = 0);

	// 識別子に対応する会話イベント列の取得
	static const std::vector<TalkEvent>& GetTalkData(TalkDataIndex dataIndex);

};

using TD = TalkDatas::TalkEvent;
using TDI = TalkDatas::TalkDataIndex;
