#pragma once
#include "GameObject.h"
#include "Player.h"

class Archer : public GameObject
{
public:
    Archer();
    ~Archer(); // �Ҹ��� �߰�
    enum EnemyState { LEFT, RIGHT };
    void Update(Player& p);
    void LateUpdate() override;
    void Render(HDC hdc, Player& p);

    void SetPosition(float x, float y) override;
    void TakeDamage(int playerDamage) { hp -= playerDamage; }

    EnemyState GetState() { return state; }
    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    float GetSpeed() override { return speed; }
    RECT GetRect() override { return rect; }
    bool GetIsShot() { return mIsShot; }

private:
    float mX;
    float mY;
    Vector2 position;
    int hp;

    // Right �ִϸ��̼�
    CImage mRightIdleAnimation;
    CImage mRightAttackAnimation[4];
    CImage mRightDieAnimaion[6];
    CImage mRightHitAnimation[2];
    CImage mRightWalkAnimation[5];

    // Left �ִϸ��̼�
    CImage mLeftIdleAnimation;
    CImage mLeftAttackAnimation[4];
    CImage mLeftDieAnimaion[6];
    CImage mLeftHitAnimation[2];
    CImage mLeftWalkAnimation[5];

    // ��ó�� ����ִ� Ȱ
    CImage mBowAttackAnimation[4];

    EnemyState state = EnemyState::RIGHT;

    // ������ ����
    bool mIsMoving;
    int mCurrentWalkFrame;
    bool mIsAttack;
    int mCurrenAttackFrame;
    bool mIsdead;
    int mCurrentDeadFrame;
    bool mIsHit;
    int mCurrentHitFrame;

    bool mIsShot;

    float PlayerDetectRange = 300.0f;
    float AttackRange = 250.0f;

    float speed;
    RECT rect;

    // ���� Ÿ�̹�
    float mAttackFrameTime; // ���� �ִϸ��̼� Ÿ�̸�
    float mAttackCooldown; // ���� ��Ÿ��


    // GDI+ ���� ��� ����
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
};

