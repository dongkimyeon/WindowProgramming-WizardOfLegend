#include "MapManager.h"


void MapManager::Initialize()
{
    // Initialize floor tiles
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

    // Initialize wall tiles
    int wallIndex = 0;
    for (int x = 0; x < MAP_WIDTH; x += TILE_SIZE)
    {
        if (wallIndex < (MAP_WIDTH / TILE_SIZE * 4))
        {
            wallTile[wallIndex].SetPosition(x + TILE_SIZE / 2.0f, TILE_SIZE / 2.0f);
            wallTile[wallIndex + 1].SetPosition(x + TILE_SIZE / 2.0f, MAP_HEIGHT - TILE_SIZE / 2.0f);
            wallIndex += 2;
        }
    }

    for (int y = 0; y < MAP_HEIGHT; y += TILE_SIZE)
    {
        if (wallIndex < (MAP_WIDTH / TILE_SIZE * 4))
        {
            wallTile[wallIndex].SetPosition(TILE_SIZE / 2.0f, y + TILE_SIZE / 2.0f);
            wallTile[wallIndex + 1].SetPosition(MAP_WIDTH - TILE_SIZE / 2.0f, y + TILE_SIZE / 2.0f);
            wallIndex += 2;
        }
    }

    // Load tile images
    floorImage.Load(L"resources/Tile/Tile0.png");
    wallImage.Load(L"resources/Tile/Tile38.png");

    // Assign images to tiles
    for (int i = 0; i < (MAP_WIDTH / TILE_SIZE * MAP_HEIGHT / TILE_SIZE); i++) {
        floorTile[i].SetImage(&floorImage);
    }
    for (int i = 0; i < (MAP_WIDTH / TILE_SIZE * 4); i++) {
        wallTile[i].SetImage(&wallImage);
    }

    // Initialize buffer
    buffer.Create(MAP_WIDTH, MAP_HEIGHT, 24);
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
    HDC bufferDC = buffer.GetDC();

    // 버퍼에 타일 렌더링
    for (int i = 0; i < (MAP_WIDTH / TILE_SIZE * MAP_HEIGHT / TILE_SIZE); i++)
        floorTile[i].Render(bufferDC);
    for (int i = 0; i < (MAP_WIDTH / TILE_SIZE * 4); i++)
        wallTile[i].Render(bufferDC);

    // 고품질 스트레칭
    SetStretchBltMode(hdc, HALFTONE);
    buffer.StretchBlt(hdc, 0, 0, 1280, 720, 0, 0, MAP_WIDTH, MAP_HEIGHT, SRCCOPY);

    buffer.ReleaseDC();
}