#pragma once

#include <string>
#include <vector>
#include <DxLib.h>
#include <memory>

#include "Text/Text/TalkScriptParser.h"
#include "../../../Application.h"
#include "../TalkData.h"
#include "Live2DTalkController.h"

class TalkWindow
{
public:
	static constexpr int IMG_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int IMG_Y = 450;

	static constexpr int INVISIBLE_Y = 280;
	static constexpr int MOVEY_ = 10;

	static constexpr int TEXT_X = 50;
	static constexpr int TEXT_Y = 370;

	static constexpr float LINE_ADVANCE_TIME = 90.0f;

public:
	TalkWindow();
	~TalkWindow();

	void Init();
	void Load();
	void LoadEnd();
	void Update();
	void Draw();
	void Delete();
	
	// 単行表示（既存）
	void Speak(const std::string& talk, float advanceTime = 1.0f);

	// 会話（複数行）制御
	void StartConversation(const std::vector<TD>& data); // 会話開始（外部からデータを渡す）
	bool IsConversationActive() const { return conversationActive_; }

	bool IsSpeaking() const { return unitPos_ < units_.size(); }
	bool IsVisible() const { return isVisible_; }

	// Live2D コントローラ受け取り（非所有）
	void AttachPatientController(Live2DTalkController* controller) { patientController_ = controller; }
	void AttachPlayerController(Live2DTalkController* controller) { playerController_ = controller; }

private:
	void UpdateVisibleWindow();
	void UpdateLineAdvance();
	std::string GetVisibleText() const;

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
	int imgX_ = 0;
	int imgY_ = 0;

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
	// 会話データ（複数行）
	// ------------------------------
	std::vector<TD> convData_;
	size_t convPos_ = 0;
	bool waitingForClick_ = false;
	bool conversationActive_ = false;

	// 現在の話者
	TalkDatas::Speaker currentSpeaker_ = TalkDatas::Speaker::Patient;

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

	// マウス＆キーのエッジ検出
	bool prevMouseLeftDown_ = false;
	bool prevKeySpaceDown_ = false;
	bool prevKeyReturnDown_ = false;

	// Live2D コントローラ参照（非所有）
	Live2DTalkController* patientController_ = nullptr;
	Live2DTalkController* playerController_ = nullptr;
};