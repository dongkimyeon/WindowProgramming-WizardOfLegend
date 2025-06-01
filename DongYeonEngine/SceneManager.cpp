#include "SceneManager.h"
#include "Input.h"
#include "Time.h"


static ULONG_PTR gdiplusToken;

// 정적 멤버 변수 초기화
std::map<std::wstring, Scene*> SceneManager::mScene = {};
Scene* SceneManager::mActiveScene = nullptr;
CImage SceneManager::mMouseCursorImage;
Player* SceneManager::mSharedPlayer = nullptr;
float SceneManager::playTime = 0.0f;
bool SceneManager::mIsGameStart = false;
bool SceneManager::mESCstate = false;

// 페이드 관련 초기화
SceneManager::eFadeState SceneManager::mFadeState = eFadeState::None;
float SceneManager::mFadeAlpha = 0.0f;
float SceneManager::mFadeTimer = 0.0f;
const float SceneManager::mFadeDuration = 2.0f;
std::wstring SceneManager::mTargetScene = L"";


static RECT resumeButtonRect;
static RECT titleButtonRect;

void SceneManager::Initialize()
{
    mMouseCursorImage.Load(L"resources/MouseCursor.png");
    mSharedPlayer = new Player();
    playTime = 0.0f;
    MapManager::GetInstance()->Initialize();

    // GDI+ 초기화
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

void SceneManager::StartFadeOut(const std::wstring& name)
{
    mFadeState = eFadeState::FadeOut;
    mFadeTimer = 0.0f;
    mFadeAlpha = 0.0f;
    mTargetScene = name; // 전환할 대상 씬 저장
}

void SceneManager::StartFadeIn()
{
    mFadeState = eFadeState::FadeIn;
    mFadeTimer = 0.0f;
    mFadeAlpha = 1.0f;
}

Scene* SceneManager::LoadScene(const std::wstring& name)
{
    auto iter = mScene.find(name);
    if (iter == mScene.end())
        return nullptr;

    mActiveScene = iter->second;
    mActiveScene->ObjectInitialize();
    return iter->second;
}

void SceneManager::Update()
{
    // 페이드 상태 처리
    if (mFadeState != eFadeState::None)
    {
        mFadeTimer += Time::DeltaTime();
        float t = mFadeTimer / mFadeDuration;
        t = (t > 1.0f) ? 1.0f : t;

        if (mFadeState == eFadeState::FadeOut)
        {
            mFadeAlpha = t;
            if (mFadeTimer >= mFadeDuration)
            {
                mFadeAlpha = 1.0f;
                if (!mTargetScene.empty())
                {
                    LoadScene(mTargetScene);
                    StartFadeIn(); // 페이드인 시작
                    mTargetScene = L"";
                }
                mFadeState = eFadeState::None;
            }
        }
        else if (mFadeState == eFadeState::FadeIn)
        {
            mFadeAlpha = 1.0f - t;
            if (mFadeTimer >= mFadeDuration)
            {
                mFadeAlpha = 0.0f;
                mFadeState = eFadeState::None;
            }
        }
    }

    // ESC 키 입력 처리
    if (Input::GetKeyDown(eKeyCode::ESC) && mActiveScene->GetName() != L"TitleScene"
        && mActiveScene->GetName() != L"GameOverScene" && mActiveScene->GetName() != L"GameClearScene")
    {
        mESCstate = !mESCstate;
        Time::SetTimeStop(mESCstate);
    }

    // 일시정지 메뉴에서 마우스 입력 처리
    if (mESCstate)
    {
        if (Input::GetKeyDown(eKeyCode::LButton)) // 마우스 왼쪽 클릭
        {
            POINT mousePos = { Input::GetMousePosition().x, Input::GetMousePosition().y };
            // "게임 재개" 버튼 클릭
            if (PtInRect(&resumeButtonRect, mousePos))
            {
                mESCstate = false;
                Time::SetTimeStop(false);
            }
            // "타이틀 화면으로" 버튼 클릭
            else if (PtInRect(&titleButtonRect, mousePos))
            {
                mESCstate = false;
                Time::SetTimeStop(false);
                playTime = 0.0f; // 게임 시간 초기화
                mIsGameStart = false; // 게임 시작 상태 초기화
                StartFadeIn();
                LoadScene(L"TitleScene");
            }
        }
    }
    else
    {
        if (mActiveScene)
            mActiveScene->Update();
    }

    // 게임 진행 중이고 플레이어가 살아있을 때만 playTime 증가
    if (mIsGameStart && !SceneManager::GetSharedPlayer()->GetIsDead() && !mESCstate)
    {
        playTime += Time::DeltaTime();
    }
}

void SceneManager::LateUpdate()
{
    if (mActiveScene && !mESCstate) // 일시정지 상태가 아닐 때만
        mActiveScene->LateUpdate();
}

void SceneManager::Render(HDC hdc)
{
    using namespace Gdiplus;
    Graphics graphics(hdc);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

    if (mActiveScene)
        mActiveScene->Render(hdc);

    // 플레이 타임 렌더링
    if (mIsGameStart && !SceneManager::GetSharedPlayer()->GetIsDead())
    {
        FontFamily fontFamily(L"Exo 2"); // 폰트명을 "Exo 2"로 설정
        Gdiplus::Font font(&fontFamily, 30, FontStyleRegular, UnitPixel);
        StringFormat stringFormat;
        stringFormat.SetAlignment(StringAlignmentNear);
        SolidBrush brush(Color(255, 255, 255, 255));

        int minutes = static_cast<int>(playTime) / 60;
        int seconds = static_cast<int>(playTime) % 60;
        wchar_t playTimeText[32];
        swprintf_s(playTimeText, L"Play Time: %02d:%02d", minutes, seconds);

        graphics.DrawString(playTimeText, -1, &font, PointF(1000, 0), &stringFormat, &brush);
    }

    // 페이드 효과 렌더링
    if (mFadeState != eFadeState::None)
    {
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, 1280, 720);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

        HBRUSH fadeBrush = CreateSolidBrush(RGB(0, 0, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, fadeBrush);
        RECT screenRect = { 0, 0, 1280, 720 };
        FillRect(memDC, &screenRect, fadeBrush);

        BLENDFUNCTION blend = { AC_SRC_OVER, 0, static_cast<BYTE>(mFadeAlpha * 255), 0 };
        AlphaBlend(hdc, 0, 0, 1280, 720, memDC, 0, 0, 1280, 720, blend);

        SelectObject(memDC, oldBrush);
        DeleteObject(fadeBrush);
        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);
    }

    // 일시정지 메뉴 렌더링
    if (mESCstate)
    {
        // 반투명 배경
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, 1280, 720);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

        HBRUSH bgBrush = CreateSolidBrush(RGB(0, 0, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, bgBrush);
        RECT screenRect = { 0, 0, 1280, 720 };
        FillRect(memDC, &screenRect, bgBrush);

        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 128, 0 };
        AlphaBlend(hdc, 0, 0, 1280, 720, memDC, 0, 0, 1280, 720, blend);

        SelectObject(memDC, oldBrush);
        DeleteObject(bgBrush);
        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        // GDI+ 폰트 설정
        FontFamily fontFamily(L"Exo 2");
        Gdiplus::Font font(&fontFamily, 40, FontStyleBold, UnitPixel);
        StringFormat stringFormat;
        stringFormat.SetAlignment(StringAlignmentCenter); // 중앙 정렬
        stringFormat.SetLineAlignment(StringAlignmentCenter); // 수직 중앙 정렬

        // 텍스트 크기 계산
        RectF resumeTextBounds, titleTextBounds;
        graphics.MeasureString(L"GameContinue", -1, &font, PointF(0, 0), &stringFormat, &resumeTextBounds);
        graphics.MeasureString(L"Go to Title", -1, &font, PointF(0, 0), &stringFormat, &titleTextBounds);

        // 버튼 크기 조절 (패딩 추가)
        const float paddingX = 20.0f; // 수평 패딩
        const float paddingY = 10.0f; // 수직 패딩

        // "게임 재개" 버튼 사각형
        float resumeWidth = resumeTextBounds.Width + 2 * paddingX;
        float resumeHeight = resumeTextBounds.Height + 2 * paddingY;
        float resumeX = 640.0f - resumeWidth / 2; // 화면 중앙 (1280/2 = 640)
        float resumeY = 300.0f;
        resumeButtonRect = {
            static_cast<LONG>(resumeX),
            static_cast<LONG>(resumeY),
            static_cast<LONG>(resumeX + resumeWidth),
            static_cast<LONG>(resumeY + resumeHeight)
        };

        // "타이틀 화면으로" 버튼 사각형
        float titleWidth = titleTextBounds.Width + 2 * paddingX;
        float titleHeight = titleTextBounds.Height + 2 * paddingY;
        float titleX = 640.0f - titleWidth / 2; // 화면 중앙
        float titleY = 400.0f;
        titleButtonRect = {
            static_cast<LONG>(titleX),
            static_cast<LONG>(titleY),
            static_cast<LONG>(titleX + titleWidth),
            static_cast<LONG>(titleY + titleHeight)
        };

        // 마우스 위치 확인
        POINT mousePos = { Input::GetMousePosition().x, Input::GetMousePosition().y };
        bool isResumeHovered = PtInRect(&resumeButtonRect, mousePos);
        bool isTitleHovered = PtInRect(&titleButtonRect, mousePos);

        // "게임 재개" 텍스트
        SolidBrush resumeBrush(Color(isResumeHovered ? 255 : 128, 255, 255, 255));
        RectF resumeRect(resumeX, resumeY, resumeWidth, resumeHeight);
        graphics.DrawString(L"GameContinue", -1, &font, resumeRect, &stringFormat, &resumeBrush);

        // "타이틀 화면으로" 텍스트
        SolidBrush titleBrush(Color(isTitleHovered ? 255 : 128, 255, 255, 255));
        RectF titleRect(titleX, titleY, titleWidth, titleHeight);
        graphics.DrawString(L"Go to Title", -1, &font, titleRect, &stringFormat, &titleBrush);
    }

    // 마우스 커서 렌더링
    int cursorImageWidth = mMouseCursorImage.GetWidth();
    int cursorImageHeight = mMouseCursorImage.GetHeight();
    mMouseCursorImage.Draw(hdc, Input::GetMousePosition().x - 40, Input::GetMousePosition().y - 40,
        cursorImageWidth + 50, cursorImageHeight + 50);
}