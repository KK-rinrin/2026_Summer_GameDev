#include "StageBase.h"
#include "../../Manager/ResourceManager.h"
#include "../Renderer2D.h"
#include "../Actor/ActorBase.h"

StageBase::StageBase()
	: BGhandle_(-1)
	, resMng_(ResourceManager::GetInstance())
{
}

StageBase::~StageBase()
{
}

void StageBase::Init()
{
	objects_.clear();
	screenObjects_.clear();
	movementRectPercents_.clear();

	InitLoad();

	InitTransform();

	InitCollider();
}

void StageBase::Update()
{
	for (auto& obj : objects_)
	{
		obj.transform.Update();
	}
}

void StageBase::Draw()
{
	DrawBackground();

	for (const auto& obj : screenObjects_)
	{
		if (obj.handle != -1)
		{
			DrawGraph(0, 0, obj.handle, obj.trans);
		}
	}

	for (auto& obj : objects_)
	{
		obj.transform.Draw();
	}
}

void StageBase::Delete()
{
	for (auto& obj : objects_)
	{
		obj.transform.Delete();
	}
}

void StageBase::DrawBackground() const
{
	if (BGhandle_ != -1)
	{
		DrawGraph(0, 0, BGhandle_, false);
	}
}

void StageBase::RegisterObjects(Renderer2D& renderer)
{
	for (const auto& obj : screenObjects_)
	{
		renderer.Add(obj.sortY, [obj]() { if (obj.handle != -1) DrawGraph(0, 0, obj.handle, obj.trans); });
	}

	for (auto& obj : objects_)
	{
		if (obj.useSortY)
		{
			Transform2D* transform = &obj.transform;
			renderer.Add(obj.sortY, [transform]() { transform->Draw(); });
		}
		else
		{
			renderer.AddTransform(&obj.transform);
		}
	}
}

void StageBase::ApplyMovementBlocks(ActorBase& actor) const
{
	// ローカル百分率矩形ブロック（左上/右下指定）
	for (const auto& rect : movementRectPercents_)
	{
		actor.BlockCrossingLocalRect(rect.leftTop, rect.rightBottom);
	}
}


StageBase::DecideResult StageBase::Decide(const ActorBase& controlActor, const ActorBase* patientActor) const
{
	(void)controlActor;
	(void)patientActor;
	return DecideResult{};
}
void StageBase::AddScreenObject(int handle, float sortY, bool trans)
{
	screenObjects_.push_back({ handle, sortY, trans });
}

void StageBase::AddTransformObject(const Transform2D& transform)
{
	objects_.push_back({ transform, false, 0.0f });
}

void StageBase::AddTransformObject(const Transform2D& transform, float sortY)
{
	objects_.push_back({ transform, true, sortY });
}

void StageBase::AddMBRectPercent(const VECTOR& leftTopPercent, const VECTOR& rightBottomPercent)
{
	MovementRectPercent r;
	r.leftTop = leftTopPercent;
	r.rightBottom = rightBottomPercent;
	movementRectPercents_.push_back(r);
}
