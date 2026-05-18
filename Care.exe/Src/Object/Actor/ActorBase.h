#pragma once
#include "../Common/2DTransform.h"
#include "../Collider/Collider.h"
class ResourceManager;
class SceneManager;

class ActorBase
{

public:
	
	// コンストラクタ
	ActorBase(void);

	// デストラクタ
	virtual ~ActorBase(void);

	// 初期化
	void Init(void);

	// 更新
	virtual void Update(void) = 0;

	// 描画
	virtual void Draw(void);

	// 解放
	virtual void Release(void);

	// 大きさ、回転、座標等の取得
	const Transform2D& GetTransform(void) const;

	virtual void UpdateSortKey();

	float GetSortY() const { return transform_.sortY; }

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform2D transform_;

	AnimationController2D anim_;

	CircleCollider collider_;
	float radius_;

	// リソースロード
	virtual void InitLoad(void) = 0;

	// 大きさ、回転、座標の初期化
	virtual void InitTransform(void) = 0;

	// 衝突判定の初期化
	virtual void InitCollider(void) = 0;

	// アニメーションの初期化
	virtual void InitAnimation(void) = 0;

	// 初期化後の個別処理
	virtual void InitPost(void) = 0;

	virtual void UpdateAnimation(void) {}

};
