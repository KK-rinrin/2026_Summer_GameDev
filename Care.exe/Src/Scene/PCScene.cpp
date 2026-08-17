#include <DxLib.h>
#include <cstring>
#include "PCScene.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/PadInput.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/ProgressManager.h"
#include "../Manager/SoundManager.h"
#include "../Utility/SchoolUtility.h"

	static bool IsPadDecideTrgDown(const InputManager& input)
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

constexpr Vector2 PCScene::FOLDER_POS[];

PCScene::PCScene(void)
	:
	backgroundHandle_(-1),
	frameHandle_(-1),
	folderHandle_(-1),
	fileHandle_(-1),
	bpManualHandle_(-1),
	cursorHandle_(-1),
	pcFontHandle_(-1),
	pcFontHandleS_(-1),
	folderSize_(0, 0),
	fileSize_(0, 0),
	cursorPos_(0, 0),
	nurseName_("■■■"),
	symptomText_("不眠、妄想"),
	isOpen_(false),
	isBPManualVisible_(false),
	isKarteVisible_(false),
	isCharaFolderVisible_(false),
	isCharaFolderOpen_(false),
	isAccessDeniedVisible_(false),
	soundMng_(nullptr)
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
	folderHandle_ = resMng.Load(ResourceManager::SRC::PC_MANUAL_FOLDER).handleId_;
	fileHandle_ = resMng.Load(ResourceManager::SRC::PC_FILE).handleId_;
	bpManualHandle_ = resMng.Load(ResourceManager::SRC::PC_BP_MANUAL).handleId_;
	cursorHandle_ = resMng.Load(ResourceManager::SRC::PC_CURSOR).handleId_;
	pcFontHandle_ = resMng.LoadFont(ResourceManager::SRC::PC_FONT, PC_FONT_SIZE, PC_FONT_THICK, PC_FONT_SPACE, DX_FONTTYPE_NORMAL);
	pcFontHandleS_ = resMng.LoadFont(ResourceManager::SRC::PC_FONT, PC_FONT_SIZE_S, PC_FONT_THICK, PC_FONT_SPACE, DX_FONTTYPE_NORMAL);
	soundMng_ = &SoundManager::GetInstance();

	GetGraphSize(folderHandle_, &folderSize_.x, &folderSize_.y);
	GetGraphSize(fileHandle_, &fileSize_.x, &fileSize_.y);
}

void PCScene::Update(const InputManager& input)
{
	if (!isOpen_)
	{
		return;
	}

	UpdateKarteText();
	UpdatePcFolderState();
	cursorPos_ = input.GetMousePos();
	const VECTOR stickInput = PadInput::GetMoveAxis(input, InputManager::JOYPAD_NO::PAD1);
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

	if (isAccessDeniedVisible_ && input.IsTrgMouseLeft())
	{
		if (IsAccessDeniedOkHit())
		{
			isAccessDeniedVisible_ = false;
		}
		return;
	}

	if (isCharaFolderOpen_ && input.IsTrgMouseLeft())
	{
		for (int i = 0; i < 2; ++i)
		{
			if (IsFileHit(i))
			{
			soundMng_->PlaySE(SoundManager::SE::MOUSE);
				isAccessDeniedVisible_ = true;
				return;
			}
		}
	}

	if (KeyConfig::IsTrgDown(KeyConfig::ACTION::CANCEL, input))
	{
		HandleCancel();
		return;
	}

	if (input.IsTrgMouseLeft() && IsCloseButtonHit())
	{
		HandleCancel();
		return;
	}

	if (!input.IsTrgMouseLeft() && !IsPadDecideTrgDown(input))
	{
		return;
	}

	if (SchoolUtility::IsPointInRect(cursorPos_, FOLDER_POS[0], folderSize_))
	{
		soundMng_->PlaySE(SoundManager::SE::MOUSE);
		isBPManualVisible_ = true;
		isKarteVisible_ = false;
	}
	else if (SchoolUtility::IsPointInRect(cursorPos_, FOLDER_POS[1], folderSize_))
	{
		soundMng_->PlaySE(SoundManager::SE::MOUSE);
		isBPManualVisible_ = false;
		isKarteVisible_ = true;
	}
	else if (isCharaFolderVisible_ && SchoolUtility::IsPointInRect(cursorPos_, FOLDER_POS[2], folderSize_))
	{
		soundMng_->PlaySE(SoundManager::SE::MOUSE);
		isBPManualVisible_ = false;
		isKarteVisible_ = false;
		isCharaFolderOpen_ = true;
	}
}

void PCScene::Draw(void)
{
	if (!isOpen_)
	{
		return;
	}

	DrawGraph(0, 0, backgroundHandle_, TRUE);
	if (!isBPManualVisible_ && !isKarteVisible_ && !isCharaFolderOpen_)
	{
		for (int i = 0; i < (isCharaFolderVisible_ ? 3 : 2); ++i)
	{
		const Vector2& folderPos = FOLDER_POS[i];
		DrawGraph(folderPos.x, folderPos.y, folderHandle_, TRUE);

		const char* label = (i == 0) ? "マニュアル確認" : (i == 1 ? "カルテ" : "Chara");
		const int textWidth = GetDrawStringWidthToHandle(
			label, static_cast<int>(std::strlen(label)), pcFontHandle_);
		const int textX = folderPos.x + (folderSize_.x - textWidth) / 2;
		const int textY = folderPos.y + folderSize_.y;
		DrawStringToHandle(textX, textY, label, GetColor(0, 0, 0), pcFontHandle_);
		}
	}

	if (isBPManualVisible_)
	{
		DrawGraph(0, 0, bpManualHandle_, TRUE);
	}

	if (isKarteVisible_)
	{
		DrawKarte();
	}

	if (isCharaFolderOpen_)
	{
		DrawFiles();
	}

	if (pcFontHandleS_ >= 0)
	{
		const int prefixLength = static_cast<int>(std::strlen(PC_GUIDE_TEXT) - std::strlen(PC_GUIDE_CLOSE_TEXT));
		const int prefixWidth = GetDrawStringWidthToHandle(PC_GUIDE_TEXT, prefixLength, pcFontHandleS_);
		const int guideX = PC_GUIDE_CLOSE_POS_X - prefixWidth;
		DrawStringToHandle(guideX, PC_GUIDE_CLOSE_POS_Y, PC_GUIDE_TEXT, GetColor(0, 0, 0), pcFontHandleS_);
	}

	if (isAccessDeniedVisible_)
	{
		DrawAccessDeniedMessage();
	}

	DrawGraph(cursorPos_.x, cursorPos_.y, cursorHandle_, TRUE);
	DrawGraph(0, 0, frameHandle_, TRUE);
}


void PCScene::UpdateKarteText()
{
	const int progress = ProgressManager::GetInstance().GetProgress();
	nurseName_ = progress >= ProgressManager::AFTER_PC ? "ネイト" : "■■■";
	symptomText_ = progress >= ProgressManager::AFTER_PC3 ? "不眠" : "不眠、妄想";
}

void PCScene::DrawKarte() const
{
	if (pcFontHandle_ < 0)
	{
		return;
	}

	const int textColor = GetColor(0, 0, 0);
	DrawStringToHandle(KARTE_TEXT_POS.x, KARTE_TEXT_POS.y, "患者カルテ", textColor, pcFontHandle_);
	DrawStringToHandle(KARTE_TEXT_POS.x, KARTE_TEXT_POS.y + KARTE_TEXT_LINE_INTERVAL_Y, "患者名：ヌイ", textColor, pcFontHandle_);
	DrawFormatStringToHandle(KARTE_TEXT_POS.x, KARTE_TEXT_POS.y + KARTE_TEXT_LINE_INTERVAL_Y * 2, textColor, pcFontHandle_, "担当看護師：%s", nurseName_.c_str());
	DrawStringToHandle(KARTE_TEXT_POS.x, KARTE_TEXT_POS.y + KARTE_TEXT_LINE_INTERVAL_Y * 3, "病名：■■■■■■", textColor, pcFontHandle_);
	DrawFormatStringToHandle(KARTE_TEXT_POS.x, KARTE_TEXT_POS.y + KARTE_TEXT_LINE_INTERVAL_Y * 4, textColor, pcFontHandle_, "症状：%s", symptomText_.c_str());
}

void PCScene::UpdatePcFolderState()
{
	const int progress = ProgressManager::GetInstance().GetProgress();
	isCharaFolderVisible_ = progress >= ProgressManager::AFTER_PC3;
	if (!isCharaFolderVisible_)
	{
		isCharaFolderOpen_ = false;
	}
}

void PCScene::DrawFiles() const
{
	if (fileHandle_ < 0 || pcFontHandle_ < 0)
	{
		return;
	}

	for (int i = 0; i < 2; ++i)
	{
		const Vector2& filePos = FILE_POS[i];
		DrawGraph(filePos.x, filePos.y, fileHandle_, TRUE);
		const char* fileName = (i == 0) ? "neit.char" : "nui.char";
		const int textWidth = GetDrawStringWidthToHandle(fileName, static_cast<int>(std::strlen(fileName)), pcFontHandle_);
		const int textX = filePos.x + (fileSize_.x - textWidth) / 2;
		DrawStringToHandle(textX, filePos.y + FILE_LABEL_Y_OFFSET, fileName, GetColor(0, 0, 0), pcFontHandle_);
	}
}

void PCScene::DrawAccessDeniedMessage() const
{
	if (pcFontHandleS_ < 0)
	{
		return;
	}

	const int left = ACCESS_DENIED_BOX_POS.x;
	const int top = ACCESS_DENIED_BOX_POS.y;
	const int right = left + ACCESS_DENIED_BOX_SIZE.x;
	const int bottom = top + ACCESS_DENIED_BOX_SIZE.y;
	const int frameColor = GetColor(80, 80, 80);
	const int windowColor = GetColor(220, 220, 220);
	const int titleColor = GetColor(100, 100, 100);
	const int textColor = GetColor(0, 0, 0);
	DrawBox(left, top, right, bottom, frameColor, TRUE);
	DrawBox(left + 2, top + 2, right - 2, bottom - 2, windowColor, TRUE);
	DrawBox(left + 2, top + 2, right - 2, top + ACCESS_DENIED_TITLE_BAR_HEIGHT, titleColor, TRUE);
	DrawStringToHandle(left + 12, top + 6, "エラー", GetColor(255, 255, 255), pcFontHandleS_);
	DrawStringToHandle(left + 24, top + 58, "アクセス権限がありません。", textColor, pcFontHandleS_);
	DrawBox(ACCESS_DENIED_OK_POS.x, ACCESS_DENIED_OK_POS.y, ACCESS_DENIED_OK_POS.x + ACCESS_DENIED_OK_SIZE.x, ACCESS_DENIED_OK_POS.y + ACCESS_DENIED_OK_SIZE.y, frameColor, TRUE);
	DrawBox(ACCESS_DENIED_OK_POS.x + 2, ACCESS_DENIED_OK_POS.y + 2, ACCESS_DENIED_OK_POS.x + ACCESS_DENIED_OK_SIZE.x - 2, ACCESS_DENIED_OK_POS.y + ACCESS_DENIED_OK_SIZE.y - 2, windowColor, TRUE);
	DrawStringToHandle(ACCESS_DENIED_OK_POS.x + 30, ACCESS_DENIED_OK_POS.y + 5, "OK", textColor, pcFontHandleS_);
}

bool PCScene::IsFileHit(int fileIndex) const
{
	if (fileIndex < 0 || fileIndex >= 2)
	{
		return false;
	}
	return SchoolUtility::IsPointInRect(cursorPos_, FILE_POS[fileIndex], fileSize_);
}

bool PCScene::IsAccessDeniedOkHit() const
{
	return SchoolUtility::IsPointInRect(cursorPos_, ACCESS_DENIED_OK_POS, ACCESS_DENIED_OK_SIZE);
}

void PCScene::Delete(void)
{
	backgroundHandle_ = -1;
	frameHandle_ = -1;
	folderHandle_ = -1;
	fileHandle_ = -1;
	bpManualHandle_ = -1;
	cursorHandle_ = -1;
	pcFontHandle_ = -1;
	pcFontHandleS_ = -1;
	isCharaFolderOpen_ = false;
	isAccessDeniedVisible_ = false;
}

void PCScene::Open(void)
{
	isOpen_ = true;
	isBPManualVisible_ = false;
	isKarteVisible_ = false;
	isCharaFolderOpen_ = false;
	isAccessDeniedVisible_ = false;
}

void PCScene::HandleCancel()
{
	if (isAccessDeniedVisible_)
	{
		isAccessDeniedVisible_ = false;
	}
	else if (isCharaFolderOpen_)
	{
		isCharaFolderOpen_ = false;
	}
	else if (isKarteVisible_)
	{
		isKarteVisible_ = false;
	}
	else if (isBPManualVisible_)
	{
		isBPManualVisible_ = false;
	}
	else
	{
		isOpen_ = false;
	}
}

bool PCScene::IsCloseButtonHit() const
{
	if (pcFontHandle_ < 0)
	{
		return false;
	}

	const int prefixLength = static_cast<int>(std::strlen(PC_GUIDE_TEXT) - std::strlen(PC_GUIDE_CLOSE_TEXT));
	const int prefixWidth = GetDrawStringWidthToHandle(PC_GUIDE_TEXT, prefixLength, pcFontHandle_);
	const int guideX = PC_GUIDE_CLOSE_POS_X - prefixWidth;
	const int closeX = guideX + prefixWidth;
	const int closeWidth = GetDrawStringWidthToHandle(
		PC_GUIDE_CLOSE_TEXT, static_cast<int>(std::strlen(PC_GUIDE_CLOSE_TEXT)), pcFontHandle_);

	return SchoolUtility::IsPointInRect(
		 cursorPos_, Vector2(closeX, PC_GUIDE_CLOSE_POS_Y), Vector2(closeWidth, PC_GUIDE_CLOSE_HIT_SIZE_Y));
}

bool PCScene::IsOpen(void) const
{
	return isOpen_;
}
