#pragma once
#include <array>
#include "InputManager.h"

class KeyConfig
{
public:

	// ゲーム操作の割り当て対象
	enum class ACTION
	{
		DECIDE,		// 決定操作
		CANCEL,		// キャンセル操作
		MOVE_UP,	// 上方向への移動操作
		MOVE_DOWN,	// 下方向への移動操作
		MOVE_LEFT,	// 左方向への移動操作
		MOVE_RIGHT,	// 右方向への移動操作
		MAX			// 割り当て対象の操作数
	};

	// キー未割り当てを表す値
	static constexpr int INVALID_KEY = -1;

	// 一つの操作に割り当てる固定キーの最大数
	static constexpr int FIXED_KEY_MAX = 3;

	// 固定キーと変更可能キーの組
	struct KeyBinding
	{

		// 変更対象外の固定キー一覧
		std::array<int, FIXED_KEY_MAX> fixedKeys;

		// ユーザーが変更できるキー
		int configurableKey;
	};

	// 固定ボタンと変更可能ボタンの組
	struct PadBinding
	{

		// 変更対象外の固定パッドボタン
		InputManager::JOYPAD_BTN fixedButton;

		// ユーザーが変更できるパッドボタン
		InputManager::JOYPAD_BTN configurableButton;
	};

	// 全操作の既定キーの入力管理への登録
	static void RegisterDefaultKeys(InputManager& input);

	// アクションの入力判定
	static bool IsNew(ACTION action, const InputManager& input);

	// 操作に割り当てた入力の押下開始判定
	static bool IsTrgDown(ACTION action, const InputManager& input);

	// 操作に割り当てた入力の解放開始判定
	static bool IsTrgUp(ACTION action, const InputManager& input);

	// いずれかの操作入力の押下判定
	static bool IsNewAny(const InputManager& input);

	// アクションとのつながりを取得
	static const KeyBinding* FindBinding(ACTION action);

	// 操作に対応するパッド割り当ての取得
	static const PadBinding* FindPadBinding(ACTION action);

	// 変更可能キーの設定と入力管理への登録
	static bool SetConfigurableKey(ACTION action, int key, InputManager& input);

	// 変更可能パッドボタンの設定
	static bool SetConfigurablePadButton(ACTION action, InputManager::JOYPAD_BTN button);

	// 操作名の表示文字列の取得
	static const char* GetActionText(ACTION action);

	// キー名の表示文字列の取得
	static const char* GetKeyText(int key);

	// パッドボタン名の表示文字列の取得
	static const char* GetPadButtonText(InputManager::JOYPAD_BTN button);

private:

	// キー名表示用バッファの文字数
	static constexpr int KEY_TEXT_BUFFER_SIZE = 16;

	// 有効な操作インデックスの最小値
	static constexpr int ACTION_INDEX_MIN = 0;

	// 操作別配列の要素数
	static constexpr int ACTION_NUM = static_cast<int>(ACTION::MAX);

	// 操作ごとの既定キー割り当て
	static const std::array<KeyBinding, ACTION_NUM> DEFAULT_BINDINGS;

	// 操作ごとの既定パッド割り当て
	static const std::array<PadBinding, ACTION_NUM> DEFAULT_PAD_BINDINGS;

	// 操作ごとの表示名
	static const std::array<const char*, ACTION_NUM> ACTION_TEXTS;

	// 現在のキー割り当て
	static std::array<KeyBinding, ACTION_NUM> bindings_;

	// 現在のパッド割り当て
	static std::array<PadBinding, ACTION_NUM> padBindings_;

};
