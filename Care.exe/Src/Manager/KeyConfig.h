#pragma once
#include <array>
#include "InputManager.h"

class KeyConfig
{

public:
	enum class ACTION
	{
		DECIDE,
		CANCEL,
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		MAX
	};

	static constexpr int INVALID_KEY = -1;
	static constexpr int FIXED_KEY_MAX = 3;

	struct KeyBinding
	{
		std::array<int, FIXED_KEY_MAX> fixedKeys;
		int configurableKey;
	};

	struct PadBinding
	{
		InputManager::JOYPAD_BTN fixedButton;
		InputManager::JOYPAD_BTN configurableButton;
	};

	static void RegisterDefaultKeys(InputManager& input);

	static bool IsNew(ACTION action, const InputManager& input);
	static bool IsTrgDown(ACTION action, const InputManager& input);
	static bool IsTrgUp(ACTION action, const InputManager& input);

	static const KeyBinding* FindBinding(ACTION action);
	static const PadBinding* FindPadBinding(ACTION action);
	static bool SetConfigurableKey(ACTION action, int key, InputManager& input);
	static bool SetConfigurablePadButton(ACTION action, InputManager::JOYPAD_BTN button);
	static const char* GetActionText(ACTION action);
	static const char* GetKeyText(int key);
	static const char* GetPadButtonText(InputManager::JOYPAD_BTN button);

private:
	static constexpr int ACTION_INDEX_MIN = 0;
	static constexpr int ACTION_NUM = static_cast<int>(ACTION::MAX);

	static const std::array<KeyBinding, ACTION_NUM> DEFAULT_BINDINGS;
	static const std::array<PadBinding, ACTION_NUM> DEFAULT_PAD_BINDINGS;
	static const std::array<const char*, ACTION_NUM> ACTION_TEXTS;
	static std::array<KeyBinding, ACTION_NUM> bindings_;
	static std::array<PadBinding, ACTION_NUM> padBindings_;

};
