#pragma once
#include "../../Common/Vector2.h"
#include "../../Application.h"

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

	// 会話中のLive2Dモデルの表示倍率
	static constexpr float EXTEND_RATE = 2.0f;

	// 患者Live2Dモデルの表示位置
	static constexpr Vector2<float> PATIENT_POS = { 230.0f, -100.0f };

	// 操作キャラLive2Dモデルの表示位置
	static constexpr Vector2<float> PLAYER_POS = { -230.0f, -100.0f };

	// 会話終了時の暗幕のアルファ減少量
	static constexpr int FADE_ALPHA = 10;

	// 会話終了時の最終明転時間のミリ秒数
	static constexpr int FINAL_FADE_IN_MS = 1000;

	// TalkEventの進行状態。入力待ちと自動進行をここで切り分ける。
	enum class EventState
	{
		NONE,			// 会話イベントの待機状態
		SPEAK,			// TalkWindowの文字送り中または発話後のクリック待ち
		IMAGE_WAIT,		// 一枚絵表示中のクリック待ち
		CLEAR_IMAGE,	// 一枚絵の短いフェードアウトと次イベントへの自動進行
		FADING_OUT,		// 入力待ちを伴わないTalk内Faderによる暗転中
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

	// 指定識別子の会話イベント列の設定
	void SetTalk(TDI dataIndex);
	// 暗幕やLive2D描画を使わず、TalkWindowだけでその場限りの短い会話を再生する。
	// 文字列だけなら操作キャラのつぶやき、Speaker指定ありなら任意の話者として扱う。
	void SetTemporaryTalk(const std::string& talk, float advanceTime = TalkDatas::DEFAULT_ADVANCE_TIME);

	// 話者を指定した一時会話の設定
	void SetTemporaryTalk(TalkDatas::Speaker speaker, const std::string& talk, float advanceTime = TalkDatas::DEFAULT_ADVANCE_TIME);

	// 現在の会話イベント列の識別子の取得（未使用）
	TDI GetCurrentTDI() const { return currentDataIndex_; }

	// 会話または終了演出の実行中判定
	bool IsActive() const { return talkData_ != nullptr || isTemporaryTalk_ || isFinishing_; }

	// 指定会話の終了通知の取得と消費
	bool ConsumeTalkEnd(TDI dataIndex);

private:

	// 暗幕に使用する色
	static constexpr int FADE_COLOR = Application::DEFAULT_COLOR_0;

	// 完全不透明時のアルファ値
	static constexpr int MAX_ALPHA = 255;

	// 会話中に重ねる暗幕のアルファ値
	static constexpr int OVERLAY_ALPHA = 30;

	// マイクロ秒からミリ秒への換算除数
	static constexpr long long MICROSECONDS_PER_MILLISECOND = 1000;

	// 一枚絵消去フェードの最小ミリ秒数
	static constexpr int MIN_CLEAR_IMAGE_FADE_MS = 1;

	// 表示対象のLive2Dモデルの更新
	void UpdateModels();

	// 一時会話の進行と終了状態の更新
	void UpdateTemporaryTalk();

	// 現在の会話イベントの進行状態の更新
	void UpdateEvent();

	// 現在位置の会話イベントの開始
	void StartCurrentEvent();

	// 発話イベントの開始
	void StartEvent(const TalkDatas::SpeakEvent& eventData);

	// 画像表示または効果音再生イベントの開始
	void StartEvent(const TalkDatas::AssetEvent& eventData);

	// 一枚絵消去イベントの開始
	void StartEvent(const TalkDatas::ClearImageEvent& eventData);

	// 暗転イベントの開始
	void StartEvent(const TalkDatas::FadeOutEvent& eventData);

	// 明転イベントの開始
	void StartEvent(const TalkDatas::FadeInEvent& eventData);

	// BGM切り替えイベントの開始
	void StartEvent(const TalkDatas::BgmEvent& eventData);

	// 次の会話イベントへの進行
	void AdvanceEvent();

	// 会話イベント列の終了処理
	void FinishTalkEvents();

	// 会話終了時の最終明転の開始
	void StartFinalFadeIn();

	// 会話終了状態の確定
	void CompleteTalkEnd();

	// 末尾イベントが暗転かの判定
	bool IsLastEventFadeOut() const;

	// 会話イベント列に登場する話者の表示状態の更新
	void RefreshVisibleSpeakers(const std::vector<TD>& talkData);

	// 決定キーまたはマウスの押下開始判定
	bool IsDecideTriggered();

	// 指定開始時刻からの経過ミリ秒数の取得
	long long GetElapsedMs(long long startTime) const;

	// 表示対象のLive2Dキャラクターの描画
	void DrawCharacters();

	// 会話用一枚絵の描画
	void DrawImage();

	// 会話終了時の最終明転の描画
	void DrawFinalFadeIn();

private:
	// Live2DTalkController をハブ経由で共有するため shared_ptr に変更
	std::shared_ptr<Live2DTalkController> patientController_;

	// 操作キャラLive2D制御の共有ポインタ
	std::shared_ptr<Live2DTalkController> playerController_;

	// 可視フラグ。発話イベントを走査して、登場するキャラだけ表示する。
	bool PatientVisible_ = true;

	// 操作キャラの表示状態
	bool PlayerVisible_ = true;

	// TalkWindowは単行発話、FaderはTalk内だけの暗転・明転を担当する。
	std::unique_ptr<TalkWindow> talkWindow_;

	// 会話内の暗転と明転の制御
	std::unique_ptr<Fader> fader_;

	// 会話イベント列の現在位置。
	TalkDatas::TalkDataIndex currentDataIndex_ = TDI::NONE;

	// 再生中の会話イベント列への参照ポインタ
	const std::vector<TD>* talkData_ = nullptr;

	// 現在の会話イベントの添字
	size_t eventIndex_ = 0;

	// 現在の会話イベントの進行状態
	EventState eventState_ = EventState::NONE;

	// 現在のイベントの開始済み状態
	bool isEventStarted_ = false;

	// 一時会話の再生状態
	bool isTemporaryTalk_ = false;

	// 会話終了の未消費通知
	bool isTalkEnd_ = false;

	// 会話終了演出の実行状態
	bool isFinishing_ = false;

	// 最終明転演出の実行状態
	bool isFinalFadeIn_ = false;

	// 一枚絵レイヤー。Live2Dの上、会話ウィンドウの下に描画する。
	int imageHandle_ = -1;

	// 一枚絵の現在のアルファ値
	int imageAlpha_ = MAX_ALPHA;

	// 一枚絵消去フェードの開始時刻
	long long clearImageStartTime_ = 0;

	// 一枚絵消去フェード時間のミリ秒数
	int clearImageFadeMs_ = TalkDatas::DEFAULT_CLEAR_IMAGE_FADE_MS;

	// FadeInは直前のFadeOut完了時刻から待ち時間を数える。
	long long fadeOutEndTime_ = 0;

	// 明転開始までの待機時間のミリ秒数
	int fadeInWaitMs_ = 0;

	// 最終明転の開始時刻
	long long finalFadeInStartTime_ = 0;

	// 最終明転の現在のアルファ値
	int finalFadeInAlpha_ = MAX_ALPHA;

	// TalkWindowから移した入力エッジ検出と、会話終了時の薄いフェード用透明度。
	bool prevMouseLeftDown_ = false;

	// 会話終了時に薄くする暗幕のアルファ値
	int alpha_;
};
