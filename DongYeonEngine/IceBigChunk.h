#include "CommonInclude.h"

class IceBigChunk
{
private:
	CImage mIceBigChunkImage0;
	CImage mIceBigChunkImage1;
	int mX;
	int mY;
	int mImageNum;
public:
	void Initialize()
	{
		mIceBigChunkImage0.Load(L"resources/MapObject/IceChunk0.png");
		mIceBigChunkImage1.Load(L"resources/MapObject/IceChunk1.png");
		mX = 0;
		mY = 0;
		mImageNum = 0;
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
			int width = mIceBigChunkImage0.GetWidth();
			int height = mIceBigChunkImage0.GetHeight();
			mIceBigChunkImage0.Draw(hdc, mX - width / 2, mY - height / 2, width, height);
		}
		else
		{
			int width = mIceBigChunkImage1.GetWidth();
			int height = mIceBigChunkImage1.GetHeight();
			mIceBigChunkImage1.Draw(hdc, mX - width / 2, mY - height / 2, width, height);
		}
	}
};