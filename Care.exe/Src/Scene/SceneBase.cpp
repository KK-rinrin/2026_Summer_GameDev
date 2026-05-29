#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/ProgressManager.h"
#include "SceneBase.h"

SceneBase::SceneBase(void) 
	: 
	resMng_(ResourceManager::GetInstance()),
	sceMng_(SceneManager::GetInstance()),
	iptMng_(InputManager::GetInstance()),
	prgMng_(ProgressManager::GetInstance())
{
}

SceneBase::~SceneBase(void)
{
}

void SceneBase::Init(void)
{
	InitLoad();
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}
