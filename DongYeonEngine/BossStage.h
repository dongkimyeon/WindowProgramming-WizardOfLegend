#pragma once
#include "Scene.h"
#include "Player.h"
#include "Wizard.h"
#include "SwordMan.h"
#include "Archer.h"
#include "Arrow.h"
#include "FireBall.h"
#include "MapManager.h"
#include "Portal.h"
#include "Camera.h"
#include "Player_Skill_FireBall.h"
#include "Player_Skill_FireDragon.h"
#include "BossSkill_Spear.h"    
#include "BossSkill_AquaBall.h"
#include "Boss.h"

class BossStage : public Scene
{
public:

    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;
    void AddArrow(Arrow* arrow) override { arrows.push_back(arrow); }
    void AddFireBall(FireBall* fireball) override
    {
        fireballs.push_back(fireball);
    }
    void AddPlayerSkillFireBall(Player_Skill_FireBall* fireball) override
    {
        playerFireballs.push_back(fireball);
    }
    void AddPlayerSkillFireDragon(Player_Skill_FireDragon* fireDragon) override
    {
        playerFireDragon.push_back(fireDragon);
    }
    void AddBossSkillIceSpear(BossSkill_Spear* spear) override
    {
        spears.push_back(spear);
    }
    void AddBossSkillAquaBall(BossSkill_AquaBall* aquaball) override
    {
        aquaBalls.push_back(aquaball);
    }
    
    void HandleCollision();
    void ResolveCollision(GameObject& obj1, GameObject& obj2);
    void HandleCollisionMap(int (*map)[40], GameObject& obj); // 수정: Tile** -> int (*)[40]
    void ResolveCollisionMap(RECT wallRect, GameObject& obj); // 수정: Tile& -> REC
private:
    std::vector<Wizard*> wizards;
    std::vector<SwordMan*> swordmans;
    std::vector<Archer*> archers;
    std::vector<Arrow*> arrows;
    std::vector<FireBall*> fireballs;
    std::vector<Player_Skill_FireBall*> playerFireballs;
    std::vector<Player_Skill_FireDragon*> playerFireDragon;
    std::vector<BossSkill_Spear*> spears;
    std::vector<BossSkill_AquaBall*> aquaBalls;
    bool portalCreate;
    Boss iceBoss;
    Portal portal;
    Camera camera;

};

