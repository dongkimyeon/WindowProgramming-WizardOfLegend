#define _CRT_SECURE_NO_WARNINGS
#include "MapManager.h"
#include <windows.h>

void MapManager::Initialize()
{
    // �� ������ �б�
    FILE* fp = fopen("StageTutorial.txt", "r");
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
                if (fscanf(fp, "%d", &value) == 1)
                {
                    if (value == 0 || value == 1 || value == 2)
                        map[i][j] = value;
                    else
                        map[i][j] = 0;
                }
                else
                    map[i][j] = 0;
            }
        }
        fclose(fp);
    }

    // �̹��� �ε�
    floorImage.Load(L"resources/Tile/Tile0.png");
    wallImage.Load(L"resources/Tile/Tile38.png");
    emptyImage.Load(L"resources/Tile/Tile121.png");

    // Ÿ�� �ʱ�ȭ
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLS; j++)
        {
            tiles[i][j] = new Tile();
            float x = (j * 50) + (50 / 2.0f);
            float y = (i * 50) + (50 / 2.0f);
            tiles[i][j]->SetPosition(x, y);

            switch (map[i][j])
            {
            case 0:
                tiles[i][j]->SetImage(&floorImage);
                tiles[i][j]->SetTileType(TileType::Floor);
                break;
            case 1:
                tiles[i][j]->SetImage(&wallImage);
                tiles[i][j]->SetTileType(TileType::Wall);
                break;
            case 2:
                tiles[i][j]->SetImage(&emptyImage);
                tiles[i][j]->SetTileType(TileType::Floor);
                break;
            default:
                tiles[i][j]->SetImage(&floorImage);
                tiles[i][j]->SetTileType(TileType::Floor);
                break;
            }
        }
    }

    // ���� �ʱ�ȭ (2000x2000)
    buffer.Create(MAP_WIDTH, MAP_HEIGHT, 24);
    HDC bufferDC = buffer.GetDC();

    // ����� ȸ������ ä���
    RECT fullRect = { 0, 0, MAP_WIDTH, MAP_HEIGHT };
    HBRUSH bgBrush = CreateSolidBrush(RGB(100, 100, 100));
    FillRect(bufferDC, &fullRect, bgBrush);
    DeleteObject(bgBrush);

    // ��� Ÿ���� ���ۿ� ������
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
    return map; // map �迭 ��ȯ
}

void MapManager::LoadMap(const std::wstring& name)
{
    // �� ������ �б�
	FILE* fp = _wfopen(name.c_str(), L"r");

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
                if (fscanf(fp, "%d", &value) == 1)
                {
                    if (value == 0 || value == 1 || value == 2)
                        map[i][j] = value;
                    else
                        map[i][j] = 0;
                }
                else
                    map[i][j] = 0;
            }
        }
        fclose(fp);
    }

	
	// Ÿ�� �ʱ�ȭ

	for (int i = 0; i < MAP_ROWS; i++)
	{
		for (int j = 0; j < MAP_COLS; j++)
		{
			switch (map[i][j])
			{
			case 0:
				tiles[i][j]->SetImage(&floorImage);
				tiles[i][j]->SetTileType(TileType::Floor);
				break;
			case 1:
				tiles[i][j]->SetImage(&wallImage);
				tiles[i][j]->SetTileType(TileType::Wall);
				break;
			case 2:
				tiles[i][j]->SetImage(&emptyImage);
				tiles[i][j]->SetTileType(TileType::Floor);
				break;
			default:
				tiles[i][j]->SetImage(&floorImage);
				tiles[i][j]->SetTileType(TileType::Floor);
				break;
			}
		}
	}

	// ���� �ʱ�ȭ (2000x2000)
    buffer.Destroy();
	buffer.Create(MAP_WIDTH, MAP_HEIGHT, 24);
	HDC bufferDC = buffer.GetDC();
	
	// ��� Ÿ���� ���ۿ� ������
	for (int i = 0; i < MAP_ROWS; i++)
	{
		for (int j = 0; j < MAP_COLS; j++)
		{
			tiles[i][j]->Render(bufferDC);
		}
	}
	buffer.ReleaseDC();
}