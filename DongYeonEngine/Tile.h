#pragma once
#include "GameObject.h"

#define TILE_SIZE 50

enum class TileType
{
    Floor,  // 바닥 타일 (통과 가능)
    Wall,   // 벽 타일 (통과 불가)
    Other   // 기타 (필요 시 확장)
};

class Tile
{
private:
    CImage* tileImage;
    RECT range;
    TileType tileType; // 타일 종류 추가

public:
    Tile();
    ~Tile();

    void Update();
    void Render(HDC hdc, RECT rect = {});

    RECT GetRect() const { return range; }
    void SetRange(RECT rect) { range = rect; }
    void SetPosition(float x, float y);
    void SetImage(CImage* image);

    // 추가 메서드
    void SetTileType(TileType type) { tileType = type; }
    TileType GetTileType() const { return tileType; }
    bool IsWall() const { return tileType == TileType::Wall; }
};