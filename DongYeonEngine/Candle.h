#include "CommonInclude.h"

class Candle
{
private:
	CImage mCandleImage;
	
	int mX;
	int mY;

public:
	void Initialize()
	{
		mCandleImage.Load(L"resources/MapObject/Candle.png");

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
		
			int width = mCandleImage.GetWidth();
			int height = mCandleImage.GetHeight();
			mCandleImage.Draw(hdc, mX - width / 2, mY - height / 2, width, height);
		
	
	}
};