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
	InitLoad();

	InitTransform();
}

void StageBase::Update()
{
	UpdateProcess();

	for (auto& obj : objects_)
	{
		obj.Update();
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

void StageBase::Delete()
{
	for (auto& obj : objects_)
	{
		obj.Delete();
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
		renderer.AddTransform(&obj);
	}
}

void StageBase::ApplyMovementBlocks(ActorBase& actor) const
{
	for (const auto& block : movementBlocks_)
	{
		actor.BlockCrossingWorldY(block.worldY, block.thickness);
	}
}

void StageBase::AddScreenObject(int handle, float sortY, bool trans)
{
	screenObjects_.push_back({ handle, sortY, trans });
}

void StageBase::AddTransformObject(const Transform2D& transform)
{
	objects_.push_back(transform);
}

void StageBase::AddMovementBlock(float worldY, float thickness)
{
	movementBlocks_.push_back({ worldY, thickness });
}