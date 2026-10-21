#pragma once
#include "../../Application.h"
#include "../../Common/Vector2.h"
#include "../SceneBase.h"

class BPMiniGameScene : public SceneBase
{
public:
	BPMiniGameScene();
	~BPMiniGameScene() override;

	void Update() override;
	void Draw() override;
	void Delete() override;
	static constexpr Vector2 OK_RELATIVE_POS = { 178, 18 };
	static constexpr Vector2 PULSE_WAVE_RELATIVE_POS = { 154, 46 };

private:
	struct Rgb
	{
		int r, g, b;
		int ToDxColor() const { return GetColor(r, g, b); }
	};
	static constexpr float DISPLAY_ROUNDING_OFFSET = 0.5f;
	static constexpr int DISPLAY_VALUE_MAX = 199;
	static constexpr int PRESSURE_JITTER_FRAMES = 10;
	static constexpr int JITTER_STATE_COUNT = 3;
	static constexpr int JITTER_CENTER_INDEX = 1;
	static constexpr int PRESSURE_JITTER_AMPLITUDE = 4;
	static constexpr int BLINK_PHASE_COUNT = 2;
	static constexpr int STOPPED_PULSE_BLINK_FRAMES = 20;
	static constexpr int STOPPED_PULSE_VALUE = 69;
	static constexpr int PULSE_BASE_VALUE = 72;
	static constexpr float PULSE_PHASE_STEP = 0.18f;
	static constexpr float PULSE_AMPLITUDE = 4.0f;
	static constexpr int ABNORMAL_PULSE_JITTER_FRAMES = 9;
	static constexpr int ABNORMAL_PULSE_JITTER_AMPLITUDE = 6;
	static constexpr int TEXT_JITTER_FRAMES = 8;
	static constexpr int TEXT_JITTER_PIXELS = 2;
	static constexpr int INVALID_PULSE_BLINK_FRAMES = 10;
	static constexpr int MONITOR_FRAME_INSET = 10;
	static constexpr int GAUGE_BORDER_WIDTH = 2;
	static constexpr float MARKER_PHASE_STEP = 0.42f;
	static constexpr float MARKER_ABNORMAL_AMPLITUDE = 14.0f;
	static constexpr float MARKER_NORMAL_AMPLITUDE = 10.0f;
	static constexpr int MARKER_ACTIVE_RADIUS = 9;
	static constexpr float MARKER_RADIUS_AMPLITUDE = 5.0f;
	static constexpr int MARKER_IDLE_RADIUS = 7;
	static constexpr int WAVE_SAMPLE_COUNT = 92;
	static constexpr float WAVE_PHASE_STEP = 0.25f;
	static constexpr float WAVE_ABNORMAL_AMPLITUDE = 20.0f;
	static constexpr float WAVE_NORMAL_AMPLITUDE = 14.0f;
	static constexpr int WAVE_SPIKE_PERIOD_FRAMES = 24;
	static constexpr int WAVE_SPIKE_WIDTH_FRAMES = 4;
	static constexpr float WAVE_SPIKE_GAIN = 2.0f;
	static constexpr float WAVE_IDLE_PHASE_STEP = 0.12f;
	static constexpr float WAVE_IDLE_AMPLITUDE = 2.0f;
	static constexpr int WAVE_LINE_WIDTH = 2;
	static constexpr int GLITCH_LINE_COUNT = 4;
	static constexpr int GLITCH_X_INSET = 8;
	static constexpr int GLITCH_X_STRIDE = 37;
	static constexpr int GLITCH_X_RANGE = 40;
	static constexpr int GLITCH_Y_INSET = 14;
	static constexpr int GLITCH_SPEED_BIAS = 3;
	static constexpr int GLITCH_Y_STRIDE = 29;
	static constexpr int GLITCH_Y_MARGIN = 28;
	static constexpr int GLITCH_MIN_LENGTH = 60;
	static constexpr int GLITCH_LENGTH_STRIDE = 23;
	static constexpr int GLITCH_LENGTH_OFFSET = 34;
	static constexpr int GLITCH_LENGTH_RANGE = 140;
	static constexpr int GLITCH_LINE_HEIGHT = 3;
	static constexpr int ERROR_BLINK_FRAMES = 24;
	static constexpr Rgb ROOM_COLOR = { 225, 229, 232 };
	static constexpr Rgb FLOOR_COLOR = { 204, 214, 218 };
	static constexpr Rgb BED_COLOR = { 238, 242, 242 };
	static constexpr Rgb BED_SHADOW_COLOR = { 178, 189, 194 };
	static constexpr Rgb LIMB_COLOR = { 235, 186, 156 };
	static constexpr Rgb WRAP_COLOR = { 38, 55, 71 };
	static constexpr Rgb MONITOR_COLOR = { 34, 38, 42 };
	static constexpr Rgb MONITOR_FRAME_COLOR = { 72, 78, 84 };
	static constexpr Rgb DISPLAY_COLOR = { 8, 28, 26 };
	static constexpr Rgb STEM_COLOR = { 46, 52, 56 };
	static constexpr Rgb BASE_COLOR = { 44, 49, 53 };
	static constexpr Rgb TEXT_COLOR = { 170, 245, 214 };
	static constexpr Rgb SUB_TEXT_COLOR = { 112, 178, 154 };
	static constexpr Rgb ABNORMAL_TEXT_COLOR = { 255, 120, 120 };
	static constexpr Rgb SUCCESS_COLOR = { 120, 235, 130 };
	static constexpr Rgb GAUGE_BG_COLOR = { 18, 58, 52 };
	static constexpr Rgb GAUGE_FILL_COLOR = { 112, 228, 183 };
	static constexpr Rgb PULSE_ACTIVE_COLOR = { 235, 62, 74 };
	static constexpr Rgb PULSE_IDLE_COLOR = { 116, 101, 104 };
	static constexpr Rgb WAVE_COLOR = { 96, 230, 170 };
	static constexpr Rgb GLITCH_EVEN_COLOR = { 42, 116, 114 };
	static constexpr Rgb GLITCH_ODD_COLOR = { 112, 42, 70 };
	static constexpr Rgb ERROR_COLOR = { 235, 76, 90 };
	static constexpr Rgb FAILED_COLOR = { 225, 60, 70 };
	static constexpr Rgb BLACK_COLOR = { 0, 0, 0 };

	enum class State
	{
		INFLATE,
		DEFLATE,
		SUCCESS,
		FAILED,
	};

	enum class Image
	{
		BACKGROUND,
		MONITOR,
		PULSE,
		FAILED,
		INTRUCT,
		MAX
	};

	static constexpr int FONT_SIZE = 24;
	static constexpr int LARGE_FONT_SIZE = 42;
	static constexpr int FONT_THICK = 3;
	static constexpr int LARGE_FONT_THICK = 5;
	static constexpr float PRESSURE_MIN = 0.0f;
	static constexpr float PRESSURE_MAX = 180.0f;
	static constexpr float INFLATE_SPEED = 72.0f;
	static constexpr float DEFLATE_SPEED = 5.0f;
	static constexpr float SYSTOLIC_WINDOW_HIGH = 124.0f;
	static constexpr float SYSTOLIC_WINDOW_LOW = 116.0f;
	static constexpr float DIASTOLIC_WINDOW_HIGH = 82.0f;
	static constexpr float DIASTOLIC_WINDOW_LOW = 74.0f;
	static constexpr float SUCCESS_WAIT_SEC = 0.6f;
	static constexpr int MONITOR_ABNORMAL_PROGRESS = 8;
	static constexpr Vector2 SCREEN_SIZE = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y };
	static constexpr Vector2 MONITOR_POS = { 240, 64 };
	static constexpr Vector2 MONITOR_SIZE = { 320, 250 };
	static constexpr Vector2 DISPLAY_POS = { MONITOR_POS.x + 28, MONITOR_POS.y + 30 };
	static constexpr Vector2 DISPLAY_SIZE = { MONITOR_SIZE.x - 56, 178 };
	static constexpr Vector2 PULSE_MARKER_POS = { 394, 314 };
	static constexpr Vector2 FLOOR_POS = { 0, 418 };
	static constexpr Vector2 FLOOR_SIZE = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y - FLOOR_POS.y };
	static constexpr Vector2 BED_POS = { 86, 350 };
	static constexpr Vector2 BED_SIZE = { 358, 138 };
	static constexpr Vector2 BED_SHADOW_POS = { 86, 480 };
	static constexpr Vector2 BED_SHADOW_SIZE = { 358, 20 };
	static constexpr Vector2 FALLBACK_LIMB_CENTER = { 250, 380 };
	static constexpr Vector2 FALLBACK_LIMB_RADIUS = { 180, 48 };
	static constexpr Vector2 FALLBACK_WRAP_POS = { 244, 316 };
	static constexpr Vector2 FALLBACK_WRAP_SIZE = { 118, 118 };
	static constexpr int IMAGE_NUM = static_cast<int>(Image::MAX);
	static constexpr Vector2 MONITOR_STEM_POS = { 130, MONITOR_SIZE.y };
	static constexpr Vector2 MONITOR_STEM_SIZE = { 60, 50 };
	static constexpr Vector2 MONITOR_BASE_POS = { 82, MONITOR_SIZE.y + 48 };
	static constexpr Vector2 MONITOR_BASE_SIZE = { 156, 20 };
	static constexpr Vector2 PRESSURE_LABEL_OFFSET = { 18, 14 };
	static constexpr Vector2 PRESSURE_VALUE_OFFSET = { 24, 42 };
	static constexpr Vector2 PRESSURE_UNIT_OFFSET = { 158, 68 };
	static constexpr Vector2 PULSE_LABEL_OFFSET = { 18, 98 };
	static constexpr Vector2 PULSE_VALUE_OFFSET = { 28, 120 };
	static constexpr Vector2 SYSTOLIC_LABEL_OFFSET = { 142, 112 };
	static constexpr Vector2 SYSTOLIC_VALUE_OFFSET = { 196, 112 };
	static constexpr Vector2 DIASTOLIC_LABEL_OFFSET = { 142, 142 };
	static constexpr Vector2 DIASTOLIC_VALUE_OFFSET = { 196, 142 };
	static constexpr Vector2 PRESSURE_GAUGE_POS = { MONITOR_POS.x + MONITOR_SIZE.x - 42, DISPLAY_POS.y + 18 };
	static constexpr Vector2 PRESSURE_GAUGE_SIZE = { 10, DISPLAY_SIZE.y - 36 };
	static constexpr Vector2 ERROR_TEXT_OFFSET = { 154, 20 };
	static constexpr Vector2 FAILED_TEXT_CENTER_OFFSET = { 108, 34 };


	void InitLoad() override;
	void LoadImages();
	void UpdateInflate(float deltaTime);
	void UpdateDeflate(float deltaTime);
	void UpdateSuccess(float deltaTime);
	void UpdateFailed();
	void Fail();
	void RecordSystolic();
	void RecordDiastolic();

	bool IsSystolicDetectable() const;
	bool IsDiastolicDetectable() const;
	bool IsPulseMoving() const;
	bool IsPulseMissed() const;
	bool IsDiastolicMissed() const;
	int GetDisplayPressure() const;
	int GetDisplayPulse() const;

	void DrawRoom() const;
	void DrawBackground() const;
	void DrawMonitor() const;
	void DrawPressureGauge() const;
	void DrawPulseMarker() const;
	void DrawPulseWave() const;
	void DrawGlitch() const;
	void DrawFailed() const;

	static int ClampInt(int value, int minValue, int maxValue);
	static int MaxInt(int lhs, int rhs);
	static int MinInt(int lhs, int rhs);

	State state_;
	float pressure_;
	float resultTimer_;
	int frame_;
	int font_;
	int largeFont_;
	int images_[IMAGE_NUM];
	int systolic_;
	int diastolic_;
	bool isMonitorAbnormal_;
	bool isSystolicMeasured_;
	bool isDeleted_;

};
