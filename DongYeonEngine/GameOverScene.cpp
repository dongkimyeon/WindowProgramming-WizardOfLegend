#include "GameOverScene.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "MapManager.h"
#include "Time.h"
#include "Input.h"
#include "Boss.h"
#include <cmath> // sin �Լ� ����� ���� �߰�

extern const UINT width;
extern const UINT height;

// ������
GameOverScene::GameOverScene()
    : mAlpha(255.0f)
    , mAlphaDir(-1)
    , mSineTime(0.0f) // ������ �ð� �ʱ�ȭ
{
}

// �Ҹ���
GameOverScene::~GameOverScene()
{
}

void GameOverScene::Initialize()
{
    mBackGroundImage.Load(L"resources/GameOverImage.png");
}

void GameOverScene::Update()
{
    if (Input::GetKeyDown(eKeyCode::SPACE))
    {
        SceneManager::LoadScene(L"TitleScene");
        SoundManager::GetInstance()->mPlaySound("TitleScreen", true);
        Boss::GetInstance()->revive();
        SceneManager::GetSharedPlayer()->SetHp(300);
        SceneManager::GetSharedPlayer()->Setrevive();
        SceneManager::GetActiveScene()->ObjectDestroy();
        SceneManager::ResetPlayTime();
        SceneManager::SetmIsGameStart(false);
    }


    mAlpha += mAlphaDir;
    if (mAlpha >= 255 || mAlpha <= 50)
        mAlphaDir *= -1;

   
    mSineTime += Time::DeltaTime(); 
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

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    HFONT hFont = CreateFont(50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    // �÷���Ÿ�� ��������
    float playTime = SceneManager::GetPlayTime();
    int minutes = static_cast<int>(playTime) / 60;
    int seconds = static_cast<int>(playTime) % 60;
    wchar_t playTimeText[32];
    swprintf_s(playTimeText, L"Play Time: %02d:%02d", minutes, seconds);

    std::wstring deadStageName = SceneManager::GetSharedPlayer()->GetDeadStage();
    int killcount = SceneManager::GetSharedPlayer()->GetKillCount();

    wchar_t killCountText[32];
    swprintf_s(killCountText, L"Enemies Killed: %d", killcount);

    wchar_t stageText[64];
    swprintf_s(stageText, L"Stage Reached: %s", deadStageName.c_str());

    // ������ ������ ���� �ؽ�Ʈ ũ�� ���
    int rightMargin = width - 50; // ������ ���� 50px
    int y = 250;
    SIZE textSize;

    // Play Time �ؽ�Ʈ
    GetTextExtentPoint32(hdc, playTimeText, wcslen(playTimeText), &textSize);
    int xPlayTime = rightMargin - textSize.cx;
    TextOut(hdc, xPlayTime, y, playTimeText, wcslen(playTimeText));

    // Kill Count �ؽ�Ʈ
    GetTextExtentPoint32(hdc, killCountText, wcslen(killCountText), &textSize);
    int xKillCount = rightMargin - textSize.cx;
    TextOut(hdc, xKillCount, y + 70, killCountText, wcslen(killCountText));

    // Stage Reached �ؽ�Ʈ
    GetTextExtentPoint32(hdc, stageText, wcslen(stageText), &textSize);
    int xStage = rightMargin - textSize.cx;
    TextOut(hdc, xStage, y + 140, stageText, wcslen(stageText));

    // GDI+�� "Press Space to Title"�� ������ ó��
    Gdiplus::Graphics graphics(hdc);
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
    Gdiplus::FontFamily fontFamily(L"EXO 2");
    Gdiplus::Font font(&fontFamily, 50, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    float sineAlpha = 50.0f + (205.0f * (sin(mSineTime * 2.0f) + 1.0f) / 2.0f);
    Gdiplus::SolidBrush alphaBrush(Gdiplus::Color(static_cast<BYTE>(sineAlpha), 255, 255, 255));
    wchar_t pressSpaceText[] = L"Press Space to Title";
    graphics.DrawString(pressSpaceText, -1, &font, Gdiplus::PointF(50, height - 100), &alphaBrush);

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}