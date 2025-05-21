#pragma once
#include "Scene.h"
#include "Player.h"
#include "Wizard.h"
#include "SwordMan.h"
#include "Archer.h"
#include "Arrow.h"
#include "FireBall.h"
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

private:
    Player player;
    std::vector<Wizard*> wizards;    // ������ ��ü ����
    std::vector<SwordMan*> swordmans;// �ҵ�� ��ü ����
    std::vector<Archer*> archers;    // �ü� ��ü ����
    std::vector<Arrow*> arrows;      // ȭ�� ��ü ����
    std::vector<FireBall*> fireballs;// ���̾ ��ü ����
  
};