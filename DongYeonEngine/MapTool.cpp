#define _CRT_SECURE_NO_WARNINGS
#include "MapTool.h"




void MapTool::Initialize() {
	Mapfp = fopen("StageCustom.txt", "r");
	Imagefp = fopen("StageCustomImage.txt", "r");

    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 40; j++) {
            map[i][j] = 0;
            ImageMap[i][j] = "f1"; // Default to floor tile 1
        }
    }

    if (Mapfp) {
        for (int i = 0; i < 40; i++) {
            for (int j = 0; j < 40; j++) {
                int value;
                if (fscanf(Mapfp, "%d", &value) == 1) {
                    map[j][i] = value;
                }
            }
        }
        fclose(Mapfp);
        Mapfp = nullptr;
    }

    // Read Stage1Image.txt
    if (Imagefp) {
        for (int i = 0; i < 40; i++) {
            for (int j = 0; j < 40; j++) {
                char tile[10] = { 0 };
                if (fscanf(Imagefp, "%s", tile) == 1) {
                    ImageMap[j][i] = std::string(tile);
                }
            }
        }
        fclose(Imagefp);
        Imagefp = nullptr;
    }

    for (int i = 0; i < 4; i++) {
        wchar_t path[32];
        swprintf(path, 32, L"floorTile%d.png", i + 1);
        floorTile[i].Load(path);
    }

    EmptyTile.Load(L"emptyTile.png");

    for (int i = 0; i < 4; i++) {
        wchar_t path[32];
        swprintf(path, 32, L"wallTile%d.png", i + 1);
        wallTile[i].Load(path);
    }
    for (int i = 0; i < 10; i++) {
        wchar_t path[32];
        swprintf(path, 32, L"WallCornerTile%d.png", i + 1);
        wallConerTile[i].Load(path);
    }
}

void MapTool::Render(HDC hdc) {
    HPEN hPen, hOldPen;

    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 40; j++) {
            int left = i * 20;
            int top = j * 20;
            int right = (i + 1) * 20;
            int bottom = (j + 1) * 20;
            if (map[i][j] == 0) {
                if (ImageMap[i][j] == "f1" && !floorTile[0].IsNull()) floorTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "f2" && !floorTile[1].IsNull()) floorTile[1].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "f3" && !floorTile[2].IsNull()) floorTile[2].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "f4" && !floorTile[3].IsNull()) floorTile[3].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (!floorTile[0].IsNull()) floorTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY); // Fallback to f1
            }
            else if (map[i][j] == 1) {
                if (ImageMap[i][j] == "w1" && !wallTile[0].IsNull()) wallTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "w2" && !wallTile[1].IsNull()) wallTile[1].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "w3" && !wallTile[2].IsNull()) wallTile[2].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "w4" && !wallTile[3].IsNull()) wallTile[3].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc1" && !wallConerTile[0].IsNull()) wallConerTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc2" && !wallConerTile[1].IsNull()) wallConerTile[1].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc3" && !wallConerTile[2].IsNull()) wallConerTile[2].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc4" && !wallConerTile[3].IsNull()) wallConerTile[3].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc5" && !wallConerTile[4].IsNull()) wallConerTile[4].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc6" && !wallConerTile[5].IsNull()) wallConerTile[5].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc7" && !wallConerTile[6].IsNull()) wallConerTile[6].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc8" && !wallConerTile[7].IsNull()) wallConerTile[7].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc9" && !wallConerTile[8].IsNull()) wallConerTile[8].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc10" && !wallConerTile[9].IsNull()) wallConerTile[9].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (!wallTile[0].IsNull()) wallTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY); // Fallback to w1
            }
            else if (map[i][j] == 2) {
                if (!EmptyTile.IsNull()) {
                    EmptyTile.StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                }
            }
        }
    }

    // Draw grid lines for tile map
    hPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
    hOldPen = (HPEN)SelectObject(hdc, hPen);
    for (int i = 0; i <= 40; i++) {
        MoveToEx(hdc, i * 20, 0, NULL);
        LineTo(hdc, i * 20, 800);
        MoveToEx(hdc, 0, i * 20, NULL);
        LineTo(hdc, 800, i * 20);
    }
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    // Draw tile palette in two columns (x: 850 and 910, y: 50+, 40x40 tiles)
        // Left column (9 tiles)
    if (!floorTile[0].IsNull()) floorTile[0].StretchBlt(hdc, 850, 50, 40, 40, SRCCOPY); // f1
    if (!floorTile[1].IsNull()) floorTile[1].StretchBlt(hdc, 850, 100, 40, 40, SRCCOPY); // f2
    if (!floorTile[2].IsNull()) floorTile[2].StretchBlt(hdc, 850, 150, 40, 40, SRCCOPY); // f3
    if (!floorTile[3].IsNull()) floorTile[3].StretchBlt(hdc, 850, 200, 40, 40, SRCCOPY); // f4
    if (!EmptyTile.IsNull()) EmptyTile.StretchBlt(hdc, 850, 250, 40, 40, SRCCOPY); // e
    if (!wallTile[0].IsNull()) wallTile[0].StretchBlt(hdc, 850, 300, 40, 40, SRCCOPY); // w1
    if (!wallTile[1].IsNull()) wallTile[1].StretchBlt(hdc, 850, 350, 40, 40, SRCCOPY); // w2
    if (!wallTile[2].IsNull()) wallTile[2].StretchBlt(hdc, 850, 400, 40, 40, SRCCOPY); // w3
    if (!wallTile[3].IsNull()) wallTile[3].StretchBlt(hdc, 850, 450, 40, 40, SRCCOPY); // w4

    // Right column (9 tiles)
    if (!wallConerTile[0].IsNull()) wallConerTile[0].StretchBlt(hdc, 910, 50, 40, 40, SRCCOPY); // wc1
    if (!wallConerTile[1].IsNull()) wallConerTile[1].StretchBlt(hdc, 910, 100, 40, 40, SRCCOPY); // wc2
    if (!wallConerTile[2].IsNull()) wallConerTile[2].StretchBlt(hdc, 910, 150, 40, 40, SRCCOPY); // wc3
    if (!wallConerTile[3].IsNull()) wallConerTile[3].StretchBlt(hdc, 910, 200, 40, 40, SRCCOPY); // wc4
    if (!wallConerTile[4].IsNull()) wallConerTile[4].StretchBlt(hdc, 910, 250, 40, 40, SRCCOPY); // wc5
    if (!wallConerTile[5].IsNull()) wallConerTile[5].StretchBlt(hdc, 910, 300, 40, 40, SRCCOPY); // wc6
    if (!wallConerTile[6].IsNull()) wallConerTile[6].StretchBlt(hdc, 910, 350, 40, 40, SRCCOPY); // wc7
    if (!wallConerTile[7].IsNull()) wallConerTile[7].StretchBlt(hdc, 910, 400, 40, 40, SRCCOPY); // wc8
    if (!wallConerTile[8].IsNull()) wallConerTile[8].StretchBlt(hdc, 910, 450, 40, 40, SRCCOPY); // wc9
    if (!wallConerTile[9].IsNull()) wallConerTile[9].StretchBlt(hdc, 910, 500, 40, 40, SRCCOPY); // wc10
}

void MapTool::Update() {

}

void MapTool::LateUpdate() {

}
