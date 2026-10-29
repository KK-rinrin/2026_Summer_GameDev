#pragma once
#include <memory>
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{
public:

	// キャラクター画像シートの横コマ数
	static constexpr int CHARACTER_SHEET_COLUMNS = 3;

	// キャラクター画像シートの縦コマ数
	static constexpr int CHARACTER_SHEET_ROWS = 2;

	// キャラクター画像1コマの幅
	static constexpr int CHARACTER_CELL_WIDTH = 120;

	// キャラクター画像1コマの高さ
	static constexpr int CHARACTER_CELL_HEIGHT = 300;

	// 正面向きの静止コマのインデックス
	static constexpr int CHARACTER_FRONT_STILL_INDEX = 1;

	// 背面向きの静止コマのインデックス（未使用）
	static constexpr int CHARACTER_BACK_STILL_INDEX = 4;

	// リソース名
	enum class SRC
	{
		TITLE_IMG,				// タイトル画面の画像
		MAIN_FONT,				// 通常画面で使用するフォント
		PC_FONT,				// PC画面で使用するフォント
		PLAYER_GRAPHICS,		// アドベンチャー画面 / プレイヤーグラフィック
		PATIENT_GRAPHICS,		// ヌイグラフィック
		BG_1,					// 患者部屋背景
		BG_1_WALL,				// 患者部屋壁画像
		BG_1_TOILET,			// 患者部屋トイレ画像
		BG_2,					// ナースステーション背景
		BG_2_PC,				// パソコンデスク画像
		BG_2_CHAIR,				// パソコンデスクの椅子
		BG_2_LOCKER,			// ナースステーションのロッカー
		BG_2_TABLE,				// ナースステーションの配膳台
		BG_2_PLATE,				// 昼食の配膳プレート
		BG_2_PLATE_FOLDER,		// 夕食の配膳プレート
		PC_BG,					// パソコン画面背景
		PC_FRAME,				// パソコン画面フレーム
		PC_MANUAL_FOLDER,		// PC画面のマニュアルフォルダ画像
		PC_FILE,				// ミニゲームマニュアルフォルダ
		PC_BP_MANUAL,			// 血圧測定ミニゲームマニュアル
		PC_CURSOR,				// パソコンカーソル
		GUIDE_MOVE,				// 移動可能ドアガイド画像
		BGM_TITLE,				// タイトル画面のBGM
		BGM_GAME,				// ゲーム中BGM（アドベンチャーモード）
		BGM_GAME_2,				// ゲーム中のBGM 2
		BGM_KOWAI,				// 怖いBGM　※やさしい
		BGM_ENDING,				// エンディングBGM
		SE_DECIDE,				// システム決定音
		SE_CANCEL,				// システムキャンセル音
		SE_CURSOR_MOVE,			// システムカーソル移動音
		SE_BEEP,				// システムエラー音
		SE_DOOR,				// ドアの音
		SE_RINGTONE,			// 呼び出し音
		SE_TYPING,				// タイピング音
		SE_PC_MOUSE,			// パソコンのシステムマウス音
		PLAYER_MODEL,			// 会話画面 / プレイヤーLive2Dモデル
		PATIENT_MODEL,			// ヌイLive2Dモデル
		TALK_WINDOW_IMG,		// 会話ウィンドウ画像
		TALK_WINDOW_NEXT_IMG,	// 会話ウィンドウ▼画像
		TALK_SPEAKER_IMG,		// 会話話者画像
		TALK_IMAGE_0,			// 会話用一枚絵
		BPMG_BG_IMG,			// ミニゲーム画面 / 背景
		BPMG_MONITOR_IMG,		// モニター
		BPMG_PULSE_IMG,			// 脈
		BPMG_FAILED_IMG,		// 失敗画像
		BPMG_INTRUCT_IMG,		// 操作説明
		STILL_1,				// キャラクター一枚絵 / 不気味な笑顔をうかべるヌイ
		STILL_2,				// 看護師
		STILL_STAGE_PR,			// 無人の患者部屋
		STILL_STAGE_NS,			// 無人のナースステーション
		STILL_END_NUR,			// ゲームクリア一枚絵 / ヌイ削除
		STILL_END_NUI,			// 看護師削除
		STILL_END_BOTH,			// 両方削除
		STILL_END_RUIN			// 削除しないまま破滅
	};

	// 明示的にインスタンスを生成する
	static void CreateInstance();

	// 静的インスタンスの取得
	static ResourceManager& GetInstance();

	// 初期化
	void Init();

	// 解放(シーン切替時に一旦解放)
	void Release();

	// リソースの完全破棄
	void Destroy();

	// 登録リソースのファイルパスの取得（未使用）
	const char* GetResourcePath(SRC src) const;

	// リソースのロード
	const Resource& Load(SRC src);

	// フォントを指定設定でロード
	int LoadFont(SRC src, int fontSize, int thick = DEFAULT_FONT_THICK, int fontSpace = DEFAULT_FONT_SPACE, int fontType = DX_FONTTYPE_ANTIALIASING);

	// 三次元モデルを複製したハンドルの取得（未使用）

	//

private:

	// PC用フォントの既定サイズ
	static constexpr int DEFAULT_PC_FONT_SIZE = 22;

	// PC用フォントの既定の太さ
	static constexpr int DEFAULT_PC_FONT_THICK = 0;

	// PC用フォントの既定文字間隔
	static constexpr int DEFAULT_PC_FONT_SPACE = -1;

	// 通常フォントの基本サイズ
	static constexpr int MAIN_FONT_SIZE = 30;

	// フォントの既定の太さ
	static constexpr int DEFAULT_FONT_THICK = -1;

	// フォントの既定文字間隔
	static constexpr int DEFAULT_FONT_SPACE = 0;

	// 素材登録するヘルパー関数
	void CreateResource(SRC src, Resource::TYPE type, const std::string& path);

	// 静的インスタンス
	static std::shared_ptr<ResourceManager> instance_;

	// リソース管理の対象
	std::map<SRC, std::unique_ptr<Resource>> resourcesMap_;

	// 読み込み済みリソースの保管
	std::map<SRC, Resource*> loadedMap_;

	// 未登録リソースに対する代替リソース
	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager();
	ResourceManager(const ResourceManager& manager) = delete;
	~ResourceManager() = default;

	// 内部ロード
	Resource& _Load(SRC src);

};
