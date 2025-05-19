#pragma once
#include "Scene.h"
#include "Player.h"
#include "Wizard.h"
#include "SwordMan.h"
#include "Archer.h"
#include "Arrow.h"
#include "FireBall.h"
#include "MapManager.h"
#include "Camera.h"
#include <vector>

class Stage1 : public Scene {
public:
    Stage1();
    ~Stage1();
    void Initialize();
    void Update();
    void LateUpdate();
    void Render(HDC hdc);

    void AddArrow(Arrow* arrow) { arrows.push_back(arrow); }
    void AddFireBall(FireBall* fireball) { fireballs.push_back(fireball); }

    void HandleCollision();
    void ResolveCollision(GameObject& obj1, GameObject& obj2);
private:
    Player player;
    std::vector<Wizard*> wizards;
    std::vector<SwordMan*> swordmans;
    std::vector<Archer*> archers;
    std::vector<Arrow*> arrows;
    std::vector<FireBall*> fireballs;
    Camera camera; // Add camera
};