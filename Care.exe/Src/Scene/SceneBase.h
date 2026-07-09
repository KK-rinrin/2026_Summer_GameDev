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
	SceneBase(void);

	// デストラクタ
	virtual ~SceneBase(void) = 0;

	// 初期化
	void Init(void);

	// 更新
	virtual void Update(void) = 0;

	// 描画
	virtual void Draw(void) = 0;

	// 解放
	virtual void Delete(void) = 0;

protected:
	virtual void InitLoad() = 0;
	virtual void InitPost(void) {}

	// リソース管理
	ResourceManager& resMng_;

	// シーン管理
	SceneManager& sceMng_;

	// 入力管理
	InputManager& iptMng_;

	ProgressManager& prgMng_;

	SoundManager& sndMng_;

};
