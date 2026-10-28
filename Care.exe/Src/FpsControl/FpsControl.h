#pragma once

// フレームレート

class FpsControl
{
public:
	FpsControl();	// コンストラクタ
	~FpsControl();	// デストラクタ

	// 初期化
	void Init();

	// フレームレート更新
	bool UpdateFrameRate();

	// フレームレート計算
	void CalcFrameRate();

	// フレームレート表示(デバッグ表示)
	void DrawFrameRate();

private:

	// 1秒当たりのミリ秒数
	static constexpr int MILLISECONDS_PER_SECOND = 1000;

	// 目標フレームレート
	static constexpr int TARGET_FPS = 60;
	// 整数除算による現在の値（16ミリ秒）を維持する。
	static constexpr float FRAME_INTERVAL_MS = MILLISECONDS_PER_SECOND / TARGET_FPS;

	// フレーム待機中のスリープ時間のミリ秒数
	static constexpr int YIELD_SLEEP_MS = 1;

	// FPS表示の画面右端からの余白
	static constexpr int FPS_RIGHT_MARGIN = 90;

	// FPS表示の画面下端からの余白
	static constexpr int FPS_BOTTOM_MARGIN = 20;

	// FPS表示の文字色
	static constexpr int FPS_TEXT_COLOR = 0xFF0000;

	int currentTime_;			// 現在の時間
	int prevFrameTime_;			// 前回のフレーム実行時の時間

	int frameCount_;			// フレームカウント用
	int updateFrameRateTime_;	// フレームレートを更新した時間

	float frameRate_;			// フレームレート
};
