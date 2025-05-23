#pragma once
#include "GameObject.h"
#include "Player.h"

class Scene;

class Archer : public GameObject
{
public:
    Archer();
    ~Archer();
    enum EnemyState { LEFT, RIGHT };
    void Update(Player& p, Scene* stage);
    void LateUpdate() override;
    void Render(HDC hdc, Player& p);
    void SetPosition(float x, float y) override;

    void TakeDamage(int d);

    EnemyState GetState() { return state; }
    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    float GetSpeed() override { return speed; }
    RECT GetRect() override { return rect; }
    bool GetIsDead() override { return mIsDead; }

    bool HasBeenHit() const { return mHasBeenHit; }
    void SetHitFlag(bool hit) { mHasBeenHit = hit; }

private:
    float mX;
    float mY;
    Vector2 position;
    int hp;

    CImage mRightIdleAnimation;
    CImage mRightAttackAnimation[4];
    CImage mRightDieAnimaion[6];
    CImage mRightHitAnimation[2];
    CImage mRightWalkAnimation[5];

    CImage mLeftIdleAnimation;
    CImage mLeftAttackAnimation[4];
    CImage mLeftDieAnimaion[6];
    CImage mLeftHitAnimation[2];
    CImage mLeftWalkAnimation[5];

    CImage mBowAttackAnimation[4];

    CImage mHitEffectAnimation[4];
    float mHitEffectAngle;

    EnemyState state = EnemyState::RIGHT;

    bool mIsMoving;
    int mCurrentWalkFrame;
    bool mIsAttack;
    int mCurrenAttackFrame;
    bool mIsDead;
    int mCurrentDeadFrame;
    bool mIsHit;
    int mCurrentHitFrame;

    float PlayerDetectRange = 300.0f;
    float AttackRange = 250.0f;

    float mHitTimer;

    float speed;
    RECT rect;

    float mAttackFrameTime;
    float mAttackCooldown;

    bool mHasBeenHit = false; // 피격 플래그

    // 데미지 표시 변수
    int mDamageTaken;           // 받은 데미지 양
    float mDamageTextY;         // 데미지 텍스트의 Y 위치
    float mDamageTextSpeed;     // 텍스트의 상승 속도
    bool mShowDamage;           // 데미지 텍스트 표시 플래그

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
};