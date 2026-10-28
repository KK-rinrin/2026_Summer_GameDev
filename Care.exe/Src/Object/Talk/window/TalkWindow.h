#pragma once

#include <string>
#include <vector>
#include <DxLib.h>

#include "TalkScriptParser.h"
#include "../../../Application.h"
#include "../TalkData.h"
#include "Live2DTalkController.h"

class TalkWindow
{
public:

	// 会話ウィンドウ画像の中心X座標
	static constexpr int IMG_X = Application::SCREEN_SIZE_X / 2;

	// 会話ウィンドウ画像の中心Y座標
	static constexpr int IMG_Y = 450;

	// 本文の描画開始X座標
	static constexpr int TEXT_X = 50;

	// 本文の描画開始Y座標
	static constexpr int TEXT_Y = 370;

	// 話者名の描画Y座標
	static constexpr int SPEAKER_Y = 324;

	// 操作キャラの話者名の描画X座標
	static constexpr int SPEAKER_PLAYER_X = 150;

	// 患者の話者名の描画X座標
	static constexpr int SPEAKER_PATIENT_X = 620;

	// 会話本文のフォントサイズ
	static constexpr int TALK_FONT_SIZE = 30;

	// 1文字当たりの基準表示時間のミリ秒数
	static constexpr float LINE_ADVANCE_TIME = 90.0f;

	// 次送りアイコンの基準X座標
	static constexpr int NEXT_ICON_X = 735;

	// 次送りアイコンの基準Y座標
	static constexpr int NEXT_ICON_Y = 525;

	// ▼が上下に動く範囲
	static constexpr int NI_MOVEY_RANGE = 2;

	// 次送りアイコンの移動先Y座標の最小値
	static constexpr int NI_MOVE_MIN = NEXT_ICON_Y - NI_MOVEY_RANGE;

	// 次送りアイコンの移動先Y座標の最大値
	static constexpr int NI_MOVE_MAX = NEXT_ICON_Y + NI_MOVEY_RANGE;

	// 次送りアイコンの移動速度（ピクセル毎秒）
	static constexpr float NI_MOVE_TIME = 10.0f;

public:
	TalkWindow();
	~TalkWindow();

	void Init();
	void Load();

	// 読み込み完了時の空の拡張処理（未使用）
	void LoadEnd();
	void Update();
	void Draw();
	void Delete();

	// 話者と本文と文字送り時間倍率を指定した発話開始
	void StartSpeak(TalkDatas::Speaker speaker, const std::string& talk, float advanceTime = TalkDatas::DEFAULT_ADVANCE_TIME);

	// 次のクリック待機タグまたは末尾までの即時表示
	void CompleteSpeak();

	// 発話の終了とウィンドウの非表示化
	void FinishSpeak();

	// 途中待ちから続ける（WAIT_C を消費して表示再開）
	void ContinueSpeak();

	// 単行発話の有効状態の取得（未使用）
	bool IsSpeakActive() const { return speakActive_; }

	// 文字送りが継続中かの判定
	bool IsSpeaking() const { return speakActive_ && unitPos_ < units_.size(); }

	// クリックによる再開待ちの判定
	bool IsWaitingForClick() const { return waitingForClick_; }

	// 保持しているウィンドウ可視フラグの取得（未使用）
	bool IsVisible() const { return isVisible_; }

	// Live2D コントローラ受け取り（非所有）
	void AttachPatientController(Live2DTalkController* controller) { patientController_ = controller; }

	// 操作キャラの非所有Live2D制御ポインタの設定
	void AttachPlayerController(Live2DTalkController* controller) { playerController_ = controller; }

private:

	// 1秒当たりのマイクロ秒数
	static constexpr float MICROSECONDS_PER_SECOND = 1000000.0f;

	// 秒からミリ秒への換算倍率
	static constexpr float MILLISECONDS_PER_SECOND = 1000.0f;

	// 1更新で扱う経過秒数の上限
	static constexpr float MAX_DELTA_SECONDS = 0.1f;

	// 句読点の既定待機ミリ秒数
	static constexpr float PUNCT_DEFAULT_WAIT_MS = 200.0f;

	// 終止符に対する待機ミリ秒数
	static constexpr float PUNCT_PERIOD_WAIT_MS = 500.0f;

	// 読点に対する待機ミリ秒数
	static constexpr float PUNCT_COMMA_WAIT_MS = 250.0f;

	// 単独の感嘆符または疑問符に対する待機ミリ秒数
	static constexpr float PUNCT_SINGLE_MARK_WAIT_MS = 250.0f;

	// 連続した感嘆符または疑問符に対する待機ミリ秒数
	static constexpr float PUNCT_REPEATED_MARK_WAIT_MS = 400.0f;

	// 混在した感嘆符と疑問符に対する待機ミリ秒数
	static constexpr float PUNCT_MIXED_MARK_WAIT_MS = 300.0f;

	// 本文の色成分の最大値
	static constexpr int TEXT_RGB_MAX = 255;

	// 本文の解析と文字送り状態の設定
	void Speak(const std::string& talk, float advanceTime = TalkDatas::DEFAULT_ADVANCE_TIME);

	// ウィンドウ表示更新用の空の処理
	void UpdateVisibleWindow();

	// 制御タグと待機時間を含む文字送りの更新
	void UpdateLineAdvance();

	// 両話者の口パク状態の初期化
	void ResetMouths();

	// 現在の話者の口を閉じる処理
	void CloseMouthForCurrentSpeaker();

	// 表示済みの文字ユニットを連結した本文の取得
	std::string GetVisibleText() const;

	// 次送りアイコンの表示位置の更新
	void UpdateNextIcon();

	// 次送りアイコンの描画
	void DrawNextIcon();

private:

	// 会話ウィンドウの表示状態
	bool isVisible_;

	// 会話ウィンドウ全体の縦位置補正
	int y_;

	// 会話ウィンドウ背景画像のハンドル
	int handle_;

	// 会話ウィンドウ画像の中心X座標と中心Y座標
	int imgX_, imgY_;

	// 話者表示用画像のハンドル
	int speakerHandle_;

	int nextHandle_;	// 右下の▼

	// 次送りアイコンの現在X座標
	int nextIconX_;

	// 次送りアイコンの整数Y座標
	int nextIconY_;

	// 次送りアイコンの浮動小数Y座標
	float nextIconYf_;

	// 次送りアイコンの上向き移動状態
	bool isMoveUp_;

	// 本文の描画開始X座標の保存値（未使用）
	int textX_;

	// 本文の描画開始Y座標の保存値（未使用）
	int textY_;

	// 解析前の単行会話スクリプト
	std::string talkScript_;

	// 解析済みの文字ユニット一覧
	std::vector<CharUnit> units_;

	// 次に処理する文字ユニットの添字
	size_t unitPos_;

	// 単行発話の有効状態
	bool speakActive_;

	// クリックによる途中再開待ちの状態
	bool waitingForClick_;


	// 現在の話者
	TalkDatas::Speaker currentSpeaker_;

	// ------------------------------
	// 表示制御
	// ------------------------------
	int font_; // フォントハンドル

	// 現在の文字の表示開始からの経過ミリ秒数
	float visCharaTime_;

	// 1文字当たりの基準表示時間のミリ秒数
	float baseCharTime_;

	// 文字表示時間に掛ける会話固有の倍率
	float lineAdTimeMulti_;

	// 句読点と制御タグによる残り待機ミリ秒数
	float waitTimer_;

	// 前回更新時刻のマイクロ秒値
	long long prevTime_;

	// 前回更新からの経過秒数
	float deltaTime_;

	// 用途不明の補助状態値（要確認）（未使用）
	int enableB_;

	// Live2D コントローラ参照（非所有）
	Live2DTalkController* patientController_;

	// 操作キャラの非所有Live2D制御ポインタ
	Live2DTalkController* playerController_;
};
