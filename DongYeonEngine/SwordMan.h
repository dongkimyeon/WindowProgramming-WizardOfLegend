#pragma once
#include "GameObject.h"
#include "Player.h"

class SwordMan : public GameObject
{
public:
    SwordMan();
    ~SwordMan(); // GDI+ 종료를 위한 소멸자
    enum EnemyState { LEFT, RIGHT };
    void Update(Player& p);
    void LateUpdate();
    void Render(HDC hdc, Player& p);
    void SetPosition(float x, float y) override;

    void TakeDamage(int d);

    int GetDamage() { return damage; }
    int GetHp() { return hp; }
    EnemyState GetState() { return state; }
    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    float GetSpeed() override { return speed; }
    bool GetIsDead() override  { return mIsDead; }

    bool GetEffectHitbox(POINT outPoints[4]);
    bool CheckCollisionWithRect(RECT& otherRect); // 충돌 감지 메서드

    RECT GetRect() override { return rect; }

    // 공격 플래그 Getter와 Setter
    bool HasAttackedPlayer() const { return mHasAttackedPlayer; }
    void ResetAttackFlag() { mHasAttackedPlayer = false; }


    bool HasBeenHit() const { return mHasBeenHit; }
    void SetHitFlag(bool hit) { mHasBeenHit = hit; }

private:
    float mX;
    float mY;
    int hp;
    int damage;
    Vector2 position;

    CImage mShadowImage;
    CImage mRightIdleAnimation;
    CImage mRightAttackAnimation[3];
    CImage mRightDieAnimaion[6];
    CImage mRightHitAnimation[2];
    CImage mRightWalkAnimation[6];
    CImage mLeftIdleAnimation;
    CImage mLeftAttackAnimation[3];
    CImage mLeftDieAnimaion[6];
    CImage mLeftHitAnimation[2];
    CImage mLeftWalkAnimation[6];
    CImage mAttackEffectAnimation[4];

    CImage mHitEffectAnimation[4];
    float mHitEffectAngle;
    EnemyState state = EnemyState::RIGHT;

    bool mIsMoving = false;
    int mCurrentWalkFrame = 0;
    bool mIsAttack = false;
    int mCurrenAttackFrame = 0;
    bool mIsDead = false;
    int mCurrentDeadFrame = 0;
    bool mIsHit = false;
    int mCurrentHitFrame = 0;

    // 공격 시작 방향
    float mAttackDirectionX;
    float mAttackDirectionY;

    float mHitTimer;
    float mAttackCooldown = 0;
    float mAttackFrameTime;
    float PlayerDetectRange = 300.0f;
    float AttackRange = 30.0f;
    float speed = 100.0f;
    RECT rect;

    POINT mEffectHitboxPoints[4];
    bool mHasEffectHitbox;

    // 공격 이펙트로 플레이어에게 데미지를 입혔는지 여부
    bool mHasAttackedPlayer = false;

    bool CheckPointInPolygon(POINT& point, POINT polygon[4]);


    bool mHasBeenHit = false; // 피격 플래그 추가


    // 데미지 표시 변수
    int mDamageTaken;           // 받은 데미지 양
    float mDamageTextY;         // 데미지 텍스트의 Y 위치
    float mDamageTextSpeed;     // 텍스트의 상승 속도
    bool mShowDamage;           // 데미지 텍스트 표시 플래그

    // GDI+ 관련
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
};