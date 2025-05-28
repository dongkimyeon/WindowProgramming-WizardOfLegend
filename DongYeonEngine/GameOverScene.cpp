#include "GameOverScene.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "Time.h"
#include "Input.h"
#include "Stage1.h"
#include "Stage2.h" 
#include "BossStage.h"
#include "TutorialStage.h"
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
    mBackGroundImage.Load(L"resources/GameOverImage.png");


    //mLogoImage.Load(L"resources/TitileLogo.png");

}

void GameOverScene::Update()
{
    if (Input::GetKeyDown(eKeyCode::SPACE))
    {
        SceneManager::LoadScene(L"TitleScene");
        SceneManager::GetSharedPlayer()->SetHp(100);
        SceneManager::GetSharedPlayer()->Setrevive();
        SceneManager::ResetPlayTime();
        SceneManager::SetmIsGameStart(false);
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

    // 플레이타임, 킬 수, 도달 스테이지 텍스트 렌더링
    SetBkMode(hdc, TRANSPARENT); // 텍스트 배경 투명 설정
    SetTextColor(hdc, RGB(255, 255, 255)); // 흰색 텍스트
    HFONT hFont = CreateFont(60, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    // 플레이타임 가져오기
    float playTime = SceneManager::GetPlayTime(); // 초 단위로 가정
    int minutes = static_cast<int>(playTime) / 60; // 분 계산
    int seconds = static_cast<int>(playTime) % 60; // 초 계산
    wchar_t playTimeText[32];
    swprintf_s(playTimeText, L"Play Time: %02d:%02d", minutes, seconds);

    // 해치운 적
    std::wstring deadStageName = SceneManager::GetSharedPlayer()->GetDeadStage();
    int killcount = SceneManager::GetSharedPlayer()->GetKillCount();

    // 킬 수 텍스트
    wchar_t killCountText[32];
    swprintf_s(killCountText, L"Enemies Killed: %d", killcount);

    // 도달 스테이지 텍스트
    wchar_t stageText[64];
    swprintf_s(stageText, L"Stage Reached: %s", deadStageName.c_str());

    // 텍스트 출력 (수직으로 쌓이도록 y 좌표 조정)
    TextOut(hdc, 600, 10, playTimeText, wcslen(playTimeText));
    TextOut(hdc, 600, 80, killCountText, wcslen(killCountText)); // 플레이타임 아래 70픽셀
    TextOut(hdc, 600, 150, stageText, wcslen(stageText));       // 킬 수 아래 70픽셀

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}