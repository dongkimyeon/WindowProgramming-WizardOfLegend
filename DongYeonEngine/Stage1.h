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

class Stage1 : public Scene 
{
private:
    // 파티클 구조체
    struct Particle {
        float x, y;           // 파티클 위치
        float velX, velY;     // 파티클 속도
        float lifetime;       // 파티클 수명
        int frame;            // 애니메이션 프레임
        float initialLifetime; // 초기 수명
    };

    std::vector<Particle> mParticles; // 파티클 리스트
    float mParticleTimer;             // 파티클 생성 타이머
    float mParticleSpawnInterval;     // 파티클 생성 간격
    CImage mFireParticleImage[20];    // 파티클 이미지 배열


    CImage mPlayerSpawnPoint;

    std::vector<Wizard*> wizards;
    std::vector<SwordMan*> swordmans;
    std::vector<Archer*> archers;
    std::vector<Arrow*> arrows;
    std::vector<FireBall*> fireballs;
    std::vector<Player_Skill_FireBall*> playerFireballs;
    std::vector<Player_Skill_FireDragon*> playerFireDragon;
    
    Portal portal;
    Camera camera;

public:

    void Initialize() override;
    void ObjectInitialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;

    void AddArrow(Arrow* arrow) override { arrows.push_back(arrow); }
    void AddFireBall(FireBall* fireball) override { fireballs.push_back(fireball); }
    void AddPlayerSkillFireBall(Player_Skill_FireBall* fireball) override { playerFireballs.push_back(fireball); }
    void AddPlayerSkillFireDragon(Player_Skill_FireDragon* fireDragon) override { playerFireDragon.push_back(fireDragon); }
    void HandleCollision();
    void ResolveCollision(GameObject& obj1, GameObject& obj2);
    void HandleCollisionMap(int (*map)[40], GameObject& obj); // 수정: Tile** -> int (*)[40]
    void ResolveCollisionMap(RECT wallRect, GameObject& obj); // 수정: Tile& -> RECT
    void CreateFireParticles(std::vector<Particle>& particles, float x, float y);


};