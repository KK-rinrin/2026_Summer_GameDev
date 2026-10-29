#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "ActorBase.h"
#include "../Stage/StageProjection2D.h"
#include "../../Common/Vector2.h"
#include "../../Utility/SchoolUtility.h"
#include "ProcessMove.h"

ActorBase::ActorBase()
	: resMng_(ResourceManager::GetInstance())
	, scnMng_(SceneManager::GetInstance())
	, transform_()
	, beforePos_(Vector2<float>{})
	, radius_(0.0f)
	, collider_(radius_)
{
	processMove_ = std::make_unique<ProcessMove>();
	processMove_->SetMoveSpeedPercent(INIT_MOVE_SPEED_PER);
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

void ActorBase::Update(bool acceptMoveInput)
{
	BeginMovement();
	
	if (acceptMoveInput)
	{
		UpdateProcessMove();
	}

	// transform2D 側でパーセンテージのクランプとワールド座標への収め込みを行う
	transform_.Update();

	UpdateAnimation();
}

void ActorBase::Draw()
{
	const Vector2<float> world = transform_.GetWorldPos();
	sprite_.Draw(world, transform_.scaleX, transform_.scaleY, StageProjection2D::GetPerspectiveScale(world.y));
}

void ActorBase::Release()
{
	transform_.Delete();

	processMove_.reset();
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
	return Collision::CollisionCircle(transform_.pos, collider_.radius, other.transform_.pos, other.collider_.radius);
}

void ActorBase::MoveToBeforePos()
{
	transform_.pos = beforePos_;
	transform_.Update();
}

void ActorBase::BlockCrossingLocalRect(const Vector2<float>& leftTopPercent, const Vector2<float>& rightBottomPercent)
{
	transform_.pos = Collision::BlockCrossingLocalRect(beforePos_, transform_.pos, leftTopPercent, rightBottomPercent);
}

void ActorBase::LoadMoveGraph(const Resource& res)
{
	// IMGS（分割画像）の場合は numX_ * numY_ 個の handleIds_ が存在する
	if (res.type_ == Resource::TYPE::IMGS && res.handleIds_ != nullptr)
	{
		int num = res.numX_ * res.numY_;
		std::vector<int> handles;
		handles.reserve(num);
		for (int i = 0; i < num; ++i)
			handles.push_back(res.handleIds_[i]);
		sprite_.SetHandles(handles, res.numX_, res.numY_);
	}
	else if (res.type_ == Resource::TYPE::IMG)
	{
		// 単一画像なら handleId_ を 1 要素の vector にして渡す
		sprite_.SetHandles(std::vector<int>{ res.handleId_ }, 1, 1);
	}
	else
	{
		// その他は空ベクタを渡す（安全側）
		sprite_.SetHandles(std::vector<int>{}, 1, 1);
	}
}
