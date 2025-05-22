#pragma once
#include "GameObject.h"
#include "Player.h"

class Scene;

class Dummy : public GameObject
{
public:
    Dummy();
    ~Dummy();
    void Render(HDC hdc, Player& p);

    void TakeDamage(int d);
    void SetPosition(float x, float y) override;
    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    RECT GetRect() override { return rect; }

    bool HasBeenHit() const { return mHasBeenHit; }
    void SetHitFlag(bool hit) { mHasBeenHit = hit; }

private:
    float mX;
    float mY;
    RECT rect;
    CImage DummyImage;
    float mScale; // 이미지 스케일 값
    bool mHasBeenHit = false; // 피격 플래그
    int mDamageTaken;         // 받은 데미지 양
    float mDamageTextY;       // 데미지 텍스트의 Y 위치
    float mDamageTextSpeed;   // 텍스트 상승 속도
    bool mShowDamage;         // 데미지 텍스트 표시 플래그
    float mHitTimer;          // 피격 표시 지속 시간
};