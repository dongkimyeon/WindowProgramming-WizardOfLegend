#pragma once
#include "Scene.h"


class TitleScene : public Scene
{
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;

private:

  
    CImage mBackGroundImage;
    CImage mLogoImage;

	
	RECT mStartButtonRect;
	RECT mQuitButtonRect;
	RECT mSettingButtonRect;
	RECT mTutorialButtonRect;
	RECT mCustomStageButtonRect;
	RECT mMapToolButtonRect;

	Gdiplus::PrivateFontCollection mFontCollection; // 폰트 컬렉션
	bool mButtonHovered[6]; // 버튼 호버 상태 (Start, Quit, Settings, Tutorial, Custom, MapTool)

};