#include "Tile.h"

Tile::Tile() : tileImage(nullptr)
{
}

Tile::~Tile()
{
}

void Tile::Update()
{
}

void Tile::Render(HDC hdc)
{
    if (tileImage && !tileImage->IsNull()) {
        tileImage->Draw(hdc, range);
    }
}

void Tile::SetPosition(float x, float y)
{
    range.left = x - TILE_SIZE / 2.0f;
    range.top = y - TILE_SIZE / 2.0f;
    range.right = x + TILE_SIZE / 2.0f;
    range.bottom = y + TILE_SIZE / 2.0f;
}

void Tile::SetImage(CImage* image)
{
    tileImage = image; // 포인터 저장
}