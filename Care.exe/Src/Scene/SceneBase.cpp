#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/ProgressManager.h"
#include "../Manager/SoundManager.h"
#include "SceneBase.h"

SceneBase::SceneBase()
	:
	resMng_(ResourceManager::GetInstance()),
	sceMng_(SceneManager::GetInstance()),
	iptMng_(InputManager::GetInstance()),
	prgMng_(ProgressManager::GetInstance()),
	sndMng_(SoundManager::GetInstance())
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	InitLoad();

	// Init“à‚ÌÅŒã‚ÉŒÄ‚ÔŒãˆ—
	InitPost();
}

void SceneBase::Update()
{
}

void SceneBase::Draw()
{
}
