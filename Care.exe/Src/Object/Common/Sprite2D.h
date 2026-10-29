#pragma once
#include "../../Common/Vector2.h"
#include <DxLib.h>
#include <vector>

// 画像ハンドルは Resource が所有し、このクラスでは解放しない。
class Sprite2D
{
public:
	Sprite2D() = default;
	explicit Sprite2D(int handle, bool applyScaling = true);

	// 分割画像ハンドルとシートの列数と行数の設定
	void SetHandles(const std::vector<int>& handles, int columns, int rows);

	// 表示するコマ番号の設定
	void SetFrame(int frame);

	// 左右反転の有効状態の設定
	void SetFlipX(bool flip) { flipX_ = flip; }

	// 左右反転状態の取得
	bool IsFlippedX() const { return flipX_; }

	// 画像シートの列数の取得
	int GetColumns() const { return columns_; }

	// 画像シートの行数の取得
	int GetRows() const { return rows_; }

	// 保持する画像の総コマ数の取得
	int GetFrameCount() const { return static_cast<int>(handles_.size()); }
	void Draw(const Vector2<float>& worldPos, float scaleX, float scaleY, float perspectiveScale) const;

private:

	// 分割画像のハンドル一覧
	std::vector<int> handles_;

	// 画像シートの列数
	int columns_ = 1;

	// 画像シートの行数
	int rows_ = 1;

	// 現在表示するコマ番号
	int currentFrame_ = 0;

	// 左右反転の有効状態
	bool flipX_ = false;

	// 画像自体の拡大縮小の有効状態
	bool enableImageScaling_ = true;
};
