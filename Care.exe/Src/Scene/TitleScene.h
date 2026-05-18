#pragma once
#include "SceneBase.h"
#include "../Object/Common/2DTransform.h"
#include "../Manager/Live2D.h"

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

	struct FaceParams
	{
		float angleX;
		float angleY;
		float angleZ;
		float bodyAngleX;
		float eyeBallX;
		float eyeBallY;
		float eyeLOpen;
		float eyeROpen;
		float eyeLSmile;
		float eyeRSmile;
		float mouthForm;
		float mouthOpenY;
		float cheek;
		float browLY;
		float browRY;
		float browLAngle;
		float browRAngle;
	};

	void InitLoad() override;

	void UpdateSelectMenu(void);

	void UpdatePlayerModel(void);

	void DrawPlayerModel(void);

	void DrawSelectMenu(void);

	void MoveSelectMenu(int move);

	void DecideSelectMenu(void);

	void ApplyPlayerFace(void);

	FaceParams GetTargetPlayerFace(void) const;

	FaceParams LerpPlayerFace(const FaceParams& start, const FaceParams& end) const;

	void SetPlayerFaceParams(const FaceParams& params);

	int imgTitle_;
	int font_;
	int selectMenu_;
	Live2D playerModel_;
	FaceParams playerFaceParams_;
	bool isPlayerFaceInitialized_;
};
