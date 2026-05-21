#pragma once
#include "SceneBase.h"
#include "../Object/Common/2DTransform.h"
#include <memory>

class Live2DTalkController;

class TitleScene : public SceneBase
{
	static constexpr int PUSH_START_BLINK_INTERVAL = 30;
	static constexpr int MENU_ITEM_NUM = 3;
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
	enum class Menu
	{
		START,
		SETTING,
		CLOSE,
	};

	void InitLoad() override;

	void UpdateSelectMenu(void);

	void UpdatePlayerModel(void);

	void DrawPlayerModel(void);

	void DrawSelectMenu(void);

	void MoveSelectMenu(int move);

	void DecideSelectMenu(void);

	int imgTitle_;
	int font_;
	int selectMenu_;
	std::shared_ptr<Live2DTalkController> liveTalkController_;
};