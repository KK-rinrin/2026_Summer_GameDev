#pragma once

#include <string>
#include <vector>
#include <DxLib.h>

#include "Text/Text/TalkScriptParser.h"
#include "../../../Application.h"
#include "../TalkData.h"
#include "Live2DTalkController.h"

class TalkWindow
{
public:
	static constexpr int IMG_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int IMG_Y = 450;

	static constexpr int TEXT_X = 50;
	static constexpr int TEXT_Y = 370;

	static constexpr float LINE_ADVANCE_TIME = 90.0f;

	static constexpr int NEXT_ICON_X = 735;
	static constexpr int NEXT_ICON_Y = 525;

	// ▼が上下に動く範囲
	static constexpr int NI_MOVEY_RANGE = 2;

	static constexpr int NI_MOVE_MIN = NEXT_ICON_Y - NI_MOVEY_RANGE;
	static constexpr int NI_MOVE_MAX = NEXT_ICON_Y + NI_MOVEY_RANGE;

	// ▼が動く速さ（秒/フレーム？）
	static constexpr float NI_MOVE_TIME = 10.0f;

public:
	TalkWindow();
	~TalkWindow();

	void Init();
	void Load();
	void LoadEnd();
	void Update();
	void Draw();
	void Delete();
	
	void StartSpeak(TalkDatas::Speaker speaker, const std::string& talk, float advanceTime = 1.0f);
	void CompleteSpeak();
	void FinishSpeak();

	bool IsSpeakActive() const { return speakActive_; }
	bool IsSpeaking() const { return speakActive_ && unitPos_ < units_.size(); }
	bool IsWaitingForClick() const { return waitingForClick_; }
	bool IsVisible() const { return isVisible_; }

	// Live2D コントローラ受け取り（非所有）
	void AttachPatientController(Live2DTalkController* controller) { patientController_ = controller; }
	void AttachPlayerController(Live2DTalkController* controller) { playerController_ = controller; }

private:
	void Speak(const std::string& talk, float advanceTime = 1.0f);
	void UpdateVisibleWindow();
	void UpdateLineAdvance();
	void ResetMouths();
	void CloseMouthForCurrentSpeaker();
	std::string GetVisibleText() const;

	void UpdateNextIcon();
	void DrawNextIcon();

private:
	// ------------------------------
	// UI
	// ------------------------------
	bool isVisible_ = true;
	int y_ = 0;

	// ------------------------------
	// 描画
	// ------------------------------
	int handle_ = -1;
	int imgX_,imgY_ = 0;

	int nextHandle_ = -1;	// 右下の▼
	int nextIconX_ = NEXT_ICON_X; 
	int nextIconY_ = NEXT_ICON_Y;
	float nextIconYf_ = static_cast<float>(NEXT_ICON_Y);
	bool isMoveUp_ = false;

	// テキスト表示位置（Resource のフォント情報で調整）
	int textX_ = TEXT_X;
	int textY_ = TEXT_Y;
	int resourceFontSize_ = 0;

	// ------------------------------
	// トークデータ（単行パース済み）
	// ------------------------------
	std::string talkScript_;

	std::vector<CharUnit> units_;
	size_t unitPos_ = 0;

	// ------------------------------
	// 単行発話制御
	// ------------------------------
	bool speakActive_ = false;
	bool waitingForClick_ = false;

	// 現在の話者
	TalkDatas::Speaker currentSpeaker_ = TalkDatas::Speaker::NONE;

	// ------------------------------
	// 表示制御
	// ------------------------------
	int font_; // フォントハンドル
	float visCharaTime_ = 0.0f;
	float baseCharTime_ = LINE_ADVANCE_TIME;
	float lineAdTimeMulti_ = 1.0f;
	float waitTimer_ = 0.0f;

	// ------------------------------
	// 時間
	// ------------------------------
	long long prevTime_ = 0;
	float deltaTime_ = 0.0f;

	// ------------------------------
	// その他
	// ------------------------------
	int enableB_ = 0;

	// Live2D コントローラ参照（非所有）
	Live2DTalkController* patientController_ = nullptr;
	Live2DTalkController* playerController_ = nullptr;
};
