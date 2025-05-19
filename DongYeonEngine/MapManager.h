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
	Tile** GetTiles() { return &tiles[0][0]; } // 2D 배열의 첫 번째 요소 주소 반환
	

private:
    static const int MAP_WIDTH = 2000;
    static const int MAP_HEIGHT = 2000;
    static const int MAP_ROWS = 40;
    static const int MAP_COLS = 40;
    int map[MAP_ROWS][MAP_COLS];
    Tile* tiles[MAP_ROWS][MAP_COLS]; // 2D 배열로 타일 포인터 관리
    CImage floorImage;
    CImage wallImage;
    CImage emptyImage;
    CImage buffer;
};