#pragma once
#include "GameObject.h"

#define TILE_SIZE 50

class Tile
{
private:
    CImage* tileImage;
    RECT range;

public:
    Tile();
    ~Tile();

    void Update();
    void Render(HDC hdc, RECT rect = {});
    RECT GetRect() const { return range; }
    
	void SetRange(RECT rect) { range = rect; }
    void SetPosition(float x, float y);
    void SetImage(CImage* image);
};