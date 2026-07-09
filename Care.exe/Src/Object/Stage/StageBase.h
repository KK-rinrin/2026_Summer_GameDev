#pragma once
#include <functional>
#include <vector>
#include "../../Common/Vector2.h"
#include "../Common/2DTransform.h"
class ResourceManager;
class Renderer2D;
class ActorBase;
class ProgressManager;
class SoundManager;
class Talk;

class StageBase
{
public:
	// GameSceneに依存せず遷移先ステージを伝えるためのID
	enum class StageId
	{
		PAT_ROOM,
		NURSE_STATION,
	};

	// ステージ内イベント実行時にGameScene側の所有物へアクセスするための窓口
	struct DecideContext
	{
		ActorBase& controlActor;
		const ActorBase* patientActor;
		ProgressManager& progressManager;
		Talk& talk;
		SoundManager& soundManager;
		std::function<void(StageId, const VECTOR&)> ChangeStage;
	};

	StageBase();
	virtual ~StageBase();

	void Init();
	virtual void Update();
	virtual void Draw();
	virtual void Delete();

	void DrawBackground() const;
	void RegisterObjects(Renderer2D& renderer);
	void ApplyMovementBlocks(ActorBase& actor) const;
	virtual void DrawGuide(const ActorBase& controlActor) const;
	virtual void Decide(DecideContext& context) const;

protected:
	struct RenderObject
	{
		int handle;
		float sortY;
		bool trans;
	};

	// ローカル百分率基準の矩形（pos.x/pos.y が 0..100 の空間）
	struct MovementRectPercent
	{
		VECTOR leftTop;    // 左上 (x=0..100, y=0..100)
		VECTOR rightBottom;// 右下 (x=0..100, y=0..100)
	};

	struct TransformObject
	{
		Transform2D transform;
		bool useSortY;
		float sortY;
	};

	void AddScreenObject(int handle, float sortY, bool trans = true);
	void AddTransformObject(const Transform2D& transform);
	void AddTransformObject(const Transform2D& transform, float sortY);
	void AddMBRectPercent(const VECTOR& leftTopPercent, const VECTOR& rightBottomPercent);
	void DrawMoveGuide(const Vector2& pos) const;

	virtual void InitLoad() {}
	virtual void InitTransform() {}
	virtual void InitCollider() {}

	int BGhandle_;
	int guideMoveHandle_;
	std::vector<TransformObject> objects_;	// 障害物の位置
	std::vector<RenderObject> screenObjects_;
	std::vector<MovementRectPercent> movementRectPercents_;

	ResourceManager& resMng_;
};
