#pragma once
#include "GameObject.h"
class IceSmallChunk : public GameObject
{
private:
    CImage mIceSmallChunkImage;
    int mX;
    int mY;
    float mScale; // Added scale factor
public:
    IceSmallChunk()
    {
        mIceSmallChunkImage.Load(L"resources/MapObject/IceSmallChunk.png");
        mX = 0;
        mY = 0;
        mScale = 3.0f; // Default scale is 1.0 (original size)
    }

    void TakeDamage(int d);

    void SetPosition(int x, int y)
    {
        mX = x;
        mY = y;
    }

    void SetScale(float scale)
    {
        mScale = scale;
    }

    void Render(HDC hdc)
    {
        int width = mIceSmallChunkImage.GetWidth();
        int height = mIceSmallChunkImage.GetHeight();
        // Apply scale to width and height
        int scaledWidth = static_cast<int>(width * mScale);
        int scaledHeight = static_cast<int>(height * mScale);
        // Adjust position to keep the image centered
        mIceSmallChunkImage.Draw(hdc, mX - scaledWidth / 2, mY - scaledHeight / 2, scaledWidth, scaledHeight);
    }
};