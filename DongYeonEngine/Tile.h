#pragma once
#include "GameObject.h"

#define TILE_SIZE 50

enum class TileType
{
    Floor,  // �ٴ� Ÿ�� (��� ����)
    Wall,   // �� Ÿ�� (��� �Ұ�)
    Other   // ��Ÿ (�ʿ� �� Ȯ��)
};

class Tile
{
private:
    CImage* tileImage;
    RECT range;
    TileType tileType; // Ÿ�� ���� �߰�

public:
    Tile();
    ~Tile();

    void Update();
    void Render(HDC hdc, RECT rect = {});

    RECT GetRect() const { return range; }
    void SetRange(RECT rect) { range = rect; }
    void SetPosition(float x, float y);
    void SetImage(CImage* image);

    // �߰� �޼���
    void SetTileType(TileType type) { tileType = type; }
    TileType GetTileType() const { return tileType; }
    bool IsWall() const { return tileType == TileType::Wall; }
};