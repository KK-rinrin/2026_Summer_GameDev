#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "ActorBase.h"
#include "../Stage/StageProjection2D.h"
#include "../../Common/Vector2.h"
#include "../../Utility/SchoolUtility.h"

ActorBase::ActorBase()
	: resMng_(ResourceManager::GetInstance())
	, scnMng_(SceneManager::GetInstance())
	, transform_()
	, beforePos_(SchoolUtility::VECTOR_ZERO)
	, radius_(0.0f)
	, collider_(radius_)
{
}

ActorBase::~ActorBase()
{
}

void ActorBase::Init()
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

void ActorBase::Draw()
{
	const VECTOR world = transform_.GetWorldPos();
	sprite_.Draw(world, transform_.scaleX, transform_.scaleY, StageProjection2D::GetPerspectiveScale(world.y));
}

void ActorBase::Release()
{
	transform_.Delete();
}

const Transform2D& ActorBase::GetTransform() const
{
	return transform_;
}

void ActorBase::SetLocalPercent(float x, float y)
{
	transform_.pos.x = x;
	transform_.pos.y = y;
	transform_.Update();
	beforePos_ = transform_.pos;
}

bool ActorBase::IsFacingRight() const
{
	return !sprite_.IsFlippedX();
}
void ActorBase::UpdateSortKey()
{
}



bool ActorBase::IsHitCircle(const ActorBase& other) const
{
	return SchoolUtility::IsHitSpheres(transform_.pos, collider_.radius, other.transform_.pos, other.collider_.radius);
}

void ActorBase::MoveToBeforePos()
{
	transform_.pos = beforePos_;
	transform_.Update();
}

void ActorBase::BlockCrossingLocalRect(const VECTOR& leftTopPercent, const VECTOR& rightBottomPercent)
{
	transform_.pos = Collision::BlockCrossingLocalRect(beforePos_, transform_.pos, leftTopPercent, rightBottomPercent);
}
