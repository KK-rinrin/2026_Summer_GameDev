#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "TitleScene.h"
#include "../Application.h"
#include "../Manager/Live2D.h"

TitleScene::TitleScene(void)
	:
	SceneBase()
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Update(void)
{
	UpdateSelectMenu();
}

void TitleScene::Draw(void)
{
	DrawSelectMenu();
}

void TitleScene::Delete(void)
{
}

void TitleScene::InitLoad()
{
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE_IMG).handleId_;
	font_ = resMng_.Load(ResourceManager::SRC::TITLE_FONT).handleId_;
	
}

void TitleScene::UpdateSelectMenu(void)
{
	if (iptMng_.IsNew(KEY_INPUT_SPACE))
	{
		sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void TitleScene::DrawSelectMenu(void)
{
	DrawRotaGraph(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y - 200, 1.0, 0.0, imgTitle_, TRUE);
}
