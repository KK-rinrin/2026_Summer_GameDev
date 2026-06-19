#pragma once
#include "../../Application.h"
#include "../../Common/Vector2.h"
#include "../SceneBase.h"

class BPMiniGameScene : public SceneBase
{
public:
	BPMiniGameScene(void);
	~BPMiniGameScene(void) override;

	void Update(void) override;
	void Draw(void) override;
	void Delete(void) override;

private:
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

	static constexpr float PRESSURE_MIN = 0.0f;
	static constexpr float PRESSURE_MAX = 180.0f;
	static constexpr float INFLATE_SPEED = 72.0f;
	static constexpr float DEFLATE_SPEED = 5.0f;
	static constexpr float SYSTOLIC_WINDOW_HIGH = 124.0f;
	static constexpr float SYSTOLIC_WINDOW_LOW = 116.0f;
	static constexpr float DIASTOLIC_WINDOW_HIGH = 82.0f;
	static constexpr float DIASTOLIC_WINDOW_LOW = 74.0f;
	static constexpr float SUCCESS_WAIT_SEC = 0.6f;
	static constexpr int MONITOR_ABNORMAL_PROGRESS = 7;
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

	void InitLoad(void) override;
	void LoadImages(void);
	void UpdateInflate(float deltaTime);
	void UpdateDeflate(float deltaTime);
	void UpdateSuccess(float deltaTime);
	void UpdateFailed(void);
	void Fail(void);
	void RecordSystolic(void);
	void RecordDiastolic(void);

	bool IsSystolicDetectable(void) const;
	bool IsDiastolicDetectable(void) const;
	bool IsPulseMoving(void) const;
	bool IsPulseMissed(void) const;
	bool IsDiastolicMissed(void) const;
	int GetDisplayPressure(void) const;
	int GetDisplayPulse(void) const;

	void DrawRoom(void) const;
	void DrawBackground(void) const;
	void DrawMonitor(void) const;
	void DrawPressureGauge(void) const;
	void DrawPulseMarker(void) const;
	void DrawPulseWave(void) const;
	void DrawGlitch(void) const;
	void DrawFailed(void) const;

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
