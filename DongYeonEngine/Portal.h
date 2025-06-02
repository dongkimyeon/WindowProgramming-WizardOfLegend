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
        UpdateRect(); // 사각형 초기화
    }
    ~Portal()
    {
        mPortalImage.Destroy();
    }
    void SetPosition(int x, int y)
    {
        mX = x;
        mY = y;
        UpdateRect(); // 위치 변경 시 사각형 업데이트
    }
    void SetScale(float scale)
    {
        mScale = scale;
        UpdateRect(); // 스케일 변경 시 사각형 업데이트
    }
    RECT GetRect() override 
    { 
		RECT rect = mPortalRect; // RECT 구조체를 복사하여 반환
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
    int mX; // 중심점 X
    int mY; // 중심점 Y
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