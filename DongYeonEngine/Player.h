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
    int GetDamage() { return damage; }
    PlayerState GetState() { return state; }
    float GetPositionX() override;
    float GetPositionY() override;
    float GetSpeed() override;
    RECT GetRect() override; // GetHitbox ��� GetRect ��� (���� �ڵ� ���)
    bool GetIsDead() { return mIsDead; }

    bool GetEffectHitbox(POINT outPoints[4]); // SwordMan�� ������ �������̽� �߰�
    bool CheckCollisionWithRect(RECT& otherRect); // �浹 �˻� �Լ� �߰�

private:
    float mX;
    float mY;
    Vector2 position;
    int hp;
    int damage;
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

    // ����Ʈ �ִϸ��̼� ���� ������ ���þִϸ��̼� 8�����ӱ����� ����Ʈ����Ʈ�ִϸ��̼� ���Ŀ��� ����Ʈ ����Ʈ �ִϸ��̼�
    CImage mRightAttackEffectAnimation[6];
    CImage mLeftAttackEffectAnimation[6];

    PlayerState state = PlayerState::FRONT;

    // ���� ����
    bool mIsMoving;
    bool mIsDashing;
    bool mIsAttack;
    bool mIsHit;
    bool mIsDead;

    bool mMouseClickFlag;

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

    POINT mEffectHitboxPoints[4]; // ���� ����Ʈ ��Ʈ�ڽ� ������
    bool mHasEffectHitbox; // ��Ʈ�ڽ� Ȱ��ȭ ����
    float attackAngle; // ���콺 ���� ����



    bool CheckPointInPolygon(POINT& point, POINT polygon[4]);
    

};