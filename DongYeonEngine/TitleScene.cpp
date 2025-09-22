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

    startY += setButtonRect(mStartButtonRect, startY, L"START", scaleStart);
    startY += setButtonRect(mTutorialButtonRect, startY, L"TUTORIAL", scaleTutorial);
    startY += setButtonRect(mCustomStageButtonRect, startY, L"CUSTOM STAGE", scaleCustom);
    startY += setButtonRect(mMapToolButtonRect, startY, L"MAP TOOL", scaleMapTool);
    startY += setButtonRect(mSettingButtonRect, startY, L"SETTINGS", scaleSetting);
    startY += setButtonRect(mQuitButtonRect, startY, L"QUIT", scaleQuit);

    // GitHub 버튼 사각형 초기화 (좌측 하단)
    int githubIconSize = 150;
    int margin = 10;
    mGithubButtonRect = { margin, (INT)height - githubIconSize - margin, margin + githubIconSize, (INT)height - margin };

    // 설정 창 초기화
    mSettingWindowFlag = false;
    int settingWindowWidth = 400;
    int settingWindowHeight = 300;
    mSettingWindowRect = { static_cast<LONG>((width - settingWindowWidth) / 2), static_cast<LONG>((height - settingWindowHeight) / 2),
                           static_cast<LONG>((width + settingWindowWidth) / 2), static_cast<LONG>((height + settingWindowHeight) / 2) };

    // 아이디 입력 창 초기화
    mInputWindowFlag = false;
    int inputWindowWidth = 400;
    int inputWindowHeight = 200;
    mInputWindowRect = { static_cast<LONG>((width - inputWindowWidth) / 2), static_cast<LONG>((height - inputWindowHeight) / 2),
                         static_cast<LONG>((width + inputWindowWidth) / 2), static_cast<LONG>((height + inputWindowHeight) / 2) };

    // 확인 버튼 초기화
    int confirmButtonWidth = 100;
    int confirmButtonHeight = 50;
    mConfirmButtonRect = { mInputWindowRect.left + (inputWindowWidth - confirmButtonWidth) / 2,
                           mInputWindowRect.bottom - confirmButtonHeight - 20,
                           mInputWindowRect.left + (inputWindowWidth + confirmButtonWidth) / 2,
                           mInputWindowRect.bottom - 20 };

    // 아이디 입력 필드 초기화
    int inputFieldWidth = 300;
    int inputFieldHeight = 40;
    mInputFieldRect = { mInputWindowRect.left + (inputWindowWidth - inputFieldWidth) / 2,
                        mInputWindowRect.top + 80,
                        mInputWindowRect.left + (inputWindowWidth + inputFieldWidth) / 2,
                        mInputWindowRect.top + 80 + inputFieldHeight };

    // Close 버튼 초기화
    int closeButtonWidth = 100;
    int closeButtonHeight = 50;
    mCloseButtonRect = { mSettingWindowRect.left + (settingWindowWidth - closeButtonWidth) / 2,
                         mSettingWindowRect.bottom - closeButtonHeight - 20,
                         mSettingWindowRect.left + (settingWindowWidth + closeButtonWidth) / 2,
                         mSettingWindowRect.bottom - 20 };

    // 볼륨 슬라이더 사각형 초기화
    int barWidth = 200;
    int barHeight = 20;
    int barY = mSettingWindowRect.top + 125;
    mBGMVolumeBarRect = { mSettingWindowRect.left + (settingWindowWidth - barWidth) / 2,
                          barY, mSettingWindowRect.left + (settingWindowWidth + barWidth) / 2, barY + barHeight };
    mSEVolumeBarRect = { mSettingWindowRect.left + (settingWindowWidth - barWidth) / 2,
                         barY + 90, mSettingWindowRect.left + (settingWindowWidth + barWidth) / 2, barY + 90 + barHeight };

    // 초기 볼륨 값 설정
    mBGMVolume = SoundManager::GetInstance()->GetBGMVolume();
    mSEVolume = SoundManager::GetInstance()->GetSEVolume();

    // 버튼 호버 및 드래그 상태 초기화
    for (int i = 0; i < 10; i++)
        mButtonHovered[i] = false;
    mDraggingBGM = false;
    mDraggingSE = false;
    mInputActive = false;
    mInputText = L"";

    delete[] fontFamilies;
    ReleaseDC(GetActiveWindow(), hdc);
}

void TitleScene::Update()
{
    Scene::Update();

    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(GetActiveWindow(), &mousePos);

    // 호버 상태 업데이트
    if (!mSettingWindowFlag && !mInputWindowFlag)
    {
        mButtonHovered[0] = PtInRect(&mStartButtonRect, mousePos);
        mButtonHovered[1] = PtInRect(&mTutorialButtonRect, mousePos);
        mButtonHovered[2] = PtInRect(&mCustomStageButtonRect, mousePos);
        mButtonHovered[3] = PtInRect(&mMapToolButtonRect, mousePos);
        mButtonHovered[4] = PtInRect(&mSettingButtonRect, mousePos);
        mButtonHovered[5] = PtInRect(&mQuitButtonRect, mousePos);
        mButtonHovered[6] = PtInRect(&mGithubButtonRect, mousePos);
    }
    else
    {
        for (int i = 0; i <= 6; i++)
            mButtonHovered[i] = false;
    }

    if (mSettingWindowFlag)
    {
        mButtonHovered[7] = PtInRect(&mCloseButtonRect, mousePos);
        mButtonHovered[8] = PtInRect(&mBGMVolumeBarRect, mousePos) || PtInRect(&mSEVolumeBarRect, mousePos);
    }
    else if (mInputWindowFlag)
    {
        mButtonHovered[9] = PtInRect(&mConfirmButtonRect, mousePos);
    }

    if (Input::GetKeyDown(eKeyCode::LButton))
    {
        if (mSettingWindowFlag)
        {
            if (mButtonHovered[7])
            {
                mSettingWindowFlag = false;
                SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
            }
            else if (PtInRect(&mBGMVolumeBarRect, mousePos))
            {
                mDraggingBGM = true;
            }
            else if (PtInRect(&mSEVolumeBarRect, mousePos))
            {
                mDraggingSE = true;
            }
        }
        else if (mInputWindowFlag)
        {
            if (mButtonHovered[9])
            {
                std::wstring inputText = Input::GetInputText();
                if (!inputText.empty())
                {
                    SceneManager::StartFadeIn();
                    SceneManager::LoadScene(L"Stage1");
                    SoundManager::GetInstance()->mPlaySound("Earth", true);
                    SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
                    MapManager::GetInstance()->LoadMap(L"resources/MapTextFile/Stage1.txt");
                    SceneManager::GetSharedPlayer()->SetPosition(180, 300);
                    SceneManager::SetmIsGameStart(true);
                    mInputWindowFlag = false;
                    mInputText = L"";
                    Input::ClearInputText();
                }
            }
            else if (PtInRect(&mInputFieldRect, mousePos))
            {
                mInputActive = true;
                Input::ClearInputText();
            }
            else
            {
                mInputActive = false;
            }
        }
        else
        {
            if (mButtonHovered[0])
            {
                mInputWindowFlag = true;
                SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
            }
            else if (mButtonHovered[1])
            {
                SceneManager::StartFadeIn();
                SceneManager::LoadScene(L"TutorialStage");
                MapManager::GetInstance()->LoadMap(L"resources/MapTextFile/StageTutorial.txt");
                SoundManager::GetInstance()->mPlaySound("Tutorial_Jazz", true);
                SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
                SceneManager::GetSharedPlayer()->SetPosition(949, 1124);
            }
            else if (mButtonHovered[2])
            {
                SceneManager::StartFadeIn();
                MapManager::GetInstance()->LoadMap(L"resources/MapTextFile/StageCustom.txt");
                SceneManager::LoadScene(L"StageCustom");
                SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
            }
            else if (mButtonHovered[3])
            {
                SceneManager::LoadScene(L"MapTool");
                SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
            }
            else if (mButtonHovered[4])
            {
                mSettingWindowFlag = true;
                SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
            }
            else if (mButtonHovered[5])
            {
                PostQuitMessage(0);
            }
            else if (mButtonHovered[6])
            {
                ShellExecute(NULL, L"open", L"https://github.com/dongkimyeon/WizardofLegned", NULL, NULL, SW_SHOWNORMAL);
                SoundManager::GetInstance()->mPlaySound("MenuOpen", false);
            }
        }
    }

    // 마우스 드래그로 볼륨 조절
    if (Input::GetKey(eKeyCode::LButton) && mSettingWindowFlag)
    {
        if (mDraggingBGM)
        {
            int barWidth = mBGMVolumeBarRect.right - mBGMVolumeBarRect.left;
            float t = (float)(mousePos.x - mBGMVolumeBarRect.left) / barWidth;
            mBGMVolume = max(0.0f, min(1.0f, t));
            SoundManager::GetInstance()->SetBGMVolume(mBGMVolume);
        }
        else if (mDraggingSE)
        {
            int barWidth = mSEVolumeBarRect.right - mSEVolumeBarRect.left;
            float t = (float)(mousePos.x - mSEVolumeBarRect.left) / barWidth;
            mSEVolume = max(0.0f, min(1.0f, t));
            SoundManager::GetInstance()->SetSEVolume(mSEVolume);
        }
    }

    // 입력 텍스트 업데이트
    if (mInputActive)
    {
        mInputText = Input::GetInputText();
    }

    // 마우스 버튼 뗄 때 드래그 종료
    if (Input::GetKeyUp(eKeyCode::LButton))
    {
        mDraggingBGM = false;
        mDraggingSE = false;
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
            Gdiplus::RectF textRect(static_cast<Gdiplus::REAL>(rect.left), static_cast<Gdiplus::REAL>(rect.top),
                static_cast<Gdiplus::REAL>(rect.right - rect.left), static_cast<Gdiplus::REAL>(rect.bottom - rect.top));
            graphics.DrawString(text, -1, &font, textRect, &format, &brush);
            };

        renderButton(mStartButtonRect, L"START", mButtonHovered[0]);
        renderButton(mTutorialButtonRect, L"TUTORIAL", mButtonHovered[1]);
        renderButton(mCustomStageButtonRect, L"CUSTOM STAGE", mButtonHovered[2]);
        renderButton(mMapToolButtonRect, L"MAP TOOL", mButtonHovered[3]);
        renderButton(mSettingButtonRect, L"SETTINGS", mButtonHovered[4]);
        renderButton(mQuitButtonRect, L"QUIT", mButtonHovered[5]);

        // 설정 창 렌더링
        if (mSettingWindowFlag)
        {
            brush.SetColor(Gdiplus::Color(200, 50, 50, 50));
            graphics.FillRectangle(&brush, static_cast<INT>(mSettingWindowRect.left), static_cast<INT>(mSettingWindowRect.top),
                static_cast<INT>(mSettingWindowRect.right - mSettingWindowRect.left), static_cast<INT>(mSettingWindowRect.bottom - mSettingWindowRect.top));

            HFONT hFont = CreateFont(25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
            SetTextColor(hdc, RGB(255, 255, 255));
            const wchar_t* titleText = L"SETTINGS";
            SIZE titleSize;
            GetTextExtentPoint32(hdc, titleText, static_cast<int>(wcslen(titleText)), &titleSize);
            int titleX = mSettingWindowRect.left + (mSettingWindowRect.right - mSettingWindowRect.left - titleSize.cx) / 2;
            int titleY = mSettingWindowRect.top + 20;
            TextOut(hdc, titleX, titleY, titleText, static_cast<int>(wcslen(titleText)));

            DeleteObject(hFont);
            hFont = CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
            SelectObject(hdc, hFont);
            SetTextColor(hdc, RGB(255, 255, 255));

            const wchar_t* seLabel = L"SE VOLUME";
            SIZE seSize;
            GetTextExtentPoint32(hdc, seLabel, static_cast<int>(wcslen(seLabel)), &seSize);
            TextOut(hdc, mSettingWindowRect.left + 100, mBGMVolumeBarRect.top - 30 - seSize.cy / 2, seLabel, static_cast<int>(wcslen(seLabel)));

            const wchar_t* bgmLabel = L"BGM VOLUME";
            SIZE bgmSize;
            GetTextExtentPoint32(hdc, bgmLabel, static_cast<int>(wcslen(bgmLabel)), &bgmSize);
            TextOut(hdc, mSettingWindowRect.left + 100, mSEVolumeBarRect.top - 30 - bgmSize.cy / 2, bgmLabel, static_cast<int>(wcslen(bgmLabel)));

            brush.SetColor(Gdiplus::Color(255, 100, 100, 100));
            graphics.FillRectangle(&brush, static_cast<INT>(mBGMVolumeBarRect.left), static_cast<INT>(mBGMVolumeBarRect.top),
                static_cast<INT>(mBGMVolumeBarRect.right - mBGMVolumeBarRect.left), static_cast<INT>(mBGMVolumeBarRect.bottom - mBGMVolumeBarRect.top));
            graphics.FillRectangle(&brush, static_cast<INT>(mSEVolumeBarRect.left), static_cast<INT>(mSEVolumeBarRect.top),
                static_cast<INT>(mSEVolumeBarRect.right - mSEVolumeBarRect.left), static_cast<INT>(mSEVolumeBarRect.bottom - mSEVolumeBarRect.top));

            int barWidth = mBGMVolumeBarRect.right - mBGMVolumeBarRect.left;
            int handleWidth = 10;
            brush.SetColor(Gdiplus::Color(255, 255, 255, 255));
            graphics.FillRectangle(&brush, static_cast<INT>(mBGMVolumeBarRect.left + (int)(mBGMVolume * barWidth) - handleWidth / 2),
                static_cast<INT>(mBGMVolumeBarRect.top), handleWidth, static_cast<INT>(mBGMVolumeBarRect.bottom - mBGMVolumeBarRect.top));
            graphics.FillRectangle(&brush, static_cast<INT>(mSEVolumeBarRect.left + (int)(mSEVolume * barWidth) - handleWidth / 2),
                static_cast<INT>(mSEVolumeBarRect.top), handleWidth, static_cast<INT>(mSEVolumeBarRect.bottom - mSEVolumeBarRect.top));

            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);

            renderButton(mCloseButtonRect, L"CLOSE", mButtonHovered[7]);
        }

        // 아이디 입력 창 렌더링
        if (mInputWindowFlag)
        {
            brush.SetColor(Gdiplus::Color(200, 50, 50, 50));
            graphics.FillRectangle(&brush, static_cast<INT>(mInputWindowRect.left), static_cast<INT>(mInputWindowRect.top),
                static_cast<INT>(mInputWindowRect.right - mInputWindowRect.left), static_cast<INT>(mInputWindowRect.bottom - mInputWindowRect.top));

            HFONT hFont = CreateFont(25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
            SetTextColor(hdc, RGB(255, 255, 255));
            const wchar_t* titleText = L"ENTER ID";
            SIZE titleSize;
            GetTextExtentPoint32(hdc, titleText, static_cast<int>(wcslen(titleText)), &titleSize);
            int titleX = mInputWindowRect.left + (mInputWindowRect.right - mInputWindowRect.left - titleSize.cx) / 2;
            int titleY = mInputWindowRect.top + 20;
            TextOut(hdc, titleX, titleY, titleText, static_cast<int>(wcslen(titleText)));

            DeleteObject(hFont);
            hFont = CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
            SelectObject(hdc, hFont);

            // 입력 필드 렌더링
            brush.SetColor(mInputActive ? Gdiplus::Color(255, 255, 255, 255) : Gdiplus::Color(255, 200, 200, 200));
            graphics.FillRectangle(&brush, static_cast<INT>(mInputFieldRect.left), static_cast<INT>(mInputFieldRect.top),
                static_cast<INT>(mInputFieldRect.right - mInputFieldRect.left), static_cast<INT>(mInputFieldRect.bottom - mInputFieldRect.top));

            SetTextColor(hdc, RGB(0, 0, 0));
            TextOut(hdc, mInputFieldRect.left + 5, mInputFieldRect.top + 10, mInputText.c_str(), static_cast<int>(mInputText.length()));

            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);

            renderButton(mConfirmButtonRect, L"CONFIRM", mButtonHovered[9]);
        }

        delete[] fontFamilies;
    }

    if (!mGithubIconImage.IsNull())
    {
        int maxIconSize = mGithubButtonRect.right - mGithubButtonRect.left;
        int originalWidth = mGithubIconImage.GetWidth();
        int originalHeight = mGithubIconImage.GetHeight();
        float aspectRatio = (float)originalWidth / originalHeight;

        int renderWidth, renderHeight;
        if (aspectRatio > 1.0f)
        {
            renderWidth = maxIconSize;
            renderHeight = (int)(maxIconSize / aspectRatio);
        }
        else
        {
            renderHeight = maxIconSize;
            renderWidth = (int)(maxIconSize * aspectRatio);
        }

        int offsetX = mGithubButtonRect.left + (maxIconSize - renderWidth) / 2;
        int offsetY = mGithubButtonRect.top + (maxIconSize - renderHeight) / 2;

        mGithubIconImage.Draw(hdc, offsetX, offsetY, renderWidth, renderHeight);
        if (mButtonHovered[6])
        {
            Gdiplus::Graphics graphics(hdc);
            Gdiplus::SolidBrush brush(Gdiplus::Color(100, 255, 255, 255));
            graphics.FillRectangle(&brush, static_cast<INT>(mGithubButtonRect.left), static_cast<INT>(mGithubButtonRect.top), maxIconSize, maxIconSize);
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
    GetTextExtentPoint32(hdc, devText1, static_cast<int>(wcslen(devText1)), &devSize1);
    GetTextExtentPoint32(hdc, devText2, static_cast<int>(wcslen(devText2)), &devSize2);
    GetTextExtentPoint32(hdc, devText3, static_cast<int>(wcslen(devText3)), &devSize3);
    SetTextColor(hdc, RGB(231, 231, 231));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, static_cast<int>(width - devSize1.cx - margin), static_cast<int>(height - lineHeight * 3 - margin), devText1, static_cast<int>(wcslen(devText1)));
    TextOut(hdc, static_cast<int>(width - devSize2.cx - margin), static_cast<int>(height - lineHeight * 2 - margin), devText2, static_cast<int>(wcslen(devText2)));
    TextOut(hdc, static_cast<int>(width - devSize3.cx - margin), static_cast<int>(height - lineHeight - margin), devText3, static_cast<int>(wcslen(devText3)));

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}