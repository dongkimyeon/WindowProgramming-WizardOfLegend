#pragma once
#include "GameObject.h"

class IceWindow : public GameObject
{
private:
	CImage mIceWindow0;
	CImage mIceWindow1;
	CImage mIceWindow2;
	int mX;
	int mY;
	int mImageNum;
	float mScale; // Added scale factor
public:
	IceWindow()
	{
		mIceWindow0.Load(L"resources/MapObject/IceWindow0.png");
		mIceWindow1.Load(L"resources/MapObject/IceWindow1.png");
		mIceWindow2.Load(L"resources/MapObject/IceWindow2.png");
		mX = 0;
		mY = 0;
		mImageNum = 0;
		mScale = 2.0f; // Default scale
	}
	void TakeDamage(int d);
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
	void Render(HDC hdc)
	{
		if (mImageNum == 0)
		{
			int width = mIceWindow0.GetWidth();
			int height = mIceWindow0.GetHeight();
			int scaledWidth = static_cast<int>(width * mScale);
			int scaledHeight = static_cast<int>(height * mScale);
			mIceWindow0.Draw(hdc, mX - scaledWidth / 2, mY - scaledHeight / 2, scaledWidth, scaledHeight);
		}
		else if (mImageNum == 1)
		{
			int width = mIceWindow1.GetWidth();
			int height = mIceWindow1.GetHeight();
			int scaledWidth = static_cast<int>(width * mScale);
			int scaledHeight = static_cast<int>(height * mScale);
			mIceWindow1.Draw(hdc, mX - scaledWidth / 2, mY - scaledHeight / 2, scaledWidth, scaledHeight);
		}
		else if (mImageNum == 2)
		{
			int width = mIceWindow2.GetWidth();
			int height = mIceWindow2.GetHeight();
			int scaledWidth = static_cast<int>(width * mScale);
			int scaledHeight = static_cast<int>(height * mScale);
			mIceWindow2.Draw(hdc, mX - scaledWidth / 2, mY - scaledHeight / 2, scaledWidth, scaledHeight);
		}
	}
};