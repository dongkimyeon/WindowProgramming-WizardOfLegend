#pragma once
#include "GameObject.h"
#include "Player.h"
#include "BossSkill_AquaBall.h"
#include "BossSkill_Spear.h"

class Boss : public GameObject
{
public:
    Boss();
    ~Boss(); // GDI+ ���Ḧ ���� �Ҹ���
    
    void Update(Player& p);
    void LateUpdate();
    void Render(HDC hdc, Player& p);
    void SetPosition(float x, float y) override;

    void TakeDamage(int d);

    int GetDamage() { return damage; }
    int GetHp() { return hp; }
    
    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    float GetSpeed() override { return speed; }
    bool GetIsDead() override { return mIsDead; }

    bool GetEffectHitbox(POINT outPoints[4]);
    bool CheckCollisionWithRect(RECT& otherRect); // �浹 ���� �޼���

    RECT GetRect() override { return rect; }

    // ���� �÷��� Getter�� Setter
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

    //���̵� �ִϸ��̼�
    CImage mIdleAnimation[6];

    //���� ���� �̹���
    CImage mDieImage;

    //�뽬 �̹���
	CImage mDownDashImage;
	CImage mLeftDashImage;
	CImage mRightDashImage;
	CImage mUpDashImage;

    //�ǰ� �̹���
    CImage mLeftHitAnimation[2];
	CImage mRightHitAnimation[2];

    //����Į �̹��� 
    CImage mAnimaionIceSword[4];

    //����Į �ֵθ��� �ִϸ��̼�
	CImage mSpinImage[7];

    //ĳ���� �̹���
	CImage mDownCastingImage[4];
	CImage mLeftCastingImage[4];
	CImage mRightCastingImage[4];
	CImage mUpCastingImage[4];

    

    float mHitEffectAngle;

    bool mIsMoving = false;
    int mCurrentWalkFrame = 0;
    bool mIsAttack = false;
    int mCurrenAttackFrame = 0;
    bool mIsDead = false;
    int mCurrentDeadFrame = 0;
    bool mIsHit = false;
    int mCurrentHitFrame = 0;

    // ���� ���� ����
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

    // ���� ����Ʈ�� �÷��̾�� �������� �������� ����
    bool mHasAttackedPlayer = false;

    bool CheckPointInPolygon(POINT& point, POINT polygon[4]);


    bool mHasBeenHit = false; // �ǰ� �÷��� �߰�


    // ������ ǥ�� ����
    int mDamageTaken;           // ���� ������ ��
    float mDamageTextY;         // ������ �ؽ�Ʈ�� Y ��ġ
    float mDamageTextSpeed;     // �ؽ�Ʈ�� ��� �ӵ�
    bool mShowDamage;           // ������ �ؽ�Ʈ ǥ�� �÷���

    // GDI+ ����
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
};

