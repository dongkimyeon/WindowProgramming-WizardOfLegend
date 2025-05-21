#pragma once
#include "CommonInclude.h"
#include "Player.h"
#include <vector>

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
        float initialLifetime; // �ʱ� ����
    };

    std::vector<Particle> mParticles; // ��ƼŬ ����Ʈ
    float mParticleTimer;             // ��ƼŬ ���� Ÿ�̸�
    float mParticleSpawnInterval;     // ��ƼŬ ���� ����
    CImage mFireParticleImage[20];    // ��ƼŬ �̹��� �迭

    CImage mFireBallAnimation[5];     // ���̾ �ִϸ��̼� �̹���
    POINT hitboxPoints[4];            // ��Ʈ�ڽ� ��ǥ
    float mX;                         // ���� x ��ġ
    float mY;                         // ���� y ��ġ
    float mDirectionX;                // �̵� ���� x
    float mDirectionY;                // �̵� ���� y
    float speed;                      // �̵� �ӵ�
    bool mIsActive;                   // Ȱ��ȭ ����
    int damage;                       // ������
    int mCurrentFrame;                // ���� �ִϸ��̼� ������
    float mAnimationTimer;            // �ִϸ��̼� Ÿ�̸�
    const float mFrameDuration = 0.1f; // ������ ���� �ð�

    void UpdateHitbox();
    bool CheckCollisionWithRect(const RECT& rect);
};