#pragma once
#include <chrono>
#include <DxLib.h>
class SceneBase;
class Fader;

class SceneManager
{

public:

	// 背景色
	static constexpr int BACKGROUND_COLOR_R = 226;
	static constexpr int BACKGROUND_COLOR_G = 226;
	static constexpr int BACKGROUND_COLOR_B = 226;

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		SETTING,
		BP_MINIGAME,
		CLEAR,
		DEBUG,
	};
	
	// インスタンスの生成
	static void CreateInstance();

	// インスタンスの取得
	static SceneManager& GetInstance();

	// 初期化
	void Init();
	
	// 3Dの初期化
	void Init3D();

	// 更新
	void Update();

	// 描画
	void Draw();

	// リソースの破棄
	void Destroy();

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);

	// SettingSceneから戻る先を設定する
	void SetSettingReturnScene(SCENE_ID sceneId);

	// GameSceneからSettingSceneを開くときの復帰情報を保存する
	void SetSettingReturnGameState(int stage, const VECTOR& actorPos);

	// SettingSceneのBACKで戻る先を取得する
	SCENE_ID GetSettingReturnScene() const;

	// GameScene復帰情報の有無を取得する
	bool HasSettingReturnGameState() const;

	// GameScene復帰時のステージを取得する
	int GetSettingReturnGameStage() const;

	// GameScene復帰時の操作キャラ座標を取得する
	VECTOR GetSettingReturnActorPos() const;

	// GameScene復帰情報を破棄する
	void ClearSettingReturnGameState();

	// シーンIDの取得
	SCENE_ID GetSceneID();

	// デルタタイムの取得
	float GetDeltaTime() const;

private:

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// SettingSceneのBACKで戻る先。通常はタイトルに戻る
	SCENE_ID settingReturnSceneId_;

	// GameSceneからSettingSceneを開いた時だけ使う復帰情報
	bool hasSettingReturnGameState_;
	int settingReturnGameStage_;
	VECTOR settingReturnActorPos_;

	// フェード
	Fader* fader_;

	// 各種シーン
	SceneBase* scene_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager();

	// コピーコンストラクタも同様
	SceneManager(const SceneManager& instance) = default;

	// デストラクタも同様
	~SceneManager() = default;

	// デルタタイムをリセットする
	void ResetDeltaTime();

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade();

};
