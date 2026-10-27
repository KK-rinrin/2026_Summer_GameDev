#pragma once
#include <functional>
#include <vector>
#include "../../Common/Vector2.h"
#include "../Common/Transform2D.h"
#include "../Common/Sprite2D.h"
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
	virtual void DrawForeground() const;
	virtual void Delete();
	virtual bool IsInputBlocked() const;

	void DrawBG() const;	// 背景を描画
	virtual void RegisterObjects(Renderer2D& renderer);	// オブジェクト登録
	void ApplyCollision(ActorBase& actor) const;		// 当たり判定登録
	virtual void DrawGuide(const ActorBase& controlActor) const { (void)controlActor; }
	virtual void Decide(DecideContext& context) const { (void)context; }

protected:
	// オブジェクトのレンダー情報
	struct RenderObject
	{
		int handle;	// 画像ハンドル
		float sortY;// レンダリング順
		bool trans = true;	// 背景を透明にするか？
	};

	// ローカル百分率基準の矩形（pos.x/pos.y が 0..100 の空間）
	struct Rect
	{
		VECTOR leftTop;    // 左上 (x=0..100, y=0..100)
		VECTOR rightBottom;// 右下 (x=0..100, y=0..100)
	};


	struct TransformObject
	{
		Transform2D transform;
		Sprite2D sprite;
		void Draw() const;
		bool useSortY;
		float sortY;
	};

	void AddScreenObject(int handle, float sortY, bool trans = true);
	void AddTransformObject(const Transform2D& transform, const Sprite2D& sprite);
	void AddTransformObject(const Transform2D& transform, const Sprite2D& sprite, float sortY);
	void AddMBRectPercent(const VECTOR& leftTopPercent, const VECTOR& rightBottomPercent);
	void DrawMoveGuide(const Vector2& pos) const;

	virtual void InitLoad() {}
	virtual void InitTransform() {}
	virtual void InitCollider() {}

	int BGhandle_;
	int guideMoveHandle_;
	std::vector<TransformObject> objects_;		// 障害物の位置
	std::vector<RenderObject> screenObjects_;
	std::vector<Rect> movementRectPercents_;	// 当たり判定

	ResourceManager& resMng_;
};
