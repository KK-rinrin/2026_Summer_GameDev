#pragma once
#include "SceneBase.h"
#include "../Object/Common/2DTransform.h"

class Live2D;

class TitleScene : public SceneBase
{
	static constexpr int PUSH_START_BLINK_INTERVAL = 30;
public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Delete(void) override;

private:
	void InitLoad() override;

	void UpdateSelectMenu(void);

	void DrawSelectMenu(void);

	int imgTitle_;
	int font_;
};
