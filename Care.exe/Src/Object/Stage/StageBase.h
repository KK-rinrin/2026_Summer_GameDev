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

	// GameScene‚ÉˆË‘¶‚¹‚¸‘JˆÚæƒXƒe[ƒW‚ğ“`‚¦‚é‚½‚ß‚ÌID
	enum class StageId
	{
		PAT_ROOM,		// Š³Ò•”‰®‚ÌƒXƒe[ƒWID
		NURSE_STATION,	// ƒi[ƒXƒXƒe[ƒVƒ‡ƒ“‚ÌƒXƒe[ƒWID
	};

	// ƒXƒe[ƒW“àƒCƒxƒ“ƒgÀs‚ÉGameScene‘¤‚ÌŠ—L•¨‚ÖƒAƒNƒZƒX‚·‚é‚½‚ß‚Ì‘‹Œû
	struct DecideContext
	{
		ActorBase& controlActor;			// ƒRƒ“ƒgƒ[ƒ‹‚·‚éƒLƒƒƒ‰
		const ActorBase* patientActor;		// Š³ÒƒLƒƒƒ‰
		ProgressManager& progressManager;	// is“xŠÇ—ƒ}ƒl[ƒWƒƒ
		Talk& talk;							// ‰ï˜b
		SoundManager& soundManager;			// ƒTƒEƒ“ƒhƒ}ƒl[ƒWƒƒ
		std::function<void(StageId, const Vector2<float>&)> ChangeStage;	// ˆÚ“®æ‚ÌƒXƒe[ƒWEÀ•W‚ÖˆÚ“®
	};

	StageBase();
	virtual ~StageBase();

	void Init();
	virtual void Update();
	virtual void Draw();

	// ï¿½hï¿½ï¿½ï¿½Xï¿½eï¿½[ï¿½Wï¿½Ì‘Oï¿½iï¿½`ï¿½ï¿½
	virtual void DrawForeground() const;
	virtual void Delete();

	// ƒXƒe[ƒW“à‘€ì‚É‚æ‚é“ü—ÍƒuƒƒbƒN‚Ì”»’è
	virtual bool IsInputBlocked() const;

	void DrawBG() const;	// ”wŒi‚ğ•`‰æ
	virtual void RegisterObjects(Renderer2D& renderer);	// ƒIƒuƒWƒFƒNƒg“o˜^
	void ApplyCollision(ActorBase& actor) const;		// “–‚½‚è”»’è“o˜^

	// ‘€ìƒLƒƒƒ‰‚ÌˆÊ’u‚É‰‚¶‚½ˆÚ“®ƒKƒCƒh‚Ì•`‰æ
	virtual void DrawGuide(const ActorBase& controlActor) const { (void)controlActor; }

	// ƒXƒe[ƒW“à‚ÌŒˆ’è‘€ì‚É‘Î‰‚·‚éƒCƒxƒ“ƒgÀs
	virtual void Decide(DecideContext& context) const { (void)context; }

protected:
	// ƒIƒuƒWƒFƒNƒg‚ÌƒŒƒ“ƒ_[î•ñ
	struct RenderObject
	{
		int handle;	// ‰æ‘œƒnƒ“ƒhƒ‹
		float sortY;// ƒŒƒ“ƒ_ƒŠƒ“ƒO‡
		bool trans = true;	// ”wŒi‚ğ“§–¾‚É‚·‚é‚©H
	};

	// ƒ[ƒJƒ‹•S•ª—¦Šî€‚Ì‹éŒ`ipos.x/pos.y ‚ª 0..100 ‚Ì‹óŠÔj
	struct Rect
	{
		Vector2<float> leftTop;    // ¶ã (x=0..100, y=0..100)
		Vector2<float> rightBottom;// ‰E‰º (x=0..100, y=0..100)
	};

	// À•W•ÏŠ·‚ğ”º‚¤ƒXƒe[ƒWƒIƒuƒWƒFƒNƒg‚Ì•`‰æî•ñ
	struct TransformObject
	{
		Transform2D transform;	// •¨‘Ìî•ñ
		Sprite2D sprite;		// ‰æ‘œƒXƒvƒ‰ƒCƒgƒf[ƒ^
		void Draw() const;		// •`‰æ
		bool useSortY;			// ƒŒƒ“ƒ_ƒŠƒ“ƒO‡‚ÉsortY‚ğ“K—p‚·‚é‚©Hfalse¨transform‚©‚çæ“¾
		float sortY;			// ‘OŒã‚ğØ‚è‘Ö‚¦‚éYÀ•WiƒvƒŒƒCƒ„[‚ª‚±‚ê‚æ‚è‰œ‚És‚­‚Æè‘O‚Éo‚éj
	};

	void AddScreenObject(int handle, float sortY, bool trans = true);	// ƒXƒNƒŠ[ƒ“ƒIƒuƒWƒFƒNƒg‚ğ’Ç‰Á
	void AddTransformObject(const Transform2D& transform, const Sprite2D& sprite);				// •¨‘Ì‚ğ’Ç‰Á
	void AddTransformObject(const Transform2D& transform, const Sprite2D& sprite, float sortY);	// •¨‘Ì‚ğ’Ç‰Ái‰œs‚«‚Â‚«j
	void AddMBRectPercent(const Vector2<float>& leftTopPercent, const Vector2<float>& rightBottomPercent);	// áŠQ•¨“–‚½‚è”»’èi‹éŒ`j‚ğì¬
	void DrawMoveGuide(const Vector2<int>& pos) const;	// ˆÚ“®ƒKƒCƒh‚ğ•`‰æ

	// ”h¶ƒXƒe[ƒW‚É‚æ‚éƒŠƒ\[ƒX“Ç‚İ‚İ
	virtual void InitLoad() {}

	// ”h¶ƒXƒe[ƒW‚É‚æ‚éÀ•Wî•ñ‚Ì‰Šú‰»
	virtual void InitTransform() {}

	// ”h¶ƒXƒe[ƒW‚É‚æ‚é“–‚½‚è”»’è‚Ì‰Šú‰»
	virtual void InitCollider() {}

	int BGhandle_;	// ”wŒi‰æ‘œƒnƒ“ƒhƒ‹
	int guideMoveHandle_;	// ˆÚ“®‰Â”\‚ğ¦‚·‘€ìƒKƒCƒh‰æ‘œƒnƒ“ƒhƒ‹
	std::vector<TransformObject> objects_;		// áŠQ•¨‚ÌˆÊ’u

	// ‰æ–ÊÀ•W‚Å•`‰æ‚·‚éƒIƒuƒWƒFƒNƒg‚Ìˆê——
	std::vector<RenderObject> screenObjects_;
	std::vector<Rect> movementRectPercents_;	// “–‚½‚è”»’è

	// ƒŠƒ\[ƒXŠÇ—‚Ö‚ÌQÆ
	ResourceManager& resMng_;
};
