#include "2DTransform.h"
#include "../Stage/StageProjection2D.h"

Transform2D::Transform2D()
	: pos{ 0.0f, 0.0f, 0.0f }, scaleX(1.0f), scaleY(1.0f)
{
}

Transform2D::Transform2D(const VECTOR& position)
	: Transform2D()
{
	pos = position;
}

void Transform2D::Update()
{
	pos = StageProjection2D::ClampLocalPercent(pos);
}

void Transform2D::Delete()
{
}

VECTOR Transform2D::GetWorldPos() const
{
	return GetWorldPos(pos);
}

VECTOR Transform2D::GetWorldPos(const VECTOR& localPos)
{
	return StageProjection2D::ToWorld(localPos);
}

VECTOR Transform2D::WorldToLocalPercent(const VECTOR& worldPos)
{
	return StageProjection2D::ToLocalPercent(worldPos);
}
