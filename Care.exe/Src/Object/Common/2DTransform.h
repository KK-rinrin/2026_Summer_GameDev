#pragma once
#include <DxLib.h>
#include <vector>
#include "AnimationController2D.h"

/// 2D座標、回転、拡大を管理するクラス
/// 2D画像をモデルとして扱う。
/// ここでは疑似3D（遠近）表現を用いる。
/// スプライト画像も担保
class Transform2D
{
public:
	// 描画に使用する画像ハンドルの配列（外部所有を想定するが内部でコピーして保持）
	std::vector<int> handleIds; // 画像ハンドル集合（vector化）

	int currentHandleIndex; // 現在描画に使用している画像ハンドルのインデックス

	int indexX; // 画像の分割数X（スプライトシートの場合）
	int indexY; // 画像の分割数Y（スプライトシートの場合）

	int totalFrames; // アニメーションの総フレーム数（旧実装の意図を保持）
	float AnimSpeed;	// アニメーションのコマ送りの速さ（秒単位）
	int stillIndex;			// isAnim==0のときの静止時の画像
	int stillIndex_back;	// isAnim==0のときの静止画像（後方）

	VECTOR pos;		// 座標（xyz）※zは未使用  <-- pos は 0..100 の百分率空間
	VECTOR beforePos; // 前回の座標

	int rotDir;		// 前:0 or 後ろ:1
	bool isLeft; // 左向きかどうか（回転方向に応じて計算される）
	bool enableImageScaling = true;
	float scaleX;	// 拡大率X
	float scaleY;	// 拡大率Y

	// アニメーションコントローラ
	AnimationController2D animController;

	// コンストラクタ
	Transform2D();

	// デストラクタ
	~Transform2D();

	// 画像ハンドルと分割数を指定して初期化
	Transform2D(const int handleId, VECTOR pos, bool applyScaling = true, int idxX = 1, int idxY = 1);

	// 外部配列を設定（コピーする）
	void SetHandles(const std::vector<int>& handles, int idxX, int idxY);

	void Update();

	void Draw();

	void Delete();

	VECTOR GetWorldPos() const;
	static VECTOR WorldToLocalPercent(const VECTOR& worldPos);
	void BlockCrossingWorldY(float wallY, float thickness);

	// 追加：ローカル百分率矩形での通過防止（leftTopPercent / rightBottomPercent は VECTOR、0..100）
	void BlockCrossingLocalRect(const VECTOR& leftTopPercent, const VECTOR& rightBottomPercent);

private:
	void CalcDrawParams();

	VECTOR direction; // 移動量

	int imageWidth;  // 画像の幅（描画計算に使用）
	int imageHeight; // 画像の高さ（描画計算に使用）

	VECTOR drawPos1; // 描画位置1（右上）
	VECTOR drawPos2; // 描画位置2（左下）

	// 遠近法関連
	float perspectiveScale; // 遠近法による拡大率
};