#include "CommonInclude.h"

class IceFlag
{
private:
	CImage mIceFlag;
	int mX;
	int mY;
public:
	void Initialize()
	{
		mIceFlag.Load(L"resources/MapObject/IceFlag.png");
		mX = 0;
		mY = 0;
	}
	void SetPosition(int x, int y)
	{
		mX = x;
		mY = y;

	}
	void Render(HDC hdc)
	{
		int width = mIceFlag.GetWidth();
		int height = mIceFlag.GetHeight();
		mIceFlag.Draw(hdc, mX - width / 2, mY - height / 2, width, height);
	}
};