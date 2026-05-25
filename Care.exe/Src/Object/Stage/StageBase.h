#pragma once
#include <vector>
#include "../Common/2DTransform.h"
class ResourceManager;
class Renderer2D;
class ActorBase;

class StageBase
{
public:
	StageBase();
	virtual ~StageBase();

	void Init();
	virtual void Update();
	virtual void Draw();
	virtual void Delete();

	void DrawBackground() const;
	void RegisterObjects(Renderer2D& renderer);
	void ApplyMovementBlocks(ActorBase& actor) const;

protected:
	struct RenderObject
	{
		int handle;
		float sortY;
		bool trans;
	};

	struct MovementBlock
	{
		float worldY;
		float thickness;
	};

	void AddScreenObject(int handle, float sortY, bool trans = true);
	void AddTransformObject(const Transform2D& transform);
	void AddMovementBlock(float worldY, float thickness);

	virtual void InitTransform() {}
	virtual void InitLoad() {}
	virtual void UpdateProcess() {}

	int BGhandle_;
	std::vector<Transform2D> objects_;	// è·äQï®ÇÃà íu
	std::vector<RenderObject> screenObjects_;
	std::vector<MovementBlock> movementBlocks_;

	ResourceManager& resMng_;
};