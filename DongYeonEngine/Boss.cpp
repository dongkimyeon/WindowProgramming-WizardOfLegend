#include "Boss.h"
#include "BossSkill_AquaBall.h"
#include "BossSkill_Spear.h"
#include "Time.h"
#include <cmath>

Boss::Boss()
{
    mX = 1025.0f;
    mY = 600.0f;
    hp = 1000;
    damage = 20;
    speed = 100.0f;
    mScale = 1.6f;
    mSwordX = mX + 50.0f * mScale;
    mSwordY = mY;
    mHitTimer = 0.0f;
    mHasEffectHitbox = false;
    mHasAttackedPlayer = false;
    mHasBeenHit = false;

    mDamageTaken = 0;
    mShowDamage = false;
    mDamageTextY = mY;
    mDamageTextSpeed = 50.0f;

    rect = { (int)(mX - 20), (int)(mY - 35), (int)(mX + 20), (int)(mY + 40) };

    for (int i = 0; i < 6; ++i) {
        wchar_t path[256];
        swprintf_s(path, L"resources/Boss/Idle/IceQueenIdle%d.png", i);
        mIdleAnimation[i].Load(path);
    }
    mDieImage.Load(L"resources/Boss/Dead/IceBossDead.png");
    mDownDashImage.Load(L"resources/Boss/Dash/IceBossDashDown.png");
    mLeftDashImage.Load(L"resources/Boss/Dash/IceBossDashLeft.png");
    mRightDashImage.Load(L"resources/Boss/Dash/IceBossDashRight.png");
    mUpDashImage.Load(L"resources/Boss/Dash/IceBossDashUp.png");
    for (int i = 0; i < 2; ++i) {
        wchar_t path[256];
        swprintf_s(path, L"resources/Boss/Hit/LEFT/IceBostHitLeft%d.png", i);
        mLeftHitAnimation[i].Load(path);
        swprintf_s(path, L"resources/Boss/Hit/RIGHT/IceBostHitRight%d.png", i);
        mRightHitAnimation[i].Load(path);
    }
    for (int i = 0; i < 4; ++i) {
        wchar_t path[256];
        swprintf_s(path, L"resources/Boss/SwordSkill/IceSword/IceSword%d.png", i);
        mAnimaionIceSword[i].Load(path);
    }
    for (int i = 0; i < 7; ++i) {
        wchar_t path[256];
        swprintf_s(path, L"resources/Boss/SwordSkill/Spin/IceBossSpinRight%d.png", i);
        mSpinImage[i].Load(path);
    }
    for (int i = 0; i < 4; ++i) {
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

Boss::~Boss() {}

void Boss::Update(Player& p, Scene* stage)
{
    if (mIsDead) return;

    mHitTimer -= Time::DeltaTime();

    if (mIsHit) {
        if (mHitTimer <= 0) {
            mIsHit = false;
            mCurrentHitFrame = 0;
        }
        else {
            mCurrentHitFrame = static_cast<int>(mHitTimer / 0.1f) % 2;
            return;
        }
    }

    if (mShowDamage) {
        mDamageTextY -= mDamageTextSpeed * Time::DeltaTime();
        if (mDamageTextY < mY - 100) mShowDamage = false;
    }

    stateTimer += Time::DeltaTime();

    float dx = p.GetPositionX() - mX;
    float dy = p.GetPositionY() - mY;
    float distance = sqrt(dx * dx + dy * dy);
    float dirX = distance > 0 ? dx / distance : 0;
    float dirY = distance > 0 ? dy / distance : 0;

    float detectionRadius = 10000.0f;
    playerDetected = (distance < detectionRadius);

    // 칼 위치 업데이트
    float swordDistance = 50.0f * mScale;
    float swordAngle = 0.0f;

    switch (currentState) {
    case 0: // Idle
        if (stateTimer >= 1.0f && playerDetected) {
            stateTimer = 0.0f;
            currentState = 5;
            mCurrenAttackFrame = 0;
            mAttackDirectionX = dirX; // 방향 갱신
            mAttackDirectionY = dirY;
        }
        break;

    case 5: // 검 휘두르기 준비
        if (stateTimer >= 0.1f) {
            mCurrenAttackFrame++;
            stateTimer = 0.0f;
            if (mCurrenAttackFrame >= 4) {
                mCurrenAttackFrame = 0;
                currentState = 1;
                mIsAttack = true;
            }
        }
        break;

    case 1: // 스킬1: 대쉬 후 검 휘두르기
        if (stateTimer < 0.5f) { // 대쉬
            mX += mAttackDirectionX * speed * 4 * Time::DeltaTime();
            mY += mAttackDirectionY * speed * 4 * Time::DeltaTime();
            mSwordX = mX + swordDistance * cos(swordAngle);
            mSwordY = mY + swordDistance * sin(swordAngle);
        }
        else if (stateTimer < 0.7f) { // 0.2초 멈춤 (spin0)
            mCurrenAttackFrame = 0; // spin0 이미지 고정
            mSwordX = mX + swordDistance * cos(swordAngle);
            mSwordY = mY + swordDistance * sin(swordAngle);
        }
        else if (stateTimer < 0.9f) { // 칼 애니메이션 (0.2초, 4프레임)
            mCurrenAttackFrame = static_cast<int>((stateTimer - 0.7f) / 0.05f) % 4; // 각 프레임 0.05초
            mHasEffectHitbox = true;
            mSwordX = mX + swordDistance * cos(swordAngle);
            mSwordY = mY + swordDistance * sin(swordAngle);
            // 히트박스 업데이트
            float radius = 100.0f * mScale;
            mEffectHitboxPoints[0] = { (LONG)(mSwordX), (LONG)(mSwordY) };
            mEffectHitboxPoints[1] = { (LONG)(mSwordX + radius * cos(swordAngle + 1.5708f)), (LONG)(mSwordY + radius * sin(swordAngle + 1.5708f)) };
            mEffectHitboxPoints[2] = { (LONG)(mSwordX + radius * cos(swordAngle + 3.14159f)), (LONG)(mSwordY + radius * sin(swordAngle + 3.14159f)) };
            mEffectHitboxPoints[3] = { (LONG)(mSwordX + radius * cos(swordAngle + 4.71239f)), (LONG)(mSwordY + radius * sin(swordAngle + 4.71239f)) };
        }
        else if (stateTimer < 1.5f) { // 칼 회전 (0.6초, 7프레임)
            mCurrenAttackFrame = static_cast<int>((stateTimer - 0.9f) / 0.0857f) % 7; // 각 프레임 약 0.0857초
            mHasEffectHitbox = true;
            swordAngle = mCurrenAttackFrame * (2 * 3.14159f / 7); // 한 바퀴 회전
            mSwordX = mX + swordDistance * cos(swordAngle);
            mSwordY = mY + swordDistance * sin(swordAngle);
            // 히트박스 업데이트
            float radius = 100.0f * mScale;
            mEffectHitboxPoints[0] = { (LONG)(mSwordX), (LONG)(mSwordY) };
            mEffectHitboxPoints[1] = { (LONG)(mSwordX + radius * cos(swordAngle + 1.5708f)), (LONG)(mSwordY + radius * sin(swordAngle + 1.5708f)) };
            mEffectHitboxPoints[2] = { (LONG)(mSwordX + radius * cos(swordAngle + 3.14159f)), (LONG)(mSwordY + radius * sin(swordAngle + 3.14159f)) };
            mEffectHitboxPoints[3] = { (LONG)(mSwordX + radius * cos(swordAngle + 4.71239f)), (LONG)(mSwordY + radius * sin(swordAngle + 4.71239f)) };
        }
        else {
            mIsAttack = false;
            mHasEffectHitbox = false;
            stateTimer = 0.0f;
            currentState = 2;
            mAttackDirectionX = dirX; // 스킬 종료 후 방향 갱신
            mAttackDirectionY = dirY;
        }
        break;

    case 2: // 스킬2
        if (stateTimer < 0.5f) { // 대쉬
            mX += mAttackDirectionX * speed * 4 * Time::DeltaTime();
            mY += mAttackDirectionY * speed * 4 * Time::DeltaTime();
        }
        else if (stateTimer < 1.5f) { // 캐스팅 (1.0초 -> 1.5초)
            mCurrenAttackFrame = static_cast<int>((stateTimer - 0.5f) / 0.25f) % 4; // 각 프레임 0.25초
            if (stateTimer >= 1.25f && mCurrenAttackFrame == 3 && !mHasAttackedPlayer) { // 마지막 프레임에서 발사
                BossSkill_AquaBall* aquaBall = new BossSkill_AquaBall(mX, mY, p.GetPositionX(), p.GetPositionY());
                aquaBall->ThrowAquaBall(p, mX, mY, stage);
                mHasAttackedPlayer = true;
            }
        }
        else {
            mIsAttack = false;
            mHasAttackedPlayer = false;
            stateTimer = 0.0f;
            currentState = 3;
            mAttackDirectionX = dirX; // 스킬 종료 후 방향 갱신
            mAttackDirectionY = dirY;
        }
        break;
    case 3: // 스킬3
        if (stateTimer < 0.5f) { // 대쉬
            mX += mAttackDirectionX * speed * 4 * Time::DeltaTime();
            mY += mAttackDirectionY * speed * 4 * Time::DeltaTime();
        }
        else if (stateTimer < 1.5f) { // 캐스팅 (1.0초 -> 1.5초)
            mCurrenAttackFrame = static_cast<int>((stateTimer - 0.5f) / 0.25f) % 4; // 각 프레임 0.25초
            if (stateTimer >= 1.25f && mCurrenAttackFrame == 3 && !mHasAttackedPlayer) { // 마지막 프레임에서 발사
                BossSkill_Spear* spear = new BossSkill_Spear(mX, mY, dirX, dirY);
                spear->ThrowSpear(p, mX, mY, stage);
                mHasAttackedPlayer = true;
            }
        }
        else {
            mIsAttack = false;
            mHasAttackedPlayer = false;
            stateTimer = 0.0f;
            currentState = 4;
            mAttackDirectionX = dirX; // 스킬 종료 후 방향 갱신
            mAttackDirectionY = dirY;
        }
        break;
    case 4: // 긴 Idle
        if (stateTimer >= 2.0f) {
            stateTimer = 0.0f;
            currentState = 0;
            mAttackDirectionX = dirX; // 스킬 종료 후 방향 갱신
            mAttackDirectionY = dirY;
        }
        break;
    }

    SetPosition(mX, mY);
}
void Boss::LateUpdate() {}

void Boss::Render(HDC hdc, Player& p)
{
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    if (mIsDead) {
        int width = static_cast<int>(mDieImage.GetWidth() * mScale);
        int height = static_cast<int>(mDieImage.GetHeight() * mScale);
        mDieImage.Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
        return;
    }

    if (mIsHit) {
        int width = static_cast<int>(mLeftHitAnimation[0].GetWidth() * mScale);
        int height = static_cast<int>(mLeftHitAnimation[0].GetHeight() * mScale);
        if (mAttackDirectionX < 0)
            mLeftHitAnimation[mCurrentHitFrame].Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
        else
            mRightHitAnimation[mCurrentHitFrame].Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
        return;
    }

    float dx = p.GetPositionX() - mX;
    float dy = p.GetPositionY() - mY;
    float distance = sqrt(dx * dx + dy * dy);
    float dirX = distance > 0 ? dx / distance : 0;
    float dirY = distance > 0 ? dy / distance : 0;

    CImage* dashImage = &mDownDashImage;
    if (fabs(dirX) > fabs(dirY)) {
        dashImage = (dirX > 0) ? &mRightDashImage : &mLeftDashImage;
    }
    else {
        dashImage = (dirY > 0) ? &mDownDashImage : &mUpDashImage;
    }

    CImage* castingImage = mDownCastingImage;
    if (fabs(dirX) > fabs(dirY)) {
        castingImage = (dirX > 0) ? mRightCastingImage : mLeftCastingImage;
    }
    else {
        castingImage = (dirY > 0) ? mDownCastingImage : mUpCastingImage;
    }

    // 보스 렌더링
    switch (currentState) {
    case 0: // Idle
    case 4: // 긴 Idle
    {
        int frame = static_cast<int>(stateTimer / 0.1667f) % 6;
        int width = static_cast<int>(mIdleAnimation[frame].GetWidth() * mScale);
        int height = static_cast<int>(mIdleAnimation[frame].GetHeight() * mScale);
        mIdleAnimation[frame].Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
        break;
    }
    case 5: // 긴 Idle
    {
        int frame = static_cast<int>(stateTimer / 0.1667f) % 6;
        int width = static_cast<int>(mIdleAnimation[frame].GetWidth() * mScale);
        int height = static_cast<int>(mIdleAnimation[frame].GetHeight() * mScale);
        mIdleAnimation[frame].Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
        break;
    }
    case 1: // 스킬1
    {
        if (stateTimer < 0.5f) { // 대쉬
            int width = static_cast<int>(dashImage->GetWidth() * mScale);
            int height = static_cast<int>(dashImage->GetHeight() * mScale);
            dashImage->Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
        }
        else if (stateTimer < 0.7f) { // 0.2초 멈춤 (spin0)
            int width = static_cast<int>(mSpinImage[0].GetWidth() * mScale);
            int height = static_cast<int>(mSpinImage[0].GetHeight() * mScale);
            mSpinImage[0].Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
        }
        else { // 회전
            int width = static_cast<int>(mSpinImage[mCurrenAttackFrame].GetWidth() * mScale);
            int height = static_cast<int>(mSpinImage[mCurrenAttackFrame].GetHeight() * mScale);
            mSpinImage[mCurrenAttackFrame].Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
        }
        break;
    }
    case 2: // 스킬2
    case 3: // 스킬3
        if (stateTimer < 0.5f) 
        {
            int width = static_cast<int>(dashImage->GetWidth() * mScale);
            int height = static_cast<int>(dashImage->GetHeight() * mScale);
            dashImage->Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
        }
        else 
        {
            int width = static_cast<int>(castingImage[mCurrenAttackFrame].GetWidth() * mScale);
            int height = static_cast<int>(castingImage[mCurrenAttackFrame].GetHeight() * mScale);
            castingImage[mCurrenAttackFrame].Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
        }
        break;
    }

    // 칼 렌더링
    if (currentState == 1 && stateTimer >= 0.7f && stateTimer < 1.5f && mCurrenAttackFrame < 6) { // 스킬1, 마지막 프레임(6)에서 칼 숨김
        int swordFrame = (stateTimer < 0.9f) ? static_cast<int>((stateTimer - 0.7f) / 0.05f) % 4 : 3; // 0.7~0.9초: 4프레임, 이후 3번 프레임 고정
        float swordAngle = (stateTimer >= 0.9f) ? (mCurrenAttackFrame * (2 * 3.14159f / 7)) : 0.0f; // 0.9초 이후 회전
        int swordWidth = static_cast<int>(mAnimaionIceSword[swordFrame].GetWidth() * mScale);
        int swordHeight = static_cast<int>(mAnimaionIceSword[swordFrame].GetHeight() * mScale);

        XFORM xForm = { 0 };
        xForm.eM11 = cos(swordAngle);
        xForm.eM12 = sin(swordAngle);
        xForm.eM21 = -sin(swordAngle);
        xForm.eM22 = cos(swordAngle);
        xForm.eDx = mSwordX;
        xForm.eDy = mSwordY;

        SetGraphicsMode(hdc, GM_ADVANCED);
        SetWorldTransform(hdc, &xForm);

        HDC srcDC = mAnimaionIceSword[swordFrame].GetDC();
        TransparentBlt(
            hdc,
            -swordWidth / 2, -swordHeight / 2, swordWidth, swordHeight,
            srcDC,
            0, 0, mAnimaionIceSword[swordFrame].GetWidth(), mAnimaionIceSword[swordFrame].GetHeight(),
            RGB(0, 0, 0)
        );
        mAnimaionIceSword[swordFrame].ReleaseDC();

        XFORM identity = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
        SetWorldTransform(hdc, &identity);
        SetGraphicsMode(hdc, GM_COMPATIBLE);
    }

    if (mShowDamage) {
        wchar_t damageText[32];
        swprintf_s(damageText, L"%d", mDamageTaken);
        TextOut(hdc, (int)mX, (int)mDamageTextY, damageText, wcslen(damageText));
    }
}
void Boss::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
    rect = { (int)(mX - 20), (int)(mY - 35), (int)(mX + 20), (int)(mY + 40) };
}

void Boss::TakeDamage(int d)
{
    if (mIsDead) return;
    hp -= d;
    mDamageTaken = d;
    mShowDamage = true;
    mDamageTextY = mY - 50;
    mHitEffectAngle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
    if (hp <= 0) {
        hp = 0;
        mIsDead = true;
        mIsHit = false;
        mIsAttack = false;
        mShowDamage = false;
    }
    else {
        mIsHit = true;
        mHitTimer = 0.2f;
        mCurrentHitFrame = 0;
        mIsAttack = false;
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
    for (int i = 0; i < 4; ++i) {
        auto p1 = polygon[i];
        auto p2 = polygon[(i + 1) % 4];
        if (((p1.y > point.y) != (p2.y > point.y)) &&
            (point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x)) {
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

    for (auto& p : rectPoints) {
        if (CheckPointInPolygon(p, mEffectHitboxPoints))
            return true;
    }

    for (auto& p : mEffectHitboxPoints) {
        if (p.x >= otherRect.left && p.x <= otherRect.right &&
            p.y >= otherRect.top && p.y <= otherRect.bottom)
            return true;
    }

    return false;
}