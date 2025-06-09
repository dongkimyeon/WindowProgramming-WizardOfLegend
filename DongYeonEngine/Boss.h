#pragma once
#include "GameObject.h"
#include "Player.h"

class Scene;

class Boss : public GameObject
{
public:
    Boss();
    ~Boss();
    void Init();
    static Boss* GetInstance()
    {
        static Boss instance;
        return &instance;
    }
    void Update(Player& p, Scene* stage);
    void LateUpdate();
    void Render(HDC hdc, Player& p);
    void SetPosition(float x, float y) override;

    void revive()
    {
        SetPosition(1025, 600);
        hp = 1000;
        damage = 43;
        speed = 100.0f;
        mScale = 1.6f;
        mSwordX = mX + 50.0f * mScale;
        mSwordY = mY;
        mHitTimer = 0.0f;
        mHasEffectHitbox = false;
        mHasAttackedPlayer = false;
        mHasBeenHit = false;
        mDamageTaken = 0;
        mShowDamage = false;
        mDamageTextY = mY;
        mDamageTextSpeed = 50.0f;
        mIsDead = false;
        mIsHit = false;
        mIsAttack = false;
        mCurrenAttackFrame = 0;
        mCurrentHitFrame = 0;
        mAttackDirectionX = 0.0f;
        mAttackDirectionY = 0.0f;
        mHitEffectAngle = 0.0f;
        stateTimer = 0.0f;
        currentState = 0;
        playerDetected = false;
        rect = { (int)(mX - 20), (int)(mY - 35), (int)(mX + 20), (int)(mY + 40) };
        for (int i = 0; i < 4; ++i) {
            mEffectHitboxPoints[i] = { 0, 0 };
        }
    }
    void TakeDamage(int d);

    int GetDamage() { return damage; }
    int GetHp() { return hp; }
   
    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    float GetSpeed() override { return speed; }
    bool GetIsDead() override { return mIsDead; }

    bool GetEffectHitbox(POINT outPoints[4]);
    bool CheckCollisionWithRect(RECT& otherRect);

    RECT GetRect() override { return rect; }

    bool HasAttackedPlayer() const { return mHasAttackedPlayer; }
    void ResetAttackFlag() { mHasAttackedPlayer = false; }

    bool HasBeenHit() const { return mHasBeenHit; }
    void SetHitFlag(bool hit) { mHasBeenHit = hit; }

private:
    static float mX;
    static float mY;
    static int hp;
    int damage;
    float mScale = 1.0f;
    CImage mShadowImage;
    CImage mIdleAnimation[6];
    CImage mDieImage;
    CImage mDownDashImage;
    CImage mLeftDashImage;
    CImage mRightDashImage;
    CImage mUpDashImage;
    CImage mLeftHitAnimation[2];
    CImage mRightHitAnimation[2];
    CImage mAnimaionIceSword[4];
    CImage mSpinImage[7];
    CImage mDownCastingImage[4];
    CImage mLeftCastingImage[4];
    CImage mRightCastingImage[4];
    CImage mUpCastingImage[4];
    float mSwordX, mSwordY;
    float mHitEffectAngle;
    bool mIsAttack = false;
    int mCurrenAttackFrame = 0;
    static bool mIsDead;
    bool mIsHit = false;
    int mCurrentHitFrame = 0;
    float mAttackDirectionX;
    float mAttackDirectionY;
    float mHitTimer;
    float speed = 100.0f;
    RECT rect;

    POINT mEffectHitboxPoints[4];
    bool mHasEffectHitbox;
    bool mHasAttackedPlayer = false;

    bool CheckPointInPolygon(POINT& point, POINT polygon[4]);

    bool mHasBeenHit = false;
    int mDamageTaken;
    float mDamageTextY;
    float mDamageTextSpeed;
    bool mShowDamage;

    float stateTimer = 0.0f;
    int currentState = 0; // 0: Idle, 1: 스킬1(검 휘두르기), 2: 스킬2, 3: 스킬3, 4: 긴 Idle, 5: 검 휘두르기 준비
    bool playerDetected = false; // 플레이어 감지 플래그
};