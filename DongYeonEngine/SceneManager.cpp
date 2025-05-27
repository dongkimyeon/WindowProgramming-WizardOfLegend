#include "SceneManager.h"
#include "Input.h"
#include "Time.h"

// ���� ��� ���� �ʱ�ȭ
std::map<std::wstring, Scene*> SceneManager::mScene = {};
Scene* SceneManager::mActiveScene = nullptr;
CImage SceneManager::mMouseCursorImage;
Player* SceneManager::mSharedPlayer = nullptr;
float SceneManager::playTime = 0.0f;
bool SceneManager::mIsGameStart = false;

// ���̵� ���� �ʱ�ȭ
SceneManager::eFadeState SceneManager::mFadeState = eFadeState::None;
float SceneManager::mFadeAlpha = 0.0f;
float SceneManager::mFadeTimer = 0.0f;
const float SceneManager::mFadeDuration = 2.0f; // ���̵� 2�� ����
std::wstring SceneManager::mTargetScene = L"";

void SceneManager::Initialize()
{
    mMouseCursorImage.Load(L"resources/MouseCursor.png");
    mSharedPlayer = new Player();
    playTime = 0.0f;
    MapManager::GetInstance()->Initialize();
}

void SceneManager::StartFadeOut(const std::wstring& name)
{
    mFadeState = eFadeState::FadeOut;
    mFadeTimer = 0.0f;
    mFadeAlpha = 0.0f; // ���̵�ƿ�: 0.0 -> 1.0 (ȭ���� ��ο���)
}

void SceneManager::StartFadeIn()
{
    mFadeState = eFadeState::FadeIn;
    mFadeTimer = 0.0f;
    mFadeAlpha = 1.0f; // ���̵���: 1.0 -> 0.0 (ȭ���� �����)
}

Scene* SceneManager::LoadScene(const std::wstring& name)
{
    auto iter = mScene.find(name);
    if (iter == mScene.end())
        return nullptr;

    mActiveScene = iter->second;
    return iter->second;
}

void SceneManager::Update()
{
    // ���̵� ���� ó��
    if (mFadeState != eFadeState::None)
    {
        mFadeTimer += Time::DeltaTime();
        float t = mFadeTimer / mFadeDuration; // 0.0 ~ 1.0
        t = (t > 1.0f) ? 1.0f : t; // Ŭ����

        if (mFadeState == eFadeState::FadeOut)
        {
            mFadeAlpha = 0.0f + t; // 1.0 -> 0.0
            if (mFadeTimer >= mFadeDuration)
            {
                mFadeAlpha = 1.0f;
                mFadeState = eFadeState::None;
            }
        }
        else if (mFadeState == eFadeState::FadeIn)
        {
            mFadeAlpha = 1.0f - t; // 1.0 -> 0.0
            if (mFadeTimer >= mFadeDuration)
            {
                mFadeAlpha = 0.0f;
                mFadeState = eFadeState::None;
            }
        }
    }

    if (mActiveScene)
        mActiveScene->Update();

    if (Input::GetKeyDown(eKeyCode::ESC))
    {
        PostQuitMessage(0);
    }

    if (mIsGameStart)
    {
        playTime += Time::DeltaTime();
    }
}

void SceneManager::LateUpdate()
{
    if (mActiveScene)
        mActiveScene->LateUpdate();
}

void SceneManager::Render(HDC hdc)
{
    if (mActiveScene)
        mActiveScene->Render(hdc);

    // ���콺 Ŀ�� ������
    int cursorImageWidth = mMouseCursorImage.GetWidth();
    int cursorImageHeight = mMouseCursorImage.GetHeight();
    mMouseCursorImage.Draw(hdc, Input::GetMousePosition().x - 40, Input::GetMousePosition().y - 40,
        cursorImageWidth + 50, cursorImageHeight + 50);

    // ���̵� ȿ�� ������
    if (mFadeState != eFadeState::None)
    {
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, 1280, 720); // ȭ�� ũ�� ���� �ʿ�
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

        // ������ �簢�� �׸���
        HBRUSH fadeBrush = CreateSolidBrush(RGB(0, 0, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, fadeBrush);
        RECT screenRect = { 0, 0, 1280, 720 }; // ȭ�� ũ�� ���� �ʿ�
        FillRect(memDC, &screenRect, fadeBrush);

        // ���� ���� ����
        BLENDFUNCTION blend = { AC_SRC_OVER, 0, static_cast<BYTE>(mFadeAlpha * 255), 0 };
        AlphaBlend(hdc, 0, 0, 1280, 720, memDC, 0, 0, 1280, 720, blend);

        // �ڿ� ����
        SelectObject(memDC, oldBrush);
        DeleteObject(fadeBrush);
        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);
    }

    Time::Render(hdc);

    // �÷��� Ÿ�� ������
    if (mIsGameStart)
    {
        SetTextColor(hdc, RGB(255, 255, 255));
        HFONT hFont = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        int minutes = static_cast<int>(playTime) / 60;
        int seconds = static_cast<int>(playTime) % 60;
        wchar_t playTimeText[32];
        swprintf_s(playTimeText, L"Play Time: %02d:%02d", minutes, seconds);
        TextOut(hdc, 1000, 0, playTimeText, wcslen(playTimeText));

        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
    }
}