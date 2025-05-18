#include "MapManager.h"

void MapManager::Initialize()
{
    
    int floorIndex = 0;
    for (int y = 0; y < MAP_HEIGHT; y += TILE_SIZE)
    {
        for (int x = 0; x < MAP_WIDTH; x += TILE_SIZE)
        {
            if (floorIndex < (MAP_WIDTH / TILE_SIZE * MAP_HEIGHT / TILE_SIZE))
            {
                floorTile[floorIndex].SetPosition(x + TILE_SIZE / 2.0f, y + TILE_SIZE / 2.0f);
                floorIndex++;
            }
        }
    }

    
    int wallIndex = 0;
    
    for (int x = 0; x < MAP_WIDTH; x += TILE_SIZE)
    {
        if (wallIndex < (MAP_WIDTH / TILE_SIZE * 4))
        {
            wallTile[wallIndex].SetPosition(x + TILE_SIZE / 2.0f, TILE_SIZE / 2.0f); // Top
            wallTile[wallIndex + 1].SetPosition(x + TILE_SIZE / 2.0f, MAP_HEIGHT - TILE_SIZE / 2.0f); // Bottom
            wallIndex += 2;
        }
    }
    
    for (int y = 0; y < MAP_HEIGHT; y += TILE_SIZE)
    {
        if (wallIndex < (MAP_WIDTH / TILE_SIZE * 4))
        {
            wallTile[wallIndex].SetPosition(TILE_SIZE / 2.0f, y + TILE_SIZE / 2.0f); // Left
            wallTile[wallIndex + 1].SetPosition(MAP_WIDTH - TILE_SIZE / 2.0f, y + TILE_SIZE / 2.0f); // Right
            wallIndex += 2;
        }
    }
}

void MapManager::Update()
{
    for (int i = 0; i < (MAP_WIDTH / TILE_SIZE * MAP_HEIGHT / TILE_SIZE); i++)
        floorTile[i].Update();
    for (int i = 0; i < (MAP_WIDTH / TILE_SIZE * 4); i++)
        wallTile[i].Update();
}

void MapManager::LateUpdate()
{
}

void MapManager::Render(HDC hdc)
{
    for (int i = 0; i < (MAP_WIDTH / TILE_SIZE * MAP_HEIGHT / TILE_SIZE); i++)
        floorTile[i].Render(hdc);
    for (int i = 0; i < (MAP_WIDTH / TILE_SIZE * 4); i++)
        wallTile[i].Render(hdc);
}