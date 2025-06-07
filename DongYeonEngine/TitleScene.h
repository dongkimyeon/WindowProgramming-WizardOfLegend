#pragma once
#include "Scene.h"
#include "StageCustom.h"

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

    // ���� â ����
    bool mSettingWindowFlag;
    RECT mSettingWindowRect;
    RECT mCloseButtonRect;
    RECT mBGMVolumeBarRect;
    RECT mSEVolumeBarRect;
    bool mButtonHovered[9]; // ���� 7�� + ���� â�� Close ��ư, BGM �����̴�, SE �����̴�
    bool mDraggingBGM; // BGM �����̴� �巡�� ����
    bool mDraggingSE;  // SE �����̴� �巡�� ����
    float mBGMVolume;  // BGM ���� (0.0f ~ 1.0f)
    float mSEVolume;   // SE ���� (0.0f ~ 1.0f)

    Gdiplus::PrivateFontCollection mFontCollection; // ��Ʈ �÷���
};