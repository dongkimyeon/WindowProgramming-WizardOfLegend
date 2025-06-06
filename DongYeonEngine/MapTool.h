#pragma once
#include "Scene.h"
#include "MapManager.h"
#include "CommonInclude.h"
#include "Input.h"

class MapTool :
    public Scene
{
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;

private:
    int map[40][40];
    bool drag;
    bool floordrag;
    bool walldrag;
    bool emptydrag;
    bool ObjectCLK;
    RECT drawRect;
    std::string ImageMap[40][40];
    std::string ObjectMap[40][40];
    std::string selectedTile;
    std::string selectedObject;
    CImage wallTile[4];
    CImage wallConerTile[10]; // Increased to 10CImage buffer;
    CImage EmptyTile; 
    CImage floorTile[4]; 
    CImage Objects[13];
    FILE* Mapfp;
    FILE* Imagefp;
    FILE* Objectfp;
};  