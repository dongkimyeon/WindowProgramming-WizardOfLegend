#pragma once
#include "CommonInclude.h"
#include "Player.h"
#include <vector>

class Scene;
class Player;

class Player_Skill_FireDragon
{
public:
    // 생성자: 초기 위치(x, y), 방향(dirX, dirY), 위상 오프셋(phaseOffset)을 설정
    Player_Skill_FireDragon(float x, float y, float dirX, float dirY, float phaseOffset);
    ~Player_Skill_FireDragon();

    // 업데이트 및 렌더링 함수
    void Move(); // 이동 로직
    bool CheckCollision(GameObject& obj); // 충돌 체크
    void Render(HDC hdc);

    // 정적 함수: 파이어 드래곤을 활성화하여 씬에 추가
    static void Active(float mX, float mY, float angle, Scene* stage, Player* player, bool triggerFire = false);

    // 상태 확인
    bool IsActive() const { return mIsActive; }

    // 상태 변경
    void SetActive(bool active) { mIsActive = active; }

    // 히트박스 및 위치 정보 반환
    POINT* GetHitboxPoints() { return hitboxPoints; }
    float GetPositionX() const { return mX; }
    float GetPositionY() const { return mY; }
    int GetDamage() const { return damage; } // 데미지 반환

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
        float initialLifetime;
    };

    std::vector<Particle> mParticles; // 파티클 리스트
    float mParticleTimer;             // 파티클 생성 타이머
    float mParticleSpawnInterval;     // 파티클 생성 간격
    CImage mFireParticleImage[20];    // 파티클 이미지 배열

    CImage mFireDragonLeftImage;      // 왼쪽 방향 파이어 드래곤 이미지
    CImage mFireDragonRightImage;     // 오른쪽 방향 파이어 드래곤 이미지

    POINT hitboxPoints[4];            // 히트박스 좌표
    float mX;                         // 현재 x 위치
    float mY;                         // 현재 y 위치
    float mDirectionX;                // 이동 방향 x
    float mDirectionY;                // 이동 방향 y
    float speed;                      // 이동 속도
    bool mIsActive;                   // 활성화 여부
    int damage;                       // 데미지
    float mWaveTime;                  // 물결 효과를 위한 시간 변수
    float phaseOffset;                // 사인파 위상 오프셋
    float mInstantDirX;               // 순간적인 x축 속도
    float mInstantDirY;               // 순간적인 y축 속도
    int mCurrentFrame;                // 현재 애니메이션 프레임
    float mAnimationTimer;            // 애니메이션 타이머
    const float mFrameDuration = 0.1f; // 프레임 지속 시간

    // 내부 함수
    void UpdateHitbox();              // 히트박스 업데이트
    bool CheckCollisionWithRect(const RECT& rect); // 충돌 체크
};