#pragma once
#include "GameObject.h"
#include "Player.h"

class SwordMan : public GameObject
{
public:
    SwordMan();
    ~SwordMan(); // GDI+ ���Ḧ ���� �Ҹ���
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