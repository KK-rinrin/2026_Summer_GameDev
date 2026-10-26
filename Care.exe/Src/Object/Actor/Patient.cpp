#include "Patient.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/SchoolUtility.h"
#include "ProcessMove.h"
#include "../../Manager/ProgressManager.h"

Patient::Patient()
{
	// 看護師charファイルがなく患者charファイルがある場合は操作クラスを生成
	if (!ProgressManager::GetInstance().IsNurseCharExists() && 
		ProgressManager::GetInstance().IsPatientCharExists())
	{
		processMove_ = std::make_unique<ProcessMove>();
		processMove_->SetMoveSpeedPercent(INIT_MOVE_SPEED_PER);
	}
	else
	{
		processMove_ = nullptr;

	}
}

Patient::~Patient() = default;

void Patient::Update()
{
	BeginMovement();
	if (processMove_ != nullptr) processMove_->Update(transform_);

	transform_.Update();

	UpdateAnimation();
}

void Patient::Release()
{
	processMove_.reset();

	ActorBase::Release();
}

void Patient::InitLoad()
{
	const Resource& res = resMng_.Load(ResourceManager::SRC::PATIENT_GRAPHICS);

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
	else
	{
		// その他は空ベクタを渡す（安全側）
		sprite_.SetHandles(std::vector<int>{}, 1, 1);
	}
}

void Patient::InitTransform()
{
	// 初期位置
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
	// 静止画像
	animator_.Configure(sprite_.GetColumns(), ResourceManager::CHARACTER_FRONT_STILL_INDEX);
	sprite_.SetFlipX(true);
}

void Patient::InitPost()
{
}

void Patient::UpdateAnimation()
{
	animator_.ShowStill(sprite_);

	// 話しかけられたらその方向を向く
}
