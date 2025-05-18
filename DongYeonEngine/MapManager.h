#pragma once
#include "CommonInclude.h"
#include "Tile.h"

class MapManager
{
public:
    static MapManager* GetInstance()
    {
        static MapManager instance;
        return &instance;
    }
    void Initialize();
    void Update();
    void LateUpdate();
    void Render(HDC hdc);
private:
    static const int MAP_WIDTH = 1000;
    static const int MAP_HEIGHT = 1000;
    static const int TILE_SIZE = 50; // 50x50 tiles
    Tile floorTile[MAP_WIDTH / TILE_SIZE * MAP_HEIGHT / TILE_SIZE];
    Tile wallTile[MAP_WIDTH / TILE_SIZE * 4]; // Perimeter walls
};