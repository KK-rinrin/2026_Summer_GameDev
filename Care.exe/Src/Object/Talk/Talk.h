#pragma once

#include "TalkData.h"
#include <DxLib.h>
#include "window/Live2DTalkController.h"
#include <memory>
#include <string>
#include <vector>

class Fader;
class TalkWindow;

class Talk
{
	static constexpr float EXTEND_RATE = 2.0f;
	static constexpr VECTOR PATIENT_POS = { 230.0f, -100.0f, 0.0f };
	static constexpr VECTOR PLAYER_POS = { -230.0f, -100.0f, 0.0f };

	static constexpr int FADE_ALPHA = 10;
	static constexpr int FINAL_FADE_IN_MS = 1000;

	// TalkEventの進行状態。入力待ちと自動進行をここで切り分ける。
	enum class EventState
	{
		NONE,
		SPEAK,			// TalkWindowの文字送り中、または発話後のクリック待ち
		IMAGE_WAIT,		// 一枚絵を表示し、クリックで次イベントへ進む
		CLEAR_IMAGE,	// 一枚絵を短くフェードアウトし、自動で次イベントへ進む
		FADING_OUT,		// Talk内Faderによる暗転中。入力待ちはしない
		FADE_IN_WAIT,	// 暗転完了から指定時間待ち
		FADING_IN,		// Talk内Faderによる明転中
	};

public:
	Talk();
	~Talk();

	void Load();
	bool Update();	// 会話中なら true を返す
	void Draw();
	void Delete();

	void SetTalk(TDI dataIndex);
	// 暗幕やLive2D描画を使わず、TalkWindowだけでその場限りの短い会話を再生する。
	// 文字列だけなら操作キャラのつぶやき、Speaker指定ありなら任意の話者として扱う。
	void SetTemporaryTalk(const std::string& talk, float advanceTime = 1.0f);
	void SetTemporaryTalk(TalkDatas::Speaker speaker, const std::string& talk, float advanceTime = 1.0f);
	TDI GetCurrentTDI() const { return currentDataIndex_; }
	bool IsActive() const { return talkData_ != nullptr || isTemporaryTalk_ || isFinishing_; }
	bool ConsumeTalkEnd(TDI dataIndex);

private:
	void UpdateModels();
	void UpdateTemporaryTalk();
	void UpdateEvent();
	void StartCurrentEvent();
	void StartEvent(const TalkDatas::SpeakEvent& eventData);
	void StartEvent(const TalkDatas::AssetEvent& eventData);
	void StartEvent(const TalkDatas::ClearImageEvent& eventData);
	void StartEvent(const TalkDatas::FadeOutEvent& eventData);
	void StartEvent(const TalkDatas::FadeInEvent& eventData);
	void AdvanceEvent();
	void FinishTalkEvents();
	void StartFinalFadeIn();
	void CompleteTalkEnd();
	bool IsLastEventFadeOut() const;
	void RefreshVisibleSpeakers(const std::vector<TD>& talkData);
	bool IsDecideTriggered();
	long long GetElapsedMs(long long startTime) const;
	void DrawCharacters();
	void DrawImage();
	void DrawFinalFadeIn();

private:
	// Live2DTalkController をハブ経由で共有するため shared_ptr に変更
	std::shared_ptr<Live2DTalkController> patientController_;
	std::shared_ptr<Live2DTalkController> playerController_;

	// 可視フラグ。発話イベントを走査して、登場するキャラだけ表示する。
	bool PatientVisible_ = true;
	bool PlayerVisible_ = true;

	// TalkWindowは単行発話、FaderはTalk内だけの暗転・明転を担当する。
	std::unique_ptr<TalkWindow> talkWindow_;
	std::unique_ptr<Fader> fader_;

	// 会話イベント列の現在位置。
	TalkDatas::TalkDataIndex currentDataIndex_ = TDI::NONE;
	const std::vector<TD>* talkData_ = nullptr;
	size_t eventIndex_ = 0;
	EventState eventState_ = EventState::NONE;
	bool isEventStarted_ = false;
	bool isTemporaryTalk_ = false;
	bool isTalkEnd_ = false;
	bool isFinishing_ = false;
	bool isFinalFadeIn_ = false;

	// 一枚絵レイヤー。Live2Dの上、会話ウィンドウの下に描画する。
	int imageHandle_ = -1;
	int imageAlpha_ = 255;
	long long clearImageStartTime_ = 0;
	int clearImageFadeMs_ = 300;

	// FadeInは直前のFadeOut完了時刻から待ち時間を数える。
	long long fadeOutEndTime_ = 0;
	int fadeInWaitMs_ = 0;
	long long finalFadeInStartTime_ = 0;
	int finalFadeInAlpha_ = 255;

	// TalkWindowから移した入力エッジ検出と、会話終了時の薄いフェード用透明度。
	bool prevMouseLeftDown_ = false;
	int alpha_;
};
