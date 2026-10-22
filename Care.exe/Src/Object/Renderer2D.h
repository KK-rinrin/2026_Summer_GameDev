#pragma once
#include <algorithm>
#include <functional>
#include <vector>
#include "Actor/ActorBase.h"
#include "Common/2DTransform.h"
#include "Common/Sprite2D.h"

class Renderer2D
{
public:
	// レイヤー順を優先し、同じレイヤー内ではY座標による奥行き順で描画する
	enum class Layer
	{
		Back,
		Object,
		Front,
	};

	void Clear();

	void AddActor(ActorBase* actor, Layer layer = Layer::Object);
	void AddSprite(const Transform2D* transform, const Sprite2D* sprite, Layer layer = Layer::Object);
	void Add(float sortY, const std::function<void()>& drawFunc, Layer layer = Layer::Object);
	void Add(const std::function<float()>& sortYFunc, const std::function<void()>& drawFunc, Layer layer = Layer::Object);

	void Render();

private:
	struct RenderItem
	{
		Layer layer;
		int order;
		std::function<float()> sortYFunc;
		std::function<void()> drawFunc;
	};

	std::vector<RenderItem> drawList_;
	int nextOrder_ = 0;
};