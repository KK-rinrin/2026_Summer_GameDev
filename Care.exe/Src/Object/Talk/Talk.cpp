#include "Talk.h"
#include "window/TalkWindow.h"
#include "../../Common/Fader.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/KeyConfig.h"
#include "../../Application.h"
#include "../../Manager/Live2DModelHub.h" // 追加
#include "../../Manager/ProgressManager.h"
#include <utility>

Talk::Talk()
	: patientController_(nullptr)
	, playerController_(nullptr)
	, PatientVisible_(true)
	, PlayerVisible_(true)
	, talkWindow_(nullptr)
	, fader_(nullptr)
	, currentDataIndex_(TDI::NONE)
	, talkData_(nullptr)
	, eventIndex_(0)
	, eventState_(EventState::NONE)
	, isEventStarted_(false)
	, isTemporaryTalk_(false)
	, isTalkEnd_(false)
	, isFinishing_(false)
	, imageHandle_(-1)
	, imageAlpha_(255)
	, clearImageStartTime_(0)
	, clearImageFadeMs_(300)
	, fadeOutEndTime_(0)
	, fadeInWaitMs_(0)
	, prevMouseLeftDown_(false)
	, alpha_(255)
{
}

Talk::~Talk()
{
	// 安全に解放
	Delete();
}

void Talk::Load()
{
	// ハブから共有コントローラを取得（shared_ptr を受け取る）
	if (ProgressManager::GetInstance().IsPatientCharExists())
	{
		patientController_ = Live2DModelHub::Instance().GetController(ResourceManager::SRC::PATIENT_MODEL);
	}
	if (ProgressManager::GetInstance().IsNurceCharExists())
	{
		playerController_ = Live2DModelHub::Instance().GetController(ResourceManager::SRC::PLAYER_MODEL);
	}

	// TalkWindow を作り、controller を渡す
	talkWindow_ = std::make_unique<TalkWindow>();
	talkWindow_->Init();
	talkWindow_->Load();
	talkWindow_->AttachPatientController(patientController_.get());
	talkWindow_->AttachPlayerController(playerController_.get());

	fader_ = std::make_unique<Fader>();
	fader_->Init();
}

bool Talk::Update()
{
	// 必要なオブジェクトが揃っていなければ無視
	if (talkWindow_ == nullptr || (talkData_ == nullptr && !isTemporaryTalk_)) return false;

	if (isTemporaryTalk_)
	{
		UpdateTemporaryTalk();
		return true;
	}

	if (fader_ == nullptr) return false;

	UpdateModels();
	fader_->Update();
	talkWindow_->Update();

	if (isFinishing_)
	{
		alpha_ -= FADE_ALPHA;
		if (alpha_ <= 0)
		{
			currentDataIndex_ = TDI::NONE;
			talkData_ = nullptr;
			imageHandle_ = -1;
			imageAlpha_ = 255;
			isFinishing_ = false;
		}
		return true;
	}

	UpdateEvent();

	return true;
}

void Talk::Draw()
{
	// 必要なオブジェクトが揃っていなければ描画しない
	if (talkWindow_ == nullptr || (talkData_ == nullptr && !isTemporaryTalk_)) return;

	if (isTemporaryTalk_)
	{
		talkWindow_->Draw();
		return;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 30);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	DrawCharacters();
	DrawImage();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	talkWindow_->Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	if (fader_)
	{
		fader_->Draw();
	}
}

void Talk::Delete()
{
	// TalkWindow の内部リソースを明示的に解放してからポインタを破棄
	if (talkWindow_)
	{
		talkWindow_->Delete();
		talkWindow_.reset();
	}

	fader_.reset();

	// shared_ptr を破棄するだけ（ハブが所有している）
	if (patientController_) patientController_.reset();
	if (playerController_) playerController_.reset();
}

void Talk::SetTalk(TDI dataIndex)
{
	currentDataIndex_ = dataIndex;
	talkData_ = &TalkDatas::GetTalkData(currentDataIndex_);
	RefreshVisibleSpeakers(*talkData_);

	eventIndex_ = 0;
	eventState_ = EventState::NONE;
	isEventStarted_ = false;
	isTemporaryTalk_ = false;
	isTalkEnd_ = false;
	isFinishing_ = false;
	imageHandle_ = -1;
	imageAlpha_ = 255;
	clearImageStartTime_ = 0;
	clearImageFadeMs_ = 300;
	fadeOutEndTime_ = 0;
	fadeInWaitMs_ = 0;
	prevMouseLeftDown_ = false;
	alpha_ = 255;

	if (fader_)
	{
		fader_->SetFade(Fader::STATE::NONE);
	}

	StartCurrentEvent();
}

void Talk::SetTemporaryTalk(const std::string& talk, float advanceTime)
{
	SetTemporaryTalk(TalkDatas::Speaker::Player, talk, advanceTime);
}

void Talk::SetTemporaryTalk(TalkDatas::Speaker speaker, const std::string& talk, float advanceTime)
{
	// 調べ物などの短いつぶやきは、暗幕やLive2D描画を使わずTalkWindowだけで出す。
	currentDataIndex_ = TDI::NONE;
	talkData_ = nullptr;
	eventIndex_ = 0;
	eventState_ = EventState::NONE;
	isEventStarted_ = false;
	isTemporaryTalk_ = true;
	isTalkEnd_ = false;
	isFinishing_ = false;
	imageHandle_ = -1;
	imageAlpha_ = 255;
	clearImageStartTime_ = 0;
	clearImageFadeMs_ = 300;
	fadeOutEndTime_ = 0;
	fadeInWaitMs_ = 0;
	prevMouseLeftDown_ = false;
	alpha_ = 255;

	if (fader_)
	{
		fader_->SetFade(Fader::STATE::NONE);
	}

	talkWindow_->StartSpeak(speaker, talk, advanceTime);
}

bool Talk::ConsumeTalkEnd(TDI dataIndex)
{
	if (currentDataIndex_ != dataIndex || !isTalkEnd_)
	{
		return false;
	}

	isTalkEnd_ = false;
	return true;
}

void Talk::UpdateTemporaryTalk()
{
	talkWindow_->Update();

	const bool decideTriggered = IsDecideTriggered();
	if (decideTriggered && talkWindow_->IsSpeaking())
	{
		talkWindow_->CompleteSpeak();
	}
	else if (decideTriggered && talkWindow_->IsWaitingForClick())
	{
		talkWindow_->FinishSpeak();
		isTemporaryTalk_ = false;
		prevMouseLeftDown_ = false;
	}
}

void Talk::UpdateModels()
{
	// モデル設定（位置・拡大）
	if (patientController_)
	{
		patientController_->SetExtendRate(EXTEND_RATE, EXTEND_RATE);
		patientController_->SetTranslate(PATIENT_POS.x, PATIENT_POS.y);
		patientController_->SetAlpha(alpha_);
	}

	if (playerController_)
	{
		playerController_->SetExtendRate(EXTEND_RATE, EXTEND_RATE);
		playerController_->SetTranslate(PLAYER_POS.x, PLAYER_POS.y);
		playerController_->SetAlpha(alpha_);
	}

	// モーション更新
	if (patientController_) patientController_->Update(TEXT("Idle"));
	if (playerController_) playerController_->Update(TEXT("Idle"));
}

void Talk::UpdateEvent()
{
	if (!isEventStarted_)
	{
		StartCurrentEvent();
	}

	const bool decideTriggered = IsDecideTriggered();

	switch (eventState_)
	{
	case EventState::SPEAK:
		if (decideTriggered && talkWindow_->IsSpeaking())
		{
			talkWindow_->CompleteSpeak();
		}
		else if (decideTriggered && talkWindow_->IsWaitingForClick())
		{
			talkWindow_->FinishSpeak();
			AdvanceEvent();
		}
		break;

	case EventState::IMAGE_WAIT:
		if (decideTriggered)
		{
			AdvanceEvent();
		}
		break;

	case EventState::CLEAR_IMAGE:
	{
		const long long elapsedMs = GetElapsedMs(clearImageStartTime_);
		if (elapsedMs >= clearImageFadeMs_)
		{
			imageHandle_ = -1;
			imageAlpha_ = 255;
			AdvanceEvent();
		}
		else
		{
			imageAlpha_ = 255 - static_cast<int>(255 * elapsedMs / clearImageFadeMs_);
		}
	}
		break;


	case EventState::FADING_OUT:
		if (fader_->IsEnd())
		{
			fadeOutEndTime_ = GetNowHiPerformanceCount();
			AdvanceEvent();
		}
		break;

	case EventState::FADE_IN_WAIT:
		if (fadeOutEndTime_ == 0)
		{
			fadeOutEndTime_ = GetNowHiPerformanceCount();
		}
		if (GetElapsedMs(fadeOutEndTime_) >= fadeInWaitMs_)
		{
			fader_->SetFade(Fader::STATE::FADE_IN);
			eventState_ = EventState::FADING_IN;
		}
		break;

	case EventState::FADING_IN:
		if (fader_->IsEnd())
		{
			fader_->SetFade(Fader::STATE::NONE);
			AdvanceEvent();
		}
		break;
	}
}

void Talk::StartCurrentEvent()
{
	if (talkData_ == nullptr || eventIndex_ >= talkData_->size())
	{
		FinishTalkEvents();
		return;
	}

	isEventStarted_ = true;
	std::visit([this](const auto& eventData) { StartEvent(eventData); }, (*talkData_)[eventIndex_]);
}

void Talk::StartEvent(const TalkDatas::SpeakEvent& eventData)
{
	talkWindow_->StartSpeak(eventData.speaker, eventData.talk, eventData.advanceTime);
	eventState_ = EventState::SPEAK;
}

void Talk::StartEvent(const TalkDatas::ImageEvent& eventData)
{
	imageHandle_ = ResourceManager::GetInstance().Load(eventData.src).handleId_;
	imageAlpha_ = 255;
	talkWindow_->FinishSpeak();
	eventState_ = EventState::IMAGE_WAIT;
}

void Talk::StartEvent(const TalkDatas::ClearImageEvent& eventData)
{
	if (imageHandle_ < 0)
	{
		AdvanceEvent();
		return;
	}

	clearImageFadeMs_ = (eventData.fadeMs > 0) ? eventData.fadeMs : 1;
	clearImageStartTime_ = GetNowHiPerformanceCount();
	talkWindow_->FinishSpeak();
	eventState_ = EventState::CLEAR_IMAGE;
}

void Talk::StartEvent(const TalkDatas::FadeOutEvent& eventData)
{
	(void)eventData;
	talkWindow_->FinishSpeak();
	// FadeOutは入力待ちせず、イベント行に到達した時点ですぐ暗転を開始する。
	fader_->SetFade(Fader::STATE::FADE_OUT);
	eventState_ = EventState::FADING_OUT;
}

void Talk::StartEvent(const TalkDatas::FadeInEvent& eventData)
{
	talkWindow_->FinishSpeak();
	fadeInWaitMs_ = eventData.waitMs;
	eventState_ = EventState::FADE_IN_WAIT;
}

void Talk::AdvanceEvent()
{
	eventIndex_++;
	isEventStarted_ = false;
	eventState_ = EventState::NONE;
	StartCurrentEvent();
}

void Talk::FinishTalkEvents()
{
	if (talkWindow_)
	{
		talkWindow_->FinishSpeak();
	}
	eventState_ = EventState::NONE;
	isEventStarted_ = false;
	isTemporaryTalk_ = false;
	isTalkEnd_ = true;
	isFinishing_ = true;
}

void Talk::RefreshVisibleSpeakers(const std::vector<TD>& talkData)
{
	PatientVisible_ = false;
	PlayerVisible_ = false;
	const bool patientExists = ProgressManager::GetInstance().IsPatientCharExists();
	const bool playerExists = ProgressManager::GetInstance().IsNurceCharExists();
	for (const TD& data : talkData)
	{
		const TalkDatas::SpeakEvent* speak = std::get_if<TalkDatas::SpeakEvent>(&data);
		if (speak == nullptr)
		{
			continue;
		}

		if (speak->speaker == TalkDatas::Speaker::Patient && patientExists)
		{
			PatientVisible_ = true;
		}
		if (speak->speaker == TalkDatas::Speaker::Player && playerExists)
		{
			PlayerVisible_ = true;
		}
	}
}

bool Talk::IsDecideTriggered()
{
	int mouseState = GetMouseInput();
	bool leftDown = (mouseState & MOUSE_INPUT_LEFT) != 0;
	const bool decideDown = KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, InputManager::GetInstance());
	const bool triggered = (leftDown && !prevMouseLeftDown_) || decideDown;
	prevMouseLeftDown_ = leftDown;
	return triggered;
}

long long Talk::GetElapsedMs(long long startTime) const
{
	return (GetNowHiPerformanceCount() - startTime) / 1000;
}

void Talk::DrawCharacters()
{
	// controller に描画を委譲
	if (PatientVisible_ && patientController_)
	{
		patientController_->DrawBegin();
		patientController_->DrawModel();
		patientController_->DrawEnd();
	}

	if (PlayerVisible_ && playerController_)
	{
		playerController_->DrawBegin();
		playerController_->DrawModel();
		playerController_->DrawEnd();
	}
}

void Talk::DrawImage()
{
	if (imageHandle_ < 0)
	{
		return;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_ * imageAlpha_ / 255);
	DrawExtendGraph(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		imageHandle_, true);
}
