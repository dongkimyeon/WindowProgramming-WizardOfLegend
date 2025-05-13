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
    // ��� �̹��� �ε�
    mBackGroundImage.Load(L"resources/Maintitle.png"); // ���� �̹��� ��η� ����
    if (mBackGroundImage.IsNull())
    {
        // �̹��� �ε� ���� �� ���� ó��
        MessageBox(nullptr, L"��� �̹����� �ε����� ���߽��ϴ�!", L"����", MB_OK | MB_ICONERROR);
    }

    // �ΰ� �̹��� �ε�
    mLogoImage.Load(L"resources/TitileLogo.png"); // ���� �̹��� ��η� ����
    if (mLogoImage.IsNull())
    {
        // �̹��� �ε� ���� �� ���� ó��
        MessageBox(nullptr, L"�ΰ� �̹����� �ε����� ���߽��ϴ�!", L"����", MB_OK | MB_ICONERROR);
    }
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
    // ��� �̹��� �׸��� (â ũ�⿡ �°� Ȯ��)
    if (!mBackGroundImage.IsNull())
    {
        mBackGroundImage.Draw(hdc, 0, 0, width, height);
    }

    // �ΰ� �̹��� �׸��� (ȭ�� �߾ӿ� ��ġ, �ʿ信 ���� ��ġ �� ũ�� ����)
    if (!mLogoImage.IsNull())
    {
        int logoWidth = mLogoImage.GetWidth();
        int logoHeight = mLogoImage.GetHeight();
        int logoX = (width - logoWidth) / 2; // ���� �߾�
        int logoY = (height - logoHeight) / 2 ; // ���� �߾�
        mLogoImage.Draw(hdc, logoX, logoY, logoWidth, logoHeight);
    }

    // Time ������ (���� �ڵ� ����)
    Time::Render(hdc);
}