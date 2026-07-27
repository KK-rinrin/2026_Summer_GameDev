#include <DxLib.h>
#include "PCScene.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/ResourceManager.h"

namespace
{
	constexpr float CURSOR_MOVE_SPEED = 12.0f;

	bool IsPadDecideTrgDown(const InputManager& input)
	{
		const KeyConfig::PadBinding* binding =
			KeyConfig::FindPadBinding(KeyConfig::ACTION::DECIDE);
		if (binding == nullptr)
		{
			return false;
		}

		const InputManager::JOYPAD_BTN invalidButton = InputManager::JOYPAD_BTN::MAX;
		return (binding->fixedButton != invalidButton &&
			input.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, binding->fixedButton)) ||
			(binding->configurableButton != invalidButton &&
				input.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, binding->configurableButton));
	}
}

constexpr Vector2 PCScene::MANUAL_FOLDER_POS;

PCScene::PCScene(void)
	:
	backgroundHandle_(-1),
	frameHandle_(-1),
	manualFolderHandle_(-1),
	bpManualHandle_(-1),
	cursorHandle_(-1),
	manualFolderSize_(0, 0),
	cursorPos_(0, 0),
	isOpen_(false),
	isBPManualVisible_(false)
{
}

PCScene::~PCScene(void)
{
}

void PCScene::Load(void)
{
	ResourceManager& resMng = ResourceManager::GetInstance();
	backgroundHandle_ = resMng.Load(ResourceManager::SRC::PC_BG).handleId_;
	frameHandle_ = resMng.Load(ResourceManager::SRC::PC_FRAME).handleId_;
	manualFolderHandle_ = resMng.Load(ResourceManager::SRC::PC_MANUAL_FOLDER).handleId_;
	bpManualHandle_ = resMng.Load(ResourceManager::SRC::PC_BP_MANUAL).handleId_;
	cursorHandle_ = resMng.Load(ResourceManager::SRC::PC_CURSOR).handleId_;

	GetGraphSize(manualFolderHandle_, &manualFolderSize_.x, &manualFolderSize_.y);
}

void PCScene::Update(const InputManager& input)
{
	if (!isOpen_)
	{
		return;
	}

	cursorPos_ = input.GetMousePos();
	const VECTOR stickInput = input.GetLeftStickInput(InputManager::JOYPAD_NO::PAD1);
	if (stickInput.x != 0.0f || stickInput.y != 0.0f)
	{
		cursorPos_.x += static_cast<int>(stickInput.x * CURSOR_MOVE_SPEED);
		cursorPos_.y += static_cast<int>(stickInput.y * CURSOR_MOVE_SPEED);

		if (cursorPos_.x < 0)
		{
			cursorPos_.x = 0;
		}
		else if (cursorPos_.x >= Application::SCREEN_SIZE_X)
		{
			cursorPos_.x = Application::SCREEN_SIZE_X - 1;
		}
		if (cursorPos_.y < 0)
		{
			cursorPos_.y = 0;
		}
		else if (cursorPos_.y >= Application::SCREEN_SIZE_Y)
		{
			cursorPos_.y = Application::SCREEN_SIZE_Y - 1;
		}

		SetMousePoint(cursorPos_.x, cursorPos_.y);
	}

	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, input))
	{
		if (isBPManualVisible_)
		{
			isBPManualVisible_ = false;
		}
		else
		{
			isOpen_ = false;
		}
		return;
	}

	if (!input.IsTrgMouseLeft() && !IsPadDecideTrgDown(input))
	{
		return;
	}

	if (cursorPos_.x >= MANUAL_FOLDER_POS.x &&
		cursorPos_.x <= MANUAL_FOLDER_POS.x + manualFolderSize_.x &&
		cursorPos_.y >= MANUAL_FOLDER_POS.y &&
		cursorPos_.y <= MANUAL_FOLDER_POS.y + manualFolderSize_.y)
	{
		isBPManualVisible_ = true;
	}
}

void PCScene::Draw(void)
{
	if (!isOpen_)
	{
		return;
	}

	DrawGraph(0, 0, backgroundHandle_, TRUE);
	DrawGraph(MANUAL_FOLDER_POS.x, MANUAL_FOLDER_POS.y, manualFolderHandle_, TRUE);

	if (isBPManualVisible_)
	{
		DrawGraph(0, 0, bpManualHandle_, TRUE);
	}

	DrawGraph(cursorPos_.x, cursorPos_.y, cursorHandle_, TRUE);
	DrawGraph(0, 0, frameHandle_, TRUE);
}

void PCScene::Delete(void)
{
	backgroundHandle_ = -1;
	frameHandle_ = -1;
	manualFolderHandle_ = -1;
	bpManualHandle_ = -1;
	cursorHandle_ = -1;
}

void PCScene::Open(void)
{
	isOpen_ = true;
	isBPManualVisible_ = false;
}

bool PCScene::IsOpen(void) const
{
	return isOpen_;
}
