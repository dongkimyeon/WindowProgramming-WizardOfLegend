#pragma once
#include "Scene.h"
#include "MapManager.h"


class MapTool :
    public Scene
{
public:
    void Initialize();
    void Update();
    void LateUpdate();
    void Render();

private:
    int map[40][40];

};

