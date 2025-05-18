#pragma once
#include "GameObject.h"

#define TILE_SIZE 50

class Tile
{
private:
    CImage* tileImage; // 포인터로 변경
    RECT range;

public:
    Tile();
    ~Tile();

    void Update();
    void Render(HDC hdc);

    void SetPosition(float x, float y);
    void SetImage(CImage* image); // 포10 포인터 매개변수 수정
};