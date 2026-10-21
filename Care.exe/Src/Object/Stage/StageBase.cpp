#include "StageBase.h"
#include "StageProjection2D.h"
#include "../../Manager/ResourceManager.h"
#include "../Renderer2D.h"
#include "../Actor/ActorBase.h"

StageBase::StageBase()
	: BGhandle_(-1)
	, guideMoveHandle_(-1)
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

	guideMoveHandle_ = resMng_.Load(ResourceManager::SRC::GUIDE_MOVE).handleId_;

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
		obj.Draw();
	}
}

void StageBase::DrawForeground() const
{
}

void StageBase::Delete()
{
	for (auto& obj : objects_)
	{
		obj.transform.Delete();
	}
}

bool StageBase::IsInputBlocked() const
{
	return false;
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
			const TransformObject* object = &obj;
			renderer.Add(obj.sortY, [object]() { object->Draw(); });
		}
		else
		{
			renderer.AddSprite(&obj.transform, &obj.sprite);
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


void StageBase::DrawGuide(const ActorBase& controlActor) const
{
	(void)controlActor;
}

void StageBase::Decide(DecideContext& context) const
{
	(void)context;
}

void StageBase::AddScreenObject(int handle, float sortY, bool trans)
{
	screenObjects_.push_back({ handle, sortY, trans });
}

void StageBase::AddTransformObject(const Transform2D& transform, const Sprite2D& sprite)
{
	objects_.push_back({ transform, sprite, false, 0.0f });
}

void StageBase::AddTransformObject(const Transform2D& transform, const Sprite2D& sprite, float sortY)
{
	objects_.push_back({ transform, sprite, true, sortY });
}

void StageBase::DrawMoveGuide(const Vector2& pos) const
{
	if (guideMoveHandle_ != -1)
	{
		DrawGraph(pos.x, pos.y, guideMoveHandle_, true);
	}
}

void StageBase::AddMBRectPercent(const VECTOR& leftTopPercent, const VECTOR& rightBottomPercent)
{
	Rect r = {};
	r.leftTop = leftTopPercent;
	r.rightBottom = rightBottomPercent;
	movementRectPercents_.push_back(r);
}

void StageBase::TransformObject::Draw() const
{
	const VECTOR world = transform.GetWorldPos();
	sprite.Draw(world, transform.scaleX, transform.scaleY, StageProjection2D::GetPerspectiveScale(world.y));
}
