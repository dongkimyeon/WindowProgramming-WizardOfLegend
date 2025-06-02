#include "TitleScene.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "Time.h"
#include "Input.h"
#include "SoundManager.h"
#include <shellapi.h>

extern const UINT width;
extern const UINT height;

void TitleScene::Initialize()
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    mBackGroundImage.Load(L"resources/Maintitle.png");
    mLogoImage.Load(L"resources/TitileLogo.png");
    mGithubIconImage.Load(L"resources/GitHub-Logo.png");

    mFontCollection.AddFontFile(L"resources/Font/NotoSans-ExtraBold.ttf");

    float scaleStart = 1.0f;
    float scaleTutorial = 1.0f;
    float scaleCustom = 1.0f;
    float scaleMapTool = 1.0f;
    float scaleSetting = 1.0f;
    float scaleQuit = 1.0f;

    int baseHeight = 50;
    int spacing = 1;
    int centerX = width / 2;
    int startY = height / 2 + 50;

    INT fontCount = mFontCollection.GetFamilyCount();
    if (fontCount == 0) return;
    Gdiplus::FontFamily* fontFamilies = new Gdiplus::FontFamily[fontCount];
    INT foundCount;
    mFontCollection.GetFamilies(fontCount, fontFamilies, &foundCount);
    Gdiplus::Font font(&fontFamilies[0], 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
    format.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);

    HDC hdc = GetDC(GetActiveWindow());
    Gdiplus::Graphics graphics(hdc);

    auto setButtonRect = [&](RECT& rect, int y, const wchar_t* text, float scale) {
        Gdiplus::RectF layoutRect(0, 0, 0, 0);
        Gdiplus::RectF boundRect;
        graphics.MeasureString(text, -1, &font, layoutRect, &format, &boundRect);
        int buttonWidth = (int)(boundRect.Width * scale) + 70;
        int buttonHeight = (int)(baseHeight * scale);
        int left = centerX - buttonWidth / 2;
        rect = { left, y, left + buttonWidth, y + buttonHeight };
        return buttonHeight + spacing;
        };

    startY += setButtonRect(mStartButtonRect, startY, L"Start", scaleStart);
    startY += setButtonRect(mTutorialButtonRect, startY, L"Tutorial", scaleTutorial);
    startY += setButtonRect(mCustomStageButtonRect, startY, L"Custom Stage", scaleCustom);
    startY += setButtonRect(mMapToolButtonRect, startY, L"Map Tool", scaleMapTool);
    startY += setButtonRect(mSettingButtonRect, startY, L"Settings", scaleSetting);
    startY += setButtonRect(mQuitButtonRect, startY, L"quit", scaleQuit);

    // GitHub 버튼 사각형 초기화 (좌측 하단)
    int githubIconSize = 150; // 아이콘 크기 축소 (정사각형)
    int margin = 10; // 화면 경계로부터의 여백
    mGithubButtonRect = { margin, (INT)height - githubIconSize - margin, margin + githubIconSize, (INT)height - margin };

    delete[] fontFamilies;
    ReleaseDC(GetActiveWindow(), hdc);

    for (int i = 0; i < 7; i++)
        mButtonHovered[i] = false;
}

void TitleScene::Update()
{
    Scene::Update();

    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(GetActiveWindow(), &mousePos);

    mButtonHovered[0] = PtInRect(&mStartButtonRect, mousePos);
    mButtonHovered[1] = PtInRect(&mTutorialButtonRect, mousePos);
    mButtonHovered[2] = PtInRect(&mCustomStageButtonRect, mousePos);
    mButtonHovered[3] = PtInRect(&mMapToolButtonRect, mousePos);
    mButtonHovered[4] = PtInRect(&mSettingButtonRect, mousePos);
    mButtonHovered[5] = PtInRect(&mQuitButtonRect, mousePos);
    mButtonHovered[6] = PtInRect(&mGithubButtonRect, mousePos);

    if (Input::GetKeyDown(eKeyCode::LButton))
    {
        if (mButtonHovered[0])
        {
            SceneManager::StartFadeIn();
            SceneManager::LoadScene(L"Stage1");
            SoundManager::GetInstance()->mPlaySound("Earth", true);
            SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
            MapManager::GetInstance()->LoadMap(L"Stage1.txt");
            SceneManager::GetSharedPlayer()->SetPosition(180, 300);
            SceneManager::SetmIsGameStart(true);
        }
        if (mButtonHovered[1])
        {
            SceneManager::StartFadeIn();
            SceneManager::LoadScene(L"TutorialStage");
            MapManager::GetInstance()->LoadMap(L"StageTutorial.txt");
            SoundManager::GetInstance()->mPlaySound("Tutorial_Jazz", true);
            SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
            SceneManager::GetSharedPlayer()->SetPosition(1250, 1250);
        }
        if (mButtonHovered[2])
        {
            SceneManager::StartFadeIn();
            MapManager::GetInstance()->LoadMap(L"StageCustom.txt");
            SceneManager::LoadScene(L"StageCustom");
            SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
        }
        if (mButtonHovered[3])
        {
            //맵툴
            SceneManager::LoadScene(L"MapTool");
            SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
        }
        if (mButtonHovered[4])
        {
            SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
        }
        if (mButtonHovered[5])
        {
            PostQuitMessage(0);
        }
        if (mButtonHovered[6])
        {
            ShellExecute(NULL, L"open", L"https://github.com/dongkimyeon/WizardofLegned", NULL, NULL, SW_SHOWNORMAL);
            SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
        }
    }

    SoundManager::GetInstance()->Update();
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

    INT fontCount = mFontCollection.GetFamilyCount();
    if (fontCount > 0)
    {
        Gdiplus::FontFamily* fontFamilies = new Gdiplus::FontFamily[fontCount];
        INT foundCount;
        mFontCollection.GetFamilies(fontCount, fontFamilies, &foundCount);

        Gdiplus::Font font(&fontFamilies[0], 30, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
        Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0, 0));
        Gdiplus::StringFormat format;
        format.SetAlignment(Gdiplus::StringAlignmentCenter);
        format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
        format.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);

        Gdiplus::Graphics graphics(hdc);

        auto renderButton = [&](RECT& rect, const wchar_t* text, bool isHovered) {
            brush.SetColor(isHovered ? Gdiplus::Color(255, 0, 0, 0) : Gdiplus::Color(100, 0, 0, 0));
            Gdiplus::RectF textRect((float)rect.left, (float)rect.top,
                (float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
            graphics.DrawString(text, -1, &font, textRect, &format, &brush);
            };

        renderButton(mStartButtonRect, L"START", mButtonHovered[0]);
        renderButton(mTutorialButtonRect, L"TUTORIAL", mButtonHovered[1]);
        renderButton(mCustomStageButtonRect, L"CUSTOM STAGE", mButtonHovered[2]);
        renderButton(mMapToolButtonRect, L"MAP TOOL", mButtonHovered[3]);
        renderButton(mSettingButtonRect, L"SETTING", mButtonHovered[4]);
        renderButton(mQuitButtonRect, L"EXIT", mButtonHovered[5]);

        delete[] fontFamilies;
    }

    if (!mGithubIconImage.IsNull())
    {
        int maxIconSize = mGithubButtonRect.right - mGithubButtonRect.left; // 100
        int originalWidth = mGithubIconImage.GetWidth();
        int originalHeight = mGithubIconImage.GetHeight();
        float aspectRatio = (float)originalWidth / originalHeight;

        int renderWidth, renderHeight;
        if (aspectRatio > 1.0f) // 이미지가 더 넓은 경우
        {
            renderWidth = maxIconSize;
            renderHeight = (int)(maxIconSize / aspectRatio);
        }
        else // 이미지가 더 높은 경우 또는 정사각형
        {
            renderHeight = maxIconSize;
            renderWidth = (int)(maxIconSize * aspectRatio);
        }

        // 버튼 영역 중앙에 아이콘 배치
        int offsetX = mGithubButtonRect.left + (maxIconSize - renderWidth) / 2;
        int offsetY = mGithubButtonRect.top + (maxIconSize - renderHeight) / 2;

        mGithubIconImage.Draw(hdc, offsetX, offsetY, renderWidth, renderHeight);
        if (mButtonHovered[6])
        {
            Gdiplus::Graphics graphics(hdc);
            Gdiplus::SolidBrush brush(Gdiplus::Color(100, 255, 255, 255)); // 호버 시 반투명 하이라이트
            graphics.FillRectangle(&brush, mGithubButtonRect.left, mGithubButtonRect.top, maxIconSize, maxIconSize);
        }
    }

    HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    const wchar_t* devText1 = L"DEVELOPER";
    const wchar_t* devText2 = L"DONGYEON";
    const wchar_t* devText3 = L"HYEONGMIN";
    int margin = 10;
    int lineHeight = 20;

    SIZE devSize1, devSize2, devSize3;
    GetTextExtentPoint32(hdc, devText1, wcslen(devText1), &devSize1);
    GetTextExtentPoint32(hdc, devText2, wcslen(devText2), &devSize2);
    GetTextExtentPoint32(hdc, devText3, wcslen(devText3), &devSize3);
    SetTextColor(hdc, RGB(231, 231, 231));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, width - devSize1.cx - margin, height - lineHeight * 3 - margin, devText1, wcslen(devText1));
    TextOut(hdc, width - devSize2.cx - margin, height - lineHeight * 2 - margin, devText2, wcslen(devText2));
    TextOut(hdc, width - devSize3.cx - margin, height - lineHeight - margin, devText3, wcslen(devText3));

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}