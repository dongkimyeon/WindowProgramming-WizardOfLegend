#pragma once
#include "CommonInclude.h"
#include "Player.h"
#include <vector>

class Scene;

class Player_Skill_FireBall
{
public:
    Player_Skill_FireBall(float x, float y, float dirX, float dirY);
    ~Player_Skill_FireBall();

    void Update(GameObject& obj);
    void Render(HDC hdc);

    static void Active(float mX, float mY, float angle, Scene* stage);

    bool IsActive() const { return mIsActive; }


    POINT* GetHitboxPoints() { return hitboxPoints; }

    float GetPositionX() const { return mX; }
    float GetPositionY() const { return mY; }

    // 파티클 관련 함수
    void SpawnParticle();
    void UpdateParticles();

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

    CImage mFireBallAnimation[5];     // 파이어볼 애니메이션 이미지
    POINT hitboxPoints[4];            // 히트박스 좌표
    float mX;                         // 현재 x 위치
    float mY;                         // 현재 y 위치
    float mDirectionX;                // 이동 방향 x
    float mDirectionY;                // 이동 방향 y
    float speed;                      // 이동 속도
    bool mIsActive;                   // 활성화 여부
    int damage;                       // 데미지
    int mCurrentFrame;                // 현재 애니메이션 프레임
    float mAnimationTimer;            // 애니메이션 타이머
    const float mFrameDuration = 0.1f; // 프레임 지속 시간

    void UpdateHitbox();
    bool CheckCollisionWithRect(const RECT& rect);
};