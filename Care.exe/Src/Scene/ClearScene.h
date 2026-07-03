#pragma once
#include "SceneBase.h"
#include "../Manager/ResourceManager.h"

class ClearScene : public SceneBase
{
public:
	ClearScene(void);
	~ClearScene(void) override;

	void Update(void) override;
	void Draw(void) override;
	void Delete(void) override;

private:
	void InitLoad(void) override;
	ResourceManager::SRC GetStillSrc(void) const;

	int stillHandle_;
};
