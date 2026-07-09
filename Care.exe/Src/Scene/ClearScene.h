#pragma once
#include "SceneBase.h"
#include "../Manager/ProgressManager.h"
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
	struct EndInfo
	{
		ProgressManager::STORY_PROGRESS progress;
		ResourceManager::SRC stillSrc;
		const char* title;
	};

	void InitLoad(void) override;
	const EndInfo& GetEndInfo(void) const;
	ResourceManager::SRC GetStillSrc(void) const;
	std::string GetEndTitle(void) const;

	int stillHandle_;
};
