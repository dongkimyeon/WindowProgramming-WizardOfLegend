#pragma once
#include "GameObject.h"

class Candle : public GameObject
{
private:
    CImage mCandleImage;
    int mX;
    int mY;
    float mScale; // ������ �� �߰�

public:

    Candle()
    {
        mCandleImage.Load(L"resources/MapObject/Candle.png");
        mX = 0;
        mY = 0;
        mScale = 0.5f; // �⺻ ������ 1.0 (���� ũ��)
    }

    void SetPosition(int x, int y)
    {
        mX = x;
        mY = y;
    }

    void SetScale(float scale)
    {
        mScale = scale; // ������ �� ����
    }

    void TakeDamage(int d);
   
    void Render(HDC hdc)
    {
        int width = mCandleImage.GetWidth();
        int height = mCandleImage.GetHeight();

        // ������ ����� ũ�� ���
        int scaledWidth = static_cast<int>(width * mScale);
        int scaledHeight = static_cast<int>(height * mScale);

        // �к� �̹��� �׸��� (������ ����)
        mCandleImage.Draw(hdc, mX - scaledWidth / 2, mY - scaledHeight / 2, scaledWidth, scaledHeight);

        // GDI+�� �������� �� �׸��� (������ ����)
        Gdiplus::Graphics graphics(hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias); 
        Gdiplus::Color fillColor(80, 255, 127, 0);
        Gdiplus::SolidBrush brush(fillColor);
        int circleSize = static_cast<int>(70 * mScale); // �� ũ�⵵ �����Ͽ� ���

        graphics.FillEllipse(&brush, mX - circleSize / 2, mY - circleSize / 2 - 32, circleSize, circleSize);
        graphics.FillEllipse(&brush, mX - circleSize / 2 - 12, mY - circleSize / 2 - 27, circleSize, circleSize);
        graphics.FillEllipse(&brush, mX - circleSize / 2 + 12, mY - circleSize / 2 - 27, circleSize, circleSize);
    }
};