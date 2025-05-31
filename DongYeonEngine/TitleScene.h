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
	CImage mGithubIconImage;
	
	RECT mStartButtonRect;
	RECT mQuitButtonRect;
	RECT mSettingButtonRect;
	RECT mTutorialButtonRect;
	RECT mCustomStageButtonRect;
	RECT mMapToolButtonRect;
	RECT mGithubButtonRect;

	Gdiplus::PrivateFontCollection mFontCollection; // ��Ʈ �÷���
	bool mButtonHovered[7]; // ��ư ȣ�� ���� (Start, Quit, Settings, Tutorial, Custom, MapTool)

};