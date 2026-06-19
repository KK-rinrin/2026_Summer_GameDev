#pragma once

#include "TalkData.h"
#include <DxLib.h>
#include "window/Live2DTalkController.h"
#include <memory>

class TalkWindow;

class Talk
{
	static constexpr float EXTEND_RATE = 2.0f;
	static constexpr VECTOR PATIENT_POS = { 230.0f, -100.0f, 0.0f };
	static constexpr VECTOR PLAYER_POS = { -230.0f, -100.0f, 0.0f };

	static constexpr int FADE_ALPHA = 10;

public:
	Talk();
	~Talk();

	void Load();
	bool Update();	// 会話中なら true を返す
	void Draw();
	void Delete();

	void SetTalk(TDI dataIndex);
	TDI GetCurrentTDI() const { return currentDataIndex_; }
	bool IsActive() const { return currentDataIndex_ != TDI::NONE; }
	bool ConsumeTalkEnd(TDI dataIndex);

private:
	// Live2DTalkController をハブ経由で共有するため shared_ptr に変更
	std::shared_ptr<Live2DTalkController> patientController_;
	std::shared_ptr<Live2DTalkController> playerController_;

	// 可視フラグ
	bool PatientVisible_ = true;
	bool PlayerVisible_ = true;

	// TalkWindow を RAII 管理
	std::unique_ptr<TalkWindow> talkWindow_;

	// 会話状態
	TalkDatas::TalkDataIndex currentDataIndex_ = TDI::NONE;
	bool isTalkEnd_ = false;

	int alpha_;
};