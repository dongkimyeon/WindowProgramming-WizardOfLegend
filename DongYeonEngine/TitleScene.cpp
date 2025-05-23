#include "TitleScene.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "Time.h"

extern const UINT width;
extern const UINT height;

//버튼 류 
//맵 툴로 넘어가기
//튜토리얼 스테이지
//설정
//게임 시작
//게임 종료
TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    mBackGroundImage.Load(L"resources/Maintitle.png"); 

    
    mLogoImage.Load(L"resources/TitileLogo.png"); 
   
}

void TitleScene::Update()
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

void TitleScene::LateUpdate()
{
    Scene::LateUpdate();
}

void TitleScene::Render(HDC hdc)
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