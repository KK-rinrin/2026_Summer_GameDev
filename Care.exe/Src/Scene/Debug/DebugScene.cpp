#include "DebugScene.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/InputManager.h"
#include "../../Debug/DebugPerspective.h"
#include "../../Debug/DebugCursorPosition.h"
#include "DxLib.h"

DebugScene::DebugScene(void)
{
}

DebugScene::~DebugScene(void)
{
}

void DebugScene::Update(void)
{
	// 上下キーでY分割数を変更
	if (iptMng_.IsTrgDown(KEY_INPUT_UP))
	{
		gridDivisionY_++;
	}
	if (iptMng_.IsTrgDown(KEY_INPUT_DOWN))
	{
		gridDivisionY_ = max(1, gridDivisionY_ - 1);
	}

	// 左右キーでX分割数を変更
	if (iptMng_.IsTrgDown(KEY_INPUT_RIGHT))
	{
		gridDivisionX_++;
	}
	if (iptMng_.IsTrgDown(KEY_INPUT_LEFT))
	{
		gridDivisionX_ = max(1, gridDivisionX_ - 1);
	}

	// 決定キーで背景を変更
	if (iptMng_.IsTrgDown(KEY_INPUT_RETURN))
	{
		currentBG_ = (currentBG_ + 1) % BG_.size();
	}

	cursorPos_->Update();
}

void DebugScene::Draw(void)
{
	DrawGraph(0, 0, BG_[currentBG_], true);

	// 格子を描画
	float stepX = 100.0f / gridDivisionX_;
	float stepY = 100.0f / gridDivisionY_;

	// 縦線を描画
	for (int i = 0; i <= gridDivisionX_; i++)
	{
		float x = i * stepX;
		debugPers_->DrawLineDebug(VECTOR{ x, 0.0f, 0.0f }, VECTOR{ x, 100.0f, 0.0f }, GetColor(0, 0, 0));
	}

	// 横線を描画
	for (int i = 0; i <= gridDivisionY_; i++)
	{
		float y = i * stepY;
		debugPers_->DrawLineDebug(VECTOR{ 0.0f, y, 0.0f }, VECTOR{ 100.0f, y, 0.0f }, GetColor(0, 0, 0));
	}

	cursorPos_->Draw();
}

void DebugScene::Delete(void)
{
	delete cursorPos_;
}

void DebugScene::InitLoad()
{
	cursorPos_ = new DebugCursorPosition();

	BG_.emplace_back(resMng_.Load(ResourceManager::SRC::BG_1).handleId_);
	BG_.emplace_back(resMng_.Load(ResourceManager::SRC::BG_2).handleId_);
}