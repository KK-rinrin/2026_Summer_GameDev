#include "Player.h"
#include "ProcessMove.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/ProgressManager.h"

Player::Player()
{
}

void Player::Draw()
{
	if (!ProgressManager::GetInstance().IsNurseCharExists())
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
	
}

void Player::Release()
{
}

void Player::InitLoad()
{
	LoadMoveGraph(resMng_.Load(ResourceManager::SRC::PLAYER_GRAPHICS));
}

void Player::InitTransform()
{
	// ‰ŠúˆÊ’u
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
	// ÃŽ~‰æ‘œ
	animator_.Configure(sprite_.GetColumns(), ResourceManager::CHARACTER_FRONT_STILL_INDEX);

	animator_.SetPingPong(true);
}

void Player::InitPost()
{
}

void Player::UpdateProcessMove()
{
	if (processMove_ != nullptr) processMove_->Update(transform_);
}

void Player::UpdateAnimation()
{
	animator_.Update(sprite_, VSub(transform_.pos, beforePos_), transform_.GetWorldPos().x);
}
