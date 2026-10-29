#include "Patient.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/SchoolUtility.h"
#include "ProcessMove.h"
#include "../../Manager/ProgressManager.h"

Patient::Patient()
{
}

void Patient::Draw()
{
	if (!ProgressManager::GetInstance().IsPatientCharExists())
	{
		return;
	}
	ActorBase::Draw();
}

void Patient::Release()
{
}

void Patient::InitLoad()
{
	LoadMoveGraph(resMng_.Load(ResourceManager::SRC::PATIENT_GRAPHICS));
}

void Patient::InitTransform()
{
	// ‰ŠúˆÊ’u
	transform_.pos.x = INIT_PER.x;
	transform_.pos.y = INIT_PER.y;
	beforePos_ = transform_.pos;

}

void Patient::InitCollider()
{
	radius_ = COL_RADIUS;
	collider_.radius = radius_;
}

void Patient::InitAnimation()
{
	// ÃŽ~‰æ‘œ
	animator_.Configure(sprite_.GetColumns(), ResourceManager::CHARACTER_FRONT_STILL_INDEX);
	sprite_.SetFlipX(true);
}

void Patient::InitPost()
{
}

void Patient::UpdateProcessMove()
{
	if (ProgressManager::GetInstance().IsPatientCharExists() && processMove_ != nullptr)
	{
		processMove_->Update(transform_);
	}
}

void Patient::UpdateAnimation()
{
	animator_.ShowStill(sprite_);

	// ˜b‚µ‚©‚¯‚ç‚ê‚½‚ç‚»‚Ì•ûŒü‚ðŒü‚­
}
