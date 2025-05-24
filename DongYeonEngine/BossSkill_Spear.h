#pragma once
#include "CommonInclude.h"
#include "Player.h"

class Scene;

class BossSkill_Spear
{
public:
    BossSkill_Spear(float x, float y, float dirX, float dirY);
    ~BossSkill_Spear();

    void Update(Player& player);
    void Render(HDC hdc);

    static void ThrowSpear(Player& player, float mX, float mY, Scene* stage);

    bool IsActive() const { return mIsActive; }

    // 상태 변경
    void SetActive(bool active) { mIsActive = active; }

    POINT* GetHitboxPoints() { return hitboxPoints; } // 히트박스 포인터 반환


    float GetPositionX() const { return mX; }
    float GetPositionY() const { return mY; }

    float GetDirectionX() const { return mDirectionX; }
    float GetDirectionY() const { return mDirectionY; }

private:
	CImage mSpearImage;
    POINT hitboxPoints[4]; // 회전된 충돌 박스
    float mX;
    float mY;
    float mDirectionX;
    float mDirectionY;
    float speed;
    bool mIsActive;
    int damage;



    void UpdateHitbox();
    bool CheckCollisionWithRect(const RECT& rect);
};

