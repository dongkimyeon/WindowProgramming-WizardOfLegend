#pragma once
#include "CommonInclude.h"
#include "Tile.h"

class MapManager : public std::enable_shared_from_this<MapManager>
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
    Tile* (*GetTiles())[40];
    int (*GetMap())[40]; // map 배열 반환 추가

	void LoadMap(const std::wstring& name);

private:
    static const int MAP_WIDTH = 2000;
    static const int MAP_HEIGHT = 2000;
    static const int MAP_ROWS = 40;
    static const int MAP_COLS = 40;
    int map[MAP_ROWS][MAP_COLS];
    Tile* tiles[MAP_ROWS][MAP_COLS];
    CImage floorImage;
    CImage wallImage;
    CImage emptyImage;
    CImage buffer;
};