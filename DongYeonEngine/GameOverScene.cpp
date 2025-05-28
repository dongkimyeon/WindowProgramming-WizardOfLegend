#include "GameOverScene.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "Time.h"

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

    // �÷���Ÿ�� �ؽ�Ʈ ������
    SetBkMode(hdc, TRANSPARENT); // �ؽ�Ʈ ��� ���� ����
    SetTextColor(hdc, RGB(255, 255, 255)); // ��� �ؽ�Ʈ
    HFONT hFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    // �÷���Ÿ�� ��������
    float playTime = SceneManager::GetPlayTime(); // �� ������ ����
    wchar_t playTimeText[32];
    swprintf_s(playTimeText, L"Play Time: %.1f s", playTime);

    // ���� ��ܿ� �ؽ�Ʈ ��� (x=10, y=10)
    TextOut(hdc, 10, 10, playTimeText, wcslen(playTimeText));

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}