#include "AnimationController2D.h"
#include "../../Utility/AsoUtility.h"

AnimationController2D::AnimationController2D()
    : frames_(1)
    , currentFrame_(0)
    , stillIndex_(0)
    , loop_(true)
    , pingpong_(false)
    , direction_(1)
    , playing_(false)
    , lastTimeMs_(GetNowCount())
    , frameDurationMs_(100.0f) // 100ms default
{
}

void AnimationController2D::SetFrames(int frames)
{
    frames_ = (frames > 0) ? frames : 1;
    currentFrame_ = 0;
    direction_ = 1;
}

void AnimationController2D::SetFrameDuration(float seconds)
{
    frameDurationMs_ = (seconds > 0.0f) ? (seconds * 1000.0f) : 100.0f;
}

void AnimationController2D::SetLoop(bool loop)
{
    loop_ = loop;
}

void AnimationController2D::SetPingPong(bool pingpong)
{
    pingpong_ = pingpong;
}

void AnimationController2D::SetStillIndex(int idx)
{
    stillIndex_ = (idx >= 0) ? idx : 0;
}

void AnimationController2D::Play()
{
    if (!playing_) {
        playing_ = true;
        lastTimeMs_ = GetNowCount();
    }
}

void AnimationController2D::Stop()
{
    playing_ = false;
    currentFrame_ = AsoUtility::Clamp(stillIndex_, 0, frames_ - 1);
    direction_ = 1;
}

void AnimationController2D::Reset()
{
    currentFrame_ = 0;
    direction_ = 1;
    lastTimeMs_ = GetNowCount();
}

void AnimationController2D::Update()
{
    if (!playing_ || frames_ <= 1) return;

    unsigned int now = GetNowCount();
    unsigned int elapsed = now - lastTimeMs_;
    if (elapsed < 1u) return;

    // 経過フレーム数を逐次更新（ping-pong の場合は1フレームずつ処理）
    unsigned int frameMs = static_cast<unsigned int>(frameDurationMs_);
    while (elapsed >= frameMs) {
        elapsed -= frameMs;
        // advance one step according to mode
        if (pingpong_) {
            currentFrame_ += direction_;
            if (currentFrame_ >= frames_) {
                // reached after-last
                if (frames_ <= 1) {
                    currentFrame_ = 0;
                }
                else {
                    currentFrame_ = frames_ - 2; // bounce back
                    direction_ = -1;
                    if (!loop_ && currentFrame_ < 0) currentFrame_ = 0;
                }
            }
            else if (currentFrame_ < 0) {
                // reached before-first
                if (frames_ <= 1) {
                    currentFrame_ = 0;
                }
                else {
                    currentFrame_ = 1;
                    direction_ = 1;
                    if (!loop_ && currentFrame_ >= frames_) currentFrame_ = frames_ - 1;
                }
            }
        }
        else {
            // normal forward
            currentFrame_++;
            if (loop_) {
                currentFrame_ %= frames_;
            }
            else {
                if (currentFrame_ >= frames_) currentFrame_ = frames_ - 1;
            }
        }
    }

    // 消費した分だけ lastTimeMs_ を進める
    lastTimeMs_ = now - elapsed;
}

int AnimationController2D::GetFrame() const
{
    if (frames_ <= 0) return 0;
    // ensure in-range
    int f = currentFrame_;
    if (f < 0) f = 0;
    if (f >= frames_) f = frames_ - 1;
    return f;
}

bool AnimationController2D::IsPlaying() const
{
    return playing_;
}