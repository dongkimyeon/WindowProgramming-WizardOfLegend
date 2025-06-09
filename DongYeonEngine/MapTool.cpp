#define _CRT_SECURE_NO_WARNINGS
#include "MapTool.h"
#include "SceneManager.h"
#include <algorithm>
#include <vector>

#define GRID_SIZE 18
#define MAP_WIDTH 40
#define MAP_HEIGHT 40
#define MAP_PIXEL_WIDTH (MAP_WIDTH * GRID_SIZE)
#define MAP_PIXEL_HEIGHT (MAP_HEIGHT * GRID_SIZE)
#define UI_BASE_X 850
#define UI_BASE_Y 50
#define UI_TILE_SIZE 40
#define UI_SPACING 10
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 40
#define BUTTON_OFFSET_X 300
#define COPY_BUTTON_TOP 400
#define PASTE_BUTTON_TOP 450
#define SAVE_BUTTON_TOP 500
#define REDO_BUTTON_TOP 550
#define UNDO_BUTTON_TOP 600
#define EXIT_BUTTON_TOP 650
#define TEXT_OFFSET_X 30
#define TEXT_OFFSET_Y 15
#define ERASE_TEXT_OFFSET_X 2
#define ERASE_TEXT_OFFSET_Y 10
#define UI_FLOOR_Y_OFFSET 20
#define UI_WALL_Y_OFFSET 70
#define UI_WALL_CORNER_Y_OFFSET 140
#define UI_TILE_ERASER_Y_OFFSET 250
#define UI_OBJECT_Y_OFFSET 310
#define UI_OBJECT_ERASER_Y_OFFSET 470
#define UI_FLOOR_Y 20
#define UI_WALL_Y 90
#define UI_WALL_CORNER_Y 160
#define UI_TILE_ERASER_Y 270
#define UI_OBJECT_Y 330
#define UI_OBJECT_ERASER_Y 490

std::wstring const mChangeStageMapfp = L"StageBoss.txt";
std::wstring const mChangeStageImagefp = L"StageBossImage.txt";
std::wstring const mChangeStageObjectfp = L"StageBossObject.txt";

void MapTool::Initialize() {
    Mapfp = _wfopen(mChangeStageMapfp.c_str(), L"r");
    Imagefp = _wfopen(mChangeStageImagefp.c_str(), L"r");
    Objectfp = _wfopen(mChangeStageObjectfp.c_str(), L"r");

    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            map[i][j] = 0;
            ImageMap[i][j] = "f1";
            ObjectMap[i][j] = "empty";
        }
    }

    if (Mapfp) {
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
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
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
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
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
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
    copyBuffer.clear();
    copyRect = { 0, 0, 0, 0 };

    mCopyButton = { UI_BASE_X + BUTTON_OFFSET_X, COPY_BUTTON_TOP, UI_BASE_X + BUTTON_OFFSET_X + BUTTON_WIDTH, COPY_BUTTON_TOP + BUTTON_HEIGHT };
    mPasteButton = { UI_BASE_X + BUTTON_OFFSET_X, PASTE_BUTTON_TOP, UI_BASE_X + BUTTON_OFFSET_X + BUTTON_WIDTH, PASTE_BUTTON_TOP + BUTTON_HEIGHT };
    mSaveButton = { UI_BASE_X + BUTTON_OFFSET_X, SAVE_BUTTON_TOP, UI_BASE_X + BUTTON_OFFSET_X + BUTTON_WIDTH, SAVE_BUTTON_TOP + BUTTON_HEIGHT };
    mRedoButton = { UI_BASE_X + BUTTON_OFFSET_X, REDO_BUTTON_TOP, UI_BASE_X + BUTTON_OFFSET_X + BUTTON_WIDTH, REDO_BUTTON_TOP + BUTTON_HEIGHT };
    mCancelButton = { UI_BASE_X + BUTTON_OFFSET_X, UNDO_BUTTON_TOP, UI_BASE_X + BUTTON_OFFSET_X + BUTTON_WIDTH, UNDO_BUTTON_TOP + BUTTON_HEIGHT };
    mExitButton = { UI_BASE_X + BUTTON_OFFSET_X, EXIT_BUTTON_TOP, UI_BASE_X + BUTTON_OFFSET_X + BUTTON_WIDTH, EXIT_BUTTON_TOP + BUTTON_HEIGHT };

    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

void MapTool::Render(HDC hdc) {
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HBRUSH hButtonBrush = CreateSolidBrush(RGB(200, 200, 200));
    HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush;
    HPEN hOldPen;
    SetTextColor(hdc, RGB(0, 0, 0));

    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            int left = i * GRID_SIZE;
            int top = j * GRID_SIZE;
            int right = (i + 1) * GRID_SIZE;
            int bottom = (j + 1) * GRID_SIZE;
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
    for (int i = 0; i <= MAP_WIDTH; i++) {
        MoveToEx(hdc, i * GRID_SIZE, 0, NULL);
        LineTo(hdc, i * GRID_SIZE, MAP_PIXEL_HEIGHT);
        MoveToEx(hdc, 0, i * GRID_SIZE, NULL);
        LineTo(hdc, MAP_PIXEL_WIDTH, i * GRID_SIZE);
    }
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
    TextOut(hdc, UI_BASE_X, UI_BASE_Y, L"Floor Tiles:", 12);
    for (int i = 0; i < 4; i++) {
        int x = UI_BASE_X + i * (UI_TILE_SIZE + UI_SPACING);
        int y = UI_BASE_Y + UI_FLOOR_Y;
        floorTile[i].StretchBlt(hdc, x, y, UI_TILE_SIZE, UI_TILE_SIZE, SRCCOPY);
        if (currentMode == EditMode::TILE_PLACE && selectedTile == "f" + std::to_string(i + 1)) {
            SelectObject(hdc, hRedPen);
            Rectangle(hdc, x, y, x + UI_TILE_SIZE, y + UI_TILE_SIZE);
            SelectObject(hdc, hOldPen);
        }
    }

    TextOut(hdc, UI_BASE_X, UI_BASE_Y + UI_WALL_Y_OFFSET, L"Wall Tiles:", 11);
    for (int i = 0; i < 4; i++) {
        int x = UI_BASE_X + i * (UI_TILE_SIZE + UI_SPACING);
        int y = UI_BASE_Y + UI_WALL_Y;
        wallTile[i].StretchBlt(hdc, x, y, UI_TILE_SIZE, UI_TILE_SIZE, SRCCOPY);
        if (currentMode == EditMode::TILE_PLACE && selectedTile == "w" + std::to_string(i + 1)) {
            SelectObject(hdc, hRedPen);
            Rectangle(hdc, x, y, x + UI_TILE_SIZE, y + UI_TILE_SIZE);
            SelectObject(hdc, hOldPen);
        }
    }

    TextOut(hdc, UI_BASE_X, UI_BASE_Y + UI_WALL_CORNER_Y_OFFSET, L"Wall Corner Tiles:", 18);
    for (int i = 0; i < 10; i++) {
        int row = i / 5;
        int col = i % 5;
        int x = UI_BASE_X + col * (UI_TILE_SIZE + UI_SPACING);
        int y = UI_BASE_Y + UI_WALL_CORNER_Y + row * (UI_TILE_SIZE + UI_SPACING);
        wallConerTile[i].StretchBlt(hdc, x, y, UI_TILE_SIZE, UI_TILE_SIZE, SRCCOPY);
        if (currentMode == EditMode::TILE_PLACE && selectedTile == "wc" + std::to_string(i + 1)) {
            SelectObject(hdc, hRedPen);
            Rectangle(hdc, x, y, x + UI_TILE_SIZE, y + UI_TILE_SIZE);
            SelectObject(hdc, hOldPen);
        }
    }

    TextOut(hdc, UI_BASE_X, UI_BASE_Y + UI_TILE_ERASER_Y_OFFSET, L"Tile Eraser:", 12);
    int tileEraserX = UI_BASE_X;
    int tileEraserY = UI_BASE_Y + UI_TILE_ERASER_Y;
    Rectangle(hdc, tileEraserX, tileEraserY, tileEraserX + UI_TILE_SIZE, tileEraserY + UI_TILE_SIZE);
    TextOut(hdc, tileEraserX + ERASE_TEXT_OFFSET_X, tileEraserY + ERASE_TEXT_OFFSET_Y, L"Erase", 5);
    if (currentMode == EditMode::TILE_ERASE) {
        SelectObject(hdc, hRedPen);
        Rectangle(hdc, tileEraserX, tileEraserY, tileEraserX + UI_TILE_SIZE, tileEraserY + UI_TILE_SIZE);
        SelectObject(hdc, hOldPen);
    }

    TextOut(hdc, UI_BASE_X, UI_BASE_Y + UI_OBJECT_Y_OFFSET, L"Objects:", 8);
    std::string objectNames[] = { "Archer", "SwordMan", "Wizard", "Candle", "IceChunk0", "IceChunk1", "IceFlag", "IceSmallChunk", "IceWindow0", "IceWindow1", "IceWindow2", "Jar", "Statue" };
    for (int i = 0; i < 13; i++) {
        int row = i / 5;
        int col = i % 5;
        int x = UI_BASE_X + col * (UI_TILE_SIZE + UI_SPACING);
        int y = UI_BASE_Y + UI_OBJECT_Y + row * (UI_TILE_SIZE + UI_SPACING);
        Objects[i].StretchBlt(hdc, x, y, UI_TILE_SIZE, UI_TILE_SIZE, SRCCOPY);
        if (currentMode == EditMode::OBJECT_PLACE && selectedObject == objectNames[i]) {
            SelectObject(hdc, hRedPen);
            Rectangle(hdc, x, y, x + UI_TILE_SIZE, y + UI_TILE_SIZE);
            SelectObject(hdc, hOldPen);
        }
    }

    TextOut(hdc, UI_BASE_X, UI_BASE_Y + UI_OBJECT_ERASER_Y_OFFSET, L"Object Eraser:", 14);
    int objectEraserX = UI_BASE_X;
    int objectEraserY = UI_BASE_Y + UI_OBJECT_ERASER_Y;
    Rectangle(hdc, objectEraserX, objectEraserY, objectEraserX + UI_TILE_SIZE, objectEraserY + UI_TILE_SIZE);
    TextOut(hdc, objectEraserX + ERASE_TEXT_OFFSET_X, objectEraserY + ERASE_TEXT_OFFSET_Y, L"Erase", 5);
    if (currentMode == EditMode::OBJECT_ERASE) {
        SelectObject(hdc, hRedPen);
        Rectangle(hdc, objectEraserX, objectEraserY, objectEraserX + UI_TILE_SIZE, objectEraserY + UI_TILE_SIZE);
        SelectObject(hdc, hOldPen);
    }

    SelectObject(hdc, hButtonBrush);
    Rectangle(hdc, mCopyButton.left, mCopyButton.top, mCopyButton.right, mCopyButton.bottom);
    SIZE copyTextSize;
    GetTextExtentPoint32(hdc, L"Copy", 4, &copyTextSize);
    TextOut(hdc, mCopyButton.left + (mCopyButton.right - mCopyButton.left - copyTextSize.cx) / 2,
        mCopyButton.top + (mCopyButton.bottom - mCopyButton.top - copyTextSize.cy) / 2, L"Copy", 4);

    Rectangle(hdc, mPasteButton.left, mPasteButton.top, mPasteButton.right, mPasteButton.bottom);
    SIZE pasteTextSize;
    GetTextExtentPoint32(hdc, L"Paste", 5, &pasteTextSize);
    TextOut(hdc, mPasteButton.left + (mPasteButton.right - mPasteButton.left - pasteTextSize.cx) / 2,
        mPasteButton.top + (mPasteButton.bottom - mPasteButton.top - pasteTextSize.cy) / 2, L"Paste", 5);

    Rectangle(hdc, mSaveButton.left, mSaveButton.top, mSaveButton.right, mSaveButton.bottom);
    SIZE saveTextSize;
    GetTextExtentPoint32(hdc, L"Save", 4, &saveTextSize);
    TextOut(hdc, mSaveButton.left + (mSaveButton.right - mSaveButton.left - saveTextSize.cx) / 2,
        mSaveButton.top + (mSaveButton.bottom - mSaveButton.top - saveTextSize.cy) / 2, L"Save", 4);

    Rectangle(hdc, mRedoButton.left, mRedoButton.top, mRedoButton.right, mRedoButton.bottom);
    SIZE redoTextSize;
    GetTextExtentPoint32(hdc, L"Redo", 4, &redoTextSize);
    TextOut(hdc, mRedoButton.left + (mRedoButton.right - mRedoButton.left - redoTextSize.cx) / 2,
        mRedoButton.top + (mRedoButton.bottom - mRedoButton.top - redoTextSize.cy) / 2, L"Redo", 4);

    Rectangle(hdc, mCancelButton.left, mCancelButton.top, mCancelButton.right, mCancelButton.bottom);
    SIZE undoTextSize;
    GetTextExtentPoint32(hdc, L"Undo", 4, &undoTextSize);
    TextOut(hdc, mCancelButton.left + (mCancelButton.right - mCancelButton.left - undoTextSize.cx) / 2,
        mCancelButton.top + (mCancelButton.bottom - mCancelButton.top - undoTextSize.cy) / 2, L"Undo", 4);

    Rectangle(hdc, mExitButton.left, mExitButton.top, mExitButton.right, mExitButton.bottom);
    SIZE exitTextSize;
    GetTextExtentPoint32(hdc, L"Exit", 4, &exitTextSize);
    TextOut(hdc, mExitButton.left + (mExitButton.right - mExitButton.left - exitTextSize.cx) / 2,
        mExitButton.top + (mExitButton.bottom - mExitButton.top - exitTextSize.cy) / 2, L"Exit", 4);
    SelectObject(hdc, hOldBrush);

    if (!copyBuffer.empty()) {
        int left = copyRect.left * GRID_SIZE;
        int top = copyRect.top * GRID_SIZE;
        int right = copyRect.right * GRID_SIZE;
        int bottom = copyRect.bottom * GRID_SIZE;
        SelectObject(hdc, hRedPen);
        SelectObject(hdc, hNullBrush);
        Rectangle(hdc, left, top, right, bottom);
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
    }

    if (drag) {
        int left, top, right, bottom;
        if (currentMode == EditMode::PASTE && !copyBuffer.empty()) {
            left = drawRect.left * GRID_SIZE;
            top = drawRect.top * GRID_SIZE;
            right = (drawRect.left + copyWidth) * GRID_SIZE;
            bottom = (drawRect.top + copyHeight) * GRID_SIZE;
        }
        else {
            left = min(drawRect.left, drawRect.right) * GRID_SIZE;
            top = min(drawRect.top, drawRect.bottom) * GRID_SIZE;
            right = max(drawRect.left, drawRect.right) * GRID_SIZE;
            bottom = max(drawRect.top, drawRect.bottom) * GRID_SIZE;
        }
        SelectObject(hdc, hRedPen);
        SelectObject(hdc, hNullBrush);
        Rectangle(hdc, left, top, right, bottom);
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
    }

    DeleteObject(hButtonBrush);
    DeleteObject(hRedPen);
}

void MapTool::Update() {
    auto SaveMapState = [&]() {
        MapState state;
        for (int i = 0; i < MAP_WIDTH; i++) {
            for (int j = 0; j < MAP_HEIGHT; j++) {
                state.map[i][j] = map[i][j];
                state.ImageMap[i][j] = ImageMap[i][j];
                state.ObjectMap[i][j] = ObjectMap[i][j];
            }
        }
        undoStack.push(state);
        while (!redoStack.empty()) {
            redoStack.pop();
        }
        };

    if (Input::GetKeyDown(eKeyCode::LButton)) {
        int mx = Input::GetMousePosition().x;
        int my = Input::GetMousePosition().y;

        if (mx >= mCopyButton.left && mx <= mCopyButton.right && my >= mCopyButton.top && my <= mCopyButton.bottom) {
            currentMode = EditMode::COPY;
            copyBuffer.clear();
            return;
        }

        if (mx >= mPasteButton.left && mx <= mPasteButton.right && my >= mPasteButton.top && my <= mPasteButton.bottom) {
            if (!copyBuffer.empty()) {
                currentMode = EditMode::PASTE;
            }
            return;
        }

        if (mx >= mSaveButton.left && mx <= mSaveButton.right && my >= mSaveButton.top && my <= mSaveButton.bottom) {
            Mapfp = _wfopen(mChangeStageMapfp.c_str(), L"w");
            Imagefp = _wfopen(mChangeStageImagefp.c_str(), L"w");
            Objectfp = _wfopen(mChangeStageObjectfp.c_str(), L"w");

            if (!Mapfp || !Imagefp || !Objectfp) {
                if (Mapfp) fclose(Mapfp);
                if (Imagefp) fclose(Imagefp);
                if (Objectfp) fclose(Objectfp);
                Mapfp = nullptr;
                Imagefp = nullptr;
                Objectfp = nullptr;
                return;
            }

            for (int i = 0; i < MAP_HEIGHT; i++) {
                for (int j = 0; j < MAP_WIDTH; j++) {
                    fwprintf(Mapfp, L"%d ", map[j][i]);
                    fwprintf(Imagefp, L"%S ", ImageMap[j][i].c_str());
                    fwprintf(Objectfp, L"%S ", ObjectMap[j][i].c_str());
                }
                fwprintf(Mapfp, L"\n");
                fwprintf(Imagefp, L"\n");
                fwprintf(Objectfp, L"\n");
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

        if (mx >= mRedoButton.left && mx <= mRedoButton.right && my >= mRedoButton.top && my <= mRedoButton.bottom) {
            if (!redoStack.empty()) {
                MapState currentState;
                for (int i = 0; i < MAP_WIDTH; i++) {
                    for (int j = 0; j < MAP_HEIGHT; j++) {
                        currentState.map[i][j] = map[i][j];
                        currentState.ImageMap[i][j] = ImageMap[i][j];
                        currentState.ObjectMap[i][j] = ObjectMap[i][j];
                    }
                }
                undoStack.push(currentState);

                MapState nextState = redoStack.top();
                redoStack.pop();
                for (int i = 0; i < MAP_WIDTH; i++) {
                    for (int j = 0; j < MAP_HEIGHT; j++) {
                        map[i][j] = nextState.map[i][j];
                        ImageMap[i][j] = nextState.ImageMap[i][j];
                        ObjectMap[i][j] = nextState.ObjectMap[i][j];
                    }
                }
            }
            return;
        }

        if (mx >= mCancelButton.left && mx <= mCancelButton.right && my >= mCancelButton.top && my <= mCancelButton.bottom) {
            if (!undoStack.empty()) {
                MapState currentState;
                for (int i = 0; i < MAP_WIDTH; i++) {
                    for (int j = 0; j < MAP_HEIGHT; j++) {
                        currentState.map[i][j] = map[i][j];
                        currentState.ImageMap[i][j] = ImageMap[i][j];
                        currentState.ObjectMap[i][j] = ObjectMap[i][j];
                    }
                }
                redoStack.push(currentState);

                MapState prevState = undoStack.top();
                undoStack.pop();
                for (int i = 0; i < MAP_WIDTH; i++) {
                    for (int j = 0; j < MAP_HEIGHT; j++) {
                        map[i][j] = prevState.map[i][j];
                        ImageMap[i][j] = prevState.ImageMap[i][j];
                        ObjectMap[i][j] = prevState.ObjectMap[i][j];
                    }
                }
            }
            return;
        }

        if (mx >= mExitButton.left && mx <= mExitButton.right && my >= mExitButton.top && my <= mExitButton.bottom) {
            Mapfp = _wfopen(mChangeStageMapfp.c_str(), L"w");
            Imagefp = _wfopen(mChangeStageImagefp.c_str(), L"w");
            Objectfp = _wfopen(mChangeStageObjectfp.c_str(), L"w");

            if (!Mapfp || !Imagefp || !Objectfp) {
                if (Mapfp) fclose(Mapfp);
                if (Imagefp) fclose(Imagefp);
                if (Objectfp) fclose(Objectfp);
                Mapfp = nullptr;
                Imagefp = nullptr;
                Objectfp = nullptr;
                return;
            }

            for (int i = 0; i < MAP_HEIGHT; i++) {
                for (int j = 0; j < MAP_WIDTH; j++) {
                    fwprintf(Mapfp, L"%d ", map[j][i]);
                    fwprintf(Imagefp, L"%S ", ImageMap[j][i].c_str());
                    fwprintf(Objectfp, L"%S ", ObjectMap[j][i].c_str());
                }
                fwprintf(Mapfp, L"\n");
                fwprintf(Imagefp, L"\n");
                fwprintf(Objectfp, L"\n");
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

            SceneManager::StartFadeIn();
            SceneManager::LoadScene(L"TitleScene");
            return;
        }

        if (mx >= 0 && mx < MAP_PIXEL_WIDTH && my >= 0 && my < MAP_PIXEL_HEIGHT) {
            if (currentMode == EditMode::TILE_PLACE || currentMode == EditMode::TILE_ERASE ||
                currentMode == EditMode::OBJECT_ERASE || currentMode == EditMode::COPY ||
                currentMode == EditMode::PASTE) {
                if (currentMode != EditMode::COPY && currentMode != EditMode::PASTE) {
                    SaveMapState();
                }
                drag = true;
                drawRect.left = mx / GRID_SIZE;
                drawRect.top = my / GRID_SIZE;
                if (currentMode == EditMode::PASTE) {
                    drawRect.right = drawRect.left + copyWidth;
                    drawRect.bottom = drawRect.top + copyHeight;
                }
                else {
                    drawRect.right = drawRect.left + 1;
                    drawRect.bottom = drawRect.top + 1;
                }
            }
        }
        else {
            for (int i = 0; i < 4; i++) {
                int x = UI_BASE_X + i * (UI_TILE_SIZE + UI_SPACING);
                int y = UI_BASE_Y + UI_FLOOR_Y;
                if (mx >= x && mx <= x + UI_TILE_SIZE && my >= y && my <= y + UI_TILE_SIZE) {
                    selectedTile = "f" + std::to_string(i + 1);
                    currentMode = EditMode::TILE_PLACE;
                }
            }

            for (int i = 0; i < 4; i++) {
                int x = UI_BASE_X + i * (UI_TILE_SIZE + UI_SPACING);
                int y = UI_BASE_Y + UI_WALL_Y;
                if (mx >= x && mx <= x + UI_TILE_SIZE && my >= y && my <= y + UI_TILE_SIZE) {
                    selectedTile = "w" + std::to_string(i + 1);
                    currentMode = EditMode::TILE_PLACE;
                }
            }

            for (int i = 0; i < 10; i++) {
                int row = i / 5;
                int col = i % 5;
                int x = UI_BASE_X + col * (UI_TILE_SIZE + UI_SPACING);
                int y = UI_BASE_Y + UI_WALL_CORNER_Y + row * (UI_TILE_SIZE + UI_SPACING);
                if (mx >= x && mx <= x + UI_TILE_SIZE && my >= y && my <= y + UI_TILE_SIZE) {
                    selectedTile = "wc" + std::to_string(i + 1);
                    currentMode = EditMode::TILE_PLACE;
                }
            }

            int tileEraserX = UI_BASE_X;
            int tileEraserY = UI_BASE_Y + UI_TILE_ERASER_Y;
            if (mx >= tileEraserX && mx <= tileEraserX + UI_TILE_SIZE && my >= tileEraserY && my <= tileEraserY + UI_TILE_SIZE) {
                currentMode = EditMode::TILE_ERASE;
            }

            std::string objectNames[] = { "Archer", "SwordMan", "Wizard", "Candle", "IceChunk0", "IceChunk1", "IceFlag", "IceSmallChunk", "IceWindow0", "IceWindow1", "IceWindow2", "Jar", "Statue" };
            for (int i = 0; i < 13; i++) {
                int row = i / 5;
                int col = i % 5;
                int x = UI_BASE_X + col * (UI_TILE_SIZE + UI_SPACING);
                int y = UI_BASE_Y + UI_OBJECT_Y + row * (UI_TILE_SIZE + UI_SPACING);
                if (mx >= x && mx <= x + UI_TILE_SIZE && my >= y && my <= y + UI_TILE_SIZE) {
                    selectedObject = objectNames[i];
                    currentMode = EditMode::OBJECT_PLACE;
                }
            }

            int objectEraserX = UI_BASE_X;
            int objectEraserY = UI_BASE_Y + UI_OBJECT_ERASER_Y;
            if (mx >= objectEraserX && mx <= objectEraserX + UI_TILE_SIZE && my >= objectEraserY && my <= objectEraserY + UI_TILE_SIZE) {
                currentMode = EditMode::OBJECT_ERASE;
            }
        }
    }
    else if (Input::GetKeyUp(eKeyCode::LButton)) {
        if (drag) {
            int mx = Input::GetMousePosition().x / GRID_SIZE;
            int my = Input::GetMousePosition().y / GRID_SIZE;
            if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT) {
                if (currentMode != EditMode::PASTE) {
                    drawRect.right = mx + 1;
                    drawRect.bottom = my + 1;
                }
                int left = min(drawRect.left, drawRect.right - 1);
                int right = max(drawRect.left, drawRect.right - 1);
                int top = min(drawRect.top, drawRect.bottom - 1);
                int bottom = max(drawRect.top, drawRect.bottom - 1);

                if (currentMode == EditMode::COPY) {
                    copyBuffer.clear();
                    for (int i = left; i <= right; i++) {
                        for (int j = top; j <= bottom; j++) {
                            if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                                CopyTile tile;
                                tile.mapValue = map[i][j];
                                tile.image = ImageMap[i][j];
                                tile.object = ObjectMap[i][j];
                                copyBuffer.push_back(tile);
                            }
                        }
                    }
                    copyWidth = right - left + 1;
                    copyHeight = bottom - top + 1;
                    copyRect = { left, top, right + 1, bottom + 1 };
                    currentMode = EditMode::NONE;
                }
                else if (currentMode == EditMode::PASTE && !copyBuffer.empty()) {
                    SaveMapState();
                    int pasteIndex = 0;
                    for (int j = 0; j < copyHeight && pasteIndex < copyBuffer.size(); j++) {
                        for (int i = 0; i < copyWidth && pasteIndex < copyBuffer.size(); i++) {
                            int pasteX = drawRect.left + i;
                            int pasteY = drawRect.top + j;
                            if (pasteX >= 0 && pasteX < MAP_WIDTH && pasteY >= 0 && pasteY < MAP_HEIGHT) {
                                map[pasteX][pasteY] = copyBuffer[pasteIndex].mapValue;
                                ImageMap[pasteX][pasteY] = copyBuffer[pasteIndex].image;
                                ObjectMap[pasteX][pasteY] = copyBuffer[pasteIndex].object;
                            }
                            pasteIndex++;
                        }
                    }
                    currentMode = EditMode::NONE;
                }
                else if (currentMode == EditMode::TILE_PLACE) {
                    for (int i = left; i <= right; i++) {
                        for (int j = top; j <= bottom; j++) {
                            if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                                if (selectedTile[0] == 'f') {
                                    map[i][j] = 0;
                                }
                                else if (selectedTile[0] == 'w' || selectedTile.substr(0, 2) == "wc") {
                                    map[i][j] = 1;
                                }
                                ImageMap[i][j] = selectedTile;
                            }
                        }
                    }
                }
                else if (currentMode == EditMode::TILE_ERASE) {
                    for (int i = left; i <= right; i++) {
                        for (int j = top; j <= bottom; j++) {
                            if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                                map[i][j] = 2;
                                ImageMap[i][j] = "e";
                            }
                        }
                    }
                }
                else if (currentMode == EditMode::OBJECT_ERASE) {
                    for (int i = left; i <= right; i++) {
                        for (int j = top; j <= bottom; j++) {
                            if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                                ObjectMap[i][j] = "empty";
                            }
                        }
                    }
                }
            }
            drag = false;
        }
        else if (currentMode == EditMode::OBJECT_PLACE) {
            int mx = Input::GetMousePosition().x / GRID_SIZE;
            int my = Input::GetMousePosition().y / GRID_SIZE;
            if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT) {
                SaveMapState();
                ObjectMap[mx][my] = selectedObject;
            }
        }
        else if (currentMode == EditMode::OBJECT_ERASE) {
            int mx = Input::GetMousePosition().x / GRID_SIZE;
            int my = Input::GetMousePosition().y / GRID_SIZE;
            if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT) {
                SaveMapState();
                ObjectMap[mx][my] = "empty";
            }
        }
    }
    else if (Input::GetKey(eKeyCode::LButton)) {
        if (drag && currentMode != EditMode::PASTE) {
            int mx = Input::GetMousePosition().x / GRID_SIZE;
            int my = Input::GetMousePosition().y / GRID_SIZE;
            if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT) {
                drawRect.right = mx + 1;
                drawRect.bottom = my + 1;
            }
        }
        else if (currentMode == EditMode::PASTE && !copyBuffer.empty()) {
            int mx = Input::GetMousePosition().x / GRID_SIZE;
            int my = Input::GetMousePosition().y / GRID_SIZE;
            if (mx >= 0 && mx < MAP_WIDTH && my >= 0 && my < MAP_HEIGHT) {
                drawRect.left = mx;
                drawRect.top = my;
                drawRect.right = mx + copyWidth;
                drawRect.bottom = my + copyHeight;
            }
        }
    }
}

void MapTool::LateUpdate() {
}