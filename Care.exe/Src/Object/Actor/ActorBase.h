#pragma once
#include "../Common/Transform2D.h"
#include "../Common/Sprite2D.h"
#include "../Common/CharacterAnimator2D.h"
#include "../Collider/Collider.h"
#include <memory>

class Resource;
class ResourceManager;
class SceneManager;
class ProcessMove;

class ActorBase
{

public:
	static constexpr float INIT_MOVE_SPEED_PER = 0.6f;	// コントロール移動速度

	// コンストラクタ
	ActorBase();

	// デストラクタ
	virtual ~ActorBase() = default;

	// 初期化
	void Init();

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw();

	// 解放
	virtual void Release();

	// 変換情報取得
	const Transform2D& GetTransform() const;

	// 移動コントローラークラスを取得
	const ProcessMove* GetProcessMove() const { return processMove_.get(); }

	// ステージ内百分率座標による位置の設定
	void SetLocalPercent(float x, float y);

	// 右向き状態の判定
	bool IsFacingRight() const;

	// 描画順の基準値の更新
	virtual void UpdateSortKey();

	// 描画順に使用する画面Y座標の取得
	float GetSortY() const { return transform_.GetWorldPos().y; }

	// ローカル百分率矩形による通過防止（leftTop / rightBottom は VECTOR、0..100）
	void BlockCrossingLocalRect(const VECTOR& leftTopPercent, const VECTOR& rightBottomPercent);

	// 他のキャラクターとの円形衝突判定
	bool IsHitCircle(const ActorBase& other) const;

	// 移動前の座標への復帰
	void MoveToBeforePos();

protected:

	// リソース管理への参照
	ResourceManager& resMng_;

	// シーン管理への参照
	SceneManager& scnMng_;

	// 位置など
	Transform2D transform_;

	// キャラクター画像の描画情報
	Sprite2D sprite_;

	// 移動開始前のローカル座標
	VECTOR beforePos_;

	// 移動前のローカル座標の保存
	void BeginMovement() { beforePos_ = transform_.pos; }

	// 派生クラスによる移動制御の更新
	virtual void UpdateProcessMove() = 0;

	// 移動コントローラークラス
	std::unique_ptr<ProcessMove> processMove_;

	// アニメーション
	CharacterAnimator2D animator_;

	// コライダ
	CircleCollider collider_;

	// キャラクターの当たり判定半径
	float radius_;

	// 初期化（継承側）
	virtual void InitLoad() = 0;

	// 派生クラスによる座標情報の初期化
	virtual void InitTransform() = 0;

	// 派生クラスによる当たり判定の初期化
	virtual void InitCollider() = 0;

	// 派生クラスによるアニメーションの初期化
	virtual void InitAnimation() = 0;

	// 派生クラスによる初期化後処理
	virtual void InitPost() = 0;

	// 派生クラスによるアニメーションの更新
	virtual void UpdateAnimation() {}

	// 移動用の分割画像の読み込み
	void LoadMoveGraph(const Resource& res);

};
