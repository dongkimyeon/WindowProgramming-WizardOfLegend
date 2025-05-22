#pragma once
#include "CommonInclude.h"
#include "Player.h"
#include <vector>

class Scene;
class Player;

class Player_Skill_FireDragon
{
public:
    // ������: �ʱ� ��ġ(x, y), ����(dirX, dirY), ���� ������(phaseOffset)�� ����
    Player_Skill_FireDragon(float x, float y, float dirX, float dirY, float phaseOffset);
    ~Player_Skill_FireDragon();

    // ������Ʈ �� ������ �Լ�
    void Move(); // �̵� ����
    bool CheckCollision(GameObject& obj); // �浹 üũ
    void Render(HDC hdc);

    // ���� �Լ�: ���̾� �巡���� Ȱ��ȭ�Ͽ� ���� �߰�
    static void Active(float mX, float mY, float angle, Scene* stage, Player* player, bool triggerFire = false);

    // ���� Ȯ��
    bool IsActive() const { return mIsActive; }

    // ���� ����
    void SetActive(bool active) { mIsActive = active; }

    // ��Ʈ�ڽ� �� ��ġ ���� ��ȯ
    POINT* GetHitboxPoints() { return hitboxPoints; }
    float GetPositionX() const { return mX; }
    float GetPositionY() const { return mY; }
    int GetDamage() const { return damage; } // ������ ��ȯ

    // ��ƼŬ ���� �Լ�
    void SpawnParticle();
    void UpdateParticles();

private:
    // ��ƼŬ ����ü
    struct Particle {
        float x, y;           // ��ƼŬ ��ġ
        float velX, velY;     // ��ƼŬ �ӵ�
        float lifetime;       // ��ƼŬ ����
        int frame;            // �ִϸ��̼� ������
        float initialLifetime;
    };

    std::vector<Particle> mParticles; // ��ƼŬ ����Ʈ
    float mParticleTimer;             // ��ƼŬ ���� Ÿ�̸�
    float mParticleSpawnInterval;     // ��ƼŬ ���� ����
    CImage mFireParticleImage[20];    // ��ƼŬ �̹��� �迭

    CImage mFireDragonLeftImage;      // ���� ���� ���̾� �巡�� �̹���
    CImage mFireDragonRightImage;     // ������ ���� ���̾� �巡�� �̹���

    POINT hitboxPoints[4];            // ��Ʈ�ڽ� ��ǥ
    float mX;                         // ���� x ��ġ
    float mY;                         // ���� y ��ġ
    float mDirectionX;                // �̵� ���� x
    float mDirectionY;                // �̵� ���� y
    float speed;                      // �̵� �ӵ�
    bool mIsActive;                   // Ȱ��ȭ ����
    int damage;                       // ������
    float mWaveTime;                  // ���� ȿ���� ���� �ð� ����
    float phaseOffset;                // ������ ���� ������
    float mInstantDirX;               // �������� x�� �ӵ�
    float mInstantDirY;               // �������� y�� �ӵ�
    int mCurrentFrame;                // ���� �ִϸ��̼� ������
    float mAnimationTimer;            // �ִϸ��̼� Ÿ�̸�
    const float mFrameDuration = 0.1f; // ������ ���� �ð�

    // ���� �Լ�
    void UpdateHitbox();              // ��Ʈ�ڽ� ������Ʈ
    bool CheckCollisionWithRect(const RECT& rect); // �浹 üũ
};