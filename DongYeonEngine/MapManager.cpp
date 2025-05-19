#define _CRT_SECURE_NO_WARNINGS
#include "MapManager.h"
#include <windows.h>


void MapManager::Initialize()
{
    // 맵 데이터 읽기
    FILE* fp = fopen("map.txt", "r"); // 읽기 모드로 열기
    if (fp == nullptr)
    {
        // 파일 열기 실패 시 기본 맵 생성 (예: 모두 바닥)
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
                        map[i][j] = 0; // 잘못된 값은 바닥으로 설정
                }
                else
                    map[i][j] = 0; // 읽기 실패 시 바닥
            }
        }
        fclose(fp);
    }

    // 이미지 로드
    floorImage.Load(L"resources/Tile/Tile0.png");
    wallImage.Load(L"resources/Tile/Tile38.png");
    emptyImage.Load(L"resources/Tile/Tile121.png");

    // 타일 초기화
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


            // map[i][j] 값에 따라 이미지 설정
            switch (map[i][j])
            {
            case 0: // 바닥
                tiles[i][j]->SetImage(&floorImage);
                break;
            case 1: // 벽
                tiles[i][j]->SetImage(&wallImage);
                break;
            case 2: // 빈 타일
                tiles[i][j]->SetImage(&emptyImage);
                break;
            default:
                tiles[i][j]->SetImage(&floorImage); // 안전을 위해 기본값
                break;
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

    // 모든 타일을 버퍼에 렌더링 + 벽 타일 range 출력
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLS; j++)
        {
            tiles[i][j]->Render(bufferDC);
            if (map[i][j] == 1) {
				// 벽 타일의 range를 빨간색으로 출력
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
    // 필요 시 업데이트 로직 추가
}

void MapManager::LateUpdate()
{
    // 필요 시 후처리 로직 추가
}

void MapManager::Render(HDC hdc, float cameraX, float cameraY)
{
    // 뷰포트: 1280x720
    int viewWidth = 1280;
    int viewHeight = 720;

    // 카메라 위치 클램핑
    cameraX = max(0.0f, min(cameraX, static_cast<float>(MAP_WIDTH - viewWidth)));
    cameraY = max(0.0f, min(cameraY, static_cast<float>(MAP_HEIGHT - viewHeight)));

    

    // 버퍼를 뷰포트에 출력
    SetStretchBltMode(hdc, HALFTONE);
    buffer.StretchBlt(hdc, 0, 0, viewWidth, viewHeight,
        static_cast<int>(cameraX), static_cast<int>(cameraY),
        viewWidth, viewHeight, SRCCOPY);
}