#pragma once
#include "GameObject.h"

#define TILE_SIZE 50

class Tile
{
private:
    CImage* tileImage; // �����ͷ� ����
    RECT range;

public:
    Tile();
    ~Tile();

    void Update();
    void Render(HDC hdc);

    void SetPosition(float x, float y);
    void SetImage(CImage* image); // ��10 ������ �Ű����� ����
};