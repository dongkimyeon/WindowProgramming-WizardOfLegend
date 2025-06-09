#pragma once
#include "Scene.h"
#include "MapManager.h"
#include "CommonInclude.h"
#include "Input.h"
#include <stack>
#include <vector>
#include <string>

enum class EditMode {
    NONE,         // �ƹ� �۾��� ���� ����
    TILE_PLACE,   // Ÿ�� ��ġ
    TILE_ERASE,   // Ÿ�� �����
    OBJECT_PLACE, // ������Ʈ ��ġ
    OBJECT_ERASE, // ������Ʈ �����
    COPY,         // ���� ����
    PASTE         // ���� �ٿ��ֱ�
};

struct MapState {
    int map[40][40];
    std::string ImageMap[40][40];
    std::string ObjectMap[40][40];
};

struct CopyTile {
    int mapValue;
    std::string image;
    std::string object;
};

class MapTool : public Scene
{
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;

private:
    int map[40][40];
    bool drag;
    RECT drawRect;
    RECT mSaveButton;
    RECT mRedoButton;    // Redo ��ư
    RECT mCancelButton;  // Undo ��ư
    RECT mExitButton;
    RECT mCopyButton;
    RECT mPasteButton;
    std::string ImageMap[40][40];
    std::string ObjectMap[40][40];
    std::string selectedTile;
    std::string selectedObject;
    CImage wallTile[4];
    CImage wallConerTile[10];
    CImage EmptyTile;
    CImage floorTile[4];
    CImage Objects[13];
    FILE* Mapfp;
    FILE* Imagefp;
    FILE* Objectfp;
    EditMode currentMode; // ���� ��带 �����ϴ� ����
    std::stack<MapState> undoStack; // ��� ����� ���� ����
    std::stack<MapState> redoStack; // ����� ����� ���� ����
    std::vector<CopyTile> copyBuffer; // ����� Ÿ�ϰ� ������Ʈ ����
    int copyWidth; // ���� ������ �ʺ�
    int copyHeight; // ���� ������ ����
    RECT copyRect; // ����� ������ ��ǥ
};