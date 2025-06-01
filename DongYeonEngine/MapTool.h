#pragma once
#include "Scene.h"
#include "MapManager.h"
#include "CommonInclude.h"


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
    char ObjectMap[40][40];
    std::string ImageMap[40][40];
    CImage wallTile[4];
    CImage wallConerTile[10]; // Increased to 10CImage buffer;
    CImage EmptyTile; 
    CImage floorTile[4]; 
    FILE* Mapfp;
    FILE* Imagefp;
};  