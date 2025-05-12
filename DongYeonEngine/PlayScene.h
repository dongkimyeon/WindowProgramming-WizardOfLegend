#pragma once
#include "Scene.h"
#include "Player.h"


class PlayScene : public Scene {
private:
    Player player;


public:
    PlayScene();
    ~PlayScene();
    void Initialize();
    void Update();
    void LateUpdate();
    void Render(HDC hdc);
};