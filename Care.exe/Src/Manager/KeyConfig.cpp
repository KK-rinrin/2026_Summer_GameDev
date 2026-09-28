#include <DxLib.h>
#include "InputManager.h"
#include "KeyConfig.h"

const std::array<KeyConfig::KeyBinding, static_cast<int>(KeyConfig::ACTION::MAX)> KeyConfig::DEFAULT_BINDINGS =
{ {
	{ { KEY_INPUT_SPACE, KEY_INPUT_RETURN, INVALID_KEY }, KEY_INPUT_Z },
	{ { KEY_INPUT_BACK, KEY_INPUT_ESCAPE, KEY_INPUT_DELETE }, KEY_INPUT_X },
	{ { KEY_INPUT_UP, INVALID_KEY, INVALID_KEY }, KEY_INPUT_W },
	{ { KEY_INPUT_DOWN, INVALID_KEY, INVALID_KEY }, KEY_INPUT_S },
	{ { KEY_INPUT_LEFT, INVALID_KEY, INVALID_KEY }, KEY_INPUT_A },
	{ { KEY_INPUT_RIGHT, INVALID_KEY, INVALID_KEY }, KEY_INPUT_D },
} };

std::array<KeyConfig::KeyBinding, static_cast<int>(KeyConfig::ACTION::MAX)> KeyConfig::bindings_ =
	KeyConfig::DEFAULT_BINDINGS;

const std::array<KeyConfig::PadBinding, static_cast<int>(KeyConfig::ACTION::MAX)> KeyConfig::DEFAULT_PAD_BINDINGS =
{ {
	{ InputManager::JOYPAD_BTN::MAX, InputManager::JOYPAD_BTN::DOWN },
	{ InputManager::JOYPAD_BTN::START, InputManager::JOYPAD_BTN::RIGHT },
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
	case KEY_INPUT_RETURN:
`treturn "Enter";
	case KEY_INPUT_SPACE:
`treturn "Space";
	case KEY_INPUT_ESCAPE:
`treturn "Esc";
	case KEY_INPUT_BACK:
`treturn "Backspace";
	case KEY_INPUT_UP:
`treturn "Up";
	case KEY_INPUT_DOWN:
`treturn "Down";
	case KEY_INPUT_LEFT:
`treturn "Left";
	case KEY_INPUT_RIGHT:
`treturn "Right";
	case KEY_INPUT_LSHIFT:
`treturn "LShift";
	case KEY_INPUT_RSHIFT:
`treturn "RShift";
	case KEY_INPUT_A:
`treturn "A";
	case KEY_INPUT_B:
`treturn "B";
	case KEY_INPUT_C:
`treturn "C";
	case KEY_INPUT_D:
`treturn "D";
	case KEY_INPUT_E:
`treturn "E";
	case KEY_INPUT_F:
`treturn "F";
	case KEY_INPUT_G:
`treturn "G";
	case KEY_INPUT_H:
`treturn "H";
	case KEY_INPUT_I:
`treturn "I";
	case KEY_INPUT_J:
`treturn "J";
	case KEY_INPUT_K:
`treturn "K";
	case KEY_INPUT_L:
`treturn "L";
	case KEY_INPUT_M:
`treturn "M";
	case KEY_INPUT_N:
`treturn "N";
	case KEY_INPUT_O:
`treturn "O";
	case KEY_INPUT_P:
`treturn "P";
	case KEY_INPUT_Q:
`treturn "Q";
	case KEY_INPUT_R:
`treturn "R";
	case KEY_INPUT_S:
`treturn "S";
	case KEY_INPUT_T:
`treturn "T";
	case KEY_INPUT_U:
`treturn "U";
	case KEY_INPUT_V:
`treturn "V";
	case KEY_INPUT_W:
`treturn "W";
	case KEY_INPUT_X:
`treturn "X";
	case KEY_INPUT_Y:
`treturn "Y";
	case KEY_INPUT_Z:
`treturn "Z";
	default:
`tbreak;
	}

	static char keyText[16];
	sprintf_s(keyText, "Key:%d", key);
	return keyText;
}

const char* KeyConfig::GetPadButtonText(InputManager::JOYPAD_BTN button)
{
	switch (button)
	{
	case InputManager::JOYPAD_BTN::LEFT:
`treturn "左ボタン";
	case InputManager::JOYPAD_BTN::RIGHT:
`treturn "右ボタン";
	case InputManager::JOYPAD_BTN::TOP:
`treturn "上ボタン";
	case InputManager::JOYPAD_BTN::DOWN:
`treturn "下ボタン";
	case InputManager::JOYPAD_BTN::R_TRIGGER:
`treturn "Rトリガー";
	case InputManager::JOYPAD_BTN::L_TRIGGER:
`treturn "Lトリガー";
	case InputManager::JOYPAD_BTN::START:
`treturn "STARTボタン";
	case InputManager::JOYPAD_BTN::DPAD_UP:
`treturn "十字キー上";
	case InputManager::JOYPAD_BTN::DPAD_DOWN:
`treturn "十字キー下";
	case InputManager::JOYPAD_BTN::DPAD_LEFT:
`treturn "十字キー左";
	case InputManager::JOYPAD_BTN::DPAD_RIGHT:
`treturn "十字キー右";
	default:
`treturn "未設定";
	}
}
