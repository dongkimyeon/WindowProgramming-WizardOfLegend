#include "Wizard.h"

Wizard::Wizard()
{
    mX = 1920 / 2;
    mY = 1080 / 2;
    rect = { (int)(mX - 20), (int)(mY + 200), (int)(mX + 20), (int)(mY + 20) };
    speed = 100.0f;
    state = EnemyState::RIGHT;

    // Right 애니메이션 로드
    mRightIdleAnimation.Load(L"resources/Monster/WIZARD/WizardRight/WizardIdle/WIZARD_RIGHT_00.png");
    if (mRightIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/WIZARD/WizardRight/WizardIdle/WIZARD_RIGHT_00.png\n");
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardRight/WizardAttack/WIZARD_RIGHT_%02d.png", i);
        mRightAttackAnimation[i].Load(path);
        if (mRightAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 5; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardRight/WizardDie/WIZARD_RIGHT_%02d.png", i);
        mRightDieAnimaion[i].Load(path);
        if (mRightDieAnimaion[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardRight/WizardHit/WIZARD_RIGHT_%02d.png", i);
        mRightHitAnimation[i].Load(path);
        if (mRightHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardRight/WizardWalk/WIZARD_RIGHT_%02d.png", i);
        mRightWalkAnimation[i].Load(path);
        if (mRightWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    // Left 애니메이션 로드
    mLeftIdleAnimation.Load(L"resources/Monster/WIZARD/WizardLeft/WizardIdle/WIZARD_LEFT_00.png");
    if (mLeftIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/WIZARD/WizardLeft/WizardIdle/WIZARD_LEFT_00.png\n");
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardLeft/WizardAttack/WIZARD_LEFT_%02d.png", i);
        mLeftAttackAnimation[i].Load(path);
        if (mLeftAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 5; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardLeft/WizardDie/WIZARD_LEFT_%02d.png", i);
        mLeftDieAnimaion[i].Load(path);
        if (mLeftDieAnimaion[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardLeft/WizardHit/WIZARD_LEFT_%02d.png", i);
        mLeftHitAnimation[i].Load(path);
        if (mLeftHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardLeft/WizardWalk/WIZARD_LEFT_%02d.png", i);
        mLeftWalkAnimation[i].Load(path);
        if (mLeftWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
}


void Wizard::Update()
{
}

void Wizard::LateUpdate()
{
}

void Wizard::Render(HDC hdc)
{
}

void Wizard::SetPosition(float x, float y)
{
}
