#include "Wizard.h"
#include "Time.h"
#include "FireBall.h"
#include "SceneManager.h"
#include "SoundManager.h"

Wizard::Wizard()
{
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    mX = 1920 / 2;
    mY = 1080 / 2;
    rect = { (int)(mX - 22), (int)(mY - 38), (int)(mX + 20), (int)(mY + 38) };
    speed = 100.0f;
    state = EnemyState::RIGHT;
    hp = 150;
    damage = 10;
    mIsMoving = false;
    mCurrentWalkFrame = 0;
    mIsAttack = false;
    mCurrenAttackFrame = 0;
    mIsDead = false;
    mCurrentDeadFrame = 0;
    mIsHit = false;
    mCurrentHitFrame = 0;

    mHitTimer = 0.0f;

    mAttackFrameTime = 0.0f;
    mAttackCooldown = 0.0f;
    PlayerDetectRange = 300.0f;
    AttackRange = 250.0f;

    // ������ ǥ�� ���� �ʱ�ȭ
    mDamageTaken = 0;
    mDamageTextY = 0.0f;
    mDamageTextSpeed = 50.0f; // �ؽ�Ʈ ��� �ӵ� (�ʴ� �ȼ�)
    mShowDamage = false;
    mHitEffectAngle = 0.0f; // �ʱ�ȭ
    // Right �ִϸ��̼� �ε�
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

    // Left �ִϸ��̼� �ε�
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
    // �ǰ� ����Ʈ
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/HitEffect/HITEFFECT_%d.png", i);
        mHitEffectAnimation[i].Load(path); // �ڷ���Ʈ �ִϸ��̼� �ε�
        if (mHitEffectAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
}

Wizard::~Wizard()
{
    // Destroy single CImage objects
    mRightIdleAnimation.Destroy();
    mLeftIdleAnimation.Destroy();

    // Destroy CImage arrays
    for (int i = 0; i < 4; ++i)
    {
        mRightAttackAnimation[i].Destroy();
        mLeftAttackAnimation[i].Destroy();
        mRightWalkAnimation[i].Destroy();
        mLeftWalkAnimation[i].Destroy();
        mHitEffectAnimation[i].Destroy();
    }
    for (int i = 0; i < 5; ++i)
    {
        mRightDieAnimaion[i].Destroy();
        mLeftDieAnimaion[i].Destroy();
    }
    for (int i = 0; i < 2; ++i)
    {
        mRightHitAnimation[i].Destroy();
        mLeftHitAnimation[i].Destroy();
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);
}

void Wizard::Update(Player& p, Scene* stage)
{
    int imageWidth = mRightIdleAnimation.GetWidth();
    int imageHeight = mRightIdleAnimation.GetHeight();
    // rect ������Ʈ
    rect.left = mX - 22;
    rect.top = mY - 38;
    rect.right = mX + 20;
    rect.bottom = mY + 38;

    // ���� ���� ó��
    if (mIsDead) {
        
        static float dieTimer = 0.0f;
        if (mCurrentDeadFrame < 4) { // 0~4���� 5������
            dieTimer += Time::DeltaTime();
            if (dieTimer >= 0.1f) {
                mCurrentDeadFrame++;
                dieTimer = 0.0f;
            }
        }
        rect = { (int)mX, (int)mY, (int)mX, (int)mY }; // ������ rect�� ��Ȱ��ȭ
        mShowDamage = false; // ���� �� ������ �ؽ�Ʈ ��Ȱ��ȭ
        return;
    }

    // �ǰ� ���� ó��
    float deltaTime = Time::DeltaTime();
    static float animationTimer = 0.0f;
    const float hitFrameDuration = 0.1f;
    if (mIsHit) {
        mHitTimer -= deltaTime;
        if (mHitTimer <= 0.0f) {
            mIsHit = false;
            mShowDamage = false; // �ǰ� �ִϸ��̼� ���� �� ������ �ؽ�Ʈ ��Ȱ��ȭ
        }
        animationTimer += deltaTime;
        if (animationTimer >= hitFrameDuration) {
            mCurrentHitFrame = (mCurrentHitFrame + 1) % 2;
            animationTimer = 0.0f;
        }
        // ������ �ؽ�Ʈ ���� �̵�
        if (mShowDamage) {
            mDamageTextY -= mDamageTextSpeed * deltaTime;
        }
       
    }

    float playerX = p.GetPositionX();
    float playerY = p.GetPositionY();
    float distance = static_cast<float>(sqrt(pow(mX - playerX, 2) + pow(mY - playerY, 2)));

    if (mAttackCooldown > 0.0f)
    {
        mAttackCooldown -= Time::DeltaTime();
    }

    if (playerX > mX)
    {
        state = EnemyState::RIGHT;
    }
    else if (playerX < mX)
    {
        state = EnemyState::LEFT;
    }

    if (mIsAttack)
    {
        mAttackFrameTime += Time::DeltaTime();
        float frameDuration = 0.5f;
        if (mAttackFrameTime >= frameDuration)
        {
            mCurrenAttackFrame++;
            if (mCurrenAttackFrame >= 4)
            {
                SoundManager::GetInstance()->mPlaySound("EnemyFireBall", false);
                mIsAttack = false;
                mCurrenAttackFrame = 0;
                mAttackCooldown = 3.0f;
                FireBall::ThrowFireBall(p, mX, mY, stage);
            }
            mAttackFrameTime = 0.0f;
        }
    }
    else
    {
        if (distance <= AttackRange && mAttackCooldown <= 0.0f)
        {
            mIsAttack = true;
            mIsMoving = false;
            mCurrenAttackFrame = 0;
            mAttackFrameTime = 0.0f;
            mCurrentWalkFrame = 0;
        }
        else if (distance <= PlayerDetectRange)
        {
            if (!mIsMoving)
            {
                mCurrentWalkFrame = 0;
            }
            mIsAttack = false;
            mIsMoving = true;
            float directionX = (playerX - mX) / distance;
            float directionY = (playerY - mY) / distance;
            mX += directionX * speed * Time::DeltaTime();
            mY += directionY * speed * Time::DeltaTime();
        }
        else
        {
            mIsAttack = false;
            mIsMoving = false;
            mCurrenAttackFrame = 0;
            mCurrentWalkFrame = 0;
        }
    }

    static float walkFrameTime = 0.0f;
    if (!mIsAttack && mIsMoving)
    {
        walkFrameTime += Time::DeltaTime();
        if (walkFrameTime >= 0.1f)
        {
            mCurrentWalkFrame = (mCurrentWalkFrame + 1) % 4;
            walkFrameTime = 0.0f;
        }
    }
    else
    {
        walkFrameTime = 0.0f;
    }
}

void Wizard::LateUpdate()
{
}

void Wizard::Render(HDC hdc, Player& p)
{
    HPEN attackPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN detectPen = CreatePen(PS_DOT, 1, RGB(0, 255, 0));
    HBRUSH nullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HPEN oldPen = (HPEN)SelectObject(hdc, attackPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, nullBrush);

    Ellipse(hdc,
        static_cast<int>(mX - AttackRange),
        static_cast<int>(mY - AttackRange),
        static_cast<int>(mX + AttackRange),
        static_cast<int>(mY + AttackRange));

    SelectObject(hdc, detectPen);
    Ellipse(hdc,
        static_cast<int>(mX - PlayerDetectRange),
        static_cast<int>(mY - PlayerDetectRange),
        static_cast<int>(mX + PlayerDetectRange),
        static_cast<int>(mY + PlayerDetectRange));

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(attackPen);
    DeleteObject(detectPen);

    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    CImage* currentImage = nullptr;

    if (mIsDead)
    {
        currentImage = (state == EnemyState::RIGHT) ? &mRightDieAnimaion[mCurrentDeadFrame] : &mLeftDieAnimaion[mCurrentDeadFrame];
    }
    else if (mIsHit)
    {
        currentImage = (state == EnemyState::RIGHT) ? &mRightHitAnimation[mCurrentHitFrame] : &mLeftHitAnimation[mCurrentHitFrame];
    }
    else if (mIsAttack)
    {
        currentImage = (state == EnemyState::RIGHT) ? &mRightAttackAnimation[mCurrenAttackFrame] : &mLeftAttackAnimation[mCurrenAttackFrame];
    }
    else if (mIsMoving)
    {
        currentImage = (state == EnemyState::RIGHT) ? &mRightWalkAnimation[mCurrentWalkFrame] : &mLeftWalkAnimation[mCurrentWalkFrame];
    }
    else
    {
        currentImage = (state == EnemyState::RIGHT) ? &mRightIdleAnimation : &mLeftIdleAnimation;
    }

    int imageWidth = 61;
    int imageHeight = 101;
    int drawX = static_cast<int>(mX - imageWidth / 2.0f);
    int drawY = static_cast<int>(mY - imageHeight / 2.0f);

    currentImage->Draw(hdc, drawX, drawY, imageWidth, imageHeight);

    // �ǰ� ����Ʈ ������
    if (mIsHit)
    {
        CImage& effectImage = mHitEffectAnimation[mCurrentHitFrame];
        if (!effectImage.IsNull())
        {
            int effectWidth = effectImage.GetWidth() / 3.0f;
            int effectHeight = effectImage.GetHeight() / 3.0f;
            float drawX = mX - effectWidth / 2.0f + 3;
            float drawY = mY - effectHeight / 2.0f;

            // ��ȯ ��� ����
            XFORM xForm = { 0 };
            xForm.eM11 = cos(mHitEffectAngle); // X ������ �� ȸ��
            xForm.eM12 = sin(mHitEffectAngle); // X�� ���� Y�� ����
            xForm.eM21 = -sin(mHitEffectAngle); // Y�� ���� X�� ����
            xForm.eM22 = cos(mHitEffectAngle); // Y ������ �� ȸ��
            xForm.eDx = mX; // �߽��� X�� �̵�
            xForm.eDy = mY; // �߽��� Y�� �̵�

            // �׷��� ��� �� ��ȯ ����
            SetGraphicsMode(hdc, GM_ADVANCED);
            SetWorldTransform(hdc, &xForm);

            // ����Ʈ ������
            HDC srcDC = effectImage.GetDC();
            TransparentBlt(
                hdc,
                -effectWidth / 2, -effectHeight / 2, effectWidth, effectHeight,
                srcDC,
                0, 0, effectImage.GetWidth(), effectImage.GetHeight(),
                RGB(0, 0, 0) // �����
            );
            effectImage.ReleaseDC();

            // ��ȯ ��� �ʱ�ȭ
            XFORM identity = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
            SetWorldTransform(hdc, &identity);
            SetGraphicsMode(hdc, GM_COMPATIBLE);
        }
    }

    // ������ �ؽ�Ʈ ������
    if (mShowDamage && mIsHit)
    {
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

    if (mIsAttack)
    {
        Gdiplus::Graphics graphics(hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

        int alpha = (mCurrenAttackFrame == 0) ? 64 :
            (mCurrenAttackFrame == 1) ? 128 :
            (mCurrenAttackFrame == 2) ? 192 : 255;
        Gdiplus::Pen pen(Gdiplus::Color(alpha, 255, 0, 0), 2.0f);

        float dx = p.GetPositionX() - mX;
        float dy = p.GetPositionY() - mY;
        float distance = static_cast<float>(sqrt(dx * dx + dy * dy));
        float directionX = (distance > 0.0f) ? dx / distance : (state == EnemyState::RIGHT ? 1.0f : -1.0f);
        float directionY = (distance > 0.0f) ? dy / distance : 0.0f;
        float angle = static_cast<float>(atan2(dy, dx) * 180.0 / 3.1415926535);

        float wandOffset = 10.0f;
        // Gdiplus::PointF startPoint(mX + directionX * wandOffset, mY + directionY * wandOffset);
        // Gdiplus::PointF endPoint(mX + directionX * AttackRange * 2, mY + directionY * AttackRange * 2);
        // graphics.DrawLine(&pen, startPoint, endPoint);
    }
}

void Wizard::TakeDamage(int d)
{
    if (mIsDead) return;
    hp -= d;
    mDamageTaken = d; // ������ �� ����
    mShowDamage = true; // ������ �ؽ�Ʈ ǥ�� Ȱ��ȭ
    mDamageTextY = mY - 50; // �ʱ� �ؽ�Ʈ ��ġ (ĳ���� ����)
    mHitEffectAngle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
    if (hp <= 0) {
        SoundManager::GetInstance()->mPlaySound("EnemyDead", false);
        hp = 0;
        mIsDead = true;
        mIsHit = false;
        mIsAttack = false;
        mIsMoving = false;
        mCurrentDeadFrame = 0;
        mShowDamage = false; // ���� �� ������ �ؽ�Ʈ ��Ȱ��ȭ
        SceneManager::GetSharedPlayer()->PlusKillCount();
    }
    else {
        SoundManager::GetInstance()->mPlaySound("Hit", false);
        mIsHit = true;
        mHitTimer = 0.2f; // �ǰ� �ִϸ��̼� 0.2��
        mCurrentHitFrame = 0;
        mIsAttack = false;
        mIsMoving = false;
    }
}

void Wizard::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
}