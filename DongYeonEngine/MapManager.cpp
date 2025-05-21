#define _CRT_SECURE_NO_WARNINGS
#include "MapManager.h"
#include <windows.h>


void MapManager::Initialize()
{
    // �� ������ �б�
    FILE* fp = fopen("map.txt", "r"); // �б� ���� ����
    if (fp == nullptr)
    {
        // ���� ���� ���� �� �⺻ �� ���� (��: ��� �ٴ�)
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
                        map[i][j] = 0; // �߸��� ���� �ٴ����� ����
                }
                else
                    map[i][j] = 0; // �б� ���� �� �ٴ�
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
            float x = (j * TILE_SIZE) + (TILE_SIZE / 2.0f);
            float y = (i * TILE_SIZE) + (TILE_SIZE / 2.0f);
            tiles[i][j]->SetPosition(x, y);
			tiles[i][j]->SetRange({ (LONG)(x - TILE_SIZE / 2.0f), (LONG)(y - TILE_SIZE / 2.0f),
				(LONG)(x + TILE_SIZE / 2.0f), (LONG)(y + TILE_SIZE / 2.0f) });


            // map[i][j] ���� ���� �̹��� ����
            switch (map[i][j])
            {
            case 0: // �ٴ�
                tiles[i][j]->SetImage(&floorImage);
                break;
            case 1: // ��
                tiles[i][j]->SetImage(&wallImage);
                break;
            case 2: // �� Ÿ��
                tiles[i][j]->SetImage(&emptyImage);
                break;
            default:
                tiles[i][j]->SetImage(&floorImage); // ������ ���� �⺻��
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

    // ��� Ÿ���� ���ۿ� ������ + �� Ÿ�� range ���
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLS; j++)
        {
            tiles[i][j]->Render(bufferDC);
            if (map[i][j] == 1) {
				// �� Ÿ���� range�� ���������� ���
				HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
				SelectObject(bufferDC, redBrush);
				Rectangle(bufferDC, tiles[i][j]->GetRect().left, tiles[i][j]->GetRect().top,
					tiles[i][j]->GetRect().right, tiles[i][j]->GetRect().bottom);
				DeleteObject(redBrush);
            }
			
        }
    }

    buffer.ReleaseDC();
}

void MapManager::Update()
{
    // �ʿ� �� ������Ʈ ���� �߰�
}

void MapManager::LateUpdate()
{
    // �ʿ� �� ��ó�� ���� �߰�
}

void MapManager::Render(HDC hdc, float cameraX, float cameraY)
{
    // ����Ʈ: 1280x720
    int viewWidth = 1280;
    int viewHeight = 720;

    // ī�޶� ��ġ Ŭ����
    cameraX = max(0.0f, min(cameraX, static_cast<float>(MAP_WIDTH - viewWidth)));
    cameraY = max(0.0f, min(cameraY, static_cast<float>(MAP_HEIGHT - viewHeight)));

    

    // ���۸� ����Ʈ�� ���
    SetStretchBltMode(hdc, HALFTONE);
    buffer.StretchBlt(hdc, 0, 0, viewWidth, viewHeight,
        static_cast<int>(cameraX), static_cast<int>(cameraY),
        viewWidth, viewHeight, SRCCOPY);
}