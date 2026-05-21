#include "Talk.h"
#include "window/TalkWindow.h"
#include "../../Manager/ResourceManager.h"
#include "../../Application.h"
#include "../../Manager/Live2DModelHub.h" // 追加
#include <utility>

Talk::Talk()
	: patientController_(nullptr)
	, playerController_(nullptr)
	, PatientVisible_(true)
	, PlayerVisible_(true)
	, talkWindow_(nullptr)
	, currentDataIndex_(TDI::NONE)
	, isTalkEnd_(false)
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
	patientController_ = Live2DModelHub::Instance().GetController(ResourceManager::SRC::PATIENT_MODEL);
	playerController_ = Live2DModelHub::Instance().GetController(ResourceManager::SRC::PLAYER_MODEL);

	// TalkWindow を作り、controller を渡す
	talkWindow_ = std::make_unique<TalkWindow>();
	talkWindow_->Init();
	talkWindow_->Load();
	talkWindow_->AttachPatientController(patientController_.get());
	talkWindow_->AttachPlayerController(playerController_.get());
}

bool Talk::Update()
{
	// 必要なオブジェクトが揃っていなければ無視
	if ((!patientController_ || !playerController_ || !talkWindow_) ||
		(currentDataIndex_ == TDI::NONE)) return false;

	if (!talkWindow_->IsConversationActive())
	{
		alpha_ -= FADE_ALPHA;
		if (alpha_ <= 0) currentDataIndex_ = TDI::NONE;
	}

	// モデル設定（位置・拡大）
	patientController_->SetExtendRate(EXTEND_RATE, EXTEND_RATE);
	patientController_->SetTranslate(PATIENT_POS_X, MODEL_POS_Y);
	patientController_->SetAlpha(alpha_);

	playerController_->SetExtendRate(EXTEND_RATE, EXTEND_RATE);
	playerController_->SetTranslate(PLAYER_POS_X, MODEL_POS_Y);
	playerController_->SetAlpha(alpha_);

	// モーション更新
	patientController_->Update(TEXT("Idle"));
	playerController_->Update(TEXT("Idle"));

	talkWindow_->Update();

	return true;
}

void Talk::Draw()
{
	// 必要なオブジェクトが揃っていなければ描画しない
	if ((!patientController_ || !playerController_ || !talkWindow_) ||
		(currentDataIndex_ == TDI::NONE)) return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 30);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);

	if (!talkWindow_->IsConversationActive())
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
	else SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	// controller に描画を委譲
	if (PatientVisible_)
	{
		patientController_->DrawBegin();
		patientController_->DrawModel();
		patientController_->DrawEnd();
	}

	if (PlayerVisible_)
	{
		playerController_->DrawBegin();
		playerController_->DrawModel();
		playerController_->DrawEnd();
	}

	talkWindow_->Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

void Talk::Delete()
{
	// TalkWindow の内部リソースを明示的に解放してからポインタを破棄
	if (talkWindow_)
	{
		talkWindow_->Delete();
		talkWindow_.reset();
	}

	// shared_ptr を破棄するだけ（ハブが所有している）
	if (patientController_) patientController_.reset();
	if (playerController_) playerController_.reset();
}

void Talk::SetTalk(TDI dataIndex)
{
	currentDataIndex_ = dataIndex;
	talkWindow_->StartConversation(TalkDatas::GetTalkData(currentDataIndex_));
	alpha_ = 255;
}