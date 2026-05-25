#include "Patient.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"

Patient::Patient()
{
}

Patient::~Patient()
{
}

void Patient::Update(void)
{
	transform_.Update();

	UpdateAnimation();

	anim_.Update();
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
		transform_.SetHandles(handles, 3, 2);
	}
	else
	{
		// その他は空ベクタを渡す（安全側）
		transform_.SetHandles(std::vector<int>{}, 1, 1);
	}
}

void Patient::InitTransform()
{
	// 初期位置
	transform_.pos.x = INIT_PER_X;
	transform_.pos.y = INIT_PER_Y;
	transform_.beforePos = transform_.pos;


}

void Patient::InitCollider()
{
	radius_ = COL_RADIUS;
	collider_.radius = radius_;
}

void Patient::InitAnimation()
{
	// 静止画像
	transform_.stillIndex = 1;
	transform_.stillIndex_back = 4;
	transform_.isLeft = true;
}

void Patient::InitPost()
{
}

void Patient::UpdateAnimation()
{
	transform_.currentHandleIndex = transform_.stillIndex;

	// 話しかけられたらその方向を向く
}
