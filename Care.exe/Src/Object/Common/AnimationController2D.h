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

    void SetLoop(bool loop);
    void SetPingPong(bool pingpong); // 追加: 往復再生
    void SetStillIndex(int idx);

    void Play();   // 再生（動作状態）
    void Stop();   // 停止（フレーム固定）
    void Reset();  // インデックスを先頭に戻す

    // 毎フレーム呼ぶ（内部で GetNowCount() を使って時間差で進める）
    void Update();

    // 0..frames-1 の現在フレームオフセット
    int GetFrame() const;
    bool IsPlaying() const;

private:
    int frames_;
    int currentFrame_;
    int stillIndex_;
    bool loop_;
    bool pingpong_;
    int direction_;
    bool playing_;
    unsigned int lastTimeMs_;
    float frameDurationMs_;
};