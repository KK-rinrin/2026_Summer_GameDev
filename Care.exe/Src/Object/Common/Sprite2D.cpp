#include "Sprite2D.h"
#include "../../Utility/SchoolUtility.h"
#include <algorithm>

Sprite2D::Sprite2D(int handle, bool applyScaling)
	: enableImageScaling_(applyScaling)
{
	if (handle != -1) SetHandles({ handle }, 1, 1);
}

void Sprite2D::SetHandles(const std::vector<int>& handles, int columns, int rows)
{
	handles_ = handles;
	columns_ = columns > 0 ? columns : 1;
	rows_ = rows > 0 ? rows : 1;
	currentFrame_ = 0;
	if (!handles_.empty() && columns_ * rows_ != GetFrameCount())
		OutputDebugStringA("スプライトの分割数と画像ハンドル数が一致していません。\n");
}

void Sprite2D::SetFrame(int frame)
{
	currentFrame_ = handles_.empty() ? 0 : SchoolUtility::Clamp(frame, 0, GetFrameCount() - 1);
}

void Sprite2D::Draw(const Vector2<float>& worldPos, float scaleX, float scaleY, float perspectiveScale) const
{
	if (handles_.empty()) return;
	const int handle = handles_[currentFrame_];
	if (handle == -1) return;

	int imageWidth = 0;
	int imageHeight = 0;
	GetGraphSize(handles_.front(), &imageWidth, &imageHeight);
	const float perspective = enableImageScaling_ ? perspectiveScale : 1.0f;
	const float drawW = imageWidth * scaleX * perspective;
	const float drawH = imageHeight * scaleY * perspective;
	float left = worldPos.x - drawW * 0.5f;
	float right = worldPos.x + drawW * 0.5f;
	if (flipX_) std::swap(left, right);
	DrawExtendGraph(static_cast<int>(left), static_cast<int>(worldPos.y - drawH),
		static_cast<int>(right), static_cast<int>(worldPos.y), handle, true);
}
