#include <DxLib.h>
#include "InputManager.h"
#include "KeyConfig.h"

const std::array<KeyConfig::KeyBinding, static_cast<int>(KeyConfig::ACTION::MAX)> KeyConfig::DEFAULT_BINDINGS =
{ {
	{ { KEY_INPUT_SPACE, KEY_INPUT_RETURN }, KEY_INPUT_Z },
	{ { KEY_INPUT_BACK, KEY_INPUT_ESCAPE }, KEY_INPUT_X },
	{ { KEY_INPUT_UP, INVALID_KEY }, KEY_INPUT_W },
	{ { KEY_INPUT_DOWN, INVALID_KEY }, KEY_INPUT_S },
	{ { KEY_INPUT_LEFT, INVALID_KEY }, KEY_INPUT_A },
	{ { KEY_INPUT_RIGHT, INVALID_KEY }, KEY_INPUT_D },
} };

std::array<KeyConfig::KeyBinding, static_cast<int>(KeyConfig::ACTION::MAX)> KeyConfig::bindings_ =
	KeyConfig::DEFAULT_BINDINGS;

const std::array<KeyConfig::PadBinding, static_cast<int>(KeyConfig::ACTION::MAX)> KeyConfig::DEFAULT_PAD_BINDINGS =
{ {
	{ InputManager::JOYPAD_BTN::MAX, InputManager::JOYPAD_BTN::DOWN },
	{ InputManager::JOYPAD_BTN::MAX, InputManager::JOYPAD_BTN::RIGHT },
	{ InputManager::JOYPAD_BTN::DPAD_UP, InputManager::JOYPAD_BTN::MAX },
	{ InputManager::JOYPAD_BTN::DPAD_DOWN, InputManager::JOYPAD_BTN::MAX },
	{ InputManager::JOYPAD_BTN::DPAD_LEFT, InputManager::JOYPAD_BTN::MAX },
	{ InputManager::JOYPAD_BTN::DPAD_RIGHT, InputManager::JOYPAD_BTN::MAX },
} };

std::array<KeyConfig::PadBinding, static_cast<int>(KeyConfig::ACTION::MAX)> KeyConfig::padBindings_ =
	KeyConfig::DEFAULT_PAD_BINDINGS;

const std::array<const char*, static_cast<int>(KeyConfig::ACTION::MAX)> KeyConfig::ACTION_TEXTS =
{
	"決定", "キャンセル", "上移動", "下移動", "左移動", "右移動"
};

void KeyConfig::RegisterDefaultKeys(InputManager& input)
{
	for (const auto& binding : bindings_)
	{
		for (const int key : binding.fixedKeys)
		{
			if (key != INVALID_KEY)
			{
				input.Add(key);
			}
		}
		input.Add(binding.configurableKey);
	}
}

bool KeyConfig::IsNew(ACTION action, const InputManager& input)
{
	const auto* binding = FindBinding(action);
	const auto* padBinding = FindPadBinding(action);
	if (binding == nullptr || padBinding == nullptr)
	{
		return false;
	}

	if (padBinding->fixedButton != InputManager::JOYPAD_BTN::MAX &&
		input.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, padBinding->fixedButton))
	{
		return true;
	}
	if (padBinding->configurableButton != InputManager::JOYPAD_BTN::MAX &&
		input.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, padBinding->configurableButton))
	{
		return true;
	}

	if (input.IsNew(binding->configurableKey))
	{
		return true;
	}

	for (const int key : binding->fixedKeys)
	{
		if (key != INVALID_KEY && input.IsNew(key))
		{
			return true;
		}
	}

	return false;
}

bool KeyConfig::IsTrgDown(ACTION action, const InputManager& input)
{
	const auto* binding = FindBinding(action);
	const auto* padBinding = FindPadBinding(action);
	if (binding == nullptr || padBinding == nullptr)
	{
		return false;
	}

	if (padBinding->fixedButton != InputManager::JOYPAD_BTN::MAX &&
		input.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, padBinding->fixedButton))
	{
		return true;
	}
	if (padBinding->configurableButton != InputManager::JOYPAD_BTN::MAX &&
		input.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, padBinding->configurableButton))
	{
		return true;
	}

	if (input.IsTrgDown(binding->configurableKey))
	{
		return true;
	}

	for (const int key : binding->fixedKeys)
	{
		if (key != INVALID_KEY && input.IsTrgDown(key))
		{
			return true;
		}
	}

	return false;
}

bool KeyConfig::IsTrgUp(ACTION action, const InputManager& input)
{
	const auto* binding = FindBinding(action);
	const auto* padBinding = FindPadBinding(action);
	if (binding == nullptr || padBinding == nullptr)
	{
		return false;
	}

	if (padBinding->fixedButton != InputManager::JOYPAD_BTN::MAX &&
		input.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, padBinding->fixedButton))
	{
		return true;
	}
	if (padBinding->configurableButton != InputManager::JOYPAD_BTN::MAX &&
		input.IsPadBtnTrgUp(InputManager::JOYPAD_NO::PAD1, padBinding->configurableButton))
	{
		return true;
	}

	if (input.IsTrgUp(binding->configurableKey))
	{
		return true;
	}

	for (const int key : binding->fixedKeys)
	{
		if (key != INVALID_KEY && input.IsTrgUp(key))
		{
			return true;
		}
	}

	return false;
}

const KeyConfig::KeyBinding* KeyConfig::FindBinding(ACTION action)
{
	const int actionIndex = static_cast<int>(action);
	if (actionIndex < ACTION_INDEX_MIN || ACTION_NUM <= actionIndex)
	{
		return nullptr;
	}

	return &bindings_[actionIndex];
}

const KeyConfig::PadBinding* KeyConfig::FindPadBinding(ACTION action)
{
	const int actionIndex = static_cast<int>(action);
	if (actionIndex < ACTION_INDEX_MIN || ACTION_NUM <= actionIndex)
	{
		return nullptr;
	}

	return &padBindings_[actionIndex];
}

bool KeyConfig::SetConfigurableKey(ACTION action, int key, InputManager& input)
{
	const int actionIndex = static_cast<int>(action);
	if (actionIndex < ACTION_INDEX_MIN || ACTION_NUM <= actionIndex || key == INVALID_KEY)
	{
		return false;
	}

	int& configurableKey = bindings_[actionIndex].configurableKey;
	if (configurableKey == key)
	{
		return true;
	}

	for (const auto& binding : bindings_)
	{
		for (const int fixedKey : binding.fixedKeys)
		{
			if (fixedKey == key)
			{
				return false;
			}
		}
	}

	for (auto& binding : bindings_)
	{
		if (binding.configurableKey == key)
		{
			binding.configurableKey = configurableKey;
			configurableKey = key;
			return true;
		}
	}

	configurableKey = key;
	input.Add(key);
	return true;
}

bool KeyConfig::SetConfigurablePadButton(ACTION action, InputManager::JOYPAD_BTN button)
{
	const int actionIndex = static_cast<int>(action);
	if (actionIndex < ACTION_INDEX_MIN || ACTION_NUM <= actionIndex ||
		button == InputManager::JOYPAD_BTN::MAX)
	{
		return false;
	}

	auto& configurableButton = padBindings_[actionIndex].configurableButton;
	if (configurableButton == InputManager::JOYPAD_BTN::MAX)
	{
		return false;
	}
	if (configurableButton == button)
	{
		return true;
	}

	for (const auto& binding : padBindings_)
	{
		if (binding.fixedButton == button)
		{
			return false;
		}
	}

	for (auto& binding : padBindings_)
	{
		if (binding.configurableButton == button)
		{
			binding.configurableButton = configurableButton;
			configurableButton = button;
			return true;
		}
	}

	configurableButton = button;
	return true;
}

const char* KeyConfig::GetActionText(ACTION action)
{
	const int actionIndex = static_cast<int>(action);
	if (actionIndex < ACTION_INDEX_MIN || ACTION_NUM <= actionIndex)
	{
		return "";
	}

	return ACTION_TEXTS[actionIndex];
}

const char* KeyConfig::GetKeyText(int key)
{
	switch (key)
	{
	case KEY_INPUT_RETURN: return "Enter";
	case KEY_INPUT_SPACE: return "Space";
	case KEY_INPUT_ESCAPE: return "Esc";
	case KEY_INPUT_BACK: return "Backspace";
	case KEY_INPUT_UP: return "Up";
	case KEY_INPUT_DOWN: return "Down";
	case KEY_INPUT_LEFT: return "Left";
	case KEY_INPUT_RIGHT: return "Right";
	case KEY_INPUT_LSHIFT: return "LShift";
	case KEY_INPUT_RSHIFT: return "RShift";
	case KEY_INPUT_A: return "A";
	case KEY_INPUT_B: return "B";
	case KEY_INPUT_C: return "C";
	case KEY_INPUT_D: return "D";
	case KEY_INPUT_E: return "E";
	case KEY_INPUT_F: return "F";
	case KEY_INPUT_G: return "G";
	case KEY_INPUT_H: return "H";
	case KEY_INPUT_I: return "I";
	case KEY_INPUT_J: return "J";
	case KEY_INPUT_K: return "K";
	case KEY_INPUT_L: return "L";
	case KEY_INPUT_M: return "M";
	case KEY_INPUT_N: return "N";
	case KEY_INPUT_O: return "O";
	case KEY_INPUT_P: return "P";
	case KEY_INPUT_Q: return "Q";
	case KEY_INPUT_R: return "R";
	case KEY_INPUT_S: return "S";
	case KEY_INPUT_T: return "T";
	case KEY_INPUT_U: return "U";
	case KEY_INPUT_V: return "V";
	case KEY_INPUT_W: return "W";
	case KEY_INPUT_X: return "X";
	case KEY_INPUT_Y: return "Y";
	case KEY_INPUT_Z: return "Z";
	default:
		break;
	}

	static char keyText[16];
	sprintf_s(keyText, "Key:%d", key);
	return keyText;
}

const char* KeyConfig::GetPadButtonText(InputManager::JOYPAD_BTN button)
{
	switch (button)
	{
	case InputManager::JOYPAD_BTN::LEFT: return "左ボタン";
	case InputManager::JOYPAD_BTN::RIGHT: return "右ボタン";
	case InputManager::JOYPAD_BTN::TOP: return "上ボタン";
	case InputManager::JOYPAD_BTN::DOWN: return "下ボタン";
	case InputManager::JOYPAD_BTN::R_TRIGGER: return "Rトリガー";
	case InputManager::JOYPAD_BTN::L_TRIGGER: return "Lトリガー";
	case InputManager::JOYPAD_BTN::DPAD_UP: return "十字キー上";
	case InputManager::JOYPAD_BTN::DPAD_DOWN: return "十字キー下";
	case InputManager::JOYPAD_BTN::DPAD_LEFT: return "十字キー左";
	case InputManager::JOYPAD_BTN::DPAD_RIGHT: return "十字キー右";
	default:
		return "未設定";
	}
}
