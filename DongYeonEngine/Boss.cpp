#include "Boss.h"
#include "BossSkill_AquaBall.h"
#include "BossSkill_Spear.h"
#include "Time.h"
#include <cmath>

Boss::Boss()
{
    // Initialize member variables
    mX = 1025.0f;
    mY = 600.0f;
    hp = 1000; // Example HP value
    damage = 50; // Example damage value
    speed = 100.0f;
    PlayerDetectRange = 300.0f;
    AttackRange = 30.0f;
    mAttackCooldown = 0.0f;
    mAttackFrameTime = 0.1f; // Frame duration for animations
    mHitTimer = 0.0f;
    mHasEffectHitbox = false;
    mHasAttackedPlayer = false;
    mHasBeenHit = false;
    mDamageTaken = 0;
    mShowDamage = false;
    mDamageTextY = mY;
    mDamageTextSpeed = 50.0f; // Pixels per second upward

    
    rect = { (LONG)(mX - 50), (LONG)(mY - 50), (LONG)(mX + 50), (LONG)(mY + 50) };

    
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Boss/Idle/IceQueenIdle%d.png", i);
        mIdleAnimation[i].Load(path);
    }
    mDieImage.Load(L"resources/Boss/Dead/IceBossDead.png");
    mDownDashImage.Load(L"resources/Boss/Dash/IceBossDashDown.png");
    mLeftDashImage.Load(L"resources/Boss/Dash/IceBossDashLeft.png");
    mRightDashImage.Load(L"resources/Boss/Dash/IceBossDashRight.png");
    mUpDashImage.Load(L"resources/Boss/Dash/IceBossDashUp.png");
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Boss/Hit/LEFT/IceBostHitLeft%d.png", i);
        mLeftHitAnimation[i].Load(path);
        swprintf_s(path, L"resources/Boss/Hit/RIGHT/IceBostHitRight%d.png", i);
        mRightHitAnimation[i].Load(path);
    }
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Boss/SwordSkill/IceSword/IceSword%d.png", i);
        mAnimaionIceSword[i].Load(path);
    }
    for (int i = 0; i < 7; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Boss/SwordSkill/Spin/IceBossSpinRight%d.png", i);
        mSpinImage[i].Load(path);
    }
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Boss/SpearSkill/DownCast/IceBossDownCast%d.png", i);
        mDownCastingImage[i].Load(path);
        swprintf_s(path, L"resources/Boss/SpearSkill/LeftCast/IceBossLeftCast%d.png", i);
        mLeftCastingImage[i].Load(path);
        swprintf_s(path, L"resources/Boss/SpearSkill/RightCast/IceBossRightCast%d.png", i);
        mRightCastingImage[i].Load(path);
        swprintf_s(path, L"resources/Boss/SpearSkill/UpCast/IceBossUpCast%d.png", i);
        mUpCastingImage[i].Load(path);
    }
}

Boss::~Boss()
{
    // CImage objects are automatically cleaned up by their destructor
}

void Boss::Update(Player& p, Scene* stage)
{
    
}

void Boss::LateUpdate()
{
  
}

void Boss::Render(HDC hdc, Player& p)
{
  mIdleAnimation[0].Draw(hdc, (LONG)mX - 50, (LONG)mY - 50, 100, 100);
  
}

void Boss::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
    position = Vector2(mX, mY);
    rect = { (LONG)(mX - 50), (LONG)(mY - 50), (LONG)(mX + 50), (LONG)(mY + 50) };
}

void Boss::TakeDamage(int d)
{
    if (mIsDead) return;
    hp -= d;
    mDamageTaken = d;
    mShowDamage = true;
    mDamageTextY = mY - 50;
    mHitEffectAngle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
    if (hp <= 0)
    {
        hp = 0;
        mIsDead = true;
        mIsHit = false;
        mIsAttack = false;
        mIsMoving = false;
        mCurrentDeadFrame = 0;
        mShowDamage = false;
    }
    else
    {
        mIsHit = true;
        mHitTimer = 0.2f;
        mCurrentHitFrame = 0;
        mIsAttack = false;
        mIsMoving = false;
        mHasBeenHit = true;
    }
}

bool Boss::GetEffectHitbox(POINT outPoints[4])
{
    for (int i = 0; i < 4; ++i)
        outPoints[i] = mEffectHitboxPoints[i];
    return mHasEffectHitbox;
}

bool Boss::CheckPointInPolygon(POINT& point, POINT polygon[4])
{
    int crossings = 0;
    for (int i = 0; i < 4; ++i)
    {
        auto p1 = polygon[i];
        auto p2 = polygon[(i + 1) % 4];
        if (((p1.y > point.y) != (p2.y > point.y)) &&
            (point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x))
        {
            crossings++;
        }
    }
    return (crossings % 2) == 1;
}

bool Boss::CheckCollisionWithRect(RECT& otherRect)
{
    if (!mHasEffectHitbox) return false;

    POINT rectPoints[4] = {
        { otherRect.left, otherRect.top },
        { otherRect.right, otherRect.top },
        { otherRect.right, otherRect.bottom },
        { otherRect.left, otherRect.bottom }
    };

    for (auto& p : rectPoints)
    {
        if (CheckPointInPolygon(p, mEffectHitboxPoints))
            return true;
    }

    for (auto& p : mEffectHitboxPoints)
    {
        if (p.x >= otherRect.left && p.x <= otherRect.right &&
            p.y >= otherRect.top && p.y <= otherRect.bottom)
            return true;
    }

    return false;
}