#include "CommonInclude.h"

class IceSmallChunk
{
private:
	CImage mIceSmallChunkImage;
	int mX;
	int mY;
public:
	void Initialize()
	{
		mIceSmallChunkImage.Load(L"resources/MapObject/IceSmallChunk.png");
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
		int width = mIceSmallChunkImage.GetWidth();
		int height = mIceSmallChunkImage.GetHeight();
		mIceSmallChunkImage.Draw(hdc, mX - width / 2, mY - height / 2, width, height);
	}
};