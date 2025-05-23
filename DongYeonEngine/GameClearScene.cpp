#include "GameClearScene.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "Time.h"

extern const UINT width;
extern const UINT height;

//보스 깼을 때   
GameClearScene::GameClearScene()
{
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Initialize()
{
    mBackGroundImage.Load(L"resources/Maintitle.png");


    mLogoImage.Load(L"resources/TitileLogo.png");

}

void GameClearScene::Update()
{
    Scene::Update();

    //스테이지 넘기기
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        SceneManager::LoadScene(L"Stage1");
        MapManager::GetInstance()->LoadMap(L"Stage1.txt");
        //플레이타임 활성화
        SceneManager::SetmIsGameStart(true);
    }

}

void GameClearScene::LateUpdate()
{
    Scene::LateUpdate();
}

void GameClearScene::Render(HDC hdc)
{

    if (!mBackGroundImage.IsNull())
    {
        mBackGroundImage.Draw(hdc, 0, 0, width, height);
    }

    if (!mLogoImage.IsNull())
    {
        mLogoImage.Draw(hdc, 0, 0, width, height);
    }

}