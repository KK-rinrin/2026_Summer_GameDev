#pragma once
#include <deque>

class Transform2D;

class ProcessMove
{
public:

	ProcessMove();

	// 百分率座標における移動速度の設定
	void SetMoveSpeedPercent(float moveSpeedPercent);

	// 百分率座標における移動速度の取得（未使用）
	float GetMoveSpeedPercent() const;

	// 入力遅延フレーム数の最小値と最大値の設定
	void SetDelayFrameRange(int minDelayFrame, int maxDelayFrame);

	// 入力遅延と保留中の移動入力の解除
	void ClearDelay();
	void Reset();

	void Update(Transform2D& transform);

private:

	// 既定の百分率移動速度
	static constexpr float DEFAULT_MOVE_SPEED_PERCENT = 0.0f;

	// 既定の入力遅延フレーム数
	static constexpr int DEFAULT_DELAY_FRAME = 0;

	// 遅延乱数の線形合同法の乗数
	static constexpr unsigned int DELAY_RANDOM_A = 1103515245u;

	// 遅延乱数の線形合同法の加算値
	static constexpr unsigned int DELAY_RANDOM_C = 12345u;

	// 移動方向と実行待機時間の組
	struct MoveInput
	{
		float x;	// 横の移動
		float y;	// 縦の移動

		// 移動実行までの残り待機フレーム数
		int waitFrame;
	};

	// 現在入力に対応する百分率座標の移動量の取得
	MoveInput ReadInput() const;

	// 移動入力の座標情報への適用
	void ApplyMove(Transform2D& transform, const MoveInput& input) const;

	// 遅延なしでの移動入力の適用
	void UpdateImmediate(Transform2D& transform, const MoveInput& input) const;

	// 遅延キューを介した移動入力の適用
	void UpdateDelayed(Transform2D& transform, const MoveInput& input);

	// 指定範囲内の遅延フレーム数の生成
	int CreateDelayFrame();

	// 百分率座標における移動速度
	float moveSpeedPercent_;

	// 入力遅延の有効状態
	bool useDelay_;

	// 入力遅延の最小フレーム数
	int minDelayFrame_;

	// 入力遅延の最大フレーム数
	int maxDelayFrame_;

	// 入力遅延の乱数生成状態
	unsigned int delaySeed_;

	// 実行待ちの移動入力キュー
	std::deque<MoveInput> delayedInputs_;
};
