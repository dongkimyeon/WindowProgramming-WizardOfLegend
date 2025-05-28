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

//Ŭ�����������ϰ��׾�����
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

    // �÷���Ÿ��, ų ��, ���� �������� �ؽ�Ʈ ������
    SetBkMode(hdc, TRANSPARENT); // �ؽ�Ʈ ��� ���� ����
    SetTextColor(hdc, RGB(255, 255, 255)); // ��� �ؽ�Ʈ
    HFONT hFont = CreateFont(60, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    // �÷���Ÿ�� ��������
    float playTime = SceneManager::GetPlayTime(); // �� ������ ����
    int minutes = static_cast<int>(playTime) / 60; // �� ���
    int seconds = static_cast<int>(playTime) % 60; // �� ���
    wchar_t playTimeText[32];
    swprintf_s(playTimeText, L"Play Time: %02d:%02d", minutes, seconds);

    // ��ġ�� ��
    std::wstring deadStageName = SceneManager::GetSharedPlayer()->GetDeadStage();
    int killcount = SceneManager::GetSharedPlayer()->GetKillCount();

    // ų �� �ؽ�Ʈ
    wchar_t killCountText[32];
    swprintf_s(killCountText, L"Enemies Killed: %d", killcount);

    // ���� �������� �ؽ�Ʈ
    wchar_t stageText[64];
    swprintf_s(stageText, L"Stage Reached: %s", deadStageName.c_str());

    // �ؽ�Ʈ ��� (�������� ���̵��� y ��ǥ ����)
    TextOut(hdc, 600, 10, playTimeText, wcslen(playTimeText));
    TextOut(hdc, 600, 80, killCountText, wcslen(killCountText)); // �÷���Ÿ�� �Ʒ� 70�ȼ�
    TextOut(hdc, 600, 150, stageText, wcslen(stageText));       // ų �� �Ʒ� 70�ȼ�

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}