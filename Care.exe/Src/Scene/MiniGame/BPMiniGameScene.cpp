#include <DxLib.h>
#include <cmath>
#include "../../Manager/KeyConfig.h"
#include "../../Manager/ProgressManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Utility/SchoolUtility.h"
#include "BPMiniGameScene.h"

BPMiniGameScene::BPMiniGameScene()
	: SceneBase()
	, state_(State::INFLATE)
	, pressure_(PRESSURE_MIN)
	, resultTimer_(0.0f)
	, frame_(0)
	, font_(-1)
	, largeFont_(-1)
	, images_()
	, systolic_(0)
	, diastolic_(0)
	, isMonitorAbnormal_(false)
	, isSystolicMeasured_(false)
	, isDeleted_(false)
{
	for (int& image : images_)
	{
		image = -1;
	}
}

BPMiniGameScene::~BPMiniGameScene()
{
	Delete();
}

void BPMiniGameScene::Update()
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

void BPMiniGameScene::Draw()
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

void BPMiniGameScene::Delete()
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

void BPMiniGameScene::InitLoad()
{
	isDeleted_ = false;
	isMonitorAbnormal_ = prgMng_.GetProgress() >= MONITOR_ABNORMAL_PROGRESS;

	font_ = resMng_.LoadFont(ResourceManager::SRC::MAIN_FONT, FONT_SIZE, FONT_THICK);
	largeFont_ = resMng_.LoadFont(ResourceManager::SRC::MAIN_FONT, LARGE_FONT_SIZE, LARGE_FONT_THICK);
	LoadImages();
}

void BPMiniGameScene::LoadImages()
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
		pressure_ = SchoolUtility::Clamp(pressure_, PRESSURE_MIN, PRESSURE_MAX);
	}

	// 規定圧まで加圧するか操作を離した時点で減圧へ移り、測定の入力を受け付ける
	if (KeyConfig::IsTrgUp(KeyConfig::ACTION::DECIDE, iptMng_) || pressure_ >= PRESSURE_MAX)
	{
		state_ = State::DEFLATE;
	}
}

void BPMiniGameScene::UpdateDeflate(float deltaTime)
{
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		// 収縮期・拡張期の検出範囲内で順に入力できた場合だけ測定を完了する
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

	pressure_ = SchoolUtility::Clamp(pressure_ - DEFLATE_SPEED * deltaTime, PRESSURE_MIN, PRESSURE_MAX);

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

	// 初回成功時だけ進行を進める。再挑戦時の進行状態は開始前に復元されている
	if (prgMng_.GetProgressEnum() == ProgressManager::START_MINIGAME0)
	{
		prgMng_.SetProgress(ProgressManager::AFTER_MG);
	}
	sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
}

void BPMiniGameScene::UpdateFailed()
{
	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::DECIDE, iptMng_))
	{
		sndMng_.PlaySE(SE::DECIDE);
		prgMng_.SetProgress(ProgressManager::MINIGAME_RETRY);
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void BPMiniGameScene::Fail()
{
	state_ = State::FAILED;
	sndMng_.PlaySE(SE::BEEP);
}

void BPMiniGameScene::RecordSystolic()
{
	isSystolicMeasured_ = true;
	systolic_ = GetDisplayPressure();
	sndMng_.PlaySE(SE::DECIDE);
}

void BPMiniGameScene::RecordDiastolic()
{
	diastolic_ = GetDisplayPressure();
	state_ = State::SUCCESS;
	resultTimer_ = 0.0f;
	sndMng_.PlaySE(SE::DECIDE);
}

bool BPMiniGameScene::IsSystolicDetectable() const
{
	return state_ == State::DEFLATE &&
		pressure_ <= SYSTOLIC_WINDOW_HIGH &&
		pressure_ >= SYSTOLIC_WINDOW_LOW;
}

bool BPMiniGameScene::IsDiastolicDetectable() const
{
	return state_ == State::DEFLATE &&
		pressure_ <= DIASTOLIC_WINDOW_HIGH &&
		pressure_ >= DIASTOLIC_WINDOW_LOW;
}

bool BPMiniGameScene::IsPulseMoving() const
{
	return state_ == State::DEFLATE &&
		pressure_ <= SYSTOLIC_WINDOW_HIGH &&
		pressure_ > DIASTOLIC_WINDOW_HIGH;
}

bool BPMiniGameScene::IsPulseMissed() const
{
	return state_ == State::DEFLATE && !isSystolicMeasured_ && pressure_ < SYSTOLIC_WINDOW_LOW;
}

bool BPMiniGameScene::IsDiastolicMissed() const
{
	return state_ == State::DEFLATE && isSystolicMeasured_ && pressure_ < DIASTOLIC_WINDOW_LOW;
}

int BPMiniGameScene::GetDisplayPressure() const
{
	// 圧力は非負なので、0.5を加えて整数化し四捨五入する。
	int pressure = static_cast<int>(pressure_ + 0.5f);
	if (isMonitorAbnormal_)
	{
		// 異常演出は表示値だけに揺らぎを加え、測定判定に使う圧力は変えない
		// 3段階（-1、0、+1）を繰り返す。
		pressure += ((frame_ / PRESSURE_JITTER_FRAMES) % 3 - 1) * PRESSURE_JITTER_AMPLITUDE;
	}

	return ClampInt(pressure, 0, DISPLAY_VALUE_MAX);
}
int BPMiniGameScene::GetDisplayPulse() const
{
	if (!IsPulseMoving())
	{
		// 区間番号の偶奇（% 2）で偽の数値と未検出表示を交互に切り替える。
		return (isMonitorAbnormal_ && (frame_ / STOPPED_PULSE_BLINK_FRAMES) % 2 == 0) ? STOPPED_PULSE_VALUE : 0;
	}

	int pulse = PULSE_BASE_VALUE + static_cast<int>(std::sinf(frame_ * PULSE_PHASE_STEP) * PULSE_AMPLITUDE);
	if (isMonitorAbnormal_)
	{
		// 3段階（-1、0、+1）を繰り返す。
		pulse += ((frame_ / ABNORMAL_PULSE_JITTER_FRAMES) % 3 - 1) * ABNORMAL_PULSE_JITTER_AMPLITUDE;
	}
	return ClampInt(pulse, 0, DISPLAY_VALUE_MAX);
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

void BPMiniGameScene::DrawRoom() const
{
	DrawBox(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y, ROOM_COLOR.ToDxColor(), true);
	DrawBox(FLOOR_POS.x, FLOOR_POS.y, FLOOR_POS.x + FLOOR_SIZE.x, FLOOR_POS.y + FLOOR_SIZE.y, FLOOR_COLOR.ToDxColor(), true);
}

void BPMiniGameScene::DrawBackground() const
{
	const int background = images_[static_cast<int>(Image::BACKGROUND)];

	if (background != -1)
	{
		DrawRotaGraph(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2, 1.0, 0.0, background, true);
	}
	else
	{
		DrawBox(BED_POS.x, BED_POS.y, BED_POS.x + BED_SIZE.x, BED_POS.y + BED_SIZE.y, BED_COLOR.ToDxColor(), true);

		DrawBox(BED_SHADOW_POS.x, BED_SHADOW_POS.y, BED_SHADOW_POS.x + BED_SHADOW_SIZE.x, BED_SHADOW_POS.y + BED_SHADOW_SIZE.y, BED_SHADOW_COLOR.ToDxColor(), true);

		DrawOval(FALLBACK_LIMB_CENTER.x, FALLBACK_LIMB_CENTER.y, FALLBACK_LIMB_RADIUS.x, FALLBACK_LIMB_RADIUS.y, LIMB_COLOR.ToDxColor(), true);

		DrawBox(FALLBACK_WRAP_POS.x, FALLBACK_WRAP_POS.y, FALLBACK_WRAP_POS.x + FALLBACK_WRAP_SIZE.x, FALLBACK_WRAP_POS.y + FALLBACK_WRAP_SIZE.y, WRAP_COLOR.ToDxColor(), true);
	}

	DrawPulseMarker();
}

void BPMiniGameScene::DrawMonitor() const
{
	const int monitor = images_[static_cast<int>(Image::MONITOR)];
	if (monitor != -1)
	{
		DrawRotaGraph(MONITOR_POS.x + MONITOR_SIZE.x / 2, MONITOR_POS.y + MONITOR_SIZE.y / 2, 1.0, 0.0, monitor, true);
	}
	else
	{
		DrawBox(MONITOR_POS.x, MONITOR_POS.y, MONITOR_POS.x + MONITOR_SIZE.x, MONITOR_POS.y + MONITOR_SIZE.y, MONITOR_COLOR.ToDxColor(), true);

		DrawBox(MONITOR_POS.x + MONITOR_FRAME_INSET, MONITOR_POS.y + MONITOR_FRAME_INSET, MONITOR_POS.x + MONITOR_SIZE.x - MONITOR_FRAME_INSET,
			MONITOR_POS.y + MONITOR_SIZE.y - MONITOR_FRAME_INSET, MONITOR_FRAME_COLOR.ToDxColor(), false);

		DrawBox(DISPLAY_POS.x, DISPLAY_POS.y, DISPLAY_POS.x + DISPLAY_SIZE.x, DISPLAY_POS.y + DISPLAY_SIZE.y, DISPLAY_COLOR.ToDxColor(), true);

		DrawBox(MONITOR_POS.x + MONITOR_STEM_POS.x, MONITOR_POS.y + MONITOR_STEM_POS.y, MONITOR_POS.x + MONITOR_STEM_POS.x + MONITOR_STEM_SIZE.x,
			MONITOR_POS.y + MONITOR_STEM_POS.y + MONITOR_STEM_SIZE.y, STEM_COLOR.ToDxColor(), true);

		DrawBox(MONITOR_POS.x + MONITOR_BASE_POS.x, MONITOR_POS.y + MONITOR_BASE_POS.y, MONITOR_POS.x + MONITOR_BASE_POS.x + MONITOR_BASE_SIZE.x,
			MONITOR_POS.y + MONITOR_BASE_POS.y + MONITOR_BASE_SIZE.y, BASE_COLOR.ToDxColor(), true);
	}

	DrawPulseWave();

	const int textColor = TEXT_COLOR.ToDxColor();
	const int subColor = SUB_TEXT_COLOR.ToDxColor();
	const int pressure = GetDisplayPressure();
	const int pulse = GetDisplayPulse();
	// 3段階（-1、0、+1）の横揺れ。圧力と脈拍の表示を逆方向に動かす。
	const int jitterOffset = isMonitorAbnormal_ ? ((frame_ / TEXT_JITTER_FRAMES) % 3 - 1) * TEXT_JITTER_PIXELS : 0;

	DrawStringToHandle(DISPLAY_POS.x + PRESSURE_LABEL_OFFSET.x + jitterOffset, DISPLAY_POS.y + PRESSURE_LABEL_OFFSET.y,
		"PRESSURE", subColor, font_);

	DrawFormatStringToHandle(DISPLAY_POS.x + PRESSURE_VALUE_OFFSET.x + jitterOffset, DISPLAY_POS.y + PRESSURE_VALUE_OFFSET.y,
		textColor, largeFont_, "%03d", pressure);

	DrawStringToHandle(DISPLAY_POS.x + PRESSURE_UNIT_OFFSET.x, DISPLAY_POS.y + PRESSURE_UNIT_OFFSET.y, "mmHg", subColor, font_);

	DrawStringToHandle(DISPLAY_POS.x + PULSE_LABEL_OFFSET.x - jitterOffset, DISPLAY_POS.y + PULSE_LABEL_OFFSET.y,
		"PULSE", subColor, font_);

	if (pulse <= 0)
	{
		DrawStringToHandle(DISPLAY_POS.x + PULSE_VALUE_OFFSET.x - jitterOffset, DISPLAY_POS.y + PULSE_VALUE_OFFSET.y,
			"---", textColor, largeFont_);

	}
	else if (isMonitorAbnormal_ && (frame_ / INVALID_PULSE_BLINK_FRAMES) % 2 == 0)
	{
		DrawStringToHandle(DISPLAY_POS.x + PULSE_VALUE_OFFSET.x - jitterOffset, DISPLAY_POS.y + PULSE_VALUE_OFFSET.y,
			"???", ABNORMAL_TEXT_COLOR.ToDxColor(), largeFont_);

	}
	else	{
		DrawFormatStringToHandle(DISPLAY_POS.x + PULSE_VALUE_OFFSET.x - jitterOffset,
			DISPLAY_POS.y + PULSE_VALUE_OFFSET.y, textColor, largeFont_, "%03d", pulse);
	
	}

	DrawStringToHandle(DISPLAY_POS.x + SYSTOLIC_LABEL_OFFSET.x, 
		DISPLAY_POS.y + SYSTOLIC_LABEL_OFFSET.y, "SYS", subColor, font_);
	if (systolic_ <= 0)
	{
		DrawStringToHandle(DISPLAY_POS.x + SYSTOLIC_VALUE_OFFSET.x, DISPLAY_POS.y + SYSTOLIC_VALUE_OFFSET.y, "---", textColor, font_);
	
	}
	else
	{
		DrawFormatStringToHandle(DISPLAY_POS.x + SYSTOLIC_VALUE_OFFSET.x, DISPLAY_POS.y + SYSTOLIC_VALUE_OFFSET.y, textColor, font_, "%03d", systolic_);
	
	}

	DrawStringToHandle(DISPLAY_POS.x + DIASTOLIC_LABEL_OFFSET.x, DISPLAY_POS.y + DIASTOLIC_LABEL_OFFSET.y, "DIA", subColor, font_);
	
	if (diastolic_ <= 0)
	{
		DrawStringToHandle(DISPLAY_POS.x + DIASTOLIC_VALUE_OFFSET.x, DISPLAY_POS.y + DIASTOLIC_VALUE_OFFSET.y, "---", textColor, font_);
	}
	else
	{
		DrawFormatStringToHandle(DISPLAY_POS.x + DIASTOLIC_VALUE_OFFSET.x, DISPLAY_POS.y + DIASTOLIC_VALUE_OFFSET.y, textColor, font_, "%03d", diastolic_);
	}

	if (state_ == State::SUCCESS)
	{
		DrawStringToHandle(DISPLAY_POS.x + OK_RELATIVE_POS.x, DISPLAY_POS.y + OK_RELATIVE_POS.y, "OK", SUCCESS_COLOR.ToDxColor(), largeFont_);
	}

	DrawPressureGauge();
	DrawGlitch();
}

void BPMiniGameScene::DrawPressureGauge() const
{

	const Vector2& gaugePos = PRESSURE_GAUGE_POS;

	const Vector2& gaugeSize = PRESSURE_GAUGE_SIZE;

	const float rate = SchoolUtility::Clamp(pressure_ / PRESSURE_MAX, 0.0f, 1.0f);

	const int fillTop = gaugePos.y + gaugeSize.y - static_cast<int>(gaugeSize.y * rate);

	DrawBox(gaugePos.x, gaugePos.y, gaugePos.x + gaugeSize.x, gaugePos.y + gaugeSize.y, GAUGE_BG_COLOR.ToDxColor(), true);

	DrawBox(gaugePos.x, fillTop, gaugePos.x + gaugeSize.x, gaugePos.y + gaugeSize.y, GAUGE_FILL_COLOR.ToDxColor(), true);

	DrawBox(gaugePos.x - GAUGE_BORDER_WIDTH, gaugePos.y - GAUGE_BORDER_WIDTH, gaugePos.x + gaugeSize.x + GAUGE_BORDER_WIDTH,
		gaugePos.y + gaugeSize.y + GAUGE_BORDER_WIDTH, SUB_TEXT_COLOR.ToDxColor(), false);

}

void BPMiniGameScene::DrawPulseMarker() const
{

	const int pulse = images_[static_cast<int>(Image::PULSE)];

	const bool detectable = IsPulseMoving();

	const float beat = detectable ? std::sinf(frame_ * MARKER_PHASE_STEP) : 0.0f;

	const float beatRange = isMonitorAbnormal_ ? MARKER_ABNORMAL_AMPLITUDE : MARKER_NORMAL_AMPLITUDE;

	const Vector2 markerPos =
	{
		PULSE_MARKER_POS.x,
		PULSE_MARKER_POS.y + static_cast<int>(beat * beatRange)
	};
	if (pulse != -1)
	{
		DrawRotaGraph(markerPos.x, markerPos.y, 1.0, 0.0, pulse, true);
		return;
	}

	const int color = detectable ? PULSE_ACTIVE_COLOR.ToDxColor() : PULSE_IDLE_COLOR.ToDxColor();
	const int radius = detectable ? MARKER_ACTIVE_RADIUS + static_cast<int>(std::fabs(beat) * MARKER_RADIUS_AMPLITUDE) : MARKER_IDLE_RADIUS;
	DrawCircle(markerPos.x, markerPos.y, radius, color, true);
}

void BPMiniGameScene::DrawPulseWave() const
{
	const Vector2 waveStart = { DISPLAY_POS.x + PULSE_WAVE_RELATIVE_POS.x, DISPLAY_POS.y + PULSE_WAVE_RELATIVE_POS.y };
	const bool detectable = IsPulseMoving();
	Vector2 prevPos = waveStart;

	for (int i = 1; i <= WAVE_SAMPLE_COUNT; ++i)
	{
		Vector2 linePos = { waveStart.x + i, waveStart.y };
		float amp = 0.0f;
		if (detectable)
		{
			amp = std::sinf((frame_ + i) * WAVE_PHASE_STEP) * (isMonitorAbnormal_ ? WAVE_ABNORMAL_AMPLITUDE : WAVE_NORMAL_AMPLITUDE);
			if ((i + frame_) % WAVE_SPIKE_PERIOD_FRAMES < WAVE_SPIKE_WIDTH_FRAMES)
			{
				amp *= WAVE_SPIKE_GAIN;
			}
		}
		else if (isMonitorAbnormal_)
		{
			amp = std::sinf((frame_ + i) * WAVE_IDLE_PHASE_STEP) * WAVE_IDLE_AMPLITUDE;
		}

		linePos.y -= static_cast<int>(amp);
		DrawLine(prevPos.x, prevPos.y, linePos.x, linePos.y, WAVE_COLOR.ToDxColor(), WAVE_LINE_WIDTH);
		prevPos = linePos;
	}
}

void BPMiniGameScene::DrawGlitch() const
{
	if (!isMonitorAbnormal_)
	{
		return;
	}

	const int lineCount = GLITCH_LINE_COUNT;
	for (int i = 0; i < lineCount; ++i)
	{
		const Vector2 glitchPos =
		{
			DISPLAY_POS.x + GLITCH_X_INSET + (i * GLITCH_X_STRIDE + frame_) % GLITCH_X_RANGE,
			DISPLAY_POS.y + GLITCH_Y_INSET + (frame_ * (i + GLITCH_SPEED_BIAS) + i * GLITCH_Y_STRIDE) % (DISPLAY_SIZE.y - GLITCH_Y_MARGIN)
		};
		const int glitchLength = GLITCH_MIN_LENGTH + (i * GLITCH_LENGTH_STRIDE + GLITCH_LENGTH_OFFSET) % GLITCH_LENGTH_RANGE;
		const int color = (i % 2 == 0) ? GLITCH_EVEN_COLOR.ToDxColor() : GLITCH_ODD_COLOR.ToDxColor();
		DrawBox(glitchPos.x, glitchPos.y, MinInt(glitchPos.x + glitchLength, DISPLAY_POS.x + DISPLAY_SIZE.x - GLITCH_X_INSET), glitchPos.y + GLITCH_LINE_HEIGHT, color, true);
	}

	if ((frame_ / ERROR_BLINK_FRAMES) % 2 == 0)
	{
		DrawStringToHandle(DISPLAY_POS.x + ERROR_TEXT_OFFSET.x, DISPLAY_POS.y + ERROR_TEXT_OFFSET.y, "ERR", ERROR_COLOR.ToDxColor(), font_);
	}
}
void BPMiniGameScene::DrawFailed() const
{
	DrawBox(0, 0, SCREEN_SIZE.x, SCREEN_SIZE.y, BLACK_COLOR.ToDxColor(), true);

	const int failed = images_[static_cast<int>(Image::FAILED)];
	if (failed != -1)
	{
		DrawRotaGraph(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2, 1.0, 0.0, failed, true);
		return;
	}

	DrawStringToHandle(SCREEN_SIZE.x / 2 - FAILED_TEXT_CENTER_OFFSET.x, SCREEN_SIZE.y / 2 - FAILED_TEXT_CENTER_OFFSET.y, "FAILED", FAILED_COLOR.ToDxColor(), largeFont_);
}
