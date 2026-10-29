#pragma once
#include "../../Common/Vector2.h"
#include "ActorBase.h"
#include <DxLib.h>

class ProcessMove;

class Patient : public ActorBase
{
public:

	// 初期座標
	static constexpr Vector2<float> INIT_PER = { 60.0f, 32.0f };

	// 当たり判定（半径）
	static constexpr float COL_RADIUS = 3.0f;

	// 会話反応範囲（Player の位置がこの円に入っていて決定キーを押せば会話開始）
	static constexpr float TALK_RADIUS = 15.0f;

	// コンストラクタ
	Patient();

	// ファイルがある場合の描画
	void Draw() override;

	// 開放
	void Release() override;

private:

	void InitLoad() override;
	void InitTransform() override;
	void InitCollider() override;
	void InitAnimation() override;
	void InitPost() override;

	void UpdateProcessMove() override;
	void UpdateAnimation() override;
};