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

	void SetLocalPercent(float x, float y);
	bool IsFacingRight() const;

	virtual void UpdateSortKey();

	float GetSortY() const { return transform_.GetWorldPos().y; }

	// ローカル百分率矩形による通過防止（leftTop / rightBottom は VECTOR、0..100）
	void BlockCrossingLocalRect(const VECTOR& leftTopPercent, const VECTOR& rightBottomPercent);

	bool IsHitCircle(const ActorBase& other) const;

	void MoveToBeforePos();

protected:

	// リソース/シーン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// 位置など
	Transform2D transform_;
	Sprite2D sprite_;
	VECTOR beforePos_;

	void BeginMovement() { beforePos_ = transform_.pos; }
	virtual void UpdateProcessMove() = 0;

	// 移動コントローラークラス
	std::unique_ptr<ProcessMove> processMove_;

	// アニメーション
	CharacterAnimator2D animator_;

	// コライダ
	CircleCollider collider_;
	float radius_;

	// 初期化（継承側）
	virtual void InitLoad() = 0;
	virtual void InitTransform() = 0;
	virtual void InitCollider() = 0;
	virtual void InitAnimation() = 0;
	virtual void InitPost() = 0;

	virtual void UpdateAnimation() {}

	void LoadMoveGraph(const Resource& res);

};