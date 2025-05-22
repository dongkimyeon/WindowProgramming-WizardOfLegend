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
    float mScale; // �̹��� ������ ��
    bool mHasBeenHit = false; // �ǰ� �÷���
    int mDamageTaken;         // ���� ������ ��
    float mDamageTextY;       // ������ �ؽ�Ʈ�� Y ��ġ
    float mDamageTextSpeed;   // �ؽ�Ʈ ��� �ӵ�
    bool mShowDamage;         // ������ �ؽ�Ʈ ǥ�� �÷���
    float mHitTimer;          // �ǰ� ǥ�� ���� �ð�
};