#pragma once
#include "../Common/2DTransform.h"
#include "../Collider/Collider.h"
class ResourceManager;
class SceneManager;

class ActorBase
{

public:

	// コンストラクタ
	ActorBase();

	// デストラクタ
	virtual ~ActorBase();

	// 初期化
	void Init();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw();

	// 解放
	virtual void Release();

	// 変換情報取得
	const Transform2D& GetTransform() const;

	void SetLocalPercent(float x, float y);
	bool IsFacingRight() const;

	virtual void UpdateSortKey();

	float GetSortY() const { return transform_.GetWorldPos().y; }

	// ローカル百分率矩形による通過防止（leftTop / rightBottom は VECTOR、0..100）
	void BlockCrossingLocalRect(const VECTOR& leftTopPercent, const VECTOR& rightBottomPercent) { transform_.BlockCrossingLocalRect(leftTopPercent, rightBottomPercent); }

	bool IsHitCircle(const ActorBase& other) const;

	void MoveToBeforePos();

protected:

	// リソース/シーン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// 位置等
	Transform2D transform_;

	AnimationController2D anim_;

	CircleCollider collider_;
	float radius_;

	// 初期化（継承側）
	virtual void InitLoad() = 0;
	virtual void InitTransform() = 0;
	virtual void InitCollider() = 0;
	virtual void InitAnimation() = 0;
	virtual void InitPost() = 0;

	virtual void UpdateAnimation() {}

};