#include "CommonInclude.h"
class IceWindow
{
private:
	CImage mIceWindow0;
	CImage mIceWindow1;
	CImage mIceWindow2;
	int mX;
	int mY;
	int mImageNum;
public:

	void Initialize()
	{
		mIceWindow0.Load(L"resources/MapObject/IceWindow0.png");
		mIceWindow1.Load(L"resources/MapObject/IceWindow1.png");
		mIceWindow2.Load(L"resources/MapObject/IceWindow2.png");
		mX = 0;
		mY = 0;
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
	void Render(HDC hdc)
	{
		if (mImageNum == 0)
		{
			int width = mIceWindow0.GetWidth();
			int height = mIceWindow0.GetHeight();
			mIceWindow0.Draw(hdc, mX - width / 2, mY - height / 2, width, height);
		}
		else if (mImageNum == 1)
		{
			int width = mIceWindow1.GetWidth();
			int height = mIceWindow1.GetHeight();
			mIceWindow1.Draw(hdc, mX - width / 2, mY - height / 2, width, height);
		}
		else if (mImageNum == 2)
		{
			int width = mIceWindow2.GetWidth();
			int height = mIceWindow2.GetHeight();
			mIceWindow2.Draw(hdc, mX - width / 2, mY - height / 2, width, height);
		}
	}
};