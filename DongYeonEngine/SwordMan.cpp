#include "SwordMan.h"

SwordMan::SwordMan()
{
    mX = 1920 / 2;
    mY = 1080 / 2;
    rect = { (int)(mX - 20), (int)(mY + 200), (int)(mX + 20), (int)(mY + 20) };
    speed = 100.0f;
    state = EnemyState::RIGHT;

    // Right 애니메이션 로드
    mRightIdleAnimation.Load(L"resources/Monster/SwordMan/SwordManRight/SwordManIdle/SWORDMAN_RIGHT_0.png");
    if (mRightIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/SwordMan/SwordManRight/SwordManIdle/SWORDMAN_RIGHT_0.png\n");
    for (int i = 0; i < 3; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManRight/SwordManAttack/SWORDMAN_RIGHT_%d.png", i);
        mRightAttackAnimation[i].Load(path);
        if (mRightAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManRight/SwordManDie/SWORDMAN_RIGHT_%d.png", i);
        mRightDieAnimaion[i].Load(path);
        if (mRightDieAnimaion[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManRight/SwordManHit/SWORDMAN_RIGHT_%d.png", i);
        mRightHitAnimation[i].Load(path);
        if (mRightHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManRight/SwordManWalk/SWORDMAN_RIGHT_%d.png", i);
        mRightWalkAnimation[i].Load(path);
        if (mRightWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    // Left 애니메이션 로드
    mLeftIdleAnimation.Load(L"resources/Monster/SwordMan/SwordManLeft/SwordManIdle/SWORDMAN_LEFT_0.png");
    if (mLeftIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/SwordMan/SwordManLeft/SwordManIdle/SWORDMAN_LEFT_0.png\n");
    for (int i = 0; i < 3; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManLeft/SwordManAttack/SWORDMAN_LEFT_%d.png", i);
        mLeftAttackAnimation[i].Load(path);
        if (mLeftAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManLeft/SwordManDie/SWORDMAN_LEFT_%d.png", i);
        mLeftDieAnimaion[i].Load(path);
        if (mLeftDieAnimaion[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManLeft/SwordManHit/SWORDMAN_LEFT_%d.png", i);
        mLeftHitAnimation[i].Load(path);
        if (mLeftHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManLeft/SwordManWalk/SWORDMAN_LEFT_%d.png", i);
        mLeftWalkAnimation[i].Load(path);
        if (mLeftWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordMan_Effect/SWORDMAN_ATTACK_%d.png", i);
        mAttackEffectAnimation[i].Load(path);
        if (mAttackEffectAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
}

void SwordMan::Update()
{
}

void SwordMan::LateUpdate()
{
}

void SwordMan::Render(HDC hdc)
{
}

void SwordMan::SetPosition(float x, float y)
{
}
