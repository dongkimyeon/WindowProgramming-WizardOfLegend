#pragma once
#include "GameObject.h"

class IceBigChunk : public GameObject
{
private:
	CImage mIceBigChunkImage0;
	CImage mIceBigChunkImage1;
	int mX;
	int mY;
	int mImageNum;
	float mScale; // Added scale factor
public:
	IceBigChunk()
	{
		mIceBigChunkImage0.Load(L"resources/MapObject/IceChunk0.png");
		mIceBigChunkImage1.Load(L"resources/MapObject/IceChunk1.png");
		mX = 0;
		mY = 0;
		mImageNum = 0;
		mScale = 1.7f; // Default scale
	}
	void SetPosition(int x, int y)
	{
		mX = x;
		mY = y;
	}
	void SetImageNum(int a)
	{
		mImageNum = a;
	}
	void SetScale(float scale)
	{
		mScale = scale;
	}
	void TakeDamage(int d);
	void Render(HDC hdc)
	{
		if (mImageNum == 0)
		{
			int width = mIceBigChunkImage0.GetWidth();
			int height = mIceBigChunkImage0.GetHeight();
			int scaledWidth = static_cast<int>(width * mScale);
			int scaledHeight = static_cast<int>(height * mScale);
			mIceBigChunkImage0.Draw(hdc, mX - scaledWidth / 2, mY - scaledHeight / 2, scaledWidth, scaledHeight);
		}
		else
		{
			int width = mIceBigChunkImage1.GetWidth();
			int height = mIceBigChunkImage1.GetHeight();
			int scaledWidth = static_cast<int>(width * mScale);
			int scaledHeight = static_cast<int>(height * mScale);
			mIceBigChunkImage1.Draw(hdc, mX - scaledWidth / 2, mY - scaledHeight / 2, scaledWidth, scaledHeight);
		}
	}
};