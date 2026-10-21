#include <DxLib.h>
#include "FpsControl.h"
#include "../Application.h"

class Application;

// コンストラクタ
FpsControl::FpsControl()
	: currentTime_(0)
	, prevFrameTime_(0)
	, frameCount_(0)
	, updateFrameRateTime_(0)
	, frameRate_(0.0f)
{

}

// デストラクタ
FpsControl::~FpsControl()
{
}

void FpsControl::Init()
{
	currentTime_ = 0;
	prevFrameTime_ = 0;
	frameCount_ = 0;
	updateFrameRateTime_ = 0;
	frameRate_ = 0.0f;
}

// フレームレート更新
bool FpsControl::UpdateFrameRate()
{
	Sleep(YIELD_SLEEP_MS);	// システムに処理を返す

	// 現在の時刻を取得
	currentTime_ = GetNowCount();

	// 現在の時刻が、前回のフレーム実行時より
	// 1/60秒経過していたら処理を実行する
	if (currentTime_ - prevFrameTime_ >= FRAME_INTERVAL_MS)
	{
		// フレーム実行時の時間を更新
		prevFrameTime_ = currentTime_;

		// フレーム数をカウント
		frameCount_++;

		// 1/60秒経過した
		return true;
	}

	return false;
}

// フレームレート計算
void FpsControl::CalcFrameRate()
{
	// 前回のフレームレート更新からの経過時間を求める
	int difTime = currentTime_ - updateFrameRateTime_;

	// 前回のフレームレートの更新から
	// 1秒以上経過していたらフレームレートを更新する
	if (difTime > MILLISECONDS_PER_SECOND)
	{
		// フレーム回数をミリ秒に合わせる
		// 小数まで出したのでfloatにキャストする
		float castFrameCount = static_cast<float>(frameCount_ * MILLISECONDS_PER_SECOND);

		// フレームレートを求める
		frameRate_ = castFrameCount / difTime;

		// フレームカウントをクリア
		frameCount_ = 0;

		// フレームレート更新時間を更新
		updateFrameRateTime_ = currentTime_;
	}
}

// フレームレート描画(デバッグ用)
void FpsControl::DrawFrameRate()
{
	// フレームレートを表示
	DrawFormatString(
		Application::SCREEN_SIZE_X - FPS_RIGHT_MARGIN,	// x座標
		Application::SCREEN_SIZE_Y - FPS_BOTTOM_MARGIN,	// y座標
		FPS_TEXT_COLOR,			// 文字の色
		"FPS[%.2f]",		// FPS表示
		frameRate_			// フレームレート変数
	);
}
