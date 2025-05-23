#include "TitleScene.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "Time.h"
#include "Input.h"

extern const UINT width;
extern const UINT height;

void TitleScene::Initialize()
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    mBackGroundImage.Load(L"resources/Maintitle.png");
    mLogoImage.Load(L"resources/TitileLogo.png");

    // 폰트 파일 로드
    mFontCollection.AddFontFile(L"resources/Font/NotoSans-ExtraBold.ttf");

    // 버튼 사각형 초기화 (세로 배치, 중앙 정렬, 텍스트 크기에 맞춘 너비)
    float scaleStart = 1.0f;
    float scaleQuit = 1.0f;
    float scaleSetting = 1.0f;
    float scaleTutorial = 1.0f;
    float scaleCustom = 1.0f;
    float scaleMapTool = 1.0f;

    int baseHeight = 50;
    int spacing = 1;
    int centerX = width / 2;
    int startY = height / 2 + 50;

    // 폰트 설정 (텍스트 너비 측정용)
    INT fontCount = mFontCollection.GetFamilyCount();
    if (fontCount == 0) return; // 폰트 로드 실패 시 종료
    Gdiplus::FontFamily* fontFamilies = new Gdiplus::FontFamily[fontCount];
    INT foundCount;
    mFontCollection.GetFamilies(fontCount, fontFamilies, &foundCount);
    Gdiplus::Font font(&fontFamilies[0], 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
    format.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);

    // 임시 그래픽스 컨텍스트 (텍스트 측정용)
    HDC hdc = GetDC(GetActiveWindow());
    Gdiplus::Graphics graphics(hdc);

    // 버튼별 텍스트와 RECT 설정
    auto setButtonRect = [&](RECT& rect, int y, const wchar_t* text, float scale) {
        // 텍스트 크기 측정
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
    startY += setButtonRect(mQuitButtonRect, startY, L"quit", scaleQuit);
    startY += setButtonRect(mSettingButtonRect, startY, L"Settings", scaleSetting);
    startY += setButtonRect(mTutorialButtonRect, startY, L"Tutorial", scaleTutorial);
    startY += setButtonRect(mCustomStageButtonRect, startY, L"Custom Stage", scaleCustom);
    startY += setButtonRect(mMapToolButtonRect, startY, L"Map Tool", scaleMapTool);

    // 리소스 정리
    delete[] fontFamilies;
    ReleaseDC(GetActiveWindow(), hdc);

    // 호버 상태 초기화
    for (int i = 0; i < 6; i++)
        mButtonHovered[i] = false;
}

void TitleScene::Update()
{
    Scene::Update();

    // 마우스 위치 가져오기
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(GetActiveWindow(), &mousePos);

    // 버튼 호버 상태 업데이트
    mButtonHovered[0] = PtInRect(&mStartButtonRect, mousePos);
    mButtonHovered[1] = PtInRect(&mQuitButtonRect, mousePos);
    mButtonHovered[2] = PtInRect(&mSettingButtonRect, mousePos);
    mButtonHovered[3] = PtInRect(&mTutorialButtonRect, mousePos);
    mButtonHovered[4] = PtInRect(&mCustomStageButtonRect, mousePos);
    mButtonHovered[5] = PtInRect(&mMapToolButtonRect, mousePos);

    // 버튼 클릭 처리
    if (Input::GetKeyDown(eKeyCode::LButton))
    {
        if (mButtonHovered[0]) 
        {
            SceneManager::LoadScene(L"Stage1");
            MapManager::GetInstance()->LoadMap(L"Stage1.txt");
            SceneManager::GetSharedPlayer()->SetPosition(180, 270);
            SceneManager::SetmIsGameStart(true);
        } // Start
        if (mButtonHovered[1]) 
        {
            PostQuitMessage(0); // Quit
			
        } // Quit
        if (mButtonHovered[2]) {} // Settings
        if (mButtonHovered[3]) 
        {
            SceneManager::LoadScene(L"TutorialStage");
            MapManager::GetInstance()->LoadMap(L"StageTutorial.txt");
			SceneManager::GetSharedPlayer()->SetPosition(1000, 1000);
		
        } // Tutorial
        if (mButtonHovered[4]) {} // Custom Stage
        if (mButtonHovered[5]) {} // Map Tool
    }
}

void TitleScene::LateUpdate()
{
    Scene::LateUpdate();
}

void TitleScene::Render(HDC hdc)
{
    // 배경 렌더링
    if (!mBackGroundImage.IsNull())
    {
        mBackGroundImage.Draw(hdc, 0, 0, width, height);
    }

    // 로고 렌더링
    if (!mLogoImage.IsNull())
    {
        mLogoImage.Draw(hdc, 0, 0, width, height);
    }

    // 폰트 설정 (PrivateFontCollection에서 폰트 패밀리 가져오기)
    INT fontCount = mFontCollection.GetFamilyCount();
    if (fontCount > 0)
    {
        Gdiplus::FontFamily* fontFamilies = new Gdiplus::FontFamily[fontCount];
        INT foundCount;
        mFontCollection.GetFamilies(fontCount, fontFamilies, &foundCount);

        // 첫 번째 폰트 패밀리 사용
        Gdiplus::Font font(&fontFamilies[0], 30, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
        Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 0, 0)); // 기본 검정
        Gdiplus::StringFormat format;
        format.SetAlignment(Gdiplus::StringAlignmentCenter);
        format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
        format.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap); // 자동 줄넘김 해제

        // GDI+ 그래픽스 객체 (텍스트 렌더링용)
        Gdiplus::Graphics graphics(hdc);

        // 버튼 텍스트 렌더링
        auto renderButton = [&](RECT& rect, const wchar_t* text, bool isHovered) {
            // 호버 여부에 따라 투명도 설정
            brush.SetColor(isHovered ? Gdiplus::Color(255, 0, 0, 0) : Gdiplus::Color(100, 0, 0, 0));

            // 텍스트 위치 계산
            Gdiplus::RectF textRect((float)rect.left, (float)rect.top,
                (float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
            graphics.DrawString(text, -1, &font, textRect, &format, &brush);
            };

        renderButton(mStartButtonRect, L"START", mButtonHovered[0]);
        renderButton(mQuitButtonRect, L"EXIT", mButtonHovered[1]);
        renderButton(mSettingButtonRect, L"SETTING", mButtonHovered[2]);
        renderButton(mTutorialButtonRect, L"TUTORIAL", mButtonHovered[3]);
        renderButton(mCustomStageButtonRect, L"CUSTOM STAGE", mButtonHovered[4]);
        renderButton(mMapToolButtonRect, L"MAP TOOL", mButtonHovered[5]);

        // 동적 할당 해제
        delete[] fontFamilies;
    }
}