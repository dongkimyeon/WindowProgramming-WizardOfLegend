#pragma once
#include "GameObject.h"
class IceFlag : public GameObject
{
private:
	CImage mIceFlag;
	int mX;
	int mY;
	float mScale;
public:
	IceFlag()
	{
		mIceFlag.Load(L"resources/MapObject/IceFlag.png");
		mX = 0;
		mY = 0;
		mScale = 2.0f;
	}
	void SetPosition(int x, int y)
	{
		mX = x;
		mY = y;

	}
	void TakeDamage(int d);

	void Render(HDC hdc)
	{
		int width = mIceFlag.GetWidth();
		int height = mIceFlag.GetHeight();
		// Apply scale to width and height
		int scaledWidth = static_cast<int>(width * mScale);
		int scaledHeight = static_cast<int>(height * mScale);
		// Adjust position to keep the image centered
		mIceFlag.Draw(hdc, mX - scaledWidth / 2, mY - scaledHeight / 2, scaledWidth, scaledHeight);
	}
};