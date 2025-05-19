#pragma once
#include "CommonInclude.h"
#include "Player.h"

class Camera
{
public:
    Camera();
    ~Camera();
    void Update();
    void SetTarget(Player* target);
    float GetPositionX() const { return mX; }
    float GetPositionY() const { return mY; }
private:
    float mX;
    float mY;
    Player* mTarget;
};