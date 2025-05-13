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
    mBackGroundImage.Load(L"resources/Maintitle.png"); // ���� �̹��� ��η� ����
   

    
    mLogoImage.Load(L"resources/TitileLogo.png"); 
   
}

void TitleScene::Update()
{
    Scene::Update();

    // �����̽� Ű �Է� �� PlayScene���� ��ȯ
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

    if (!mBackGroundImage.IsNull())
    {
        mBackGroundImage.Draw(hdc, 0, 0, width, height);
    }

    if (!mLogoImage.IsNull())
    {
        int logoWidth = mLogoImage.GetWidth();
        int logoHeight = mLogoImage.GetHeight();
        int logoX = (width - logoWidth) / 2; 
        int logoY = (height - logoHeight) / 2; 
        mLogoImage.Draw(hdc, logoX, logoY, logoWidth, logoHeight);
    }

  
    Time::Render(hdc);
}