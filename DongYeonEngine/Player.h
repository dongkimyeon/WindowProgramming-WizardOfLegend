#pragma once
#include "GameObject.h"
#include "Player_Skill_FireBall.h"  
#include "Player_Skill_FireDragon.h"

class Scene;

class Player : public GameObject
{
public:
    Player();
    void Update(Scene* stage);
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
    RECT GetRect() override;
    bool GetIsDead() { return mIsDead; }

    void SetCameraX(int cameraX) { mCameraX = cameraX; }
    void SetCameraY(int cameraY) { mCameraY = cameraY; }

    bool GetEffectHitbox(POINT outPoints[4]);
    bool CheckCollisionWithRect(RECT& otherRect);
    void ResetFireDragonTriggered() { fireDragonTriggered = false; }

	float GetFireBallCooldown() { return fireBallCooldown; }    
	float GetFireDragonCooldown() { return fireDragonCooldown; }

private:
    float mX;
    float mY;
    Vector2 position;
    int hp;
    int damage;

    int mCameraX;
    int mCameraY;

    bool fireDragonTriggered; // ���̾�巡�� �߻� ���� �÷���
    float fireBallCooldown;   // ���̾ ��Ÿ�� Ÿ�̸�
    float fireDragonCooldown; // ���̾�巡�� ��Ÿ�� Ÿ�̸�
    bool fireBallReady;       // ���̾ ��Ÿ�� �غ� ����
    bool fireDragonReady;     // ���̾�巡�� ��Ÿ�� �غ� ����
    bool isUsingSkill;        // ��ų ��� ���� �÷���

    // �ִϸ��̼� �̹�����
    CImage mFrontIdleAnimation;
    CImage mFrontAttackAnimation[16];
    CImage mFrontDashAnimation[8];
    CImage mFrontDieAnimation[7];
    CImage mFrontHitAnimation[2];
    CImage mFrontWalkAnimation[10];

    CImage mRightIdleAnimation;
    CImage mRightAttackAnimation[16];
    CImage mRightDashAnimation[8];
    CImage mRightHitAnimation[2];
    CImage mRightWalkAnimation[10];

    CImage mLeftIdleAnimation;
    CImage mLeftAttackAnimation[16];
    CImage mLeftDashAnimation[8];
    CImage mLeftHitAnimation[2];
    CImage mLeftWalkAnimation[10];

    CImage mBackIdleAnimation;
    CImage mBackAttackAnimation[16];
    CImage mBackDashAnimation[8];
    CImage mBackHitAnimation[2];
    CImage mBackWalkAnimation[10];

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

    POINT mEffectHitboxPoints[4];
    bool mHasEffectHitbox;
    float attackAngle;

    bool CheckPointInPolygon(POINT& point, POINT polygon[4]);
};