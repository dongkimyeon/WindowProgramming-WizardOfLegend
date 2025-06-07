#pragma once
#include "GameObject.h"

class Candle : public GameObject
{
private:
    CImage mCandleImage;
    int mX;
    int mY;
    float mScale; // 스케일 값 추가

public:

    Candle()
    {
        mCandleImage.Load(L"resources/MapObject/Candle.png");
        mX = 0;
        mY = 0;
        mScale = 0.5f; // 기본 스케일 1.0 (원본 크기)
    }

    void SetPosition(int x, int y)
    {
        mX = x;
        mY = y;
    }

    void SetScale(float scale)
    {
        mScale = scale; // 스케일 값 설정
    }

    void TakeDamage(int d);
   
    void Render(HDC hdc)
    {
        int width = mCandleImage.GetWidth();
        int height = mCandleImage.GetHeight();

        // 스케일 적용된 크기 계산
        int scaledWidth = static_cast<int>(width * mScale);
        int scaledHeight = static_cast<int>(height * mScale);

        // 촛불 이미지 그리기 (스케일 적용)
        mCandleImage.Draw(hdc, mX - scaledWidth / 2, mY - scaledHeight / 2, scaledWidth, scaledHeight);

        // GDI+로 반투명한 원 그리기 (스케일 적용)
        Gdiplus::Graphics graphics(hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias); 
        Gdiplus::Color fillColor(80, 255, 127, 0);
        Gdiplus::SolidBrush brush(fillColor);
        int circleSize = static_cast<int>(70 * mScale); // 원 크기도 스케일에 비례

        graphics.FillEllipse(&brush, mX - circleSize / 2, mY - circleSize / 2 - 32, circleSize, circleSize);
        graphics.FillEllipse(&brush, mX - circleSize / 2 - 12, mY - circleSize / 2 - 27, circleSize, circleSize);
        graphics.FillEllipse(&brush, mX - circleSize / 2 + 12, mY - circleSize / 2 - 27, circleSize, circleSize);
    }
};