#pragma once
#include "Scene.h"
#include "Player.h"
#include "Wizard.h"
#include "SwordMan.h"
#include "Archer.h"

class Stage1 : public Scene {
public:
    Stage1();
    ~Stage1();
    void Initialize();
    void Update();
    void LateUpdate();
    void Render(HDC hdc);
private:
    Player player;
    Wizard wizard;
    SwordMan swordman;
    Archer archer;

};