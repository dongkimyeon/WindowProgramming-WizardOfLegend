#define _CRT_SECURE_NO_WARNINGS
#include "MapTool.h"
#include "SceneManager.h"

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

    selectedTile = "";
    selectedObject = "";
    currentMode = EditMode::NONE;
    drag = false;

    // 버튼 위치 초기화 (우측 하단, 상단으로 이동)
    mSaveButton = { 850 + 250, 530, 950 + 250, 580 };   // Save 버튼
    mCancelButton = { 850 + 250, 590, 950 + 250, 640 }; // Cancel 버튼
    mExitButton = { 850 + 250, 650, 950 + 250, 700 };   // Exit 버튼
}

void MapTool::Render(HDC hdc) {
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HBRUSH hButtonBrush = CreateSolidBrush(RGB(200, 200, 200)); // 버튼 배경색 (회색)
    HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH); // 투명 브러시
    HBRUSH hOldBrush;
    HPEN hOldPen;
    SetTextColor(hdc, RGB(0, 0, 0));

    // 맵 그리기
    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 40; j++) {
            int left = i * 20;
            int top = j * 20;
            int right = (i + 1) * 20;
            int bottom = (j + 1) * 20;
            if (map[i][j] == 0) {
                if (ImageMap[i][j] == "f1") floorTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "f2") floorTile[1].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "f3") floorTile[2].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "f4") floorTile[3].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else floorTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
            }
            else if (map[i][j] == 1) {
                if (ImageMap[i][j] == "w1") wallTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "w2") wallTile[1].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "w3") wallTile[2].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "w4") wallTile[3].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc1") wallConerTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc2") wallConerTile[1].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc3") wallConerTile[2].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc4") wallConerTile[3].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc5") wallConerTile[4].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc6") wallConerTile[5].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc7") wallConerTile[6].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc8") wallConerTile[7].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc9") wallConerTile[8].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else if (ImageMap[i][j] == "wc10") wallConerTile[9].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
                else wallTile[0].StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
            }
            else if (map[i][j] == 2) {
                EmptyTile.StretchBlt(hdc, left, top, right - left, bottom - top, SRCCOPY);
            }

            // 오브젝트 그리기 (TransparentBlt로 투명 배경 처리)
            if (ObjectMap[i][j] != "empty") {
                if (ObjectMap[i][j] == "Archer") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[0].GetDC(), 0, 0, Objects[0].GetWidth(), Objects[0].GetHeight(), RGB(0, 0, 0));
                }
                else if (ObjectMap[i][j] == "SwordMan") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[1].GetDC(), 0, 0, Objects[1].GetWidth(), Objects[1].GetHeight(), RGB(0, 0, 0));
                }
                else if (ObjectMap[i][j] == "Wizard") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[2].GetDC(), 0, 0, Objects[2].GetWidth(), Objects[2].GetHeight(), RGB(0, 0, 0));
                }
                else if (ObjectMap[i][j] == "Candle") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[3].GetDC(), 0, 0, Objects[3].GetWidth(), Objects[3].GetHeight(), RGB(0, 0, 0));
                }
                else if (ObjectMap[i][j] == "IceChunk0") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[4].GetDC(), 0, 0, Objects[4].GetWidth(), Objects[4].GetHeight(), RGB(0, 0, 0));
                }
                else if (ObjectMap[i][j] == "IceChunk1") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[5].GetDC(), 0, 0, Objects[5].GetWidth(), Objects[5].GetHeight(), RGB(0, 0, 0));
                }
                else if (ObjectMap[i][j] == "IceFlag") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[6].GetDC(), 0, 0, Objects[6].GetWidth(), Objects[6].GetHeight(), RGB(0, 0, 0));
                }
                else if (ObjectMap[i][j] == "IceSmallChunk") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[7].GetDC(), 0, 0, Objects[7].GetWidth(), Objects[7].GetHeight(), RGB(0, 0, 0));
                }
                else if (ObjectMap[i][j] == "IceWindow0") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[8].GetDC(), 0, 0, Objects[8].GetWidth(), Objects[8].GetHeight(), RGB(0, 0, 0));
                }
                else if (ObjectMap[i][j] == "IceWindow1") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[9].GetDC(), 0, 0, Objects[9].GetWidth(), Objects[9].GetHeight(), RGB(0, 0, 0));
                }
                else if (ObjectMap[i][j] == "IceWindow2") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[10].GetDC(), 0, 0, Objects[10].GetWidth(), Objects[10].GetHeight(), RGB(0, 0, 0));
                }
                else if (ObjectMap[i][j] == "Jar") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[11].GetDC(), 0, 0, Objects[11].GetWidth(), Objects[11].GetHeight(), RGB(0, 0, 0));
                }
                else if (ObjectMap[i][j] == "Statue") {
                    TransparentBlt(hdc, left, top, right - left, bottom - top, Objects[12].GetDC(), 0, 0, Objects[12].GetWidth(), Objects[12].GetHeight(), RGB(0, 0, 0));
                }
            }
        }
    }

    hOldPen = (HPEN)SelectObject(hdc, hPen);
    for (int i = 0; i <= 40; i++) {
        MoveToEx(hdc, i * 20, 0, NULL);
        LineTo(hdc, i * 20, 800);
        MoveToEx(hdc, 0, i * 20, NULL);
        LineTo(hdc, 800, i * 20);
    }
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    // UI 그리기
    int baseX = 850, baseY = 50, tileSize = 40, spacing = 10;

    // Floor Tiles
    hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush); // 투명 브러시 선택
    TextOut(hdc, baseX, baseY, L"Floor Tiles:", 12);
    for (int i = 0; i < 4; i++) {
        int x = baseX + i * (tileSize + spacing);
        int y = baseY + 20;
        floorTile[i].StretchBlt(hdc, x, y, tileSize, tileSize, SRCCOPY);
        if (currentMode == EditMode::TILE_PLACE && selectedTile == "f" + std::to_string(i + 1)) {
            SelectObject(hdc, hRedPen);
            Rectangle(hdc, x, y, x + tileSize, y + tileSize);
            SelectObject(hdc, hOldPen);
        }
    }

    // Wall Tiles
    TextOut(hdc, baseX, baseY + 70, L"Wall Tiles:", 11);
    for (int i = 0; i < 4; i++) {
        int x = baseX + i * (tileSize + spacing);
        int y = baseY + 90;
        wallTile[i].StretchBlt(hdc, x, y, tileSize, tileSize, SRCCOPY);
        if (currentMode == EditMode::TILE_PLACE && selectedTile == "w" + std::to_string(i + 1)) {
            SelectObject(hdc, hRedPen);
            Rectangle(hdc, x, y, x + tileSize, y + tileSize);
            SelectObject(hdc, hOldPen);
        }
    }

    // Wall Corner Tiles
    TextOut(hdc, baseX, baseY + 140, L"Wall Corner Tiles:", 18);
    for (int i = 0; i < 10; i++) {
        int row = i / 5;
        int col = i % 5;
        int x = baseX + col * (tileSize + spacing);
        int y = baseY + 160 + row * (tileSize + spacing);
        wallConerTile[i].StretchBlt(hdc, x, y, tileSize, tileSize, SRCCOPY);
        if (currentMode == EditMode::TILE_PLACE && selectedTile == "wc" + std::to_string(i + 1)) {
            SelectObject(hdc, hRedPen);
            Rectangle(hdc, x, y, x + tileSize, y + tileSize);
            SelectObject(hdc, hOldPen);
        }
    }

    // Tile Eraser
    TextOut(hdc, baseX, baseY + 250, L"Tile Eraser:", 12);
    int tileEraserX = baseX;
    int tileEraserY = baseY + 270;
    Rectangle(hdc, tileEraserX, tileEraserY, tileEraserX + tileSize, tileEraserY + tileSize);
    TextOut(hdc, tileEraserX + 2, tileEraserY + 10, L"Erase", 5);
    if (currentMode == EditMode::TILE_ERASE) {
        SelectObject(hdc, hRedPen);
        Rectangle(hdc, tileEraserX, tileEraserY, tileEraserX + tileSize, tileEraserY + tileSize);
        SelectObject(hdc, hOldPen);
    }

    // Objects
    TextOut(hdc, baseX, baseY + 310, L"Objects:", 8);
    std::string objectNames[] = { "Archer", "SwordMan", "Wizard", "Candle", "IceChunk0", "IceChunk1", "IceFlag", "IceSmallChunk", "IceWindow0", "IceWindow1", "IceWindow2", "Jar", "Statue" };
    for (int i = 0; i < 13; i++) {
        int row = i / 5;
        int col = i % 5;
        int x = baseX + col * (tileSize + spacing);
        int y = baseY + 330 + row * (tileSize + spacing);
        Objects[i].StretchBlt(hdc, x, y, tileSize, tileSize, SRCCOPY);
        if (currentMode == EditMode::OBJECT_PLACE && selectedObject == objectNames[i]) {
            SelectObject(hdc, hRedPen);
            Rectangle(hdc, x, y, x + tileSize, y + tileSize);
            SelectObject(hdc, hOldPen);
        }
    }

    // Object Eraser
    TextOut(hdc, baseX, baseY + 470, L"Object Eraser:", 14);
    int objectEraserX = baseX;
    int objectEraserY = baseY + 490;
    Rectangle(hdc, objectEraserX, objectEraserY, objectEraserX + tileSize, objectEraserY + tileSize);
    TextOut(hdc, objectEraserX + 2, objectEraserY + 10, L"Erase", 5);
    if (currentMode == EditMode::OBJECT_ERASE) {
        SelectObject(hdc, hRedPen);
        Rectangle(hdc, objectEraserX, objectEraserY, objectEraserX + tileSize, objectEraserY + tileSize);
        SelectObject(hdc, hOldPen);
    }

    // Save, Cancel, Exit 버튼 렌더링 (마지막에 렌더링하여 덮이지 않도록)
    SelectObject(hdc, hButtonBrush); // 버튼은 회색 배경 유지
    Rectangle(hdc, mSaveButton.left, mSaveButton.top, mSaveButton.right, mSaveButton.bottom);
    TextOut(hdc, mSaveButton.left + 30, mSaveButton.top + 15, L"Save", 4);

    Rectangle(hdc, mCancelButton.left, mCancelButton.top, mCancelButton.right, mCancelButton.bottom);
    TextOut(hdc, mCancelButton.left + 25, mCancelButton.top + 15, L"Cancel", 6);

    Rectangle(hdc, mExitButton.left, mExitButton.top, mExitButton.right, mExitButton.bottom);
    TextOut(hdc, mExitButton.left + 30, mExitButton.top + 15, L"Exit", 4);
    SelectObject(hdc, hOldBrush);

    // 드래그 영역 표시
    if (drag) {
        int left = min(drawRect.left, drawRect.right) * 20;
        int top = min(drawRect.top, drawRect.bottom) * 20;
        int right = max(drawRect.left, drawRect.right) * 20;
        int bottom = max(drawRect.top, drawRect.bottom) * 20;
        SelectObject(hdc, hRedPen);
        SelectObject(hdc, hNullBrush); // 투명 브러시로 드래그 영역 테두리만 표시
        Rectangle(hdc, left, top, right, bottom);
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
    }

    DeleteObject(hButtonBrush);
    DeleteObject(hRedPen);
}

void MapTool::Update() {
    // 현재 맵 상태 저장 함수
    auto SaveMapState = [&]() {
        MapState state;
        for (int i = 0; i < 40; i++) {
            for (int j = 0; j < 40; j++) {
                state.map[i][j] = map[i][j];
                state.ImageMap[i][j] = ImageMap[i][j];
                state.ObjectMap[i][j] = ObjectMap[i][j];
            }
        }
        undoStack.push(state);
        };

    if (Input::GetKeyDown(eKeyCode::LButton)) {
        int mx = Input::GetMousePosition().x;
        int my = Input::GetMousePosition().y;
        int baseX = 850, baseY = 50, tileSize = 40, spacing = 10;

        // Save 버튼 클릭
        if (mx >= mSaveButton.left && mx <= mSaveButton.right && my >= mSaveButton.top && my <= mSaveButton.bottom) {
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
                return;
            }

            for (int i = 0; i < 40; i++) {
                for (int j = 0; j < 40; j++) {
                    fprintf(Mapfp, "%d ", map[j][i]);
                    fprintf(Imagefp, "%s ", ImageMap[j][i].c_str());
                    fprintf(Objectfp, "%s ", ObjectMap[j][i].c_str());
                }
                fprintf(Mapfp, "\n");
                fprintf(Imagefp, "\n");
                fprintf(Objectfp, "\n");
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
            return;
        }

        // Cancel 버튼 클릭
        if (mx >= mCancelButton.left && mx <= mCancelButton.right && my >= mCancelButton.top && my <= mCancelButton.bottom) {
            if (!undoStack.empty()) {
                MapState prevState = undoStack.top();
                undoStack.pop();
                for (int i = 0; i < 40; i++) {
                    for (int j = 0; j < 40; j++) {
                        map[i][j] = prevState.map[i][j];
                        ImageMap[i][j] = prevState.ImageMap[i][j];
                        ObjectMap[i][j] = prevState.ObjectMap[i][j];
                    }
                }
            }
            return;
        }

        // Exit 버튼 클릭
        if (mx >= mExitButton.left && mx <= mExitButton.right && my >= mExitButton.top && my <= mExitButton.bottom) {
            SceneManager::StartFadeIn();
            SceneManager::LoadScene(L"TitleScene");
            return;
        }

        if (mx >= 0 && mx < 800 && my >= 0 && my < 800) {
            // 맵 영역 (작업 전 상태 저장)
            if (currentMode == EditMode::TILE_PLACE || currentMode == EditMode::TILE_ERASE || currentMode == EditMode::OBJECT_ERASE) {
                SaveMapState();
                drag = true;
                drawRect.left = mx / 20;
                drawRect.top = my / 20;
                drawRect.right = drawRect.left + 1;
                drawRect.bottom = drawRect.top + 1;
            }
        }
        else {
            // UI 영역
            // Floor Tiles
            for (int i = 0; i < 4; i++) {
                int x = baseX + i * (tileSize + spacing);
                int y = baseY + 20;
                if (mx >= x && mx <= x + tileSize && my >= y && my <= y + tileSize) {
                    selectedTile = "f" + std::to_string(i + 1);
                    currentMode = EditMode::TILE_PLACE;
                }
            }

            // Wall Tiles
            for (int i = 0; i < 4; i++) {
                int x = baseX + i * (tileSize + spacing);
                int y = baseY + 90;
                if (mx >= x && mx <= x + tileSize && my >= y && my <= y + tileSize) {
                    selectedTile = "w" + std::to_string(i + 1);
                    currentMode = EditMode::TILE_PLACE;
                }
            }

            // Wall Corner Tiles
            for (int i = 0; i < 10; i++) {
                int row = i / 5;
                int col = i % 5;
                int x = baseX + col * (tileSize + spacing);
                int y = baseY + 160 + row * (tileSize + spacing);
                if (mx >= x && mx <= x + tileSize && my >= y && my <= y + tileSize) {
                    selectedTile = "wc" + std::to_string(i + 1);
                    currentMode = EditMode::TILE_PLACE;
                }
            }

            // Tile Eraser
            int tileEraserX = baseX;
            int tileEraserY = baseY + 270;
            if (mx >= tileEraserX && mx <= tileEraserX + tileSize && my >= tileEraserY && my <= tileEraserY + tileSize) {
                currentMode = EditMode::TILE_ERASE;
            }

            // Objects
            std::string objectNames[] = { "Archer", "SwordMan", "Wizard", "Candle", "IceChunk0", "IceChunk1", "IceFlag", "IceSmallChunk", "IceWindow0", "IceWindow1", "IceWindow2", "Jar", "Statue" };
            for (int i = 0; i < 13; i++) {
                int row = i / 5;
                int col = i % 5;
                int x = baseX + col * (tileSize + spacing);
                int y = baseY + 330 + row * (tileSize + spacing);
                if (mx >= x && mx <= x + tileSize && my >= y && my <= y + tileSize) {
                    selectedObject = objectNames[i];
                    currentMode = EditMode::OBJECT_PLACE;
                }
            }

            // Object Eraser
            int objectEraserX = baseX;
            int objectEraserY = baseY + 490;
            if (mx >= objectEraserX && mx <= objectEraserX + tileSize && my >= objectEraserY && my <= objectEraserY + tileSize) {
                currentMode = EditMode::OBJECT_ERASE;
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
                int left = min(drawRect.left, drawRect.right - 1);
                int right = max(drawRect.left, drawRect.right - 1);
                int top = min(drawRect.top, drawRect.bottom - 1);
                int bottom = max(drawRect.top, drawRect.bottom - 1);
                for (int i = left; i <= right; i++) {
                    for (int j = top; j <= bottom; j++) {
                        if (currentMode == EditMode::TILE_PLACE) {
                            if (selectedTile[0] == 'f') {
                                map[i][j] = 0;
                            }
                            else if (selectedTile[0] == 'w' || selectedTile.substr(0, 2) == "wc") {
                                map[i][j] = 1;
                            }
                            ImageMap[i][j] = selectedTile;
                        }
                        else if (currentMode == EditMode::TILE_ERASE) {
                            map[i][j] = 2;
                            ImageMap[i][j] = "e";
                        }
                        else if (currentMode == EditMode::OBJECT_ERASE) {
                            ObjectMap[i][j] = "empty";
                        }
                    }
                }
            }
            drag = false;
        }
        else if (currentMode == EditMode::OBJECT_PLACE) {
            int mx = Input::GetMousePosition().x / 20;
            int my = Input::GetMousePosition().y / 20;
            if (mx >= 0 && mx < 40 && my >= 0 && my < 40) {
                SaveMapState(); // 오브젝트 배치 전 상태 저장
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
}

void MapTool::LateUpdate() {
}