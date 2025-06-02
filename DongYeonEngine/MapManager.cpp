#define _CRT_SECURE_NO_WARNINGS
#include "MapManager.h"
#include <windows.h>

void MapManager::Initialize()
{

    // 이미지 로드
    for (int i = 0; i < 4; i++)
    {
        wchar_t path[64];
        swprintf(path, 64, L"resources/Tile/floorTile%d.png", i + 1);
        if (floorImage[i].Load(path) != S_OK)
        {
            MessageBox(NULL, path, L"Failed to load floor tile image", MB_OK | MB_ICONERROR);
        }
    }
    for (int i = 0; i < 4; i++)
    {
        wchar_t path[64];
        swprintf(path, 64, L"resources/Tile/wallTile%d.png", i + 1);
        if (wallImage[i].Load(path) != S_OK)
        {
            MessageBox(NULL, path, L"Failed to load wall tile image", MB_OK | MB_ICONERROR);
        }
    }
    for (int i = 0; i < 10; i++)
    {
        wchar_t path[64];
        swprintf(path, 64, L"resources/Tile/WallCornerTile%d.png", i + 1);
        if (wallCornerImage[i].Load(path) != S_OK)
        {
            MessageBox(NULL, path, L"Failed to load wall corner tile image", MB_OK | MB_ICONERROR);
        }
    }
    if (emptyImage.Load(L"resources/Tile/emptyTile.png") != S_OK)
    {
        MessageBox(NULL, L"resources/Tile/emptyTile.png", L"Failed to load empty tile image", MB_OK | MB_ICONERROR);
    }

    // 타일 초기화
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLS; j++)
        {
            tiles[i][j] = new Tile();
            float x = (j * 50) + (50 / 2.0f);
            float y = (i * 50) + (50 / 2.0f);
            tiles[i][j]->SetPosition(x, y);

            std::string tileType = ImageMap[i][j];
            if (tileType == "f1")
            {
                tiles[i][j]->SetImage(&floorImage[0]);
                tiles[i][j]->SetTileType(TileType::Floor);
                map[i][j] = 0; // Floor
            }
            else if (tileType == "f2")
            {
                tiles[i][j]->SetImage(&floorImage[1]);
                tiles[i][j]->SetTileType(TileType::Floor);
                map[i][j] = 0; // Floor
            }
            else if (tileType == "f3")
            {
                tiles[i][j]->SetImage(&floorImage[2]);
                tiles[i][j]->SetTileType(TileType::Floor);
                map[i][j] = 0; // Floor
            }
            else if (tileType == "f4")
            {
                tiles[i][j]->SetImage(&floorImage[3]);
                tiles[i][j]->SetTileType(TileType::Floor);
                map[i][j] = 0; // Floor
            }
            else if (tileType == "w1")
            {
                tiles[i][j]->SetImage(&wallImage[0]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "w2")
            {
                tiles[i][j]->SetImage(&wallImage[1]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "w3")
            {
                tiles[i][j]->SetImage(&wallImage[2]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "w4")
            {
                tiles[i][j]->SetImage(&wallImage[3]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc1")
            {
                tiles[i][j]->SetImage(&wallCornerImage[0]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc2")
            {
                tiles[i][j]->SetImage(&wallCornerImage[1]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc3")
            {
                tiles[i][j]->SetImage(&wallCornerImage[2]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc4")
            {
                tiles[i][j]->SetImage(&wallCornerImage[3]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc5")
            {
                tiles[i][j]->SetImage(&wallCornerImage[4]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc6")
            {
                tiles[i][j]->SetImage(&wallCornerImage[5]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc7")
            {
                tiles[i][j]->SetImage(&wallCornerImage[6]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc8")
            {
                tiles[i][j]->SetImage(&wallCornerImage[7]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc9")
            {
                tiles[i][j]->SetImage(&wallCornerImage[8]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc10")
            {
                tiles[i][j]->SetImage(&wallCornerImage[9]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "e")
            {
                tiles[i][j]->SetImage(&emptyImage);
                tiles[i][j]->SetTileType(TileType::Floor);
                map[i][j] = 2; // Empty
            }
            else
            {
                tiles[i][j]->SetImage(&floorImage[0]);
                tiles[i][j]->SetTileType(TileType::Floor);
                map[i][j] = 0; // Default to floor
            }
        }
    }

    // 버퍼 초기화 (2000x2000)
    buffer.Create(MAP_WIDTH, MAP_HEIGHT, 24);
    HDC bufferDC = buffer.GetDC();

    // 배경을 회색으로 채우기
    RECT fullRect = { 0, 0, MAP_WIDTH, MAP_HEIGHT };
    HBRUSH bgBrush = CreateSolidBrush(RGB(100, 100, 100));
    FillRect(bufferDC, &fullRect, bgBrush);
    DeleteObject(bgBrush);

    // 모든 타일을 버퍼에 렌더링
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLS; j++)
        {
            tiles[i][j]->Render(bufferDC);
        }
    }

    buffer.ReleaseDC();
}

void MapManager::Update()
{
}

void MapManager::LateUpdate()
{
}

void MapManager::Render(HDC hdc, float cameraX, float cameraY)
{
    int viewWidth = 1280;
    int viewHeight = 720;

    cameraX = max(0.0f, min(cameraX, static_cast<float>(MAP_WIDTH - viewWidth)));
    cameraY = max(0.0f, min(cameraY, static_cast<float>(MAP_HEIGHT - viewHeight)));

    SetStretchBltMode(hdc, HALFTONE);
    buffer.StretchBlt(hdc, 0, 0, viewWidth, viewHeight,
        static_cast<int>(cameraX), static_cast<int>(cameraY),
        viewWidth, viewHeight, SRCCOPY);
}

Tile* (*MapManager::GetTiles())[40]
{
    return tiles;
}

int (*MapManager::GetMap())[40]
{
    return map; // map 배열 반환
}

void MapManager::LoadMap(const std::wstring& name)
{
    // 맵 데이터 읽기
    FILE* fp = _wfopen(name.c_str(), L"r");
    std::wstring imageFileName = name.substr(0, name.find(L".")) + L"Image.txt";
    FILE* imageFp = _wfopen(imageFileName.c_str(), L"r");

    if (fp == nullptr)
    {
        for (int i = 0; i < MAP_ROWS; i++)
            for (int j = 0; j < MAP_COLS; j++)
                map[i][j] = 0;
    }
    else
    {
        for (int i = 0; i < MAP_ROWS; i++)
        {
            for (int j = 0; j < MAP_COLS; j++)
            {
                int value = 0;
                char tile[10] = { 0 };
                if (fscanf(fp, "%d", &value) == 1)
                {
                    if (value == 0 || value == 1 || value == 2)
                        map[i][j] = value;
                    else
                        map[i][j] = 0;
                }
                else
                    map[i][j] = 0;

                if (imageFp && fscanf(imageFp, "%s", tile) == 1)
                {
                    ImageMap[i][j] = std::string(tile);
                }
                else
                {
                    ImageMap[i][j] = "f1"; // Default to floor tile
                }
            }
        }
        if (fp) fclose(fp);
        if (imageFp) fclose(imageFp);
    }

    // 타일 초기화
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLS; j++)
        {
            std::string tileType = ImageMap[i][j];
            if (tileType == "f1")
            {
                tiles[i][j]->SetImage(&floorImage[0]);
                tiles[i][j]->SetTileType(TileType::Floor);
                map[i][j] = 0; // Floor
            }
            else if (tileType == "f2")
            {
                tiles[i][j]->SetImage(&floorImage[1]);
                tiles[i][j]->SetTileType(TileType::Floor);
                map[i][j] = 0; // Floor
            }
            else if (tileType == "f3")
            {
                tiles[i][j]->SetImage(&floorImage[2]);
                tiles[i][j]->SetTileType(TileType::Floor);
                map[i][j] = 0; // Floor
            }
            else if (tileType == "f4")
            {
                tiles[i][j]->SetImage(&floorImage[3]);
                tiles[i][j]->SetTileType(TileType::Floor);
                map[i][j] = 0; // Floor
            }
            else if (tileType == "w1")
            {
                tiles[i][j]->SetImage(&wallImage[0]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "w2")
            {
                tiles[i][j]->SetImage(&wallImage[1]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "w3")
            {
                tiles[i][j]->SetImage(&wallImage[2]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "w4")
            {
                tiles[i][j]->SetImage(&wallImage[3]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc1")
            {
                tiles[i][j]->SetImage(&wallCornerImage[0]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc2")
            {
                tiles[i][j]->SetImage(&wallCornerImage[1]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc3")
            {
                tiles[i][j]->SetImage(&wallCornerImage[2]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc4")
            {
                tiles[i][j]->SetImage(&wallCornerImage[3]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc5")
            {
                tiles[i][j]->SetImage(&wallCornerImage[4]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc6")
            {
                tiles[i][j]->SetImage(&wallCornerImage[5]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc7")
            {
                tiles[i][j]->SetImage(&wallCornerImage[6]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc8")
            {
                tiles[i][j]->SetImage(&wallCornerImage[7]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc9")
            {
                tiles[i][j]->SetImage(&wallCornerImage[8]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "wc10")
            {
                tiles[i][j]->SetImage(&wallCornerImage[9]);
                tiles[i][j]->SetTileType(TileType::Wall);
                map[i][j] = 1; // Wall
            }
            else if (tileType == "e")
            {
                tiles[i][j]->SetImage(&emptyImage);
                tiles[i][j]->SetTileType(TileType::Floor);
                map[i][j] = 2; // Empty
            }
            else
            {
                tiles[i][j]->SetImage(&floorImage[0]);
                tiles[i][j]->SetTileType(TileType::Floor);
                map[i][j] = 0; // Default to floor
            }
        }
    }

    // 버퍼 초기화 (2000x2000)
    buffer.Destroy();
    buffer.Create(MAP_WIDTH, MAP_HEIGHT, 24);
    HDC bufferDC = buffer.GetDC();

    // 모든 타일을 버퍼에 렌더링
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLS; j++)
        {
            tiles[i][j]->Render(bufferDC);
        }
    }
    buffer.ReleaseDC();
}