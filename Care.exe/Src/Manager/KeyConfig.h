#pragma once
#include <array>

class InputManager;

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
	static constexpr int KEY_NUM_PER_ACTION = 3;

	static void RegisterDefaultKeys(InputManager& input);

	static bool IsNew(ACTION action, const InputManager& input);
	static bool IsTrgDown(ACTION action, const InputManager& input);
	static bool IsTrgUp(ACTION action, const InputManager& input);

	static const std::array<int, KEY_NUM_PER_ACTION>* FindKeys(ACTION action);

private:
	static constexpr int ACTION_INDEX_MIN = 0;
	static constexpr int ACTION_NUM = static_cast<int>(ACTION::MAX);

	static const std::array<std::array<int, KEY_NUM_PER_ACTION>, ACTION_NUM> DEFAULT_KEYS;

};
