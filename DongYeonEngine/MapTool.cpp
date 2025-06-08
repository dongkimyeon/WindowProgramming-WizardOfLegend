#define _CRT_SECURE_NO_WARNINGS
#include "MapTool.h"

void MapTool::Initialize() {
    Mapfp = fopen("StageCustom.txt", "r");
    Imagefp = fopen("StageCustomImage.txt", "r");
    Objectfp = fopen("StageCustomObject.txt", "r");

    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 40; j++) {
            map[i][j] = 0;
            ImageMap[i][j] = "f1";
            ObjectMap[i][j] = "empty";
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

    if (Objectfp) {
        for (int i = 0; i < 40; i++) {
            for (int j = 0; j < 40; j++) {
                char tile[15] = { 0 };
                if (fscanf(Objectfp, "%s", tile) == 1) {
                    ObjectMap[j][i] = std::string(tile);
                }
            }
        }
        fclose(Objectfp);
        Objectfp = nullptr;
    }

    for (int i = 0; i < 4; i++) {
        wchar_t path[64];
        swprintf(path, 64, L"resources/Tile/floorTile%d.png", i + 1);
        floorTile[i].Load(path);
    }

    EmptyTile.Load(L"resources/Tile/emptyTile.png");

    for (int i = 0; i < 4; i++) {
        wchar_t path[64];
        swprintf(path, 64, L"resources/Tile/wallTile%d.png", i + 1);
        wallTile[i].Load(path);
    }
    for (int i = 0; i < 10; i++) {
        wchar_t path[64];
        swprintf(path, 64, L"resources/Tile/WallCornerTile%d.png", i + 1);
        wallConerTile[i].Load(path);
    }

    Objects[0].Load(L"resources/MapToolObjectIcon/ArcherIcon.png");
    Objects[1].Load(L"resources/MapToolObjectIcon/SWORDMAN_RIGHT_0.png");
    Objects[2].Load(L"resources/MapToolObjectIcon/WizardIcon.png");
    Objects[3].Load(L"resources/MapToolObjectIcon/Candle.png");
    Objects[4].Load(L"resources/MapToolObjectIcon/IceChunk0.png");
    Objects[5].Load(L"resources/MapToolObjectIcon/IceChunk1.png");
    Objects[6].Load(L"resources/MapToolObjectIcon/IceFlag.png");
    Objects[7].Load(L"resources/MapToolObjectIcon/IceSmallChunk.png");
    Objects[8].Load(L"resources/MapToolObjectIcon/IceWindow0.png");
    Objects[9].Load(L"resources/MapToolObjectIcon/IceWindow1.png");
    Objects[10].Load(L"resources/MapToolObjectIcon/IceWindow2.png");
    Objects[11].Load(L"resources/MapToolObjectIcon/Jar.png");
    Objects[12].Load(L"resources/MapToolObjectIcon/Statue.png");

    selectedTile = "f1";
    selectedObject = "empty";
    floordrag = false;
    walldrag = false;
    emptydrag = false;
    ObjectCLK = false;
    drag = false;
}

void MapTool::Render(HDC hdc) {
    HPEN hPen, hOldPen, hRedPen;

    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 40; j++) {
            int left = i * 15;
            int top = j * 15;
            int right = (i + 1) * 15;
            int bottom = (j + 1) * 15;
            if (map[i][j] == 0) {
                if (ImageMap[i][j] == "f1" && !floorTile[0].IsNull()) floorTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "f2" && !floorTile[1].IsNull()) floorTile[1].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "f3" && !floorTile[2].IsNull()) floorTile[2].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "f4" && !floorTile[3].IsNull()) floorTile[3].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (!floorTile[0].IsNull()) floorTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
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
                else if (!wallTile[0].IsNull()) wallTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
            }
            else if (map[i][j] == 2) {
                if (!EmptyTile.IsNull()) {
                    EmptyTile.StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                }
            }

            if (ObjectMap[i][j] != "empty") {
                if (ObjectMap[i][j] == "Archer" && !Objects[0].IsNull()) Objects[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ObjectMap[i][j] == "SwordMan" && !Objects[1].IsNull()) Objects[1].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ObjectMap[i][j] == "Wizard" && !Objects[2].IsNull()) Objects[2].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ObjectMap[i][j] == "Candle" && !Objects[3].IsNull()) Objects[3].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ObjectMap[i][j] == "IceChunk0" && !Objects[4].IsNull()) Objects[4].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ObjectMap[i][j] == "IceChunk1" && !Objects[5].IsNull()) Objects[5].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ObjectMap[i][j] == "IceFlag" && !Objects[6].IsNull()) Objects[6].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ObjectMap[i][j] == "IceSmallChunk" && !Objects[7].IsNull()) Objects[7].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ObjectMap[i][j] == "IceWindow0" && !Objects[8].IsNull()) Objects[8].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ObjectMap[i][j] == "IceWindow1" && !Objects[9].IsNull()) Objects[9].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ObjectMap[i][j] == "IceWindow2" && !Objects[10].IsNull()) Objects[10].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ObjectMap[i][j] == "Jar" && !Objects[11].IsNull()) Objects[11].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ObjectMap[i][j] == "Statue" && !Objects[12].IsNull()) Objects[12].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
            }
        }
    }

    hPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
    hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    hOldPen = (HPEN)SelectObject(hdc, hPen);
    for (int i = 0; i <= 40; i++) {
        MoveToEx(hdc, i * 15, 0, NULL);
        LineTo(hdc, i * 15, 600);
        MoveToEx(hdc, 0, i * 15, NULL);
        LineTo(hdc, 600, i * 15);
    }
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    // Render tiles and objects in 4-5 rows per category, starting at y=50
    int baseX = 650, baseY = 50, tileSize = 30, spacing = 8;

    // Floor Tiles (4 items)
    for (int i = 0; i < 4; i++) {
        int x = baseX + (i % 4) * (tileSize + spacing);
        int y = baseY + (i / 4) * (tileSize + spacing);
        if (!floorTile[i].IsNull()) floorTile[i].StretchBlt(hdc, x, y, tileSize, tileSize, SRCCOPY);
        if (selectedTile == ("f" + std::to_string(i + 1))) {
            SelectObject(hdc, hRedPen);
            Rectangle(hdc, x, y, x + tileSize, y + tileSize);
            SelectObject(hdc, hOldPen);
        }
    }

    // Empty Tile (1 item)
    int emptyY = baseY + tileSize + spacing;
    if (!EmptyTile.IsNull()) EmptyTile.StretchBlt(hdc, baseX, emptyY, tileSize, tileSize, SRCCOPY);
    if (selectedTile == "e") {
        SelectObject(hdc, hRedPen);
        Rectangle(hdc, baseX, emptyY, baseX + tileSize, emptyY + tileSize);
        SelectObject(hdc, hOldPen);
    }

    // Wall Tiles (4 items)
    int wallY = emptyY + tileSize + spacing;
    for (int i = 0; i < 4; i++) {
        int x = baseX + (i % 4) * (tileSize + spacing);
        int y = wallY + (i / 4) * (tileSize + spacing);
        if (!wallTile[i].IsNull()) wallTile[i].StretchBlt(hdc, x, y, tileSize, tileSize, SRCCOPY);
        if (selectedTile == ("w" + std::to_string(i + 1))) {
            SelectObject(hdc, hRedPen);
            Rectangle(hdc, x, y, x + tileSize, y + tileSize);
            SelectObject(hdc, hOldPen);
        }
    }

    // Wall Corner Tiles (10 items, 5 per row)
    int cornerY = wallY + tileSize + spacing;
    for (int i = 0; i < 10; i++) {
        int x = baseX + (i % 5) * (tileSize + spacing);
        int y = cornerY + (i / 5) * (tileSize + spacing);
        if (!wallConerTile[i].IsNull()) wallConerTile[i].StretchBlt(hdc, x, y, tileSize, tileSize, SRCCOPY);
        if (selectedTile == ("wc" + std::to_string(i + 1))) {
            SelectObject(hdc, hRedPen);
            Rectangle(hdc, x, y, x + tileSize, y + tileSize);
            SelectObject(hdc, hOldPen);
        }
    }

    // Objects (13 items, 5 per row) + Empty and Cancel buttons
    int objectY = cornerY + 2 * (tileSize + spacing);
    std::string objectNames[] = { "Archer", "SwordMan", "Wizard", "Candle", "IceChunk0", "IceChunk1", "IceFlag", "IceSmallChunk", "IceWindow0", "IceWindow1", "IceWindow2", "Jar", "Statue" };
    for (int i = 0; i < 13; i++) {
        int x = baseX + (i % 5) * (tileSize + spacing);
        int y = objectY + (i / 5) * (tileSize + spacing);
        if (!Objects[i].IsNull()) Objects[i].StretchBlt(hdc, x, y, tileSize, tileSize, SRCCOPY);
        if (selectedObject == objectNames[i]) {
            SelectObject(hdc, hRedPen);
            Rectangle(hdc, x, y, x + tileSize, y + tileSize);
            SelectObject(hdc, hOldPen);
        }
    }

    // Empty Button
    int emptyButtonY = objectY + 3 * (tileSize + spacing);
    hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, hPen);
    Rectangle(hdc, baseX, emptyButtonY, baseX + tileSize, emptyButtonY + tileSize);
    if (selectedObject == "empty") {
        SelectObject(hdc, hRedPen);
        Rectangle(hdc, baseX, emptyButtonY, baseX + tileSize, emptyButtonY + tileSize);
        SelectObject(hdc, hOldPen);
    }
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    // Cancel Button
    int cancelButtonY = emptyButtonY + tileSize + spacing;
    hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, hPen);
    Rectangle(hdc, baseX, cancelButtonY, baseX + tileSize, cancelButtonY + tileSize);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    if (drag) {
        int left = min(drawRect.left, drawRect.right) * 15;
        int top = min(drawRect.top, drawRect.bottom) * 15;
        int right = max(drawRect.left, drawRect.right) * 15;
        int bottom = max(drawRect.top, drawRect.bottom) * 15;
        Rectangle(hdc, left, top, right, bottom);
    }

    DeleteObject(hRedPen);
}

void MapTool::Update() {
    if (Input::GetKeyDown(eKeyCode::LButton)) {
        int mx = Input::GetMousePosition().x;
        int my = Input::GetMousePosition().y;
        int baseX = 650, tileSize = 30, spacing = 8;

        // Floor Tiles
        for (int i = 0; i < 4; i++) {
            int x = baseX + (i % 4) * (tileSize + spacing);
            int y = 50 + (i / 4) * (tileSize + spacing);
            if (mx >= x && mx <= x + tileSize && my >= y && my <= y + tileSize) {
                selectedTile = "f" + std::to_string(i + 1);
                floordrag = true;
                walldrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
        }

        // Empty Tile
        int emptyY = 50 + tileSize + spacing;
        if (mx >= baseX && mx <= baseX + tileSize && my >= emptyY && my <= emptyY + tileSize) {
            selectedTile = "e";
            emptydrag = true;
            walldrag = false;
            floordrag = false;
            ObjectCLK = false;
        }

        // Wall Tiles
        int wallY = emptyY + tileSize + spacing;
        for (int i = 0; i < 4; i++) {
            int x = baseX + (i % 4) * (tileSize + spacing);
            int y = wallY + (i / 4) * (tileSize + spacing);
            if (mx >= x && mx <= x + tileSize && my >= y && my <= y + tileSize) {
                selectedTile = "w" + std::to_string(i + 1);
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
        }

        // Wall Corner Tiles
        int cornerY = wallY + tileSize + spacing;
        for (int i = 0; i < 10; i++) {
            int x = baseX + (i % 5) * (tileSize + spacing);
            int y = cornerY + (i / 5) * (tileSize + spacing);
            if (mx >= x && mx <= x + tileSize && my >= y && my <= y + tileSize) {
                selectedTile = "wc" + std::to_string(i + 1);
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
        }

        // Objects
        int objectY = cornerY + 2 * (tileSize + spacing);
        std::string objectNames[] = { "Archer", "SwordMan", "Wizard", "Candle", "IceChunk0", "IceChunk1", "IceFlag", "IceSmallChunk", "IceWindow0", "IceWindow1", "IceWindow2", "Jar", "Statue" };
        for (int i = 0; i < 13; i++) {
            int x = baseX + (i % 5) * (tileSize + spacing);
            int y = objectY + (i / 5) * (tileSize + spacing);
            if (mx >= x && mx <= x + tileSize && my >= y && my <= y + tileSize) {
                selectedObject = objectNames[i];
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
        }

        // Empty Button
        int emptyButtonY = objectY + 3 * (tileSize + spacing);
        if (mx >= baseX && mx <= baseX + tileSize && my >= emptyButtonY && my <= emptyButtonY + tileSize) {
            selectedObject = "empty";
            walldrag = false;
            floordrag = false;
            emptydrag = false;
            ObjectCLK = true;
        }

        // Cancel Button
        int cancelButtonY = emptyButtonY + tileSize + spacing;
        if (mx >= baseX && mx <= baseX + tileSize && my >= cancelButtonY && my <= cancelButtonY + tileSize) {
            selectedTile = "f1";
            selectedObject = "empty";
            walldrag = false;
            floordrag = false;
            emptydrag = false;
            ObjectCLK = false;
            drag = false;
        }

        if (walldrag || floordrag || emptydrag) {
            if (!drag) {
                mx = Input::GetMousePosition().x / 15;
                my = Input::GetMousePosition().y / 15;

                if (mx >= 0 && mx < 40 && my >= 0 && my < 40) {
                    drawRect.left = mx;
                    drawRect.top = my;
                    drawRect.right = mx + 1;
                    drawRect.bottom = my + 1;
                    drag = true;
                }
            }
        }
    }
    else if (Input::GetKeyUp(eKeyCode::LButton)) {
        if (drag) {
            int mx = Input::GetMousePosition().x / 15;
            int my = Input::GetMousePosition().y / 15;

            if (mx >= 0 && mx < 40 && my >= 0 && my < 40) {
                drawRect.right = mx + 1;
                drawRect.bottom = my + 1;

                int left = min(drawRect.left, drawRect.right);
                int right = max(drawRect.left, drawRect.right);
                int top = min(drawRect.top, drawRect.bottom);
                int bottom = max(drawRect.top, drawRect.bottom);

                for (int i = left; i < right; i++) {
                    for (int j = top; j < bottom; j++) {
                        if (i >= 0 && i < 40 && j >= 0 && j < 40) {
                            if (walldrag) {
                                map[i][j] = 1;
                                ImageMap[i][j] = selectedTile;
                            }
                            else if (floordrag) {
                                map[i][j] = 0;
                                ImageMap[i][j] = selectedTile;
                            }
                            else if (emptydrag) {
                                map[i][j] = 2;
                                ImageMap[i][j] = selectedTile;
                            }
                        }
                    }
                }
            }
            drag = false;
        }
        else if (ObjectCLK) {
            int mx = Input::GetMousePosition().x / 15;
            int my = Input::GetMousePosition().y / 15;

            if (mx >= 0 && mx < 40 && my >= 0 && my < 40) {
                ObjectMap[mx][my] = selectedObject;
            }
        }
    }
    else if (Input::GetKey(eKeyCode::LButton)) {
        if (drag) {
            int mx = Input::GetMousePosition().x / 15;
            int my = Input::GetMousePosition().y / 15;

            if (mx >= 0 && mx < 40 && my >= 0 && my < 40) {
                drawRect.right = mx + 1;
                drawRect.bottom = my + 1;
            }
        }
    }

    if (Input::GetKeyDown(eKeyCode::S)) {
        Mapfp = fopen("StageCustom.txt", "w");
        Imagefp = fopen("StageCustomImage.txt", "w");
        Objectfp = fopen("StageCustomObject.txt", "w");

        if (!Mapfp || !Imagefp || !Objectfp) {
            if (Mapfp) fclose(Mapfp);
            if (Imagefp) fclose(Imagefp);
            if (Objectfp) fclose(Objectfp);
            Mapfp = nullptr;
            Imagefp = nullptr;
            Objectfp = nullptr;
        }

        bool saveSuccess = true;
        for (int i = 0; i < 40; i++) {
            for (int j = 0; j < 40; j++) {
                if (fprintf(Mapfp, "%d ", map[j][i]) < 0 ||
                    fprintf(Imagefp, "%s ", ImageMap[j][i].c_str()) < 0 ||
                    fprintf(Objectfp, "%s ", ObjectMap[j][i].c_str()) < 0) {
                    saveSuccess = false;
                    break;
                }
            }
            fprintf(Mapfp, "\n");
            fprintf(Imagefp, "\n");
            fprintf(Objectfp, "\n");
            if (!saveSuccess) break;
        }

        fflush(Mapfp);
        fflush(Imagefp);
        fflush(Objectfp);
        fclose(Mapfp);
        fclose(Imagefp);
        fclose(Objectfp);
        Mapfp = nullptr;
        Imagefp = nullptr;
        Objectfp = nullptr;
    }
}

void MapTool::LateUpdate() {
}