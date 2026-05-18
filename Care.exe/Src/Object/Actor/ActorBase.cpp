#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "ActorBase.h"
#include "../../Common/Vector2.h"

ActorBase::ActorBase(void)
	:
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	transform_(),
	radius_(0.0f),
	collider_(radius_)
{
}

ActorBase::~ActorBase(void)
{
}

void ActorBase::Init(void)
{

	// リソースロード
	InitLoad();

	// Transform初期化
	InitTransform();

	// 衝突判定の初期化
	InitCollider();

	// アニメーションの初期化
	InitAnimation();

	// 初期化後の個別処理
	InitPost();

}

void ActorBase::Draw(void)
{
	transform_.Draw();
}

void ActorBase::Release(void)
{
	transform_.Delete();
}

const Transform2D& ActorBase::GetTransform(void) const
{
	return transform_;
}

void ActorBase::UpdateSortKey()
{
	// 足元のワールド座標をソートキーにする
	VECTOR world = transform_.GetWorldPos();
	transform_.sortY = world.y;
}