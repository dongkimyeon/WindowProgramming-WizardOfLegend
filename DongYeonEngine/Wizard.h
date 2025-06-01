#pragma once
#include "GameObject.h"
#include "Player.h"
#include "FireBall.h"

class Scene;

class Wizard : public GameObject
{
public:
    Wizard();
    ~Wizard();
    enum EnemyState { LEFT, RIGHT };

    void Update(Player& p, Scene* stage);
    void LateUpdate() override;
    void Render(HDC hdc, Player& p);

    void SetHp(int playerDamage) { hp -= playerDamage; }
    void TakeDamage(int d);
    bool GetIsDead() override { return mIsDead; }
    void SetPosition(float x, float y) override;
    EnemyState GetState() { return state; }
    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    float GetSpeed() override { return speed; }
    RECT GetRect() override { return rect; }

    bool HasBeenHit() const { return mHasBeenHit; }
    void SetHitFlag(bool hit) { mHasBeenHit = hit; }

private:
    float mX;
    float mY;
    Vector2 position;
    int hp;
    int damage;

    CImage mShadowImage;
    // Right �ִϸ��̼�
    CImage mRightIdleAnimation;
    CImage mRightAttackAnimation[4];
    CImage mRightDieAnimaion[5];
    CImage mRightHitAnimation[2];
    CImage mRightWalkAnimation[4];

    // Left �ִϸ��̼�
    CImage mLeftIdleAnimation;
    CImage mLeftAttackAnimation[4];
    CImage mLeftDieAnimaion[5];
    CImage mLeftHitAnimation[2];
    CImage mLeftWalkAnimation[4];
    
	CImage mHitEffectAnimation[4];
    float mHitEffectAngle;
    EnemyState state = EnemyState::RIGHT;

    // ������ ����
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

    bool mHasBeenHit = false; // �ǰ� �÷���

    // ������ ǥ�� ����
    int mDamageTaken;           // ���� ������ ��
    float mDamageTextY;         // ������ �ؽ�Ʈ�� Y ��ġ
    float mDamageTextSpeed;     // �ؽ�Ʈ�� ��� �ӵ�
    bool mShowDamage;           // ������ �ؽ�Ʈ ǥ�� �÷���

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
};