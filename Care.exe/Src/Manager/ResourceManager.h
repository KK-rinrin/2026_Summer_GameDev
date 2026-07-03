#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		TITLE_IMG,
		TITLE_FONT,

		SETTING_FONT,			// 設定画面用フォント
		
		// アドベンチャー画面
		PLAYER_GRAPHICS,		// プレイヤーグラフィック
		PATIENT_GRAPHICS,		// ヌイグラフィック
		BG_1,					// 患者部屋背景
		BG_1_WALL,				// 患者部屋壁画像
		BG_1_TOILET,			// 患者部屋トイレ画像

		BG_2,					// ナースステーション背景
		BG_2_PC,				// パソコンデスク画像
		BG_2_CHAIR,				// パソコンデスクの椅子
		BG_2_LOCKER,			// ナースステーションのロッカー

		PC_BG,					// パソコン画面背景
		PC_FRAME,				// パソコン画面フレーム

		BGM_GAME,				// ゲーム中BGM（アドベンチャーモード）

		SE_DECIDE,				// システム決定音
		SE_CANCEL,				// システムキャンセル音
		SE_CURSOR_MOVE,			// システムカーソル移動音
		SE_BEEP,				// システムエラー音

		SE_DOOR,				// ドアの音

		// 会話画面
		PLAYER_MODEL,			// プレイヤーLive2Dモデル
		PATIENT_MODEL,			// ヌイLive2Dモデル
		TALK_FONT,				// 会話用フォント
		TALK_WINDOW_IMG,		// 会話ウィンドウ画像
		TALK_WINDOW_NEXT_IMG,	// 会話ウィンドウ▼画像
		TALK_IMAGE_0,			// 会話用一枚絵

		// ミニゲーム画面
		BPMG_BG_IMG,		// 背景
		BPMG_MONITOR_IMG,	// モニター
		BPMG_PULSE_IMG,		// 脈
		BPMG_FAILED_IMG,	// 失敗画像
		BPMG_INTRUCT_IMG,	// 操作説明
		BPMG_FONT,			// ﾐﾆｹﾞｰﾑフォント
		BPMG_LARGE_FONT,	// ﾐﾆｹﾞｰﾑデカフォント

		// キャラクター一枚絵
		STILL_1,			


		// ゲームクリア一枚絵
		STILL_END_NUR,		// ヌイ削除
		STILL_END_NUI,		// 看護師削除
		STILL_END_BOTH,		// 両方削除
		STILL_END_RUIN		// 削除しないまま破滅
	};

	// 明示的にインスタンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのパスを取得
	char* GetResourcePath(SRC src) const;

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

	//

private:
	// 素材登録するヘルパー関数
	void CreateResource(SRC src, Resource::TYPE type, const std::string& path);

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource*> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// 内部ロード
	Resource& _Load(SRC src);

};
