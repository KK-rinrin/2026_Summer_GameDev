#pragma once
#include <algorithm>
#include <functional>
#include <vector>
#include "Actor/ActorBase.h"
#include "Common/Transform2D.h"
#include "Common/Sprite2D.h"

class Renderer2D
{
public:

	// レイヤー順を優先し、同じレイヤー内ではY座標による奥行き順で描画する
	enum class Layer
	{
		Back,	// 最背面の描画レイヤー
		Object,	// 通常オブジェクトの描画レイヤー
		Front,	// 最前面の描画レイヤー
	};

	void Clear();

	// キャラクターの描画登録
	void AddActor(ActorBase* actor, Layer layer = Layer::Object);

	// 座標情報とスプライトの描画登録
	void AddSprite(const Transform2D* transform, const Sprite2D* sprite, Layer layer = Layer::Object);

	// 固定Y基準値による描画処理の登録
	void Add(float sortY, const std::function<void()>& drawFunc, Layer layer = Layer::Object);

	// 動的Y基準値による描画処理の登録
	void Add(const std::function<float()>& sortYFunc, const std::function<void()>& drawFunc, Layer layer = Layer::Object);

	// レイヤーと奥行きと登録順による描画の実行
	void Render();

private:

	// 描画順と描画処理の登録情報
	struct RenderItem
	{

		// 描画するレイヤー
		Layer layer;

		// 同じ描画順の場合に使用する登録順
		int order;

		// 描画順のY基準値を取得する関数
		std::function<float()> sortYFunc;

		// 対象を描画する関数
		std::function<void()> drawFunc;
	};

	// 登録済み描画処理の一覧
	std::vector<RenderItem> drawList_;

	// 次の登録に割り当てる順序番号
	int nextOrder_ = 0;
};
