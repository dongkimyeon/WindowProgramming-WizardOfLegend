#pragma once
#include "CommonInclude.h"
#include "Player.h"

class Scene;

class Player_Skill_FireDragon
{
public:
    // ������: �ʱ� ��ġ(x, y), ����(dirX, dirY), ���� ��� ����(useSineWave)�� ����
    Player_Skill_FireDragon(float x, float y, float dirX, float dirY, bool useSineWave);
    ~Player_Skill_FireDragon();

    // ������Ʈ �� ������ �Լ�
    void Update(GameObject& obj);
    void Render(HDC hdc);

    // ���� �Լ�: ���̾� �巡���� Ȱ��ȭ�Ͽ� ���� �߰�
    static void Active(float mX, float mY, float angle, Scene* stage, bool triggerFire = false);

    // ���� Ȯ��
    bool IsActive() const { return mIsActive; }

    // ��Ʈ�ڽ� �� ��ġ ���� ��ȯ
    POINT* GetHitboxPoints() { return hitboxPoints; }
    float GetPositionX() const { return mX; }
    float GetPositionY() const { return mY; }

private:
    CImage mFireDragonLeftImage;   // ���� ���� ���̾� �巡�� �̹���
    CImage mFireDragonRightImage;  // ������ ���� ���̾� �巡�� �̹���
    POINT hitboxPoints[4];         // ��Ʈ�ڽ� ��ǥ
    float mX;                      // ���� x ��ġ
    float mY;                      // ���� y ��ġ
    float mDirectionX;             // �̵� ���� x
    float mDirectionY;             // �̵� ���� y
    float speed;                   // �̵� �ӵ�
    bool mIsActive;                // Ȱ��ȭ ����
    int damage;                    // ������
    float mWaveTime;               // ���� ȿ���� ���� �ð� ����
    bool useSineWave;              // ������ ��� ���� (true: ������, false: �ڻ�����)
    float mInstantDirX;            // �������� x�� �ӵ�
    float mInstantDirY;            // �������� y�� �ӵ�
    int mCurrentFrame;             // ���� �ִϸ��̼� ������
    float mAnimationTimer;         // �ִϸ��̼� Ÿ�̸�
    const float mFrameDuration = 0.1f; // ������ ���� �ð�
    
    bool mIsBalSa;
    // ���� �Լ�
    void UpdateHitbox();           // ��Ʈ�ڽ� ������Ʈ
    bool CheckCollisionWithRect(const RECT& rect); // �浹 üũ
};