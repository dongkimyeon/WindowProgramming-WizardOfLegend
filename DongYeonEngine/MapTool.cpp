#define _CRT_SECURE_NO_WARNINGS
#include "MapTool.h"

void MapTool::Initialize() {
    Mapfp = fopen("StageCustom.txt", "r");
    Imagefp = fopen("StageCustomImage.txt", "r");
    Objectfp = fopen("StageCustomObejct.txt", "r");

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
                else if (!floorTile[0].IsNull()) floorTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
            }
            else if (map[i][j] == 1) {
                if (ImageMap[i][j] == "w1" && !wallTile[0].IsNull()) wallTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "w2" && !wallTile[1].IsNull()) wallTile[1].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "w3" && !wallTile[2].IsNull()) wallTile[2].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "w4" && !wallTile[3].IsNull()) wallTile[3].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc1" && !wallConerTile[0].IsNull()) wallConerTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc2" && !wallConerTile[1].IsNull()) wallConerTile[1].StretchBlt(hdc, left, top, right - left, top - bottom, SRCCOPY);
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
    hOldPen = (HPEN)SelectObject(hdc, hPen);
    for (int i = 0; i <= 40; i++) {
        MoveToEx(hdc, i * 20, 0, NULL);
        LineTo(hdc, i * 20, 800);
        MoveToEx(hdc, 0, i * 20, NULL);
        LineTo(hdc, 800, i * 20);
    }
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    if (!floorTile[0].IsNull()) floorTile[0].StretchBlt(hdc, 850, 50, 40, 40, SRCCOPY);
    if (!floorTile[1].IsNull()) floorTile[1].StretchBlt(hdc, 850, 100, 40, 40, SRCCOPY);
    if (!floorTile[2].IsNull()) floorTile[2].StretchBlt(hdc, 850, 150, 40, 40, SRCCOPY);
    if (!floorTile[3].IsNull()) floorTile[3].StretchBlt(hdc, 850, 200, 40, 40, SRCCOPY);
    if (!EmptyTile.IsNull()) EmptyTile.StretchBlt(hdc, 850, 250, 40, 40, SRCCOPY);
    if (!wallTile[0].IsNull()) wallTile[0].StretchBlt(hdc, 850, 300, 40, 40, SRCCOPY);
    if (!wallTile[1].IsNull()) wallTile[1].StretchBlt(hdc, 850, 350, 40, 40, SRCCOPY);
    if (!wallTile[2].IsNull()) wallTile[2].StretchBlt(hdc, 850, 400, 40, 40, SRCCOPY);
    if (!wallTile[3].IsNull()) wallTile[3].StretchBlt(hdc, 850, 450, 40, 40, SRCCOPY);

    if (!wallConerTile[0].IsNull()) wallConerTile[0].StretchBlt(hdc, 910, 50, 40, 40, SRCCOPY);
    if (!wallConerTile[1].IsNull()) wallConerTile[1].StretchBlt(hdc, 910, 100, 40, 40, SRCCOPY);
    if (!wallConerTile[2].IsNull()) wallConerTile[2].StretchBlt(hdc, 910, 150, 40, 40, SRCCOPY);
    if (!wallConerTile[3].IsNull()) wallConerTile[3].StretchBlt(hdc, 910, 200, 40, 40, SRCCOPY);
    if (!wallConerTile[4].IsNull()) wallConerTile[4].StretchBlt(hdc, 910, 250, 40, 40, SRCCOPY);
    if (!wallConerTile[5].IsNull()) wallConerTile[5].StretchBlt(hdc, 910, 300, 40, 40, SRCCOPY);
    if (!wallConerTile[6].IsNull()) wallConerTile[6].StretchBlt(hdc, 910, 350, 40, 40, SRCCOPY);
    if (!wallConerTile[7].IsNull()) wallConerTile[7].StretchBlt(hdc, 910, 400, 40, 40, SRCCOPY);
    if (!wallConerTile[8].IsNull()) wallConerTile[8].StretchBlt(hdc, 910, 450, 40, 40, SRCCOPY);
    if (!wallConerTile[9].IsNull()) wallConerTile[9].StretchBlt(hdc, 910, 500, 40, 40, SRCCOPY);

    if (!Objects[0].IsNull()) Objects[0].StretchBlt(hdc, 960, 50, 40, 40, SRCCOPY);
    if (!Objects[1].IsNull()) Objects[1].StretchBlt(hdc, 960, 100, 40, 40, SRCCOPY);
    if (!Objects[2].IsNull()) Objects[2].StretchBlt(hdc, 960, 150, 40, 40, SRCCOPY);
    if (!Objects[3].IsNull()) Objects[3].StretchBlt(hdc, 960, 200, 40, 40, SRCCOPY);
    if (!Objects[4].IsNull()) Objects[4].StretchBlt(hdc, 960, 250, 40, 40, SRCCOPY);
    if (!Objects[5].IsNull()) Objects[5].StretchBlt(hdc, 960, 300, 40, 40, SRCCOPY);
    if (!Objects[6].IsNull()) Objects[6].StretchBlt(hdc, 960, 350, 40, 40, SRCCOPY);
    if (!Objects[7].IsNull()) Objects[7].StretchBlt(hdc, 960, 400, 40, 40, SRCCOPY);
    if (!Objects[8].IsNull()) Objects[8].StretchBlt(hdc, 960, 450, 40, 40, SRCCOPY);
    if (!Objects[9].IsNull()) Objects[9].StretchBlt(hdc, 960, 500, 40, 40, SRCCOPY);
    if (!Objects[10].IsNull()) Objects[10].StretchBlt(hdc, 960, 550, 40, 40, SRCCOPY);
    if (!Objects[11].IsNull()) Objects[11].StretchBlt(hdc, 960, 600, 40, 40, SRCCOPY);
    if (!Objects[12].IsNull()) Objects[12].StretchBlt(hdc, 960, 650, 40, 40, SRCCOPY);

    if (drag) {
        int left = min(drawRect.left, drawRect.right) * 20;
        int top = min(drawRect.top, drawRect.bottom) * 20;
        int right = max(drawRect.left, drawRect.right) * 20;
        int bottom = max(drawRect.top, drawRect.bottom) * 20;
        Rectangle(hdc, left, top, right, bottom);
    }
}

void MapTool::Update() {
    if (Input::GetKeyDown(eKeyCode::LButton)) {
        int mx = Input::GetMousePosition().x;
        int my = Input::GetMousePosition().y;

        if (Input::GetMousePosition().x >= 850 && Input::GetMousePosition().x <= 890) {
            if (Input::GetMousePosition().y >= 50 && Input::GetMousePosition().y <= 90) {
                selectedTile = "f1";
                floordrag = true;
                walldrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 100 && Input::GetMousePosition().y <= 140) {
                selectedTile = "f2";
                floordrag = true;
                walldrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 150 && Input::GetMousePosition().y <= 190) {
                selectedTile = "f3";
                floordrag = true;
                walldrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 200 && Input::GetMousePosition().y <= 240) {
                selectedTile = "f4";
                floordrag = true;
                walldrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 250 && Input::GetMousePosition().y <= 290) {
                selectedTile = "e";
                emptydrag = true;
                walldrag = false;
                floordrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 300 && Input::GetMousePosition().y <= 340) {
                selectedTile = "w1";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 350 && Input::GetMousePosition().y <= 390) {
                selectedTile = "w2";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 400 && Input::GetMousePosition().y <= 440) {
                selectedTile = "w3";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 450 && Input::GetMousePosition().y <= 490) {
                selectedTile = "w4";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
        }
        else if (Input::GetMousePosition().x >= 910 && Input::GetMousePosition().x <= 950) {
            if (Input::GetMousePosition().y >= 50 && Input::GetMousePosition().y <= 90) {
                selectedTile = "wc1";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 100 && Input::GetMousePosition().y <= 140) {
                selectedTile = "wc2";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 150 && Input::GetMousePosition().y <= 190) {
                selectedTile = "wc3";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 200 && Input::GetMousePosition().y <= 240) {
                selectedTile = "wc4";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 250 && Input::GetMousePosition().y <= 290) {
                selectedTile = "wc5";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 300 && Input::GetMousePosition().y <= 340) {
                selectedTile = "wc6";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 350 && Input::GetMousePosition().y <= 390) {
                selectedTile = "wc7";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 400 && Input::GetMousePosition().y <= 440) {
                selectedTile = "wc8";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 450 && Input::GetMousePosition().y <= 490) {
                selectedTile = "wc9";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
            else if (Input::GetMousePosition().y >= 500 && Input::GetMousePosition().y <= 540) {
                selectedTile = "wc10";
                walldrag = true;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = false;
            }
        }
        else if (Input::GetMousePosition().x >= 960 && Input::GetMousePosition().x <= 1000) {
            if (Input::GetMousePosition().y >= 50 && Input::GetMousePosition().y <= 90) {
                selectedObject = "Archer";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
            else if (Input::GetMousePosition().y >= 100 && Input::GetMousePosition().y <= 140) {
                selectedObject = "SwordMan";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
            else if (Input::GetMousePosition().y >= 150 && Input::GetMousePosition().y <= 190) {
                selectedObject = "Wizard";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
            else if (Input::GetMousePosition().y >= 200 && Input::GetMousePosition().y <= 240) {
                selectedObject = "Candle";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
            else if (Input::GetMousePosition().y >= 250 && Input::GetMousePosition().y <= 290) {
                selectedObject = "IceChunk0";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
            else if (Input::GetMousePosition().y >= 300 && Input::GetMousePosition().y <= 340) {
                selectedObject = "IceChunk1";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
            else if (Input::GetMousePosition().y >= 350 && Input::GetMousePosition().y <= 390) {
                selectedObject = "IceFlag";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
            else if (Input::GetMousePosition().y >= 400 && Input::GetMousePosition().y <= 440) {
                selectedObject = "IceSmallChunk";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
            else if (Input::GetMousePosition().y >= 450 && Input::GetMousePosition().y <= 490) {
                selectedObject = "IceWindow0";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
            else if (Input::GetMousePosition().y >= 500 && Input::GetMousePosition().y <= 540) {
                selectedObject = "IceWindow1";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
            else if (Input::GetMousePosition().y >= 550 && Input::GetMousePosition().y <= 590) {
                selectedObject = "IceWindow2";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
            else if (Input::GetMousePosition().y >= 600 && Input::GetMousePosition().y <= 640) {
                selectedObject = "Jar";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
            else if (Input::GetMousePosition().y >= 650 && Input::GetMousePosition().y <= 690) {
                selectedObject = "Statue";
                walldrag = false;
                floordrag = false;
                emptydrag = false;
                ObjectCLK = true;
            }
        }

        if (walldrag || floordrag || emptydrag) {
            if (!drag) {
                mx = Input::GetMousePosition().x / 20;
                my = Input::GetMousePosition().y / 20;

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
            int mx = Input::GetMousePosition().x / 20;
            int my = Input::GetMousePosition().y / 20;

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
            int mx = Input::GetMousePosition().x / 20;
            int my = Input::GetMousePosition().y / 20;

            if (mx >= 0 && mx < 40 && my >= 0 && my < 40) {
                ObjectMap[mx][my] = selectedObject;
            }
        }
    }
    else if (Input::GetKey(eKeyCode::LButton)) {
        if (drag) {
            int mx = Input::GetMousePosition().x / 20;
            int my = Input::GetMousePosition().y / 20;

            if (mx >= 0 && mx < 40 && my >= 0 && my < 40) {
                drawRect.right = mx + 1;
                drawRect.bottom = my + 1;
            }
        }
    }

    if (Input::GetKeyDown(eKeyCode::S)) {
        Mapfp = fopen("StageCustom.txt", "w");
        Imagefp = fopen("StageCustomImage.txt", "w");
        Objectfp = fopen("StageCustomObejct.txt", "w");

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