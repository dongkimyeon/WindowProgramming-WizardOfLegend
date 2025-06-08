#pragma once
#include "Scene.h"
#include "MapManager.h"
#include "CommonInclude.h"
#include "Input.h"
#include <stack>

// 편집 모드를 관리하기 위한 열거형 추가
enum class EditMode {
    NONE,         // 아무 작업도 하지 않음
    TILE_PLACE,   // 타일 배치
    TILE_ERASE,   // 타일 지우기
    OBJECT_PLACE, // 오브젝트 배치
    OBJECT_ERASE  // 오브젝트 지우기
};

// 편집 작업을 저장하기 위한 구조체
struct MapState {
    int map[40][40];
    std::string ImageMap[40][40];
    std::string ObjectMap[40][40];
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
    RECT mExitButton;
    RECT mCancelButton;
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
    EditMode currentMode; // 현재 모드를 저장하는 변수 추가
    std::stack<MapState> undoStack; // 취소 기능을 위한 스택
};