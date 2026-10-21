#pragma once
#include <DxLib.h>
#include <vector>

// 画像ハンドルは Resource が所有し、このクラスでは解放しない。
class Sprite2D
{
public:
	Sprite2D() = default;
	explicit Sprite2D(int handle, bool applyScaling = true);
	void SetHandles(const std::vector<int>& handles, int columns, int rows);
	void SetFrame(int frame);
	void SetFlipX(bool flip) { flipX_ = flip; }
	bool IsFlippedX() const { return flipX_; }
	int GetColumns() const { return columns_; }
	int GetRows() const { return rows_; }
	int GetFrameCount() const { return static_cast<int>(handles_.size()); }
	void Draw(const VECTOR& worldPos, float scaleX, float scaleY, float perspectiveScale) const;

private:
	std::vector<int> handles_;
	int columns_ = 1;
	int rows_ = 1;
	int currentFrame_ = 0;
	bool flipX_ = false;
	bool enableImageScaling_ = true;
};
