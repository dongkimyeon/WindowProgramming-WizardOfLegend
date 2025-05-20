#include "Tile.h"
#include <windows.h>

Tile::Tile() : tileImage(nullptr), tileType(TileType::Floor)
{
}

Tile::~Tile()
{
}

void Tile::Update()
{
}

void Tile::Render(HDC hdc, RECT rect)
{
    if (tileImage && !tileImage->IsNull()) {
        if (rect.left == 0 && rect.right == 0 && rect.top == 0 && rect.bottom == 0) {
            tileImage->Draw(hdc, range);
        }
        else {
            tileImage->Draw(hdc, rect);
        }
    }
    else {
        // Range Red Rectangle
        HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
        FillRect(hdc, rect.left == 0 ? &range : &rect, brush);
        DeleteObject(brush);
    }
}

void Tile::SetPosition(float x, float y)
{
    range.left = static_cast<LONG>(x - TILE_SIZE / 2.0f);
    range.top = static_cast<LONG>(y - TILE_SIZE / 2.0f);
    range.right = static_cast<LONG>(x + TILE_SIZE / 2.0f);
    range.bottom = static_cast<LONG>(y + TILE_SIZE / 2.0f);
}

void Tile::SetImage(CImage* image)
{
    tileImage = image;
}