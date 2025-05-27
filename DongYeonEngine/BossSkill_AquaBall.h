#pragma once
#include "CommonInclude.h"
#include "Player.h"

class Scene;

class BossSkill_AquaBall
{
public:
    BossSkill_AquaBall(float x, float y, float targetX, float targetY);
    ~BossSkill_AquaBall();

    void Update(Player& player);
    void Render(HDC hdc, Player& player);

    static void ThrowAquaBall(Player& player, float mX, float mY, Scene* stage);

    bool IsActive() const { return mIsActive; }
    void SetActive(bool active) { mIsActive = active; }

    POINT* GetHitboxPoints() { return hitboxPoints; }

    float GetPositionX() const { return mX; }
    float GetPositionY() const { return mY; }

private:
    CImage AquaBallAnimation[28];
    CImage AquaBallFallingPointAnimation[25];
    CImage AquaBallExplosionEffectAnimation[7];

    POINT hitboxPoints[4];
    float mX, mY; // ���� ��ġ
    float mTargetX, mTargetY; // ��ǥ ��ġ
    float mInitialX, mInitialY; // ���� ��ġ
    float speed;
    bool mIsActive;
    int damage;
    float mTime; // ������ �̵� �ð�
    float mHeight; // ������ ����
    int mCurrentFrame; // ���� �ִϸ��̼� ������
    float mAnimationTimer; // �ִϸ��̼� Ÿ�̸�
    bool mIsFalling; // ���� ����
    bool mIsExploding; // ���� ����
    bool mIsTargetIndicating;
    int mTargetIndicatorFrame;
    int mAquaBallFrame;
    int mExplosionFrame;
    void UpdateHitbox();
    bool CheckCollisionWithRect(const RECT& rect);
};