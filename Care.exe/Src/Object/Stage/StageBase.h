#pragma once
#include <vector>
#include "../Common/2DTransform.h"
class ResourceManager;
class Renderer2D;
class ActorBase;

class StageBase
{
public:
	// 決定キーで起きたステージ内イベントの種類
	enum class DecideType
	{
		NONE,
		CHANGE_STAGE,
		PATIENT_TALK,
		PC,
	};

	// GameSceneに依存せず遷移先ステージを伝えるためのID
	enum class StageId
	{
		PAT_ROOM,
		NURSE_STATION,
	};

	struct DecideResult
	{
		DecideType type = DecideType::NONE;
		StageId nextStage = StageId::PAT_ROOM;
		VECTOR movePos = { 0.0f, 0.0f, 0.0f };
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
	virtual DecideResult Decide(const ActorBase& controlActor, const ActorBase* patientActor) const;

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

	virtual void InitLoad() {}
	virtual void InitTransform() {}
	virtual void InitCollider() {}

	int BGhandle_;
	std::vector<TransformObject> objects_;	// 障害物の位置
	std::vector<RenderObject> screenObjects_;
	std::vector<MovementRectPercent> movementRectPercents_;

	ResourceManager& resMng_;
};
