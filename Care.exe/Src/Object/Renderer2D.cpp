#include "Renderer2D.h"

void Renderer2D::Clear()
{
    drawList_.clear();
    nextOrder_ = 0;
}

void Renderer2D::AddActor(ActorBase* actor, Layer layer)
{
    if (actor == nullptr) return;

    Add(
        [actor]() { return actor->GetSortY(); },
        [actor]() { actor->Draw(); },
        layer
    );
}

void Renderer2D::AddTransform(Transform2D* transform, Layer layer)
{
    if (transform == nullptr) return;

    Add(
        [transform]() { return transform->GetWorldPos().y; },
        [transform]() { transform->Draw(); },
        layer
    );
}

void Renderer2D::Add(float sortY, const std::function<void()>& drawFunc, Layer layer)
{
    Add([sortY]() { return sortY; }, drawFunc, layer);
}

void Renderer2D::Add(const std::function<float()>& sortYFunc, const std::function<void()>& drawFunc, Layer layer)
{
    drawList_.push_back({ layer, nextOrder_++, sortYFunc, drawFunc });
}

void Renderer2D::Render()
{
    std::stable_sort(drawList_.begin(), drawList_.end(),
        [](const RenderItem& a, const RenderItem& b)
        {
            if (a.layer != b.layer)
            {
                return static_cast<int>(a.layer) < static_cast<int>(b.layer);
            }

            const float aSortY = a.sortYFunc ? a.sortYFunc() : 0.0f;
            const float bSortY = b.sortYFunc ? b.sortYFunc() : 0.0f;
            if (aSortY != bSortY)
            {
                return aSortY < bSortY;
            }

            return a.order < b.order;
        }
    );

    for (const auto& item : drawList_)
    {
        if (item.drawFunc)
        {
            item.drawFunc();
        }
    }
}