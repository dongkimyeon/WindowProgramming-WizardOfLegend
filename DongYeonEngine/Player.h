#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
    Player();
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;

    void SetPosition(float x, float y) override;
    void TakeDamage(int d);
    int GetHp() { return hp; }
    PlayerState GetState() { return state; }
    float GetPositionX() override;
    float GetPositionY() override;
    float GetSpeed() override;
    RECT GetRect() override; // GetHitbox ��� GetRect ��� (���� �ڵ� ���)
    bool GetIsDead() { return mIsDead; }
private:
    float mX;
    float mY;
    Vector2 position;
    int hp;

    // Front �ִϸ��̼�
    CImage mFrontIdleAnimation;
    CImage mFrontAttackAnimation[16];
    CImage mFrontDashAnimation[8];
    CImage mFrontDieAnimation[7];
    CImage mFrontHitAnimation[2];
    CImage mFrontWalkAnimation[10];

    // Right �ִϸ��̼�
    CImage mRightIdleAnimation;
    CImage mRightAttackAnimation[16];
    CImage mRightDashAnimation[8];
    CImage mRightHitAnimation[2];
    CImage mRightWalkAnimation[10];

    // Left �ִϸ��̼�
    CImage mLeftIdleAnimation;
    CImage mLeftAttackAnimation[16];
    CImage mLeftDashAnimation[8];
    CImage mLeftHitAnimation[2];
    CImage mLeftWalkAnimation[10];

    // Back �ִϸ��̼�
    CImage mBackIdleAnimation;
    CImage mBackAttackAnimation[16];
    CImage mBackDashAnimation[8];
    CImage mBackHitAnimation[2];
    CImage mBackWalkAnimation[10];

    PlayerState state = PlayerState::FRONT;

    // ���� ����
    bool mIsMoving;
    bool mIsDashing;
    bool mIsAttack;
    bool mIsHit;
    bool mIsDead;

    // �ִϸ��̼� ������
    int mCurrentWalkFrame;
    int mCurrentDashFrame;
    int mCurrentAttackFrame;
    int mCurrentHitFrame;
    int mCurrentDieFrame;

    // Ÿ�̸� �� �ӵ�
    float mDashTimer;
    float mAttackTimer;
    float mHitTimer;
    float speed;
    RECT rect;
};