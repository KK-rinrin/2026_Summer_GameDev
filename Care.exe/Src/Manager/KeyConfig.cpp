#include <DxLib.h>
#include "InputManager.h"
#include "KeyConfig.h"

const std::array<std::array<int, KeyConfig::KEY_NUM_PER_ACTION>, static_cast<int>(KeyConfig::ACTION::MAX)> KeyConfig::DEFAULT_KEYS =
{ {
	{ { KEY_INPUT_RETURN, KEY_INPUT_SPACE, KEY_INPUT_Z } },
	{ { KEY_INPUT_ESCAPE, KEY_INPUT_BACK, KEY_INPUT_X } },
	{ { KEY_INPUT_UP, KEY_INPUT_W, INVALID_KEY } },
	{ { KEY_INPUT_DOWN, KEY_INPUT_S, INVALID_KEY } },
	{ { KEY_INPUT_LEFT, KEY_INPUT_A, INVALID_KEY } },
	{ { KEY_INPUT_RIGHT, KEY_INPUT_D, INVALID_KEY } },
} };

void KeyConfig::RegisterDefaultKeys(InputManager& input)
{
	for (const auto& keys : DEFAULT_KEYS)
	{
		for (const int key : keys)
		{
			if (key != INVALID_KEY)
			{
				input.Add(key);
			}
		}
	}
}

bool KeyConfig::IsNew(ACTION action, const InputManager& input)
{
	const auto* keys = FindKeys(action);
	if (keys == nullptr)
	{
		return false;
	}

	for (const int key : *keys)
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
	const auto* keys = FindKeys(action);
	if (keys == nullptr)
	{
		return false;
	}

	for (const int key : *keys)
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
	const auto* keys = FindKeys(action);
	if (keys == nullptr)
	{
		return false;
	}

	for (const int key : *keys)
	{
		if (key != INVALID_KEY && input.IsTrgUp(key))
		{
			return true;
		}
	}

	return false;
}

const std::array<int, KeyConfig::KEY_NUM_PER_ACTION>* KeyConfig::FindKeys(ACTION action)
{
	const int actionIndex = static_cast<int>(action);
	if (actionIndex < ACTION_INDEX_MIN || ACTION_NUM <= actionIndex)
	{
		return nullptr;
	}

	return &DEFAULT_KEYS[actionIndex];
}
