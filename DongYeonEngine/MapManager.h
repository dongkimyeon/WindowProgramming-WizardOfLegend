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
    void Render(HDC hdc, float cameraX, float cameraY);
private:
    static const int MAP_WIDTH = 5000;
    static const int MAP_HEIGHT = 5000;
    static Tile floorTile[MAP_WIDTH / TILE_SIZE * MAP_HEIGHT / TILE_SIZE];
    static Tile wallTile[(MAP_WIDTH / TILE_SIZE + MAP_HEIGHT / TILE_SIZE) * 2];
    CImage floorImage;
    CImage wallImage;
    CImage buffer;
};