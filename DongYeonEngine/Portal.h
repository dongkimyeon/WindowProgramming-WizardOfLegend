#pragma once
#include "GameObject.h"

class Portal : public GameObject
{
public:
    Portal()
    {
        mX = 0;
        mY = 0;
        mScale = 0.4f;
        mPortalImage.Load(L"resources/portal.png");
        UpdateRect(); // �簢�� �ʱ�ȭ
    }
    ~Portal()
    {
        mPortalImage.Destroy();
    }
    void SetPosition(int x, int y)
    {
        mX = x;
        mY = y;
        UpdateRect(); // ��ġ ���� �� �簢�� ������Ʈ
    }
    void SetScale(float scale)
    {
        mScale = scale;
        UpdateRect(); // ������ ���� �� �簢�� ������Ʈ
    }
    RECT GetRect() override 
    { 
		RECT rect = mPortalRect; // RECT ����ü�� �����Ͽ� ��ȯ
        return rect; 
    }
    void Render(HDC hdc) override
    {
        //Rectangle(hdc, mPortalRect.left, mPortalRect.top, mPortalRect.right, mPortalRect.bottom);
        int drawWidth = static_cast<int>(mPortalImage.GetWidth() * mScale);
        int drawHeight = static_cast<int>(mPortalImage.GetHeight() * mScale);
        int drawX = mX - drawWidth / 2;
        int drawY = mY - drawHeight / 2;
        mPortalImage.Draw(hdc, drawX, drawY, drawWidth, drawHeight);
    }
    void TakeDamage(int damage) override {  }

private:
    CImage mPortalImage;
    RECT mPortalRect;
    int mX; // �߽��� X
    int mY; // �߽��� Y
    float mScale; 

    void UpdateRect()
    {
        int scaledWidth = static_cast<int>(mPortalImage.GetWidth() * mScale);
        int scaledHeight = static_cast<int>(mPortalImage.GetHeight() * mScale);
        mPortalRect.left = mX - scaledWidth / 2;
        mPortalRect.top = mY - scaledHeight / 2;
        mPortalRect.right = mX + scaledWidth / 2;
        mPortalRect.bottom = mY + scaledHeight / 2;
    }
};