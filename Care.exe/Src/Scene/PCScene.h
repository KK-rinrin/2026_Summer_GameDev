#pragma once
#include "../Common/Vector2.h"

class InputManager;

class PCScene
{
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
	static constexpr Vector2 MANUAL_FOLDER_POS = { 30, 45 };

	int backgroundHandle_;
	int frameHandle_;
	int manualFolderHandle_;
	int bpManualHandle_;
	int cursorHandle_;
	Vector2 manualFolderSize_;
	Vector2 cursorPos_;
	bool isOpen_;
	bool isBPManualVisible_;
};
