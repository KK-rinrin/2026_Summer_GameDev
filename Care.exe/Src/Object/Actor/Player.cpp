#include "Player.h"
#include "ProcessMove.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/ProgressManager.h"

Player::Player()
{
	if (ProgressManager::GetInstance().IsNurceCharExists())
	{
		processMove_ = new ProcessMove();
		processMove_->SetMoveSpeedPercent(INIT_MOVE_SPEED_PER);

	}
	else { processMove_ = nullptr; }

}

Player::~Player()
{
	if (processMove_ != nullptr)
	{
		delete processMove_;
		processMove_ = nullptr;
	}
}

void Player::Update()
{
	BeginMovement();
	if (processMove_ != nullptr)
	{
		processMove_->Update(transform_);
	}

	// transform2D 側でパーセンテージのクランプとワールド座標への収め込みを行う
	transform_.Update();

	UpdateAnimation();
}

void Player::Draw()
{
	if (!ProgressManager::GetInstance().IsNurceCharExists())
	{
		return;
	}

	ActorBase::Draw();
}

void Player::SetLocalPercent(float x, float y)
{
	ActorBase::SetLocalPercent(x, y);
}

bool Player::IsFacingRight() const
{
	return ActorBase::IsFacingRight();
}

ProcessMove* Player::GetProcessMove()
{
	return processMove_;
}

void Player::Release()
{
	if (processMove_ != nullptr)
	{
		delete processMove_;
		processMove_ = nullptr;
	}
	ActorBase::Release();
}

void Player::InitLoad()
{
	const Resource& res = resMng_.Load(ResourceManager::SRC::PLAYER_GRAPHICS);

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
		sprite_.SetHandles(std::vector<int>{ res.handleId_ },1,1);
	}
	else
	{
		// その他は空ベクタを渡す（安全側）
		sprite_.SetHandles(std::vector<int>{},1,1);
	}
}

void Player::InitTransform()
{
	// 初期位置
	transform_.pos.x = INIT_PER.x;
	transform_.pos.y = INIT_PER.y;
	beforePos_ = transform_.pos;

}

void Player::InitCollider()
{
	radius_ = COL_RADIUS;
	collider_.radius = radius_;
}

void Player::InitAnimation()
{
	// 静止画像
	animator_.Configure(sprite_.GetColumns(), ResourceManager::CHARACTER_FRONT_STILL_INDEX);

	animator_.SetPingPong(true);
}

void Player::InitPost()
{
}

void Player::UpdateAnimation()
{
	animator_.Update(sprite_, VSub(transform_.pos, beforePos_), transform_.GetWorldPos().x);
}
