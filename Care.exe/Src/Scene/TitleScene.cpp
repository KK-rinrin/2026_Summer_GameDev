#include <array>
#include <DxLib.h>
#include <Windows.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "TitleScene.h"
#include "../Application.h"
#include "../Manager/Live2D.h"

namespace
{
	static constexpr int TITLE_IMAGE_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int TITLE_IMAGE_BOTTOM_OFFSET = 500;
	static constexpr int TITLE_IMAGE_POS_Y = Application::SCREEN_SIZE_Y - TITLE_IMAGE_BOTTOM_OFFSET;

	static constexpr float PLAYER_MODEL_EXTEND_X = 2.5f;
	static constexpr float PLAYER_MODEL_EXTEND_Y = 2.5f;
	static constexpr float PLAYER_MODEL_POS_X = -240.0f;
	static constexpr float PLAYER_MODEL_POS_Y = -300.0f;

	static constexpr int MENU_POS_X = 520;
	static constexpr int MENU_POS_Y = 285;
	static constexpr int MENU_INTERVAL_Y = 54;
	static constexpr int MENU_COLOR = 0xffffff;
	static constexpr int MENU_SELECTED_COLOR = 0xffffff;
	static constexpr const char* SELECT_MARK = "\xE2\x96\xB6 ";
	static constexpr const char* MENU_MARK_SPACE = "  ";
	static constexpr std::array<const char*, 3> MENU_TEXTS = { "Start", "Setting", "Close" };

	static constexpr float PLAYER_FACE_LERP_RATE = 0.18f;
	static constexpr float LOOK_ANGLE_X = 18.0f;
	static constexpr float LOOK_BODY_X = 8.0f;
	static constexpr float LOOK_EYE_X = 0.65f;
	static constexpr float LOOK_ANGLE_START_Y = 7.0f;
	static constexpr float LOOK_ANGLE_SETTING_Y = 0.0f;
	static constexpr float LOOK_ANGLE_CLOSE_Y = -7.0f;
	static constexpr float LOOK_EYE_START_Y = 0.25f;
	static constexpr float LOOK_EYE_SETTING_Y = 0.0f;
	static constexpr float LOOK_EYE_CLOSE_Y = -0.25f;

	static constexpr float FACE_EYE_OPEN = 1.0f;
	static constexpr float FACE_START_EYE_SMILE = 0.65f;
	static constexpr float FACE_START_MOUTH_FORM = 0.45f;
	static constexpr float FACE_START_CHEEK = 0.25f;
	static constexpr float FACE_SETTING_BROW_ANGLE = -0.45f;
	static constexpr float FACE_SETTING_MOUTH_FORM = 0.1f;
	static constexpr float FACE_CLOSE_EYE_OPEN = 0.75f;
	static constexpr float FACE_CLOSE_BROW_Y = -0.45f;
	static constexpr float FACE_CLOSE_MOUTH_FORM = -0.35f;
}

TitleScene::TitleScene(void)
	:
	SceneBase(),
	imgTitle_(-1),
	font_(-1),
	selectMenu_(static_cast<int>(Menu::START)),
	playerModel_(),
	playerFaceParams_{},
	isPlayerFaceInitialized_(false)
{
}

TitleScene::~TitleScene(void)
{
	Delete();
}

void TitleScene::Update(void)
{
	UpdateSelectMenu();
	UpdatePlayerModel();
}

void TitleScene::Draw(void)
{
	DrawRotaGraph(TITLE_IMAGE_POS_X, TITLE_IMAGE_POS_Y, 1.0, 0.0, imgTitle_, TRUE);
	DrawPlayerModel();
	DrawSelectMenu();
}

void TitleScene::Delete(void)
{
	playerModel_.DeleteModel();
}

void TitleScene::InitLoad()
{
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE_IMG).handleId_;
	font_ = resMng_.Load(ResourceManager::SRC::TITLE_FONT).handleId_;
	playerModel_.Load(ResourceManager::SRC::PLAYER_MODEL);
	playerModel_.SetExtendRate(PLAYER_MODEL_EXTEND_X, PLAYER_MODEL_EXTEND_Y);
	playerModel_.SetTranslate(PLAYER_MODEL_POS_X, PLAYER_MODEL_POS_Y);
	playerFaceParams_ = GetTargetPlayerFace();
	isPlayerFaceInitialized_ = true;
}

void TitleScene::UpdateSelectMenu(void)
{
	if (iptMng_.IsTrgDown(KEY_INPUT_UP) || iptMng_.IsTrgDown(KEY_INPUT_W))
	{
		MoveSelectMenu(-1);
	}
	if (iptMng_.IsTrgDown(KEY_INPUT_DOWN) || iptMng_.IsTrgDown(KEY_INPUT_S))
	{
		MoveSelectMenu(1);
	}
	if (iptMng_.IsTrgDown(KEY_INPUT_RETURN) || iptMng_.IsTrgDown(KEY_INPUT_SPACE))
	{
		DecideSelectMenu();
	}
}

void TitleScene::UpdatePlayerModel(void)
{
	playerModel_.SetExtendRate(PLAYER_MODEL_EXTEND_X, PLAYER_MODEL_EXTEND_Y);
	playerModel_.SetTranslate(PLAYER_MODEL_POS_X, PLAYER_MODEL_POS_Y);
	playerModel_.Update(TEXT("Idle"));
	ApplyPlayerFace();
}

void TitleScene::DrawPlayerModel(void)
{
	playerModel_.DrawBegin();
	playerModel_.DrawModel();
	playerModel_.DrawEnd();
}

void TitleScene::DrawSelectMenu(void)
{
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	for (int i = 0; i < MENU_ITEM_NUM; ++i)
	{
		const int drawY = MENU_POS_Y + MENU_INTERVAL_Y * i;
		const int color = (i == selectMenu_) ? MENU_SELECTED_COLOR : MENU_COLOR;
		const char* mark = (i == selectMenu_) ? SELECT_MARK : MENU_MARK_SPACE;
		DrawFormatStringToHandle(MENU_POS_X, drawY, color, font_, "%s%s", mark, MENU_TEXTS[i]);
	}
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_SHIFTJIS);
}

void TitleScene::MoveSelectMenu(int move)
{
	selectMenu_ = (selectMenu_ + move + MENU_ITEM_NUM) % MENU_ITEM_NUM;
}

void TitleScene::DecideSelectMenu(void)
{
	switch (static_cast<Menu>(selectMenu_))
	{
	case Menu::START:
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		break;
	case Menu::SETTING:
		break;
	case Menu::CLOSE:
		PostQuitMessage(0);
		break;
	}
}

void TitleScene::ApplyPlayerFace(void)
{
	const FaceParams target = GetTargetPlayerFace();
	if (!isPlayerFaceInitialized_)
	{
		playerFaceParams_ = target;
		isPlayerFaceInitialized_ = true;
	}
	else
	{
		playerFaceParams_ = LerpPlayerFace(playerFaceParams_, target);
	}

	SetPlayerFaceParams(playerFaceParams_);
}

TitleScene::FaceParams TitleScene::GetTargetPlayerFace(void) const
{
	FaceParams params{};
	params.angleX = LOOK_ANGLE_X;
	params.angleY = LOOK_ANGLE_SETTING_Y;
	params.angleZ = 0.0f;
	params.bodyAngleX = LOOK_BODY_X;
	params.eyeBallX = LOOK_EYE_X;
	params.eyeBallY = LOOK_EYE_SETTING_Y;
	params.eyeLOpen = FACE_EYE_OPEN;
	params.eyeROpen = FACE_EYE_OPEN;
	params.eyeLSmile = 0.0f;
	params.eyeRSmile = 0.0f;
	params.mouthForm = 0.0f;
	params.mouthOpenY = 0.0f;
	params.cheek = 0.0f;
	params.browLY = 0.0f;
	params.browRY = 0.0f;
	params.browLAngle = 0.0f;
	params.browRAngle = 0.0f;

	switch (static_cast<Menu>(selectMenu_))
	{
	case Menu::START:
		params.angleY = LOOK_ANGLE_START_Y;
		params.eyeBallY = LOOK_EYE_START_Y;
		params.eyeLSmile = FACE_START_EYE_SMILE;
		params.eyeRSmile = FACE_START_EYE_SMILE;
		params.mouthForm = FACE_START_MOUTH_FORM;
		params.cheek = FACE_START_CHEEK;
		break;
	case Menu::SETTING:
		params.angleY = LOOK_ANGLE_SETTING_Y;
		params.eyeBallY = LOOK_EYE_SETTING_Y;
		params.browLAngle = FACE_SETTING_BROW_ANGLE;
		params.browRAngle = -FACE_SETTING_BROW_ANGLE;
		params.mouthForm = FACE_SETTING_MOUTH_FORM;
		break;
	case Menu::CLOSE:
		params.angleY = LOOK_ANGLE_CLOSE_Y;
		params.eyeBallY = LOOK_EYE_CLOSE_Y;
		params.eyeLOpen = FACE_CLOSE_EYE_OPEN;
		params.eyeROpen = FACE_CLOSE_EYE_OPEN;
		params.browLY = FACE_CLOSE_BROW_Y;
		params.browRY = FACE_CLOSE_BROW_Y;
		params.mouthForm = FACE_CLOSE_MOUTH_FORM;
		break;
	}

	return params;
}

TitleScene::FaceParams TitleScene::LerpPlayerFace(const FaceParams& start, const FaceParams& end) const
{
	FaceParams params{};
	params.angleX = AsoUtility::Lerp(start.angleX, end.angleX, PLAYER_FACE_LERP_RATE);
	params.angleY = AsoUtility::Lerp(start.angleY, end.angleY, PLAYER_FACE_LERP_RATE);
	params.angleZ = AsoUtility::Lerp(start.angleZ, end.angleZ, PLAYER_FACE_LERP_RATE);
	params.bodyAngleX = AsoUtility::Lerp(start.bodyAngleX, end.bodyAngleX, PLAYER_FACE_LERP_RATE);
	params.eyeBallX = AsoUtility::Lerp(start.eyeBallX, end.eyeBallX, PLAYER_FACE_LERP_RATE);
	params.eyeBallY = AsoUtility::Lerp(start.eyeBallY, end.eyeBallY, PLAYER_FACE_LERP_RATE);
	params.eyeLOpen = AsoUtility::Lerp(start.eyeLOpen, end.eyeLOpen, PLAYER_FACE_LERP_RATE);
	params.eyeROpen = AsoUtility::Lerp(start.eyeROpen, end.eyeROpen, PLAYER_FACE_LERP_RATE);
	params.eyeLSmile = AsoUtility::Lerp(start.eyeLSmile, end.eyeLSmile, PLAYER_FACE_LERP_RATE);
	params.eyeRSmile = AsoUtility::Lerp(start.eyeRSmile, end.eyeRSmile, PLAYER_FACE_LERP_RATE);
	params.mouthForm = AsoUtility::Lerp(start.mouthForm, end.mouthForm, PLAYER_FACE_LERP_RATE);
	params.mouthOpenY = AsoUtility::Lerp(start.mouthOpenY, end.mouthOpenY, PLAYER_FACE_LERP_RATE);
	params.cheek = AsoUtility::Lerp(start.cheek, end.cheek, PLAYER_FACE_LERP_RATE);
	params.browLY = AsoUtility::Lerp(start.browLY, end.browLY, PLAYER_FACE_LERP_RATE);
	params.browRY = AsoUtility::Lerp(start.browRY, end.browRY, PLAYER_FACE_LERP_RATE);
	params.browLAngle = AsoUtility::Lerp(start.browLAngle, end.browLAngle, PLAYER_FACE_LERP_RATE);
	params.browRAngle = AsoUtility::Lerp(start.browRAngle, end.browRAngle, PLAYER_FACE_LERP_RATE);
	return params;
}

void TitleScene::SetPlayerFaceParams(const FaceParams& params)
{
	playerModel_.SetParamerterValue(TEXT("ParamAngleX"), params.angleX);
	playerModel_.SetParamerterValue(TEXT("ParamAngleY"), params.angleY);
	playerModel_.SetParamerterValue(TEXT("ParamAngleZ"), params.angleZ);
	playerModel_.SetParamerterValue(TEXT("ParamBodyAngleX"), params.bodyAngleX);
	playerModel_.SetParamerterValue(TEXT("ParamEyeBallX"), params.eyeBallX);
	playerModel_.SetParamerterValue(TEXT("ParamEyeBallY"), params.eyeBallY);
	playerModel_.SetParamerterValue(TEXT("ParamEyeLOpen"), params.eyeLOpen);
	playerModel_.SetParamerterValue(TEXT("ParamEyeROpen"), params.eyeROpen);
	playerModel_.SetParamerterValue(TEXT("ParamEyeLSmile"), params.eyeLSmile);
	playerModel_.SetParamerterValue(TEXT("ParamEyeRSmile"), params.eyeRSmile);
	playerModel_.SetParamerterValue(TEXT("ParamMouthForm"), params.mouthForm);
	playerModel_.SetParamerterValue(TEXT("ParamMouthOpenY"), params.mouthOpenY);
	playerModel_.SetParamerterValue(TEXT("ParamCheek"), params.cheek);
	playerModel_.SetParamerterValue(TEXT("ParamBrowLY"), params.browLY);
	playerModel_.SetParamerterValue(TEXT("ParamBrowRY"), params.browRY);
	playerModel_.SetParamerterValue(TEXT("ParamBrowLAngle"), params.browLAngle);
	playerModel_.SetParamerterValue(TEXT("ParamBrowRAngle"), params.browRAngle);
}
