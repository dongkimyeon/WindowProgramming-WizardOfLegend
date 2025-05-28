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

    // 플레이타임 텍스트 렌더링
    SetBkMode(hdc, TRANSPARENT); // 텍스트 배경 투명 설정
    SetTextColor(hdc, RGB(255, 255, 255)); // 흰색 텍스트
    HFONT hFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    // 플레이타임 가져오기
    float playTime = SceneManager::GetPlayTime(); // 초 단위로 가정
    wchar_t playTimeText[32];
    swprintf_s(playTimeText, L"Play Time: %.1f s", playTime);

    // 좌측 상단에 텍스트 출력 (x=10, y=10)
    TextOut(hdc, 10, 10, playTimeText, wcslen(playTimeText));

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}