#pragma once
#include "CommonInclude.h"
#include "Tile.h"
#include "Archer.h"
#include "SwordMan.h"
#include "Wizard.h"
#include "MapObjectInclude.h"



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
    std::string ImageMap[MAP_ROWS][MAP_COLS];
    Tile* tiles[MAP_ROWS][MAP_COLS];
    CImage floorImage[4];
    CImage floorImageSet[25];
    CImage wallImage[4]; // Array for wall tile variants
    CImage wallCornerImage[10]; // Array for wall corner tile variants
    CImage emptyImage;
    CImage buffer;
};