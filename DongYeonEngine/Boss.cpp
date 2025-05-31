#include "Boss.h"
#include "BossSkill_AquaBall.h"
#include "BossSkill_Spear.h"
#include "Time.h"
#include <cmath>
#include "SoundManager.h"

float detectionRadius = 700.0f;
int Boss::hp;

Boss::Boss()
{
    mX = 1025.0f;
    mY = 600.0f;
    hp = 100;
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
void Boss::Init()
{
    mX = 1025.0f;
    mY = 600.0f;
    hp = 100;
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
    mDamageTextY = mY;
    mDamageTextSpeed = 50.0f;

    mIsDead = false;
    mIsHit = false;
    mIsAttack = false;
    mShowDamage = false;
    rect = { (int)(mX - 20), (int)(mY - 35), (int)(mX + 20), (int)(mY + 40) };

}
void Boss::Update(Player& p, Scene* stage)
{
    if (mIsDead) {
        mShowDamage = false; // ���� �� ������ �ؽ�Ʈ ��Ȱ��ȭ
        return;
    }

    mHitTimer -= Time::DeltaTime();

    if (mIsHit) {
        if (mHitTimer <= 0) {
            mIsHit = false;
            mCurrentHitFrame = 0;
            mShowDamage = false; // �ǰ� �ִϸ��̼� ���� �� ������ �ؽ�Ʈ ��Ȱ��ȭ
        }
        else {
            mCurrentHitFrame = static_cast<int>(mHitTimer / 0.1f) % 2;
            // ������ �ؽ�Ʈ ���� �̵�
            if (mShowDamage) {
                mDamageTextY -= mDamageTextSpeed * Time::DeltaTime();
            }
            return;
        }
    }

    if (mShowDamage) {
        mDamageTextY -= mDamageTextSpeed * Time::DeltaTime();
        if (mDamageTextY < mY - 100) mShowDamage = false; // �ؽ�Ʈ�� ���� ���� �̻� �ö󰡸� �����
    }

    stateTimer += Time::DeltaTime();

    float dx = p.GetPositionX() - mX;
    float dy = p.GetPositionY() - mY;
    float distance = sqrt(dx * dx + dy * dy);
    float dirX = distance > 0 ? dx / distance : 0;
    float dirY = distance > 0 ? dy / distance : 0;

    playerDetected = (distance < detectionRadius);

    // Į ��ġ ������Ʈ
    float swordDistance = 50.0f * mScale;
    float swordAngle = 0.0f;

    // �뽬 �Ÿ� �� �ӵ� ���� ����
    static float dashDistance = 0.0f;
    static float dashSpeed = 0.0f;
    const float dashDuration = 0.5f; // �뽬 ���� �ð� (0.5��)

    switch (currentState) {
    case 0: // ��� ����
        if (stateTimer >= 1.0f && playerDetected) {
            stateTimer = 0.0f;
            currentState = 5;
            mCurrenAttackFrame = 0;
            mAttackDirectionX = dirX;
            mAttackDirectionY = dirY;
        }
        break;

    case 5: // �� �ֵθ��� �غ�
        if (stateTimer >= 0.1f) {
            mCurrenAttackFrame++;
            stateTimer = 0.0f;
            if (mCurrenAttackFrame >= 4) {
                mCurrenAttackFrame = 0;
                currentState = 1;
                mIsAttack = true;
                SoundManager::GetInstance()->mPlaySound("BossDash", false);
                // �뽬 ���� �� �÷��̾���� �Ÿ� ���
                dashDistance = distance;
                dashSpeed = dashDistance / dashDuration; // �÷��̾������ �Ÿ��� 0.5�� ���� �̵�
            }
        }
        break;

    case 1: // ��ų1: �÷��̾� ��ġ�� �뽬 �� �� �ֵθ���
    {
        if (stateTimer < dashDuration) { // �뽬: �÷��̾� ��ġ���� �Ÿ��� ���� �̵�
            mX += mAttackDirectionX * dashSpeed * Time::DeltaTime();
            mY += mAttackDirectionY * dashSpeed * Time::DeltaTime();
            mSwordX = mX + swordDistance * cos(swordAngle);
            mSwordY = mY + swordDistance * sin(swordAngle);
        }
        else if (stateTimer < 0.7f) { // 0.2�� ���� (spin0)
            mCurrenAttackFrame = 0;
            mSwordX = mX + swordDistance * cos(swordAngle);
            mSwordY = mY + swordDistance * sin(swordAngle);
        }
        else if (stateTimer < 0.9f) { // Į �ִϸ��̼� (0.2��, 4������)
            if(mCurrenAttackFrame == 0 && (stateTimer < 0.71f && stateTimer > 0.7f))
                SoundManager::GetInstance()->mPlaySound("BossAttackStart", false);
            mCurrenAttackFrame = static_cast<int>((stateTimer - 0.7f) / 0.05f) % 4;
            mHasEffectHitbox = true;
            swordAngle = 0.0f;
            mSwordX = mX + swordDistance * cos(swordAngle);
            mSwordY = mY + swordDistance * sin(swordAngle);
            // Į �̹����� �� ���������� ��Ʈ�ڽ� ���� (���� ����)
            int swordFrame = mCurrenAttackFrame;
            float width = mAnimaionIceSword[swordFrame].GetWidth() * mScale;
            float height = mAnimaionIceSword[swordFrame].GetHeight() * mScale;
            float halfWidth = width / 2.0f;
            float halfHeight = height / 2.0f;
            // �ǹ��� �����̷� ���� (Į �ϴ�)
            POINTF pivotOffset = { 0.0f, halfHeight }; // �����̷� �ǹ� �̵�
            POINTF localPoints[4] = {
                { -halfWidth + pivotOffset.x, -halfHeight + pivotOffset.y }, // ���� ���
                { halfWidth + pivotOffset.x, -halfHeight + pivotOffset.y },  // ������ ���
                { halfWidth + pivotOffset.x, halfHeight + pivotOffset.y },   // ������ �ϴ�
                { -halfWidth + pivotOffset.x, halfHeight + pivotOffset.y }   // ���� �ϴ�
            };
            for (int i = 0; i < 4; ++i) {
                // Į �̹����� ���� ����(0��)
                mEffectHitboxPoints[i].x = (LONG)round(mSwordX + localPoints[i].x);
                mEffectHitboxPoints[i].y = (LONG)round(mSwordY + localPoints[i].y);
            }
            // ���� ����Ʈ�� Ȱ��ȭ�� ���� �浹 ���� �� ������ ó��
            if (mCurrenAttackFrame >= 1 && !mHasAttackedPlayer) {
                RECT playerRect = p.GetRect();
                if (CheckCollisionWithRect(playerRect)) {
                    p.TakeDamage(damage);
                    mHasAttackedPlayer = true;
                }
            }
        }
        else if (stateTimer < 1.5f) { // Į ȸ�� (0.6��)
            mCurrenAttackFrame = static_cast<int>((stateTimer - 0.9f) / 0.1f) % 6;
            mHasEffectHitbox = true;
            swordAngle = (stateTimer - 0.9f) / 0.6f * 2 * 3.14159f; // 0.6�� ���� 360�� ȸ��
            mSwordX = mX + swordDistance * cos(swordAngle);
            mSwordY = mY + swordDistance * sin(swordAngle);
            // Į �̹����� �� ���������� ��Ʈ�ڽ� ���� (Į���� ���� �߽��� ����)
            int swordFrame = 3;
            float width = mAnimaionIceSword[swordFrame].GetWidth() * mScale;
            float height = mAnimaionIceSword[swordFrame].GetHeight() * mScale;
            float halfWidth = width / 2.0f;
            float halfHeight = height / 2.0f;
            // �ǹ��� �����̷� ���� (Į �ϴ�)
            POINTF pivotOffset = { 0.0f, halfHeight };
            POINTF localPoints[4] = {
                { -halfWidth + pivotOffset.x, -halfHeight + pivotOffset.y },
                { halfWidth + pivotOffset.x, -halfHeight + pivotOffset.y },
                { halfWidth + pivotOffset.x, halfHeight + pivotOffset.y },
                { -halfWidth + pivotOffset.x, halfHeight + pivotOffset.y }
            };
            // Į���� ���� �߽��� ���ϵ��� ȸ��
            float imageAngle = atan2(mY - mSwordY, mX - mSwordX); // �������� Į���� ����
            for (int i = 0; i < 4; ++i) {
                float x = localPoints[i].x;
                float y = localPoints[i].y;
                mEffectHitboxPoints[i].x = (LONG)round(mSwordX + x * cos(imageAngle) - y * sin(imageAngle));
                mEffectHitboxPoints[i].y = (LONG)round(mSwordY + x * sin(imageAngle) + y * cos(imageAngle));
            }
            // ���� ����Ʈ�� Ȱ��ȭ�� ���� �浹 ���� �� ������ ó��
            if (mCurrenAttackFrame >= 1 && !mHasAttackedPlayer) {
                RECT playerRect = p.GetRect();
                if (CheckCollisionWithRect(playerRect)) {
                    p.TakeDamage(damage);
                    mHasAttackedPlayer = true;
                }
            }

        }
        else {
            mIsAttack = false;
            mHasEffectHitbox = false;
            stateTimer = 0.0f;
            currentState = 2;
            mAttackDirectionX = dirX;
            mAttackDirectionY = dirY;
            mHasAttackedPlayer = false;
        }

        break;
    }
    case 2:
    {
        if (stateTimer < dashDuration) { // �뽬: �÷��̾� ��ġ���� �Ÿ��� ���� �̵�
            if (stateTimer == 0.0f) { // �뽬 ���� �� �Ÿ� ���
                dashDistance = distance;
                dashSpeed = dashDistance / dashDuration;
                SoundManager::GetInstance()->mPlaySound("BossDash", false);
            }
            mX += mAttackDirectionX * dashSpeed * Time::DeltaTime();
            mY += mAttackDirectionY * dashSpeed * Time::DeltaTime();
        }
        else if (stateTimer < 1.5f) {
            mCurrenAttackFrame = static_cast<int>((stateTimer - 0.5f) / 0.25f) % 4;
            if (stateTimer >= 1.25f && !mHasAttackedPlayer) {
                std::cout << "Case 2: Throwing AquaBall at stateTimer = " << stateTimer << std::endl;
                BossSkill_AquaBall* aquaBall = new BossSkill_AquaBall(mX, mY, dirX, dirY);
                aquaBall->ThrowAquaBall(p, mX, mY, stage);
                mHasAttackedPlayer = true;
            }
            else {
                std::cout << "���� ������: stateTimer = " << stateTimer << ", mHasAttackedPlayer = " << mHasAttackedPlayer << std::endl;
            }
        }
        else {
            mIsAttack = false;
            mHasAttackedPlayer = false;
            stateTimer = 0.0f;
            currentState = 3;
            mAttackDirectionX = dirX;
            mAttackDirectionY = dirY;
        }
        break;
    }
    case 3: // ��ų3: �÷��̾� ��ġ�� �뽬 �� â
    {
        if (stateTimer < dashDuration) { // �뽬: �÷��̾� ��ġ���� �Ÿ��� ���� �̵�
            if (stateTimer == 0.0f) { // �뽬 ���� �� �Ÿ� ���
                dashDistance = distance;
                dashSpeed = dashDistance / dashDuration;
                SoundManager::GetInstance()->mPlaySound("BossDash", false);
            }
            mX += mAttackDirectionX * dashSpeed * Time::DeltaTime();
            mY += mAttackDirectionY * dashSpeed * Time::DeltaTime();
        }
        else if (stateTimer < 1.5f) {
            mCurrenAttackFrame = static_cast<int>((stateTimer - 0.5f) / 0.25f) % 4;
            if (stateTimer >= 1.25f && !mHasAttackedPlayer) {
                std::cout << "Case 3: Throwing Spear at stateTimer = " << stateTimer << std::endl;
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
            mAttackDirectionX = dirX;
            mAttackDirectionY = dirY;
        }
        break;
    }
    case 4: // �� ��� ����
        if (stateTimer >= 2.0f) {
            stateTimer = 0.0f;
            currentState = 0;
            mAttackDirectionX = dirX;
            mAttackDirectionY = dirY;
        }
        break;
    }
    SetPosition(mX, mY);
}

void Boss::LateUpdate()
{
}

void Boss::Render(HDC hdc, Player& p)
{

    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    // ��Ʈ�ڽ� ����� �׸���
    if (mHasEffectHitbox)
    {
        HPEN hitboxPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
        HPEN oldPen = (HPEN)SelectObject(hdc, hitboxPen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
        Polygon(hdc, mEffectHitboxPoints, 4);
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(hitboxPen);
    }

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
    }
    else {
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

        // ���� ������
        switch (currentState) {
        case 0: // Idle
        case 4: // �� Idle
        {
            int frame = static_cast<int>(stateTimer / 0.1667f) % 6;
            int width = static_cast<int>(mIdleAnimation[frame].GetWidth() * mScale);
            int height = static_cast<int>(mIdleAnimation[frame].GetHeight() * mScale);
            mIdleAnimation[frame].Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
            break;
        }
        case 5: // �� �ֵθ��� �غ�
        {
            int frame = static_cast<int>(stateTimer / 0.1667f) % 6;
            int width = static_cast<int>(mIdleAnimation[frame].GetWidth() * mScale);
            int height = static_cast<int>(mIdleAnimation[frame].GetHeight() * mScale);
            mIdleAnimation[frame].Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
            break;
        }
        case 1: // ��ų1
        {
            if (stateTimer < 0.5f) { // �뽬
                int width = static_cast<int>(dashImage->GetWidth() * mScale);
                int height = static_cast<int>(dashImage->GetHeight() * mScale);
                dashImage->Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
            }
            else if (stateTimer < 0.7f) { // 0.2�� ���� (spin0)
                int width = static_cast<int>(mSpinImage[0].GetWidth() * mScale);
                int height = static_cast<int>(mSpinImage[0].GetHeight() * mScale);
                mSpinImage[0].Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
            }
            else { // ȸ��
                int width = static_cast<int>(mSpinImage[mCurrenAttackFrame].GetWidth() * mScale);
                int height = static_cast<int>(mSpinImage[mCurrenAttackFrame].GetHeight() * mScale);
                mSpinImage[mCurrenAttackFrame].Draw(hdc, (int)(mX - width / 2), (int)(mY - height / 2), width, height);
            }
            break;
        }
        case 2: // ��ų2
        case 3: // ��ų3
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
    }

    // Į ������
    if (currentState == 1 && stateTimer >= 0.7f && stateTimer < 1.5f) {
        int swordFrame = (stateTimer < 0.9f) ? static_cast<int>((stateTimer - 0.7f) / 0.05f) % 4 : 3;
        float imageAngle = (stateTimer < 0.9f) ? 0.0f : atan2(mY - mSwordY, mX - mSwordX); // Į���� ���� �߽��� ����
        int swordWidth = static_cast<int>(mAnimaionIceSword[swordFrame].GetWidth() * mScale);
        int swordHeight = static_cast<int>(mAnimaionIceSword[swordFrame].GetHeight() * mScale);

        XFORM xForm = { 0 };
        xForm.eM11 = cos(imageAngle);
        xForm.eM12 = sin(imageAngle);
        xForm.eM21 = -sin(imageAngle);
        xForm.eM22 = cos(imageAngle);
        xForm.eDx = mSwordX;
        xForm.eDy = mSwordY;

        SetGraphicsMode(hdc, GM_ADVANCED);
        SetStretchBltMode(hdc, HALFTONE); // ������ ǰ�� ����
        SetWorldTransform(hdc, &xForm);

        HDC srcDC = mAnimaionIceSword[swordFrame].GetDC();

        float halfHeight = swordHeight / 2.0f;
        TransparentBlt(
            hdc,
            (int)(-swordWidth / 2), (int)(-halfHeight), swordWidth, swordHeight,
            srcDC,
            0, 0, mAnimaionIceSword[swordFrame].GetWidth(), mAnimaionIceSword[swordFrame].GetHeight(),
            RGB(0, 0, 0)
        );
        mAnimaionIceSword[swordFrame].ReleaseDC();

        XFORM identity = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
        SetWorldTransform(hdc, &identity);
        SetGraphicsMode(hdc, GM_COMPATIBLE);
    }

    // ������ �ؽ�Ʈ ������
    if (mShowDamage && mIsHit) {
        SetTextColor(hdc, RGB(255, 255, 255));
        HFONT hFont = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
        wchar_t damageText[32];
        swprintf_s(damageText, L"%d", mDamageTaken);

        TextOut(hdc, static_cast<int>(mX) - 40, static_cast<int>(mDamageTextY), damageText, wcslen(damageText));
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
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