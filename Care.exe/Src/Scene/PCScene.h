#pragma once
#include <string>
#include "../Common/Vector2.h"

class InputManager;
class SoundManager;

class PCScene
{
	static constexpr float CURSOR_MOVE_SPEED = 12.0f;
	static constexpr int PC_FONT_SIZE = 22;
	static constexpr int PC_FONT_SIZE_S = 18;
	static constexpr int PC_FONT_THICK = 0;
	static constexpr int PC_FONT_SPACE = -1;
	static constexpr Vector2 KARTE_TEXT_POS = { 40, 55 };
	static constexpr int KARTE_TEXT_LINE_INTERVAL_Y = 40;
	static constexpr Vector2 FILE_POS[] = { { 60, 80 }, { 170, 80 } };
	static constexpr int FILE_LABEL_Y_OFFSET = 96;
	static constexpr Vector2 ACCESS_DENIED_BOX_POS = { 200, 180 };
	static constexpr Vector2 ACCESS_DENIED_BOX_SIZE = { 400, 180 };
	static constexpr int ACCESS_DENIED_TITLE_BAR_HEIGHT = 28;
	static constexpr Vector2 ACCESS_DENIED_OK_POS = { 480, 310 };
	static constexpr Vector2 ACCESS_DENIED_OK_SIZE = { 80, 28 };
	static constexpr const char* PC_GUIDE_TEXT = "マウス操作可　キャンセルキーで閉じる　×";
	static constexpr const char* PC_GUIDE_CLOSE_TEXT = "×";
	static constexpr int PC_GUIDE_CLOSE_POS_X = 750;
	static constexpr int PC_GUIDE_CLOSE_POS_Y = 31;
	static constexpr int PC_GUIDE_CLOSE_HIT_SIZE_Y = 24;

public:

	PCScene(void);
	~PCScene(void);

	void Load(void);
	void Update(const InputManager& input);
	void Draw(void);
	void Delete(void);
	void Open(void);
	bool IsOpen(void) const;

private:
	void HandleCancel();
	void UpdateKarteText();
	void DrawKarte() const;
	void DrawFiles() const;
	void DrawAccessDeniedMessage() const;
	void UpdatePcFolderState();
	bool IsFileHit(int fileIndex) const;
	bool IsAccessDeniedOkHit() const;
	bool IsCloseButtonHit() const;

	static constexpr Vector2 FOLDER_POS[] = { { 70, 33 }, { 70, 155 }, { 70, 277 } };

	int backgroundHandle_;
	int frameHandle_;
	int folderHandle_;
	int fileHandle_;
	int bpManualHandle_;
	int cursorHandle_;
	int pcFontHandle_;
	int pcFontHandleS_;
	Vector2 folderSize_;
	Vector2 fileSize_;
	Vector2 cursorPos_;
	std::string nurseName_;
	std::string symptomText_;
	bool isOpen_;
	bool isBPManualVisible_;
	bool isKarteVisible_;
	bool isCharaFolderVisible_;
	bool isCharaFolderOpen_;
	bool isAccessDeniedVisible_;

	SoundManager* soundMng_;
};
