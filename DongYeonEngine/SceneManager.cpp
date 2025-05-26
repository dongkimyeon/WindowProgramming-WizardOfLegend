#include "SceneManager.h"
#include "Input.h"
#include "Time.h"

// 정적 멤버 변수 초기화
std::map<std::wstring, Scene*> SceneManager::mScene = {};
Scene* SceneManager::mActiveScene = nullptr;
CImage SceneManager::mMouseCursorImage;
Player* SceneManager::mSharedPlayer = nullptr;
float SceneManager::playTime = 0.0f;
bool SceneManager::mIsGameStart = false;

// 페이드 관련 초기화
SceneManager::eFadeState SceneManager::mFadeState = eFadeState::None;
float SceneManager::mFadeAlpha = 0.0f;
float SceneManager::mFadeTimer = 0.0f;
const float SceneManager::mFadeDuration = 2.0f; // 페이드 2초 지속
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
    mFadeAlpha = 0.0f; // 페이드아웃: 0.0 -> 1.0 (화면이 어두워짐)
}

void SceneManager::StartFadeIn()
{
    mFadeState = eFadeState::FadeIn;
    mFadeTimer = 0.0f;
    mFadeAlpha = 1.0f; // 페이드인: 1.0 -> 0.0 (화면이 밝아짐)
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
    // 페이드 상태 처리
    if (mFadeState != eFadeState::None)
    {
        mFadeTimer += Time::DeltaTime();
        float t = mFadeTimer / mFadeDuration; // 0.0 ~ 1.0
        t = (t > 1.0f) ? 1.0f : t; // 클램핑

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

    // 마우스 커서 렌더링
    int cursorImageWidth = mMouseCursorImage.GetWidth();
    int cursorImageHeight = mMouseCursorImage.GetHeight();
    mMouseCursorImage.Draw(hdc, Input::GetMousePosition().x - 40, Input::GetMousePosition().y - 40,
        cursorImageWidth + 50, cursorImageHeight + 50);

    // 페이드 효과 렌더링
    if (mFadeState != eFadeState::None)
    {
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, 1280, 720); // 화면 크기 조정 필요
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

        // 검은색 사각형 그리기
        HBRUSH fadeBrush = CreateSolidBrush(RGB(0, 0, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, fadeBrush);
        RECT screenRect = { 0, 0, 1280, 720 }; // 화면 크기 조정 필요
        FillRect(memDC, &screenRect, fadeBrush);

        // 알파 블렌딩 설정
        BLENDFUNCTION blend = { AC_SRC_OVER, 0, static_cast<BYTE>(mFadeAlpha * 255), 0 };
        AlphaBlend(hdc, 0, 0, 1280, 720, memDC, 0, 0, 1280, 720, blend);

        // 자원 해제
        SelectObject(memDC, oldBrush);
        DeleteObject(fadeBrush);
        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);
    }

    Time::Render(hdc);

    // 플레이 타임 렌더링
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