#include "Transform2D.h"
#include "../Stage/StageProjection2D.h"

Transform2D::Transform2D()
	: pos{ 0.0f, 0.0f }, scaleX(1.0f), scaleY(1.0f)
{
}

Transform2D::Transform2D(const Vector2<float>& position)
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

Vector2<float> Transform2D::GetWorldPos() const
{
	return GetWorldPos(pos);
}

Vector2<float> Transform2D::GetWorldPos(const Vector2<float>& localPos)
{
	return StageProjection2D::ToWorld(localPos);
}

Vector2<float> Transform2D::WorldToLocalPercent(const Vector2<float>& worldPos)
{
	return StageProjection2D::ToLocalPercent(worldPos);
}
