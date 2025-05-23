#include "GameOverScene.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "Time.h"

extern const UINT width;
extern const UINT height;

//클리어하지못하고죽었을때
GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
    mBackGroundImage.Load(L"resources/Maintitle.png");


    mLogoImage.Load(L"resources/TitileLogo.png");

}

void GameOverScene::Update()
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

void GameOverScene::LateUpdate()
{
    Scene::LateUpdate();
}

void GameOverScene::Render(HDC hdc)
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