#pragma once
#include "SceneManager.h"
#include "Stage1.h"
#include "TitleScene.h"


void LoadScenes()
{
	SceneManager::CreateScene<TitleScene>(L"TitleScene");
	SceneManager::CreateScene<Stage1>(L"Stage1");
	//SceneManager::CreateScene<Stage2>(L"Stage2");
	SceneManager::LoadScene(L"TitleScene");
}