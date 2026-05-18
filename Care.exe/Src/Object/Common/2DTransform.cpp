#include "2DTransform.h"
#include "../../Utility/AsoUtility.h"

static constexpr int MOVE_MAX_Y = 570;
static constexpr int MOVE_MIN_Y = 210;
static constexpr int MOVE_MINY_MAX_X = 665;
static constexpr int MOVE_MINY_MIN_X = 135;
static constexpr int MOVE_MAXY_MAX_X = 750;
static constexpr int MOVE_MAXY_MIN_X = 50;

// 遠近での最小スケール（遠方でこの値になる）
static constexpr float MOVE_PERSPECTIVE_MIN_SCALE = 0.7f;

namespace {
	// pos: 0..100 圧縮されたパーセンテージを実座標に変換
	inline float PercentToY(float percentY) {
		const float t = AsoUtility::Clamp(percentY, 0.0f, 100.0f) / 100.0f;
		return MOVE_MIN_Y + t * (MOVE_MAX_Y - MOVE_MIN_Y);
	}
	// y実座標に応じた許容する x の最小値
	inline float CalcXMinForY(float y) {
		const float t = (y - MOVE_MIN_Y) / static_cast<float>(MOVE_MAX_Y - MOVE_MIN_Y);
		return MOVE_MINY_MIN_X + t * (MOVE_MAXY_MIN_X - MOVE_MINY_MIN_X);
	}
	// y実座標に応じた許容する x の最大値
	inline float CalcXMaxForY(float y) {
		const float t = (y - MOVE_MIN_Y) / static_cast<float>(MOVE_MAX_Y - MOVE_MIN_Y);
		return MOVE_MINY_MAX_X + t * (MOVE_MAXY_MAX_X - MOVE_MINY_MAX_X);
	}
	// 実座標 x を、y に応じた範囲に収めたときの pos.x(%) を返す
	inline float WorldXToPercentX(float worldX, float y) {
		const float xmin = CalcXMinForY(y);
		const float xmax = CalcXMaxForY(y);
		if (xmax - xmin == 0.0f) return 50.0f;
		const float clamped = AsoUtility::Clamp(worldX, xmin, xmax);
		return (clamped - xmin) / (xmax - xmin) * 100.0f;
	}
}

Transform2D::Transform2D()
	: currentHandleIndex(0)
	, indexX(1)
	, indexY(1)
	, pos(AsoUtility::VECTOR_ZERO)
	, rotDir(0)
{
	handleIds.clear();

	scaleX = 1.0f;
	scaleY = 1.0f;
	beforePos = AsoUtility::VECTOR_ZERO;
	isLeft = false;
	enableImageScaling = true;

	animController.SetFrames(indexX > 0 ? indexX : 1);
	animController.SetFrameDuration(AnimSpeed);
	animController.SetStillIndex(stillIndex);
	animController.SetLoop(true);

	direction = AsoUtility::VECTOR_ZERO;
	imageWidth = 0; imageHeight = 0;
	drawPos1 = AsoUtility::VECTOR_ZERO; drawPos2 = AsoUtility::VECTOR_ZERO;
	perspectiveScale = 1.0f;
}

//Transform2D::Transform2D(const std::vector<int>& handles, int idxX, int idxY, bool scaling)
//	: handleIds(handles)
//	, currentHandleIndex(0)
//	, indexX(idxX > 0 ? idxX : 1)
//	, indexY(idxY > 0 ? idxY : 1)
//	, totalFrames(static_cast<int>(handles.size()))
//	, pos(AsoUtility::VECTOR_ZERO)
//	, rotDir(0)
//	, enableImageScaling(scaling)
//{
//	scaleX = 1.0f;
//	scaleY = 1.0f;
//	AnimSpeed = 0.1f;
//	stillIndex = 0;
//	beforePos = AsoUtility::VECTOR_ZERO;
//	isLeft = false;
//
//	animController.SetFrames(indexX > 0 ? indexX : 1);
//	animController.SetFrameDuration(AnimSpeed);
//	animController.SetStillIndex(stillIndex);
//	animController.SetLoop(true);
//}

Transform2D::~Transform2D()
{
	// handleIds は外部から渡されたハンドル集合をコピーして保持しているが、解放は行わない
	handleIds.clear();
}

void Transform2D::SetHandles(const std::vector<int>& handles, int idxX, int idxY)
{
	handleIds = handles;
	indexX = (idxX > 0) ? idxX : 1;
	indexY = (idxY > 0) ? idxY : 1;
	totalFrames = static_cast<int>(handleIds.size());
	if (indexX * indexY != totalFrames) OutputDebugStringA("indexの数値がおかしいよ！");

	// アニメーション設定を更新
	animController.SetFrames(indexX);
	animController.SetFrameDuration(AnimSpeed);
	animController.SetStillIndex(stillIndex);
}

void Transform2D::Update()
{
	if (handleIds.empty()) return;

	if (currentHandleIndex >= static_cast<int>(handleIds.size()))
	{
		currentHandleIndex = 0; // インデックスが範囲外にならないようにリセット
	}

	// pos は 0..100 のパーセンテージで管理する
	pos.x = AsoUtility::Clamp(pos.x, 0.0f, 100.0f);
	pos.y = AsoUtility::Clamp(pos.y, 0.0f, 100.0f);

	// pos.y に対応する実座標 y を算出
	const float actualY = PercentToY(pos.y);

	// その y に対応する x の可動範囲を算出し、pos.x を補正する
	const float xmin = CalcXMinForY(actualY);
	const float xmax = CalcXMaxForY(actualY);
	// 現在の pos.x -> worldX
	const float worldX = xmin + (pos.x / 100.0f) * (xmax - xmin);
	// worldX を範囲内にクランプし、pos.x を再算出
	const float clampedWorldX = AsoUtility::Clamp(worldX, xmin, xmax);
	pos.x = WorldXToPercentX(clampedWorldX, actualY);
	// pos.y, pos.x はここで 0..100 の有効値に調整済み

}

void Transform2D::Draw()
{
	if (handleIds.empty()) return;

	CalcDrawParams();

	int handle = -1;
	if (!handleIds.empty()) {
		int idx = AsoUtility::Clamp(currentHandleIndex, 0, static_cast<int>(handleIds.size()) - 1);
		handle = handleIds[idx];
	}
	if (handle != -1)
	{
		// 左向きの場合は drawPos の X を入れ替えることで水平反転を実現
		float x1 = drawPos1.x;
		float x2 = drawPos2.x;
		if (isLeft) std::swap(x1, x2);
		DrawExtendGraph(static_cast<int>(x1), static_cast<int>(drawPos1.y),
			static_cast<int>(x2), static_cast<int>(drawPos2.y),
			handle, true);
	}

	// 描画後に現在座標を beforePos に保存（次フレームの比較に使う）
	beforePos = pos;
}

void Transform2D::Delete()
{

}

void Transform2D::CalcDrawParams()
{
	// 画像サイズ取得
	if (!handleIds.empty()) {
		GetGraphSize(handleIds.front(), &imageWidth, &imageHeight);
	}

	// ワールド座標を取得
	VECTOR world = GetWorldPos();

	// 描画サイズ
	float drawW = imageWidth * scaleX;
	float drawH = imageHeight * scaleY;

	if (enableImageScaling) {
		// ★ 遠近法スケールの計算（GetWorldPos ではやらない）
		const float t = (world.y - MOVE_MIN_Y) / (MOVE_MAX_Y - MOVE_MIN_Y);
		perspectiveScale = MOVE_PERSPECTIVE_MIN_SCALE +
			(1.0f - MOVE_PERSPECTIVE_MIN_SCALE) * t;

		drawW *= perspectiveScale;
		drawH *= perspectiveScale;
	}

	// アンカー（足元）から矩形を作る
	drawPos1.x = world.x - drawW * 0.5f;
	drawPos1.y = world.y - drawH;
	drawPos2.x = world.x + drawW * 0.5f;
	drawPos2.y = world.y;
}

VECTOR Transform2D::GetWorldPos() const
{
	VECTOR v = AsoUtility::VECTOR_ZERO;

	const float actualY = PercentToY(pos.y);
	const float xmin = CalcXMinForY(actualY);
	const float xmax = CalcXMaxForY(actualY);
	const float worldX = xmin + (pos.x / 100.0f) * (xmax - xmin);

	v.x = worldX;
	v.y = actualY;
	v.z = 0.0f;

	return v;
}