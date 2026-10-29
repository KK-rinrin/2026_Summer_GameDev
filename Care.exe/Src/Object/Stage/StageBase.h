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
		PAT_ROOM,		// 患者部屋のステージID
		NURSE_STATION,	// ナースステーションのステージID
	};

	// ステージ内イベント実行時にGameScene側の所有物へアクセスするための窓口
	struct DecideContext
	{
		ActorBase& controlActor;			// コントロールするキャラ
		const ActorBase* patientActor;		// 患者キャラ
		ProgressManager& progressManager;	// 進行度管理マネージャ
		Talk& talk;							// 会話
		SoundManager& soundManager;			// サウンドマネージャ
		std::function<void(StageId, const Vector2<float>&)> ChangeStage;	// 移動先のステージ・座標へ移動
	};

	StageBase();
	virtual ~StageBase();

	void Init();
	virtual void Update();
	virtual void Draw();

	// 派生ステージの前景描画
	virtual void DrawForeground() const;
	virtual void Delete();

	// ステージ内操作による入力ブロックの判定
	virtual bool IsInputBlocked() const;

	void DrawBG() const;	// 背景を描画
	virtual void RegisterObjects(Renderer2D& renderer);	// オブジェクト登録
	void ApplyCollision(ActorBase& actor) const;		// 当たり判定登録

	// 操作キャラの位置に応じた移動ガイドの描画
	virtual void DrawGuide(const ActorBase& controlActor) const { (void)controlActor; }

	// ステージ内の決定操作に対応するイベント実行
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
		Vector2<float> leftTop;    // 左上 (x=0..100, y=0..100)
		Vector2<float> rightBottom;// 右下 (x=0..100, y=0..100)
	};

	// 座標変換を伴うステージオブジェクトの描画情報
	struct TransformObject
	{
		Transform2D transform;	// 物体情報
		Sprite2D sprite;		// 画像スプライトデータ
		void Draw() const;		// 描画
		bool useSortY;			// レンダリング順にsortYを適用するか？false→transformから取得
		float sortY;			// 前後を切り替えるY座標（プレイヤーがこれより奥に行くと手前に出る）
	};

	void AddScreenObject(int handle, float sortY, bool trans = true);	// スクリーンオブジェクトを追加
	void AddTransformObject(const Transform2D& transform, const Sprite2D& sprite);				// 物体を追加
	void AddTransformObject(const Transform2D& transform, const Sprite2D& sprite, float sortY);	// 物体を追加（奥行きつき）
	void AddMBRectPercent(const Vector2<float>& leftTopPercent, const Vector2<float>& rightBottomPercent);	// 障害物当たり判定（矩形）を作成
	void DrawMoveGuide(const Vector2<int>& pos) const;	// 移動ガイドを描画

	// 派生ステージによるリソース読み込み
	virtual void InitLoad() {}

	// 派生ステージによる座標情報の初期化
	virtual void InitTransform() {}

	// 派生ステージによる当たり判定の初期化
	virtual void InitCollider() {}

	int BGhandle_;	// 背景画像ハンドル
	int guideMoveHandle_;	// 移動可能を示す操作ガイド画像ハンドル
	std::vector<TransformObject> objects_;		// 障害物の位置

	// 画面座標で描画するオブジェクトの一覧
	std::vector<RenderObject> screenObjects_;
	std::vector<Rect> movementRectPercents_;	// 当たり判定

	// リソース管理への参照
	ResourceManager& resMng_;
};
