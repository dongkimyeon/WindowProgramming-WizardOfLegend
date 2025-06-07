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

    // 설정 창 관련
    bool mSettingWindowFlag;
    RECT mSettingWindowRect;
    RECT mCloseButtonRect;
    RECT mBGMVolumeBarRect;
    RECT mSEVolumeBarRect;
    bool mButtonHovered[9]; // 기존 7개 + 설정 창의 Close 버튼, BGM 슬라이더, SE 슬라이더
    bool mDraggingBGM; // BGM 슬라이더 드래그 상태
    bool mDraggingSE;  // SE 슬라이더 드래그 상태
    float mBGMVolume;  // BGM 볼륨 (0.0f ~ 1.0f)
    float mSEVolume;   // SE 볼륨 (0.0f ~ 1.0f)

    Gdiplus::PrivateFontCollection mFontCollection; // 폰트 컬렉션
};