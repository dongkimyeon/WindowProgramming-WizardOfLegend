#include "Tile.h"
#include <windows.h>

Tile::Tile() : tileImage(nullptr)
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
            OutputDebugString(L"Tile rendered with default rect\n");
        }
        else {
            tileImage->Draw(hdc, rect);
            WCHAR debugStr[100];
            wsprintf(debugStr, L"Tile rendered at (%d, %d, %d, %d)\n",
                rect.left, rect.top, rect.right, rect.bottom);
            OutputDebugString(debugStr);
        }
    }
    else {
        // Fallback: Draw red rectangle
        HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
        FillRect(hdc, rect.left == 0 ? &range : &rect, brush);
        DeleteObject(brush);
        OutputDebugString(L"Tile image is null, drew red rectangle\n");
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
    if (image && !image->IsNull()) {
        OutputDebugString(L"Tile image set successfully\n");
    }
    else {
        OutputDebugString(L"Tile image set failed\n");
    }
}