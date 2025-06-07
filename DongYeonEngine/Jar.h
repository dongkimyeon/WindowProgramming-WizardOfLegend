#pragma once
#include "GameObject.h"

class Jar : public GameObject
{
private:
	CImage mJarImage;
	int mX;
	int mY;
	float mScale; // Added scale factor
public:
	Jar()
	{
		mJarImage.Load(L"resources/MapObject/Jar.png");
		mX = 0;
		mY = 0;
		mScale = 2.5f; // Default scale
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
		int width = mJarImage.GetWidth();
		int height = mJarImage.GetHeight();
		int scaledWidth = static_cast<int>(width * mScale);
		int scaledHeight = static_cast<int>(height * mScale);
		mJarImage.Draw(hdc, mX - scaledWidth / 2, mY - scaledHeight / 2, scaledWidth, scaledHeight);
	}
};