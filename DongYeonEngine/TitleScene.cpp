#include "TitleScene.h"
#include "SceneManager.h"
#include "Time.h"

extern const UINT width;
extern const UINT height;

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    // 배경 이미지 로드
    mBackGroundImage.Load(L"resources/Maintitle.png"); // 실제 이미지 경로로 변경
    if (mBackGroundImage.IsNull())
    {
        // 이미지 로드 실패 시 오류 처리
        MessageBox(nullptr, L"배경 이미지를 로드하지 못했습니다!", L"오류", MB_OK | MB_ICONERROR);
    }

    // 로고 이미지 로드
    mLogoImage.Load(L"resources/TitileLogo.png"); // 실제 이미지 경로로 변경
    if (mLogoImage.IsNull())
    {
        // 이미지 로드 실패 시 오류 처리
        MessageBox(nullptr, L"로고 이미지를 로드하지 못했습니다!", L"오류", MB_OK | MB_ICONERROR);
    }
}

void TitleScene::Update()
{
    Scene::Update();

    // 스페이스 키 입력 시 PlayScene으로 전환
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        SceneManager::LoadScene(L"Stage1");
    }
}

void TitleScene::LateUpdate()
{
    Scene::LateUpdate();
}

void TitleScene::Render(HDC hdc)
{
    // 배경 이미지 그리기 (창 크기에 맞게 확장)
    if (!mBackGroundImage.IsNull())
    {
        mBackGroundImage.Draw(hdc, 0, 0, width, height);
    }

    // 로고 이미지 그리기 (화면 중앙에 배치, 필요에 따라 위치 및 크기 조정)
    if (!mLogoImage.IsNull())
    {
        int logoWidth = mLogoImage.GetWidth();
        int logoHeight = mLogoImage.GetHeight();
        int logoX = (width - logoWidth) / 2; // 수평 중앙
        int logoY = (height - logoHeight) / 2 ; // 수직 중앙
        mLogoImage.Draw(hdc, logoX, logoY, logoWidth, logoHeight);
    }

    // Time 렌더링 (기존 코드 유지)
    Time::Render(hdc);
}