#pragma once
#include <memory>
#include <string>

class FpsControl;

class Application
{
public:
	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 800;	// X座標
	static constexpr int SCREEN_SIZE_Y = 600;	// Y座標

	// デフォルト色
	static constexpr int DEFAULT_COLOR_0 = 0x000000;	// 黒
	static constexpr int DEFAULT_COLOR_1 = 0x222222;	// ほぼ黒
	static constexpr int DEFAULT_COLOR_2 = 0x333333;	// とても暗いグレー
	static constexpr int DEFAULT_COLOR_3 = 0x555555;	// 暗い灰色
	static constexpr int DEFAULT_COLOR_4 = 0x666666;	// 少し暗い灰色
	static constexpr int DEFAULT_COLOR_5 = 0x7a7a7a;	// ちょうど灰色
	static constexpr int DEFAULT_COLOR_6 = 0xaaaaaa;	// 明るいグレー
	static constexpr int DEFAULT_COLOR_7 = 0xe6e6e6;	// ほぼ白
	static constexpr int DEFAULT_COLOR_8 = 0xffffff;	// 白

	// UI共通文字
	static constexpr const char* SELECT_MARK = "> ";		// 選択している時
	static constexpr const char* MENU_MARK_SPACE = "  ";	// 選択してない時

	// データパス関連
	//-------------------------------------------
	static const std::string PATH_DATA;		// データフォルダパス
	static const std::string PATH_CSV;		// CSVデータフォルダ
	static const std::string PATH_IMAGE;	// 画像フォルダ
	static const std::string PATH_TITLE;	// タイトル画面関連画像データ
	static const std::string PATH_SOUND;	// サウンドデータフォルダ
	static const std::string PATH_MODEL;	// Live2Dモデルデータフォルダ
	static const std::string PATH_FONT;		// フォントデータフォルダ
	static const std::string PATH_CHARA;	// Charaフォルダ（ストーリーに関わる）

	////-------------------------------------------

	// インスタンスを明示的に生成
	static void CreateInstance();

	// インスタンスの取得
	static Application& GetInstance();

	// 初期化
	void Init();

	// ゲームループの開始
	void Run();

	// リソースの破棄
	bool Destroy();

	// 初期化成功／失敗の判定
	bool IsInitFail() const;

	// 解放失敗状態の取得（未使用）
	bool IsReleaseFail() const;

private:

	// 画面の色深度のビット数
	static constexpr int SCREEN_COLOR_DEPTH = 32;


	// 静的インスタンス
	static std::shared_ptr<Application> instance_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application();

	// コピーコンストラクタも同様
	Application(const Application& instance) = default;

	// デストラクタも同様
	~Application();

	// Live2D描画環境の初期化
	void InitLive2D();

	// フレームレート制御の所有ポインタ
	std::unique_ptr<FpsControl> fpsControl_;
};
