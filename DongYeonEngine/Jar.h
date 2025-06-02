#include "CommonInclude.h"
class Jar
{
private:
	CImage mJarImage;
	int mX;
	int mY;
public:
	void Initialize()
	{
		mJarImage.Load(L"resources/MapObject/Jar.png");
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
		int width = mJarImage.GetWidth();
		int height = mJarImage.GetHeight();
		mJarImage.Draw(hdc, mX - width / 2, mY - height / 2, width, height);
	}
};