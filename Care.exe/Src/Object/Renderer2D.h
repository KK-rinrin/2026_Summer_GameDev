#pragma once
#include <algorithm>
#include <functional>
#include <vector>
#include "Actor/ActorBase.h"
#include "Common/2DTransform.h"

class Renderer2D
{
public:
    enum class Layer
    {
        Back,
        Object,
        Front,
    };

    void Clear();

    void AddActor(ActorBase* actor, Layer layer = Layer::Object);
    void AddTransform(Transform2D* transform, Layer layer = Layer::Object);
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