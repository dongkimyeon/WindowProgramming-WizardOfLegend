#include "MapManager.h"
#include <windows.h>



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
    WCHAR debugStr[100];
    wsprintf(debugStr, L"Floor tiles initialized: %d\n", floorIndex);
    OutputDebugString(debugStr);

    // Initialize wall tiles
    int wallIndex = 0;
    for (int x = 0; x < MAP_WIDTH; x += TILE_SIZE)
    {
        if (wallIndex < (MAP_WIDTH / TILE_SIZE + MAP_HEIGHT / TILE_SIZE) * 2)
        {
            wallTile[wallIndex].SetPosition(x + TILE_SIZE / 2.0f, TILE_SIZE / 2.0f);
            wallTile[wallIndex + 1].SetPosition(x + TILE_SIZE / 2.0f, MAP_HEIGHT - TILE_SIZE / 2.0f);
            wallIndex += 2;
        }
    }
    for (int y = 0; y < MAP_HEIGHT; y += TILE_SIZE)
    {
        if (wallIndex < (MAP_WIDTH / TILE_SIZE + MAP_HEIGHT / TILE_SIZE) * 2)
        {
            wallTile[wallIndex].SetPosition(TILE_SIZE / 2.0f, y + TILE_SIZE / 2.0f);
            wallTile[wallIndex + 1].SetPosition(MAP_WIDTH - TILE_SIZE / 2.0f, y + TILE_SIZE / 2.0f);
            wallIndex += 2;
        }
    }
    wsprintf(debugStr, L"Wall tiles initialized: %d\n", wallIndex);
    OutputDebugString(debugStr);

    // Load tile images
    HRESULT hr = floorImage.Load(L"resources/Tile/Tile0.png");
    if (FAILED(hr)) {
        OutputDebugString(L"Failed to load Tile0.png\n");
    }
    else {
        OutputDebugString(L"Tile0.png loaded successfully\n");
    }
    hr = wallImage.Load(L"resources/Tile/Tile38.png");
    if (FAILED(hr)) {
        OutputDebugString(L"Failed to load Tile38.png\n");
    }
    else {
        OutputDebugString(L"Tile38.png loaded successfully\n");
    }

    // Assign images to tiles
    for (int i = 0; i < (MAP_WIDTH / TILE_SIZE * MAP_HEIGHT / TILE_SIZE); i++) {
        floorTile[i].SetImage(&floorImage);
    }
    for (int i = 0; i < (MAP_WIDTH / TILE_SIZE + MAP_HEIGHT / TILE_SIZE) * 2; i++) {
        wallTile[i].SetImage(&wallImage);
    }

    // Initialize 5000x5000 buffer
    buffer.Create(MAP_WIDTH, MAP_HEIGHT, 24);
    HDC bufferDC = buffer.GetDC();

    // Clear buffer with gray background
    RECT fullRect = { 0, 0, MAP_WIDTH, MAP_HEIGHT };
    HBRUSH bgBrush = CreateSolidBrush(RGB(100, 100, 100));
    FillRect(bufferDC, &fullRect, bgBrush);
    DeleteObject(bgBrush);

    // Render all tiles to buffer
    for (int i = 0; i < (MAP_WIDTH / TILE_SIZE * MAP_HEIGHT / TILE_SIZE); i++) {
        floorTile[i].Render(bufferDC);
    }
    for (int i = 0; i < (MAP_WIDTH / TILE_SIZE + MAP_HEIGHT / TILE_SIZE) * 2; i++) {
        wallTile[i].Render(bufferDC);
    }

    buffer.ReleaseDC();
    OutputDebugString(L"Map buffer initialized with full 5000x5000 map\n");
}

void MapManager::Update()
{
}

void MapManager::LateUpdate()
{
}

void MapManager::Render(HDC hdc, float cameraX, float cameraY)
{
    // Viewport: 1280x720
    int viewWidth = 1280;
    int viewHeight = 720;

    // Clamp camera position
    cameraX = max(0.0f, min(cameraX, static_cast<float>(MAP_WIDTH - viewWidth)));
    cameraY = max(0.0f, min(cameraY, static_cast<float>(MAP_HEIGHT - viewHeight)));

    // Stretch camera viewport to output HDC
    SetStretchBltMode(hdc, HALFTONE);
    buffer.StretchBlt(hdc, 0, 0, viewWidth, viewHeight,
        static_cast<int>(cameraX), static_cast<int>(cameraY),
        viewWidth, viewHeight, SRCCOPY);

    WCHAR debugStr[100];
    wsprintf(debugStr, L"Rendered viewport at camera (%d, %d)\n",
        static_cast<int>(cameraX), static_cast<int>(cameraY));
    OutputDebugString(debugStr);
}