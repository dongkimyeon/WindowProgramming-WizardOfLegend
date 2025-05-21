#pragma once
#include "CommonInclude.h"
#include "Player.h"

class Stage1;

class FireBall
{
public:
    FireBall(float x, float y, float dirX, float dirY);
    ~FireBall();

    void Update(Player& player);
    void Render(HDC hdc);

    static void ThrowFireBall(Player& player, float mX, float mY, Stage1* stage);

    bool IsActive() const { return mIsActive; }

    POINT* GetHitboxPoints() { return hitboxPoints; } // ��Ʈ�ڽ� ������ ��ȯ

    float GetPositionX() const { return mX; }
    float GetPositionY() const { return mY; }



private:
    CImage mFireBallAnimation[6];
    POINT hitboxPoints[4]; // ȸ���� �浹 �ڽ�
    float mX;
    float mY;
    float mDirectionX;
    float mDirectionY;
    float speed;
    bool mIsActive;
    int damage;

    int mCurrentFrame; // ���� �ִϸ��̼� ������
    float mAnimationTimer; // �ִϸ��̼� Ÿ�̸�
    const float mFrameDuration = 0.1f; // �� ������ ���� �ð� (��)

    void UpdateHitbox();
    bool CheckCollisionWithRect(const RECT& rect);
};