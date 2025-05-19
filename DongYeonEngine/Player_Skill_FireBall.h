#pragma once
#include "CommonInclude.h"
#include "Player.h"

class Scene;

class Player_Skill_FireBall
{
public:
    Player_Skill_FireBall(float x, float y, float dirX, float dirY);
    ~Player_Skill_FireBall();

    void Update(GameObject& obj);
    void Render(HDC hdc);

    static void Active(float mX, float mY, float angle, Scene* stage);

    bool IsActive() const { return mIsActive; }

    POINT* GetHitboxPoints() { return hitboxPoints; }

    float GetPositionX() const { return mX; }
    float GetPositionY() const { return mY; }



private:
    CImage mFireBallAnimation[5];
    POINT hitboxPoints[4];
    float mX;
    float mY;
    float mDirectionX;
    float mDirectionY;
    float speed;
    bool mIsActive;
    int damage;

    int mCurrentFrame; 
    float mAnimationTimer; 
    const float mFrameDuration = 0.1f; 

    void UpdateHitbox();
    bool CheckCollisionWithRect(const RECT& rect);
};