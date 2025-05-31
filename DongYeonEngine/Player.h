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
    void SetHp(int i) { hp = i; }
    void Setrevive() { mIsDead = false; }
    void TakeDamage(int d);
	void SetTelporting(bool isTeleporting) { mIsTeleporting = isTeleporting; }
    void PlusKillCount()
    {
        KillEnemyCnt++;
    }
    void SetDeadStageName(std::wstring playerDeadStage)
    {
        deadStage = playerDeadStage;
    }
 
    int GetHp() { return hp; }
	int GetMp() { return mp; }
    int GetDamage() { return damage; }
    PlayerState GetState() { return state; }
    float GetPositionX() override;
    float GetPositionY() override;
    float GetSpeed() override;
    RECT GetRect() override;
    bool GetIsDead() { return mIsDead; }
    int GetKillCount() { return KillEnemyCnt; }
    std::wstring GetDeadStage() { return deadStage; }
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
    int mp;
    int damage;

    std::wstring deadStage;
    int KillEnemyCnt;

    int mCameraX;
    int mCameraY;

    bool fireDragonTriggered; // ���̾�巡�� �߻� ���� �÷���
    float fireBallCooldown;   // ���̾ ��Ÿ�� Ÿ�̸�
    float fireDragonCooldown; // ���̾�巡�� ��Ÿ�� Ÿ�̸�
    bool fireBallReady;       // ���̾ ��Ÿ�� �غ� ����
    bool fireDragonReady;     // ���̾�巡�� ��Ÿ�� �غ� ����
    bool isUsingSkill;        // ��ų ��� ���� �÷���

    CImage mShadowImage;
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

	CImage mTeleportEffectAnimation[8];

    bool mIsTeleporting;        // �ڷ���Ʈ �ִϸ��̼� ���� ����
    float mTeleportTimer;       // �ڷ���Ʈ �ִϸ��̼� Ÿ�̸�
    int mCurrentTeleportFrame;  // �ڷ���Ʈ �ִϸ��̼� ���� ������

    CImage mHitEffectAnimation[4];
    float mHitEffectAngle;
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