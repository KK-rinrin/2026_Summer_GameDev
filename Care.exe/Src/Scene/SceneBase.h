#pragma once
class ResourceManager;
class SceneManager;
class InputManager;
class ProgressManager;
class SoundManager;

class SceneBase
{

public:

	// コンストラクタ
	SceneBase();

	// デストラクタ
	virtual ~SceneBase() = 0;

	// 初期化
	void Init();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// 解放
	virtual void Delete() = 0;

protected:
	virtual void InitLoad() = 0;
	virtual void InitPost() {}

	// リソース管理
	ResourceManager& resMng_;

	// シーン管理
	SceneManager& sceMng_;

	// 入力管理
	InputManager& iptMng_;

	ProgressManager& prgMng_;

	SoundManager& sndMng_;

};
