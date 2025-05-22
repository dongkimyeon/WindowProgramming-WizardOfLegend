#pragma once
#include "SceneManager.h"
#include "Stage1.h"
#include "Stage2.h"
#include "BossStage.h"	
#include "TutorialStage.h"
#include "GameOverScene.h"
#include "GameClearScene.h"
#include "TitleScene.h"


void LoadScenes()
{
	SceneManager::CreateScene<TitleScene>(L"TitleScene");
	SceneManager::CreateScene<Stage1>(L"Stage1");
	SceneManager::CreateScene<Stage2>(L"Stage2");
	SceneManager::CreateScene<BossStage>(L"BossStage");
	SceneManager::CreateScene<TutorialStage>(L"TutorialStage");
	SceneManager::CreateScene<GameOverScene>(L"GameOverScene");
	SceneManager::CreateScene<GameClearScene>(L"GameClearScene");
	
	SceneManager::LoadScene(L"TutorialStage");
}