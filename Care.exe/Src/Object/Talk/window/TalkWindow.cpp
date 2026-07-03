#include "TalkWindow.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Live2D.h"
#include "Live2DTalkController.h"
#include <algorithm>
#include <cctype>  

// ヘルパー：std::string -> TCHAR系 string 変換（簡易）
#ifdef UNICODE
static std::basic_string<TCHAR> ToTString(const std::string& s)
{
	return std::basic_string<TCHAR>(s.begin(), s.end());
}
#else
static std::basic_string<TCHAR> ToTString(const std::string& s)
{
	return s;
}
#endif

// ヘルパー：文字列から Param 列挙への変換（名前 or 数値）
static Live2DTalkController::Param StringToParam(const std::string& s)
{
	std::string u = s;
	std::transform(u.begin(), u.end(), u.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

	if (u == "MOUTH") return Live2DTalkController::Param::MOUTH;
	if (u == "EYE_BLINK_R") return Live2DTalkController::Param::EYE_BLINK_R;
	if (u == "EYE_BLINK_L") return Live2DTalkController::Param::EYE_BLINK_L;
	if (u == "BROW_RY") return Live2DTalkController::Param::BROW_RY;
	if (u == "BROW_LY") return Live2DTalkController::Param::BROW_LY;
	if (u == "CUSTOM") return Live2DTalkController::Param::CUSTOM;
	if (u == "EYE_BLACK") return Live2DTalkController::Param::PAT_EYE_BLACK;

	// 数値なら enum 値として扱う
	try
	{
		int v = std::stoi(s);
		return static_cast<Live2DTalkController::Param>(v);
	}
	catch (...)
	{
		return Live2DTalkController::Param::CUSTOM;
	}
}

// ==============================
// ctor / dtor
// ==============================
TalkWindow::TalkWindow()
{
}

TalkWindow::~TalkWindow()
{
}

// ==============================
// 初期化
// ==============================
void TalkWindow::Init()
{
	imgX_ = IMG_X;
	imgY_ = IMG_Y;
	y_ = 0;

	isVisible_ = true;

	talkScript_.clear();

	visCharaTime_ = 0.0f;
	unitPos_ = 0;
	waitTimer_ = 0.0f;

	baseCharTime_ = LINE_ADVANCE_TIME;
	lineAdTimeMulti_ = 1.0f;

	prevTime_ = GetNowHiPerformanceCount();
	deltaTime_ = 0.0f;

	speakActive_ = false;
	waitingForClick_ = false;

	currentSpeaker_ = TalkDatas::Speaker::NONE;

	// デフォルトのテキスト位置
	textX_ = TEXT_X;
	textY_ = TEXT_Y;


}

// ==============================
// Load
// ==============================
void TalkWindow::Load()
{
	auto& rss = ResourceManager::GetInstance();

	// ウィンドウ画像
	handle_ = rss.Load(ResourceManager::SRC::TALK_WINDOW_IMG).handleId_;

	// ▼の画像
	nextHandle_ = rss.Load(ResourceManager::SRC::TALK_WINDOW_NEXT_IMG).handleId_;

	// フォントは ResourceManager に登録されたフォントハンドルを使う
	const Resource& fontRes = rss.Load(ResourceManager::SRC::TALK_FONT);
	font_ = fontRes.handleId_;
	resourceFontSize_ = fontRes.fontSize_;

	// フォント情報を元にテキスト位置を調整（ウィンドウ画像の中心を基準に左寄せで決定）
	imgX_ = IMG_X;
	imgY_ = IMG_Y;

}

void TalkWindow::LoadEnd()
{
}

// ==============================
// 発話開始
// ==============================
void TalkWindow::StartSpeak(TalkDatas::Speaker speaker, const std::string& talk, float advanceTime)
{
	currentSpeaker_ = speaker;
	speakActive_ = true;
	waitingForClick_ = false;
	Speak(talk, advanceTime);
}

void TalkWindow::CompleteSpeak()
{
	if (!speakActive_)
	{
		return;
	}

	// 次の WAIT_C タグまで表示して待つ
	size_t pos = unitPos_;
	Live2DTalkController* ctrl = (currentSpeaker_ == TalkDatas::Speaker::Patient) ? patientController_ : playerController_;
	bool foundWaitC = false;

	while (pos < units_.size())
	{
		const CharUnit& cu = units_[pos];
		if (cu.type == CharUnit::Type::TAG)
		{
			// WAIT_C に出会ったらそこで止める（タグは消費せず待ち状態に）
			if (cu.tagType == "WAIT_C")
			{
				foundWaitC = true;
				break;
			}

			// 他のタグは即時適用してから先へ進める
			if (cu.tagType == "WAIT")
			{
				// 行内短い一時待ちタグ。CompleteSpeak では無視して次へ（waitTimer をキープしない）
			}
			else if (cu.tagType == "FBF")
			{
				lineAdTimeMulti_ = (cu.floatValue > 0.0f) ? cu.floatValue : 1.0f;
			}
			else if (cu.tagType == "PARAM")
			{
				if (ctrl)
				{
					Live2DTalkController::Param p = StringToParam(cu.tagParam);
					ctrl->SetParamValue(p, cu.floatValue);
				}
			}
			else
			{
				// その他のタグは表情タグ / 直接パラメータ名として扱う
				if (ctrl && ctrl->GetModel())
				{
					std::basic_string<TCHAR> tid = ToTString(cu.tagType);
					ctrl->GetModel()->SetParamerterValue(tid.c_str(), cu.floatValue);
				}
			}

			pos++;
			continue;
		}

		// NORMAL / PUNCT は表示対象なので単純に進める
		pos++;
	}

	// pos は表示すべき最終ユニット位置（WAIT_C の位置または末尾）
	unitPos_ = pos;
	visCharaTime_ = 0.0f;
	waitTimer_ = 0.0f;

	// ここで待ち状態に入る（行末でも途中でも同様に待ち表示）
	waitingForClick_ = true;
	// WAIT_C 到達時は口を閉じる
	CloseMouthForCurrentSpeaker();
}


void TalkWindow::FinishSpeak()
{
	speakActive_ = false;
	waitingForClick_ = false;
	talkScript_.clear();
	units_.clear();
	unitPos_ = 0;
	visCharaTime_ = 0.0f;
	waitTimer_ = 0.0f;
	ResetMouths();
}

// 追加: WAIT_C を消費して表示を再開する
void TalkWindow::ContinueSpeak()
{
	if (!speakActive_ || !waitingForClick_)
	{
		return;
	}

	// 現在の位置が WAIT_C タグにある場合はそれを消費して次から再開する
	if (unitPos_ < units_.size())
	{
		const CharUnit& cu = units_[unitPos_];
		if (cu.type == CharUnit::Type::TAG && cu.tagType == "WAIT_C")
		{
			unitPos_++; // WAIT_C タグを消費
		}
	}

	waitingForClick_ = false;
	visCharaTime_ = 0.0f;
	prevTime_ = GetNowHiPerformanceCount();
	// 再開時に口パラメータの初期化を行う
	ResetMouths();
}

// ==============================
// Update
// ==============================
void TalkWindow::Update()
{

	// deltaTime
	long long now = GetNowHiPerformanceCount();
	deltaTime_ = (now - prevTime_) / 1000000.0f;
	prevTime_ = now;
	if (deltaTime_ > 0.1f) deltaTime_ = 0.1f;

	UpdateVisibleWindow();
	if (!waitingForClick_)
		UpdateLineAdvance();
	UpdateNextIcon();
}

void TalkWindow::UpdateNextIcon()
{
	// ▼が上下に動く範囲内で移動
	if (isMoveUp_)
	{
		nextIconYf_ += NI_MOVE_TIME * deltaTime_;
		if (nextIconYf_ >= NI_MOVE_MAX)
		{
			nextIconYf_ = NI_MOVE_MAX;
			isMoveUp_ = false;
		}
	}
	else
	{
		nextIconYf_ -= NI_MOVE_TIME * deltaTime_;
		if (nextIconYf_ <= NI_MOVE_MIN)
		{
			nextIconYf_ = NI_MOVE_MIN;
			isMoveUp_ = true;
		}
	}
	nextIconY_ = static_cast<int>(nextIconYf_);
}

// ==============================
// Draw
// ==============================
void TalkWindow::Draw()
{
	if (!speakActive_)
	{
		return;
	}

	if (handle_ >= 0)
		DrawRotaGraph(imgX_, imgY_ + y_, 1.0, 0.0, handle_, true);

	std::string vis = GetVisibleText();
	DrawStringToHandle(
		TEXT_X, TEXT_Y + y_,
		vis.c_str(), GetColor(255, 255, 255),
		font_);

	DrawNextIcon();
}

void TalkWindow::DrawNextIcon()
{
	if (!waitingForClick_ || nextHandle_ < 0)
	{
		return;
	}

	DrawRotaGraph(nextIconX_, nextIconY_, 1.0, 0.0, nextHandle_, true);
}

// ==============================
// Delete
// ==============================
void TalkWindow::Delete()
{
	if (handle_ >= 0) DeleteGraph(handle_);
	handle_ = -1;
}

// ==============================
// Speak
// ==============================
void TalkWindow::Speak(const std::string& talk, float advanceTime)
{
	talkScript_ = talk;
	lineAdTimeMulti_ = (advanceTime > 0.0f) ? advanceTime : 1.0f;
	units_ = TalkScriptParser::Parse(talk);
	unitPos_ = 0;
	visCharaTime_ = 0.0f;
	waitTimer_ = 0.0f;
	prevTime_ = GetNowHiPerformanceCount();
	ResetMouths();
}

// ==============================
// 可視ウィンドウ制御
// ==============================
void TalkWindow::UpdateVisibleWindow()
{
	// (既存のコメント化された処理はそのまま)
}

// ==============================
// 行送り処理
// ==============================
void TalkWindow::UpdateLineAdvance()
{
	if (!speakActive_)
	{
		return;
	}

	if (unitPos_ >= units_.size())
	{
		CloseMouthForCurrentSpeaker();

		// 発話完了後は外側の Talk が入力を見て次イベントへ進める
		if (!waitingForClick_)
		{
			waitingForClick_ = true;
		}
		return;
	}

	if (waitTimer_ > 0.0f)
	{
		waitTimer_ -= deltaTime_ * 1000.0f;
		if (waitTimer_ < 0.0f) waitTimer_ = 0.0f;
		return;
	}

	const CharUnit& cu = units_[unitPos_];

	// TAG
	if (cu.type == CharUnit::Type::TAG)
	{
		Live2DTalkController* ctrl = (currentSpeaker_ == TalkDatas::Speaker::Patient) ? patientController_ : playerController_;

		if (cu.tagType == "WAIT")
		{
			waitTimer_ = static_cast<float>(cu.waitMs);
			unitPos_++;
		}
		else if (cu.tagType == "FBF")
		{
			lineAdTimeMulti_ = (cu.floatValue > 0.0f) ? cu.floatValue : 1.0f;
			unitPos_++;
		}
		else if (cu.tagType == "PARAM")
		{
			// PARAM: パラ名（または数値）: 値
			if (ctrl)
			{
				Live2DTalkController::Param p = StringToParam(cu.tagParam);
				ctrl->SetParamValue(p, cu.floatValue);
			}
			unitPos_++;
		}
		else if (cu.tagType == "WAIT_C")
		{
			// 決定待ち有効化（途中待ち）
			// タグはここでは消費せず、表示を止めて口を閉じる
			waitingForClick_ = true;
			CloseMouthForCurrentSpeaker();
			// do NOT increment unitPos_ here; ContinueSpeak() will consume the tag
		}
		else
		{
			// その他のタグを「表情タグ / 直接パラメータ名」として扱う
			if (ctrl && ctrl->GetModel())
			{
				std::basic_string<TCHAR> tid = ToTString(cu.tagType);
				ctrl->GetModel()->SetParamerterValue(tid.c_str(), cu.floatValue);
			}
			unitPos_++;
		}

		return;
	}

	// PUNCT
	if (cu.type == CharUnit::Type::PUNCT)
	{
		float ms = 200.0f;

		switch (cu.punctKind)
		{
		case CharUnit::PunctKind::PERIOD:         ms = 500.0f; break;
		case CharUnit::PunctKind::COMMA:          ms = 250.0f; break;
		case CharUnit::PunctKind::EXCLAM:
		case CharUnit::PunctKind::QUESTION:       ms = 250.0f; break;
		case CharUnit::PunctKind::EXCLAM_MANY:
		case CharUnit::PunctKind::QUESTION_MANY:  ms = 400.0f; break;
		case CharUnit::PunctKind::MIXED:          ms = 300.0f; break;
		default: break;
		}

		waitTimer_ = ms;
		unitPos_++;
		return;
	}

	// NORMAL
	if (cu.type == CharUnit::Type::NORMAL)
	{
		visCharaTime_ += deltaTime_ * 1000.0f;
		float denom = baseCharTime_ * lineAdTimeMulti_;
		float t = (denom > 0.0f) ? (visCharaTime_ / denom) : 0.0f;
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;

		// noMouth が true の場合は口パラメータを更新せずリセットする
		if (cu.noMouth)
		{
			ResetMouths();
		}
		else
		{
			if (currentSpeaker_ == TalkDatas::Speaker::Patient)
			{
				if (patientController_) patientController_->SetMouthProgress(t);
				if (playerController_) playerController_->ResetMouth();
			}
			else if (currentSpeaker_ == TalkDatas::Speaker::Player)
			{
				if (patientController_) patientController_->ResetMouth();
				if (playerController_) playerController_->SetMouthProgress(t);
			}
			else
			{
				ResetMouths();
			}
		}

		if (visCharaTime_ >= denom)
		{
			visCharaTime_ = 0.0f;
			unitPos_++;
		}
	}
}

void TalkWindow::ResetMouths()
{
	if (patientController_) patientController_->ResetMouth();
	if (playerController_) playerController_->ResetMouth();
}

void TalkWindow::CloseMouthForCurrentSpeaker()
{
	if (currentSpeaker_ == TalkDatas::Speaker::Patient)
	{
		if (patientController_) patientController_->CloseMouth();
	}
	else if (currentSpeaker_ == TalkDatas::Speaker::Player)
	{
		if (playerController_) playerController_->CloseMouth();
	}
	else
	{
		ResetMouths();
	}
}

// ==============================
// 表示文字列生成
// ==============================
std::string TalkWindow::GetVisibleText() const
{
	std::string result;

	for (size_t i = 0; i < unitPos_ && i < units_.size(); i++)
	{
		const CharUnit& cu = units_[i];
		if (cu.type == CharUnit::Type::NORMAL ||
			cu.type == CharUnit::Type::PUNCT)
		{
			result += cu.text;
		}
	}
	return result;
}