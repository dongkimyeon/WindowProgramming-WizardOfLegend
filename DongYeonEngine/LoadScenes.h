#pragma once
#include "SceneManager.h"
#include "Stage1.h"
#include "TitleScene.h"


void LoadScenes()
{
	SceneManager::CreateScene<Stage1>(L"Stage1");
	SceneManager::CreateScene<TitleScene>(L"TitleScene");

	SceneManager::LoadScene(L"TitleScene");
}