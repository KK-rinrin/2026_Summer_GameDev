#pragma once
#include <DxLib.h>

class AnimationController2D
{
public:

	AnimationController2D();

	// フレーム数（1 以上）
	void SetFrames(int frames);

	// 1フレームあたりの秒数（秒）
	void SetFrameDuration(float seconds);

	void SetLoop(bool loop);	// 始めと終わりをつなげる
	void SetPingPong(bool pingpong); // 往復再生

	// 停止中に表示するフレーム番号の設定
	void SetStillIndex(int idx);

	void Play();   // 再生（動作状態）
	void Stop();   // 停止（フレーム固定）
	void Reset();  // インデックスを先頭に戻す

	// 毎フレーム呼ぶ（内部で GetNowCount() を使って時間差で進める）
	void Update();

	// 0..frames-1 の現在フレームオフセット
	int GetFrame() const;

	// アニメーション再生中の判定（未使用）
	bool IsPlaying() const;

private:

	// 1コマ当たりの既定表示時間のミリ秒数
	static constexpr float DEFAULT_FRAME_DURATION_MS = 100.0f;

	// 秒からミリ秒への換算倍率
	static constexpr float MILLISECONDS_PER_SECOND = 1000.0f;

	// アニメーションの総コマ数
	int frames_;

	// 現在のアニメーションコマ番号
	int currentFrame_;

	// 停止中に表示するコマ番号
	int stillIndex_;

	// 繰り返し再生の有効状態
	bool loop_;

	// 往復再生の有効状態
	bool pingpong_;

	// コマを進める方向
	int direction_;

	// 再生中の状態
	bool playing_;

	// 前回コマ更新時刻のミリ秒値
	unsigned int lastTimeMs_;

	// 1コマ当たりの表示時間のミリ秒数
	float frameDurationMs_;

};
