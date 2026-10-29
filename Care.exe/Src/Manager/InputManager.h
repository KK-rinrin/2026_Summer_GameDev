#pragma once
#include <memory>
#include <vector>
#include <map>
#include <Dxlib.h>
#include "../Common/Vector2.h"

class InputManager
{
public:

	// ゲームコントローラーの認識番号
	// DxLib定数、DX_INPUT_PAD1等に対応
	enum class JOYPAD_NO
	{
		KEY_PAD1,			// キー入力とパッド１入力
		PAD1,				// パッド１入力
		PAD2,				// パッド２入力
		PAD3,				// パッド３入力
		PAD4,				// パッド４入力
		INPUT_KEY = 4096	// キーボード単独入力の認識番号 / キー入力（未使用）
	};

	// ゲームコントローラータイプ
	// DxLib定数、DX_OTHER等に対応
	enum class JOYPAD_TYPE
	{
		OTHER = 0,			// その他のコントローラー種別
		XBOX_360,			// Xbox 360コントローラー
		XBOX_ONE,			// Xbox Oneコントローラー
		DUAL_SHOCK_4,		// DUALSHOCK 4コントローラー
		DUAL_SENSE,			// DualSenseコントローラー
		SWITCH_JOY_CON_L,	// 左側Joy-Conの種別
		SWITCH_JOY_CON_R,	// 右側Joy-Conの種別
		SWITCH_PRO_CTRL,	// Switch Proコントローラー
		MAX					// コントローラー種別の件数
	};

	// ゲームコントローラーボタン
	enum class JOYPAD_BTN
	{
		LEFT = 0,	// 左側のフェイスボタン
		RIGHT,		// 右側のフェイスボタン
		TOP,		// 上側のフェイスボタン
		DOWN,		// 下側のフェイスボタン
		R_TRIGGER,	// 右トリガー
		L_TRIGGER,	// 左トリガー
		START,		// スタートボタン
		DPAD_UP,	// 方向キーの上
		DPAD_DOWN,	// 方向キーの下
		DPAD_LEFT,	// 方向キーの左
		DPAD_RIGHT,	// 方向キーの右
		MAX			// 入力管理対象のボタン数
	};

	// ゲームコントローラーの入力情報
	struct JOYPAD_IN_STATE
	{

		// 前フレームの各ボタンの生入力値
		unsigned char ButtonsOld[static_cast<int>(JOYPAD_BTN::MAX)];

		// 現フレームの各ボタンの生入力値
		unsigned char ButtonsNew[static_cast<int>(JOYPAD_BTN::MAX)];

		// 前フレームの各ボタンの押下状態
		bool IsOld[static_cast<int>(JOYPAD_BTN::MAX)];

		// 現フレームの各ボタンの押下状態
		bool IsNew[static_cast<int>(JOYPAD_BTN::MAX)];

		// 各ボタンの押下開始状態
		bool IsTrgDown[static_cast<int>(JOYPAD_BTN::MAX)];

		// 各ボタンの解放開始状態
		bool IsTrgUp[static_cast<int>(JOYPAD_BTN::MAX)];

		// 左スティックの横入力値
		int AKeyLX;

		// 左スティックの縦入力値
		int AKeyLY;

		// 右スティックの横入力値
		int AKeyRX;

		// 右スティックの縦入力値
		int AKeyRY;
	};

	// インスタンスを明示的に生成
	static void CreateInstance();

	// インスタンスの取得
	static InputManager& GetInstance();

	// 初期化
	void Init();

	// 更新
	void Update();

	// リソースの破棄
	void Destroy();

	// 判定を行うキーを追加
	void Add(int key);

	// 判定を行うキーをクリア
	void Clear();

	// キーの押下判定
	bool IsNew(int key) const;

	// キーの押下判定(押しっぱなしはNG)
	bool IsTrgDown(int key) const;

	// キーを離した時の判定
	bool IsTrgUp(int key) const;

	// いずれかのキー押下判定
	bool IsNewKeyAll() const;

	// マウス座標の取得
	Vector2<int> GetMousePos() const;

	// マウスのボタン入力ビット列の取得（未使用）
	int GetMouse() const;

	// マウス左ボタンの押下状態の取得（未使用）
	bool IsClickMouseLeft() const;

	// マウス右ボタンの押下状態の取得（未使用）
	bool IsClickMouseRight() const;

	// マウスが左クリックされたか(押しっぱなしはNG)
	bool IsTrgMouseLeft() const;

	// マウス右ボタンの押下開始判定（未使用）
	bool IsTrgMouseRight() const;

	// コントローラの入力情報を取得する
	JOYPAD_IN_STATE GetJPadInputState(JOYPAD_NO no);

	// 保持済みのパッド入力状態への参照の取得
	const JOYPAD_IN_STATE& GetPadInputState(JOYPAD_NO no) const;

	// ボタンが押された
	bool IsPadBtnNew(JOYPAD_NO no, JOYPAD_BTN btn) const;

	// パッドボタンの押下開始判定
	bool IsPadBtnTrgDown(JOYPAD_NO no, JOYPAD_BTN btn) const;

	// パッドボタンの解放開始判定
	bool IsPadBtnTrgUp(JOYPAD_NO no, JOYPAD_BTN btn) const;

	// パッド1の接続判定
	bool IsPadConnected() const;

	// いずれかのパッドボタンの押下判定（未使用）
	bool IsPadBtnNewAll(JOYPAD_NO no) const;

	// スティック入力に対応する正規化済みXZ方向の取得（未使用）

private:

	// キーボード併用枠を含むパッド情報配列の要素数
	static constexpr int PAD_INFO_COUNT = static_cast<int>(JOYPAD_NO::PAD4) + 1;

	// XboxのAとBとXとYのDirectInputボタン番号
	enum XboxDInputButton : int { XBOX_A, XBOX_B, XBOX_X, XBOX_Y };

	// DirectInputのPlayStationボタン番号
	enum PlayStationDInputButton : int
	{
		PS_SQUARE, PS_CROSS, PS_CIRCLE, PS_TRIANGLE,	// 四角とバツと丸と三角のDirectInput番号
		PS_L2 = 6, PS_R2 = 7, PS_OPTIONS = 9			// 左右トリガーとOptionsのDirectInput番号
	};


	// キー情報
	struct Info
	{
		int key;			// キーID
		bool keyOld;		// 1フレーム前の押下状態
		bool keyNew;		// 現フレームの押下状態
		bool keyTrgDown;	// 現フレームでボタンが押されたか
		bool keyTrgUp;		// 現フレームでボタンが離されたか
	};

	// マウス
	struct MouseInfo
	{
		int key;			// キーID
		bool keyOld;		// 1フレーム前の押下状態
		bool keyNew;		// 現フレームの押下状態
		bool keyTrgDown;	// 現フレームでボタンが押されたか
		bool keyTrgUp;		// 現フレームでボタンが離されたか
	};

	// コントローラ情報
	DINPUT_JOYSTATE joyDInState_;

	// コントローラ情報(XBOX)
	XINPUT_STATE joyXInState_;

	// シングルトン用インスタンス
	static std::shared_ptr<InputManager> instance_;

	// キー情報
	std::map<int, InputManager::Info> keyInfos_;

	// 未登録キーに対する空の入力情報
	InputManager::Info infoEmpty_;

	// マウス情報
	std::map<int, InputManager::MouseInfo> mouseInfos_;

	// 未登録マウスボタンに対する空の入力情報
	InputManager::MouseInfo mouseInfoEmpty_;

	// マウスカーソルの位置
	Vector2<int> mousePos_;
	
	// マウスボタンの入力状態
	int mouseInput_;

	// パッド情報
	JOYPAD_IN_STATE padInfos_[PAD_INFO_COUNT];

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	InputManager();

	// コピーコンストラクタも同様
	InputManager(const InputManager& instance) = default;

	// デストラクタも同様
	~InputManager() = default;

	// 配列の中からキー情報を取得する
	const InputManager::Info& Find(int key) const;

	// 配列の中からマウス情報を取得する
	const InputManager::MouseInfo& FindMouse(int key) const;

	// 接続されたコントローラの種別を取得する
	JOYPAD_TYPE GetJPadType(JOYPAD_NO no) const;

	// コントローラの入力情報を取得する
	DINPUT_JOYSTATE GetJPadDInputState(JOYPAD_NO no);

	// コントローラ(XBOX)の入力情報を取得する
	XINPUT_STATE GetJPadXInputState(JOYPAD_NO no);

	// コントローラの入力情報を更新する
	void SetJPadInState(JOYPAD_NO jpNo);

};
