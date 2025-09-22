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
    bool mButtonHovered[10]; // 기존 7개 + 설정 창의 Close 버튼, BGM 슬라이더, SE 슬라이더
    bool mDraggingBGM; // BGM 슬라이더 드래그 상태
    bool mDraggingSE;  // SE 슬라이더 드래그 상태
    float mBGMVolume;  // BGM 볼륨 (0.0f ~ 1.0f)
    float mSEVolume;   // SE 볼륨 (0.0f ~ 1.0f)

    bool mInputWindowFlag; // 아이디 입력 창 표시 여부
    RECT mInputWindowRect; // 아이디 입력 창 사각형
    RECT mConfirmButtonRect; // 확인 버튼 사각형
    RECT mInputFieldRect; // 입력 필드 사각형
    bool mInputActive; // 입력 필드 활성화 여부

    std::wstring mInputText; // 입력된 아이디 텍스트 (정적 제거)
    Gdiplus::PrivateFontCollection mFontCollection; // 폰트 컬렉션
};