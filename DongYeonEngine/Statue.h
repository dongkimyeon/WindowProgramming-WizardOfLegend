#pragma once
#include "GameObject.h"

class Statue : public GameObject
{
private:
	CImage mStatueImage;
	RECT mRect;
	int mX;
	int mY;
public:
	Statue()
	{
		mStatueImage.Load(L"resources/MapObject/Statue.png");
		mX = 0;
		mY = 0;

	}
	void TakeDamage(int d);
	void SetPosition(int x, int y)
	{
		mX = x;
		mY = y;

		int width = mStatueImage.GetWidth() + 50;
		int height = mStatueImage.GetHeight() + 50;

		
		mRect.left = mX - width / 2 + 5;
		mRect.top = mY - height / 2 + 60;
		mRect.right = mX + width / 2 - 17;
		mRect.bottom = mY + height / 2 - 35;
	}
	RECT GetRect() { return mRect; }
	void Render(HDC hdc)
	{
		
		int width = mStatueImage.GetWidth() + 50;
		int height = mStatueImage.GetHeight() + 50;
		mStatueImage.Draw(hdc, mX - width / 2, mY - height / 2, width, height);
		//HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH); // 속이 빈 브러시 선택
		//HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush); // 기존 브러시 저장
		//Rectangle(hdc, mRect.left, mRect.top, mRect.right, mRect.bottom); // 테두리만 그리기
		//SelectObject(hdc, hOldBrush); // 원래 브러시 복원
	}
};