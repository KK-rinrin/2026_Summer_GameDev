#pragma once
#include "../Common/Vector2.h"
#include <memory>
#include <chrono>
#include <DxLib.h>
class SceneBase;
class Fader;

class SceneManager
{
public:
	// 背景色
	static constexpr int BACKGROUND_COLOR_R = 226;

	// 背景色の緑成分
	static constexpr int BACKGROUND_COLOR_G = 226;

	// 背景色の青成分
	static constexpr int BACKGROUND_COLOR_B = 226;

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,			// シーン未指定の状態
		TITLE,			// タイトル画面
		GAME,			// メインゲーム画面
		SETTING,		// 設定画面
		BP_MINIGAME,	// 血圧測定ミニゲーム画面
		CLEAR,			// エンディング画面
		DEBUG,			// デバッグ画面
	};
	
	// インスタンスの生成
	static void CreateInstance();

	// インスタンスの取得
	static SceneManager& GetInstance();

	// 初期化
	void Init();

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
	void SetSettingReturnGameState(int stage, const Vector2<float>& actorPos);

	// SettingSceneのBACKで戻る先を取得する
	SCENE_ID GetSettingReturnScene() const;

	// GameScene復帰情報の有無を取得する
	bool HasSettingReturnGameState() const;

	// GameScene復帰時のステージを取得する
	int GetSettingReturnGameStage() const;

	// GameScene復帰時の操作キャラ座標を取得する
	Vector2<float> GetSettingReturnActorPos() const;

	// GameScene復帰情報を破棄する
	void ClearSettingReturnGameState();

	// 現在のシーンIDの取得（未使用）
	SCENE_ID GetSceneID();

	// デルタタイムの取得
	float GetDeltaTime() const;

private:

	// 1秒当たりのナノ秒数
	static constexpr double NANOSECONDS_PER_SECOND = 1000000000.0;

	// 時間差が不正な場合の代替更新秒数
	static constexpr float FALLBACK_DELTA_SECONDS = 1.0f / 60.0f;

	// デルタタイム初期化時の秒数
	static constexpr float RESET_DELTA_SECONDS = 0.016f;


	// 静的インスタンス
	static std::shared_ptr<SceneManager> instance_;

	// 現在のシーンID
	SCENE_ID sceneId_;

	// フェード完了待ちの遷移先シーンID
	SCENE_ID waitSceneId_;

	// SettingSceneのBACKで戻る先。通常はタイトルに戻る
	SCENE_ID settingReturnSceneId_;

	// GameSceneからSettingSceneを開いた時だけ使う復帰情報
	bool hasSettingReturnGameState_;

	// 設定画面からの復帰先ステージ
	int settingReturnGameStage_;

	// 設定画面からの復帰時の操作キャラ座標
	Vector2<float> settingReturnActorPos_;

	// フェード
	std::unique_ptr<Fader> fader_;

	// 各種シーン
	std::unique_ptr<SceneBase> scene_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;

	// 前フレームからの経過秒数
	float deltaTime_;
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager();

	// コピーコンストラクタも同様
	SceneManager(const SceneManager& instance) = default;

	// デストラクタも同様
	~SceneManager();

	// デルタタイムをリセットする
	void ResetDeltaTime();

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade();

};
