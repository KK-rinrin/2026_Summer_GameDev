#pragma once
#include "TalkData.h"
#include "window/Live2DTalkController.h"
#include <memory>

class TalkWindow;

class Talk
{
	static constexpr float EXTEND_RATE = 2.0f;
	static constexpr float PATIENT_POS_X = 230.0f;
	static constexpr float PLAYER_POS_X = -230.0f;
	static constexpr float MODEL_POS_Y = -100.0f;

	static constexpr int FADE_ALPHA = 10;

public:
	Talk();
	~Talk();

	void Load();
	bool Update();
	void Draw();
	void Delete();

	void SetTalk(TDI dataIndex);

	void SetPatientVisible(bool visible) { PatientVisible_ = visible; }
	void SetPlayerVisible(bool visible) { PlayerVisible_ = visible; }

	
private:
	// controller ‚ª Live2D ‚ğŠ—L‚·‚é
	std::unique_ptr<Live2DTalkController> patientController_;
	std::unique_ptr<Live2DTalkController> playerController_;

	// ‰Â‹ƒtƒ‰ƒO
	bool PatientVisible_ = true;
	bool PlayerVisible_ = true;

	// TalkWindow ‚ğ RAII ŠÇ—
	std::unique_ptr<TalkWindow> talkWindow_;

	TalkDatas::TalkDataIndex currentDataIndex_ = TDI::TALK_0;
	bool isTalkEnd_;

	int alpha_;
};