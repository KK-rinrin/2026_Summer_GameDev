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
		BG_1_WALL,				// 患者部屋壁背景
		BG_1_TOILET,			// 患者部屋トイレ背景
		BG_2,					// ナースステーション背景

		// 会話画面
		PLAYER_MODEL,			// プレイヤーLive2Dモデル
		PATIENT_MODEL,			// ヌイLive2Dモデル
		TALK_FONT,				// 会話用フォント
		TALK_WINDOW_IMG,		// 会話ウィンドウ画像
		TALK_WINDOW_NEXT_IMG,	// 会話ウィンドウ▼画像

	};

	// 明示的にインステンスを生成する
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
