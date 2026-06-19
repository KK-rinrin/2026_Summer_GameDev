#include <DxLib.h>
#include <cmath>
#include "../../Manager/KeyConfig.h"
#include "../../Manager/ProgressManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Utility/AsoUtility.h"
#include "BPMiniGameScene.h"

BPMiniGameScene::BPMiniGameScene(void)
	:
	SceneBase(),
	state_(State::INFLATE),
	pressure_(PRESSURE_MIN),
	resultTimer_(0.0f),
	frame_(0),
	font_(-1),
	largeFont_(-1),
	images_(),
	systolic_(0),
	diastolic_(0),
	isMonitorAbnormal_(false),
	isSystolicMeasured_(false),
	isDeleted_(false)
{
	for (int& image : images_)
	{
		image = -1;
	}
}

BPMiniGameScene::~BPMiniGameScene(void)
{
	Delete();
}

void BPMiniGameScene::Update(void)
{
	++frame_;
	const float deltaTime = sceMng_.GetDeltaTime();

	switch (state_)
	{
	case State::INFLATE:
		UpdateInflate(deltaTime);
		break;
	case State::DEFLATE:
		UpdateDeflate(deltaTime);
		break;
	case State::SUCCESS:
		UpdateSuccess(deltaTime);
		break;
	case State::FAILED:
		UpdateFailed();
		break;
	}
}

void BPMiniGameScene::Draw(void)
{
	DrawRoom();
	DrawBackground();
	DrawMonitor();

	DrawGraph(0, 0, images_[static_cast<int>(Image::INTRUCT)], true);

	if (state_ == State::FAILED)
	{
		DrawFailed();
	}
}

void BPMiniGameScene::Delete(void)
{
	if (isDeleted_)
	{
		return;
	}

	for (int& image : images_)
	{
		image = -1;
	}
	font_ = -1;
	largeFont_ = -1;

	isDeleted_ = true;
}

void BPMiniGameScene::InitLoad(void)
{
	isDeleted_ = false;
	isMonitorAbnormal_ = prgMng_.GetProgress() >= MONITOR_ABNORMAL_PROGRESS;

	font_ = resMng_.Load(ResourceManager::SRC::BPMG_FONT).handleId_;
	largeFont_ = resMng_.Load(ResourceManager::SRC::BPMG_LARGE_FONT).handleId_;
	LoadImages();
}

void BPMiniGameScene::LoadImages(void)
{
	images_[static_cast<int>(Image::BACKGROUND)] = resMng_.Load(ResourceManager::SRC::BPMG_BG_IMG).handleId_;
	images_[static_cast<int>(Image::MONITOR)] = resMng_.Load(ResourceManager::SRC::BPMG_MONITOR_IMG).handleId_;
	images_[static_cast<int>(Image::PULSE)] = resMng_.Load(ResourceManager::SRC::BPMG_PULSE_IMG).handleId_;
	images_[static_cast<int>(Image::FAILED)] = resMng_.Load(ResourceManager::SRC::BPMG_FAILED_IMG).handleId_;
	images_[static_cast<int>(Image::INTRUCT)] = resMng_.Load(ResourceManager::SRC::BPMG_INTRUCT_IMG).handleId_;

}

void BPMiniGameScene::UpdateInflate(float deltaTime)
{
	if (KeyConfig::IsNew(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		pressure_ += INFLATE_SPEED * deltaTime;
		pressure_ = AsoUtility::Clamp(pressure_, PRESSURE_MIN, PRESSURE_MAX);
	}

	if (KeyConfig::IsTrgUp(KeyConfig::ACTION::DECIDE, iptMng_) || pressure_ >= PRESSURE_MAX)
	{
		state_ = State::DEFLATE;
	}
}

void BPMiniGameScene::UpdateDeflate(float deltaTime)
{
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		if (!isSystolicMeasured_)
		{
			if (IsSystolicDetectable())
			{
				RecordSystolic();
			}
			else
			{
				Fail();
			}
		}
		else if (IsDiastolicDetectable())
		{
			RecordDiastolic();
		}
		else
		{
			Fail();
		}
		return;
	}

	pressure_ = AsoUtility::Clamp(pressure_ - DEFLATE_SPEED * deltaTime, PRESSURE_MIN, PRESSURE_MAX);

	if (IsPulseMissed())
	{
		Fail();
	}
	if (IsDiastolicMissed())
	{
		Fail();
	}
}

void BPMiniGameScene::UpdateSuccess(float deltaTime)
{
	resultTimer_ += deltaTime;
	if (resultTimer_ < SUCCESS_WAIT_SEC)
	{
		return;
	}

	if (prgMng_.GetProgressEnum() == ProgressManager::START_MINIGAME0)
	{
		prgMng_.AddProgress();
	}
	sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
}

void BPMiniGameScene::UpdateFailed(void)
{
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		sndMng_.PlaySE(SE::DECIDE);
		sceMng_.ChangeScene(SceneManager::SCENE_ID::BLOOD_PRESSURE_MINIGAME);
	}
}

void BPMiniGameScene::Fail(void)
{
	state_ = State::FAILED;
	sndMng_.PlaySE(SE::BEEP);
}

void BPMiniGameScene::RecordSystolic(void)
{
	isSystolicMeasured_ = true;
	systolic_ = GetDisplayPressure();
	sndMng_.PlaySE(SE::DECIDE);
}

void BPMiniGameScene::RecordDiastolic(void)
{
	diastolic_ = GetDisplayPressure();
	state_ = State::SUCCESS;
	resultTimer_ = 0.0f;
	sndMng_.PlaySE(SE::DECIDE);
}

bool BPMiniGameScene::IsSystolicDetectable(void) const
{
	return state_ == State::DEFLATE &&
		pressure_ <= SYSTOLIC_WINDOW_HIGH &&
		pressure_ >= SYSTOLIC_WINDOW_LOW;
}

bool BPMiniGameScene::IsDiastolicDetectable(void) const
{
	return state_ == State::DEFLATE &&
		pressure_ <= DIASTOLIC_WINDOW_HIGH &&
		pressure_ >= DIASTOLIC_WINDOW_LOW;
}

bool BPMiniGameScene::IsPulseMoving(void) const
{
	return state_ == State::DEFLATE &&
		pressure_ <= SYSTOLIC_WINDOW_HIGH &&
		pressure_ > DIASTOLIC_WINDOW_HIGH;
}

bool BPMiniGameScene::IsPulseMissed(void) const
{
	return state_ == State::DEFLATE && !isSystolicMeasured_ && pressure_ < SYSTOLIC_WINDOW_LOW;
}

bool BPMiniGameScene::IsDiastolicMissed(void) const
{
	return state_ == State::DEFLATE && isSystolicMeasured_ && pressure_ < DIASTOLIC_WINDOW_LOW;
}

int BPMiniGameScene::GetDisplayPressure(void) const
{
	int pressure = static_cast<int>(pressure_ + 0.5f);
	if (isMonitorAbnormal_)
	{
		pressure += ((frame_ / 10) % 3 - 1) * 4;
	}

	return ClampInt(pressure, 0, 199);
}
int BPMiniGameScene::GetDisplayPulse(void) const
{
	if (!IsPulseMoving())
	{
		return (isMonitorAbnormal_ && (frame_ / 20) % 2 == 0) ? 69 : 0;
	}

	int pulse = 72 + static_cast<int>(std::sinf(frame_ * 0.18f) * 4.0f);
	if (isMonitorAbnormal_)
	{
		pulse += ((frame_ / 9) % 3 - 1) * 6;
	}
	return ClampInt(pulse, 0, 199);
}
int BPMiniGameScene::ClampInt(int value, int minValue, int maxValue)
{
	if (value < minValue)
	{
		return minValue;
	}
	if (value > maxValue)
	{
		return maxValue;
	}
	return value;
}

int BPMiniGameScene::MaxInt(int lhs, int rhs)
{
	return (lhs > rhs) ? lhs : rhs;
}

int BPMiniGameScene::MinInt(int lhs, int rhs)
{
	return (lhs < rhs) ? lhs : rhs;
}

void BPMiniGameScene::DrawRoom(void) const
{
	DrawBox(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y, GetColor(225, 229, 232), TRUE);
	DrawBox(FLOOR_POS.x, FLOOR_POS.y, FLOOR_POS.x + FLOOR_SIZE.x, FLOOR_POS.y + FLOOR_SIZE.y, GetColor(204, 214, 218), TRUE);
}

void BPMiniGameScene::DrawBackground(void) const
{
	const int background = images_[static_cast<int>(Image::BACKGROUND)];

	if (background != -1)
	{
		DrawRotaGraph(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2, 1.0, 0.0, background, TRUE);
	}
	else
	{
		DrawBox(BED_POS.x, BED_POS.y, BED_POS.x + BED_SIZE.x, BED_POS.y + BED_SIZE.y, GetColor(238, 242, 242), TRUE);
		DrawBox(BED_SHADOW_POS.x, BED_SHADOW_POS.y, BED_SHADOW_POS.x + BED_SHADOW_SIZE.x, BED_SHADOW_POS.y + BED_SHADOW_SIZE.y, GetColor(178, 189, 194), TRUE);
		DrawOval(FALLBACK_LIMB_CENTER.x, FALLBACK_LIMB_CENTER.y, FALLBACK_LIMB_RADIUS.x, FALLBACK_LIMB_RADIUS.y, GetColor(235, 186, 156), TRUE);
		DrawBox(FALLBACK_WRAP_POS.x, FALLBACK_WRAP_POS.y, FALLBACK_WRAP_POS.x + FALLBACK_WRAP_SIZE.x, FALLBACK_WRAP_POS.y + FALLBACK_WRAP_SIZE.y, GetColor(38, 55, 71), TRUE);
	}

	DrawPulseMarker();
}

void BPMiniGameScene::DrawMonitor(void) const
{
	const int monitor = images_[static_cast<int>(Image::MONITOR)];
	if (monitor != -1)
	{
		DrawRotaGraph(MONITOR_POS.x + MONITOR_SIZE.x / 2, MONITOR_POS.y + MONITOR_SIZE.y / 2, 1.0, 0.0, monitor, TRUE);
	}
	else
	{
		DrawBox(MONITOR_POS.x, MONITOR_POS.y, MONITOR_POS.x + MONITOR_SIZE.x, MONITOR_POS.y + MONITOR_SIZE.y, GetColor(34, 38, 42), TRUE);
		DrawBox(MONITOR_POS.x + 10, MONITOR_POS.y + 10, MONITOR_POS.x + MONITOR_SIZE.x - 10, MONITOR_POS.y + MONITOR_SIZE.y - 10, GetColor(72, 78, 84), FALSE);
		DrawBox(DISPLAY_POS.x, DISPLAY_POS.y, DISPLAY_POS.x + DISPLAY_SIZE.x, DISPLAY_POS.y + DISPLAY_SIZE.y, GetColor(8, 28, 26), TRUE);
		DrawBox(MONITOR_POS.x + 130, MONITOR_POS.y + MONITOR_SIZE.y, MONITOR_POS.x + 190, MONITOR_POS.y + MONITOR_SIZE.y + 50, GetColor(46, 52, 56), TRUE);
		DrawBox(MONITOR_POS.x + 82, MONITOR_POS.y + MONITOR_SIZE.y + 48, MONITOR_POS.x + 238, MONITOR_POS.y + MONITOR_SIZE.y + 68, GetColor(44, 49, 53), TRUE);
	}

	DrawPulseWave();

	const int textColor = GetColor(170, 245, 214);
	const int subColor = GetColor(112, 178, 154);
	const int pressure = GetDisplayPressure();
	const int pulse = GetDisplayPulse();
	const int jitterOffset = isMonitorAbnormal_ ? ((frame_ / 8) % 3 - 1) * 2 : 0;

	DrawStringToHandle(DISPLAY_POS.x + 18 + jitterOffset, DISPLAY_POS.y + 14, "PRESSURE", subColor, font_);
	DrawFormatStringToHandle(DISPLAY_POS.x + 24 + jitterOffset, DISPLAY_POS.y + 42, textColor, largeFont_, "%03d", pressure);
	DrawStringToHandle(DISPLAY_POS.x + 158, DISPLAY_POS.y + 68, "mmHg", subColor, font_);

	DrawStringToHandle(DISPLAY_POS.x + 18 - jitterOffset, DISPLAY_POS.y + 98, "PULSE", subColor, font_);
	if (pulse <= 0)
	{
		DrawStringToHandle(DISPLAY_POS.x + 28 - jitterOffset, DISPLAY_POS.y + 120, "---", textColor, largeFont_);
	}
	else if (isMonitorAbnormal_ && (frame_ / 10) % 2 == 0)
	{
		DrawStringToHandle(DISPLAY_POS.x + 28 - jitterOffset, DISPLAY_POS.y + 120, "???", GetColor(255, 120, 120), largeFont_);
	}
	else	{
		DrawFormatStringToHandle(DISPLAY_POS.x + 28 - jitterOffset, DISPLAY_POS.y + 120, textColor, largeFont_, "%03d", pulse);
	}

	DrawStringToHandle(DISPLAY_POS.x + 142, DISPLAY_POS.y + 112, "SYS", subColor, font_);
	if (systolic_ <= 0)
	{
		DrawStringToHandle(DISPLAY_POS.x + 196, DISPLAY_POS.y + 112, "---", textColor, font_);
	}
	else
	{
		DrawFormatStringToHandle(DISPLAY_POS.x + 196, DISPLAY_POS.y + 112, textColor, font_, "%03d", systolic_);
	}

	DrawStringToHandle(DISPLAY_POS.x + 142, DISPLAY_POS.y + 142, "DIA", subColor, font_);
	if (diastolic_ <= 0)
	{
		DrawStringToHandle(DISPLAY_POS.x + 196, DISPLAY_POS.y + 142, "---", textColor, font_);
	}
	else
	{
		DrawFormatStringToHandle(DISPLAY_POS.x + 196, DISPLAY_POS.y + 142, textColor, font_, "%03d", diastolic_);
	}

	if (state_ == State::SUCCESS)
	{
		DrawStringToHandle(DISPLAY_POS.x + 178, DISPLAY_POS.y + 68, "OK", GetColor(120, 235, 130), largeFont_);
	}

	DrawPressureGauge();
	DrawGlitch();
}

void BPMiniGameScene::DrawPressureGauge(void) const
{
	const Vector2 gaugePos = { MONITOR_POS.x + MONITOR_SIZE.x - 42, DISPLAY_POS.y + 18 };
	const Vector2 gaugeSize = { 10, DISPLAY_SIZE.y - 36 };
	const float rate = AsoUtility::Clamp(pressure_ / PRESSURE_MAX, 0.0f, 1.0f);
	const int fillTop = gaugePos.y + gaugeSize.y - static_cast<int>(gaugeSize.y * rate);

	DrawBox(gaugePos.x, gaugePos.y, gaugePos.x + gaugeSize.x, gaugePos.y + gaugeSize.y, GetColor(18, 58, 52), TRUE);
	DrawBox(gaugePos.x, fillTop, gaugePos.x + gaugeSize.x, gaugePos.y + gaugeSize.y, GetColor(112, 228, 183), TRUE);
	DrawBox(gaugePos.x - 2, gaugePos.y - 2, gaugePos.x + gaugeSize.x + 2, gaugePos.y + gaugeSize.y + 2, GetColor(112, 178, 154), FALSE);
}

void BPMiniGameScene::DrawPulseMarker(void) const
{
	const int pulse = images_[static_cast<int>(Image::PULSE)];
	const bool detectable = IsPulseMoving();
	const float beat = detectable ? std::sinf(frame_ * 0.42f) : 0.0f;
	const float beatRange = isMonitorAbnormal_ ? 14.0f : 10.0f;
	const Vector2 markerPos =
	{
		PULSE_MARKER_POS.x,
		PULSE_MARKER_POS.y + static_cast<int>(beat * beatRange)
	};
	if (pulse != -1)
	{
		DrawRotaGraph(markerPos.x, markerPos.y, 1.0, 0.0, pulse, TRUE);
		return;
	}

	const int color = detectable ? GetColor(235, 62, 74) : GetColor(116, 101, 104);
	const int radius = detectable ? 9 + static_cast<int>(std::fabs(beat) * 5.0f) : 7;
	DrawCircle(markerPos.x, markerPos.y, radius, color, TRUE);
}

void BPMiniGameScene::DrawPulseWave(void) const
{
	const Vector2 waveStart = { DISPLAY_POS.x + 154, DISPLAY_POS.y + 96 };
	const bool detectable = IsPulseMoving();
	Vector2 prevPos = waveStart;

	for (int i = 1; i <= 92; ++i)
	{
		Vector2 linePos = { waveStart.x + i, waveStart.y };
		float amp = 0.0f;
		if (detectable)
		{
			amp = std::sinf((frame_ + i) * 0.25f) * (isMonitorAbnormal_ ? 20.0f : 14.0f);
			if ((i + frame_) % 24 < 4)
			{
				amp *= 2.0f;
			}
		}
		else if (isMonitorAbnormal_)
		{
			amp = std::sinf((frame_ + i) * 0.12f) * 2.0f;
		}

		linePos.y -= static_cast<int>(amp);
		DrawLine(prevPos.x, prevPos.y, linePos.x, linePos.y, GetColor(96, 230, 170), 2);
		prevPos = linePos;
	}
}

void BPMiniGameScene::DrawGlitch(void) const
{
	if (!isMonitorAbnormal_)
	{
		return;
	}

	const int lineCount = 4;
	for (int i = 0; i < lineCount; ++i)
	{
		const Vector2 glitchPos =
		{
			DISPLAY_POS.x + 8 + (i * 37 + frame_) % 40,
			DISPLAY_POS.y + 14 + (frame_ * (i + 3) + i * 29) % (DISPLAY_SIZE.y - 28)
		};
		const int glitchLength = 60 + (i * 23 + 34) % 140;
		const int color = (i % 2 == 0) ? GetColor(42, 116, 114) : GetColor(112, 42, 70);
		DrawBox(glitchPos.x, glitchPos.y, MinInt(glitchPos.x + glitchLength, DISPLAY_POS.x + DISPLAY_SIZE.x - 8), glitchPos.y + 3, color, TRUE);
	}

	if ((frame_ / 24) % 2 == 0)
	{
		DrawStringToHandle(DISPLAY_POS.x + 154, DISPLAY_POS.y + 20, "ERR", GetColor(235, 76, 90), font_);
	}
}
void BPMiniGameScene::DrawFailed(void) const
{
	DrawBox(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y, GetColor(0, 0, 0), TRUE);

	const int failed = images_[static_cast<int>(Image::FAILED)];
	if (failed != -1)
	{
		DrawRotaGraph(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2, 1.0, 0.0, failed, TRUE);
		return;
	}

	DrawStringToHandle(SCREEN_SIZE.x / 2 - 108, SCREEN_SIZE.y / 2 - 34, "FAILED", GetColor(225, 60, 70), largeFont_);
}
