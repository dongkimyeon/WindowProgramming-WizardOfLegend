#pragma once
#include "CommonInclude.h"
#include "Player.h"

class Scene;

class BossSkill_AquaBall
{
public:
    BossSkill_AquaBall(float x, float y, float targetX, float targetY);
    ~BossSkill_AquaBall();

    void Update(Player& player);
    void Render(HDC hdc, Player& player);

    static void ThrowAquaBall(Player& player, float mX, float mY, Scene* stage);

    bool IsActive() const { return mIsActive; }
    void SetActive(bool active) { mIsActive = active; }

    POINT* GetHitboxPoints() { return hitboxPoints; }

    float GetPositionX() const { return mX; }
    float GetPositionY() const { return mY; }

private:
    CImage AquaBallAnimation[28];
    CImage AquaBallFallingPointAnimation[25];
    CImage AquaBallExplosionEffectAnimation[7];

    POINT hitboxPoints[4];
    float mX, mY; // 현재 위치
    float mTargetX, mTargetY; // 목표 위치
    float mInitialX, mInitialY; // 시작 위치
    float speed;
    bool mIsActive;
    int damage;
    float mTime; // 포물선 이동 시간
    float mHeight; // 포물선 높이
    int mCurrentFrame; // 현재 애니메이션 프레임
    float mAnimationTimer; // 애니메이션 타이머
    bool mIsFalling; // 낙하 상태
    bool mIsExploding; // 폭발 상태
    bool mIsTargetIndicating;
    int mTargetIndicatorFrame;
    int mAquaBallFrame;
    int mExplosionFrame;
    void UpdateHitbox();
    bool CheckCollisionWithRect(const RECT& rect);
};