#include "Archer.h"
#include "Time.h"
#include "Arrow.h"
#include "SoundManager.h"
#include "SceneManager.h"

Archer::Archer()
{
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    mX = 1920 / 2;
    mY = 1080 / 2;
    rect = { (int)(mX - 20), (int)(mY + 200), (int)(mX + 20), (int)(mY + 20) };
    speed = 100.0f;
    state = EnemyState::RIGHT;
    hp = 100;
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

    mDamageTaken = 0;
    mDamageTextY = 0.0f;
    mDamageTextSpeed = 50.0f; // Speed of upward movement (pixels per second)
    mShowDamage = false;
    mHitEffectAngle = 0.0f; // 초기화
    mRightIdleAnimation.Load(L"resources/Monster/ARCHER/ArcherRight/ArcherIdle/ARCHER_RIGHT_00.png");
    if (mRightIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/ARCHER/ArcherRight/ArcherIdle/ARCHER_RIGHT_00.png\n");
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherRight/ArcherAttack/ARCHER_RIGHT_%02d.png", i);
        mRightAttackAnimation[i].Load(path);
        if (mRightAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherRight/ArcherDie/ARCHER_RIGHT_%02d.png", i);
        mRightDieAnimaion[i].Load(path);
        if (mRightDieAnimaion[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherRight/ArcherHit/ARCHER_RIGHT_%02d.png", i);
        mRightHitAnimation[i].Load(path);
        if (mRightHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 5; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherRight/ArcherWalk/ARCHER_RIGHT_%02d.png", i);
        mRightWalkAnimation[i].Load(path);
        if (mRightWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    mLeftIdleAnimation.Load(L"resources/Monster/ARCHER/ArcherLeft/ArcherIdle/ARCHER_LEFT_00.png");
    if (mLeftIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/ARCHER/ArcherLeft/ArcherIdle/ARCHER_LEFT_00.png\n");
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherLeft/ArcherAttack/ARCHER_LEFT_%02d.png", i);
        mLeftAttackAnimation[i].Load(path);
        if (mLeftAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherLeft/ArcherDie/ARCHER_LEFT_%02d.png", i);
        mLeftDieAnimaion[i].Load(path);
        if (mLeftDieAnimaion[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherLeft/ArcherHit/ARCHER_LEFT_%02d.png", i);
        mLeftHitAnimation[i].Load(path);
        if (mLeftHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 5; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherLeft/ArcherWalk/ARCHER_LEFT_%02d.png", i);
        mLeftWalkAnimation[i].Load(path);
        if (mLeftWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ARCHER_BOW/Idle/ARCHER_BOW_%02d.png", i);
        mBowAttackAnimation[i].Load(path);
        if (mBowAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    // 피격 이펙트
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/HitEffect/HITEFFECT_%d.png", i);
        mHitEffectAnimation[i].Load(path); // 텔레포트 애니메이션 로드
        if (mHitEffectAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
}

Archer::~Archer()
{
    // Destroy single CImage objects
    mRightIdleAnimation.Destroy();
    mLeftIdleAnimation.Destroy();

    // Destroy CImage arrays
    for (int i = 0; i < 4; ++i)
    {
        mRightAttackAnimation[i].Destroy();
        mLeftAttackAnimation[i].Destroy();
        mBowAttackAnimation[i].Destroy();
        mHitEffectAnimation[i].Destroy();
    }
    for (int i = 0; i < 6; ++i)
    {
        mRightDieAnimaion[i].Destroy();
        mLeftDieAnimaion[i].Destroy();
    }
    for (int i = 0; i < 2; ++i)
    {
        mRightHitAnimation[i].Destroy();
        mLeftHitAnimation[i].Destroy();
    }
    for (int i = 0; i < 5; ++i)
    {
        mRightWalkAnimation[i].Destroy();
        mLeftWalkAnimation[i].Destroy();
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);
}

void Archer::Update(Player& p, Scene* stage)
{
    int imageWidth = mRightIdleAnimation.GetWidth();
    int imageHeight = mRightIdleAnimation.GetHeight();
    rect.left = static_cast<int>(mX - imageWidth / 2.0f) + 45;
    rect.top = static_cast<int>(mY - imageHeight / 2.0f) + 40;
    rect.right = static_cast<int>(mX + imageWidth / 2.0f) - 54;
    rect.bottom = static_cast<int>(mY + imageHeight / 2.0f) - 33;

    // 죽음 상태 처리
    if (mIsDead) 
    {
       
        static float dieTimer = 0.0f;
        if (mCurrentDeadFrame < 5) { // 0~5까지 6프레임
            dieTimer += Time::DeltaTime();
            if (dieTimer >= 0.1f) {
                mCurrentDeadFrame++;
                dieTimer = 0.0f;
            }
        }
        rect = { (int)mX,(int)mY,(int)mX,(int)mY }; // 죽으면 rect를 비활성화
        mShowDamage = false; // 죽을 때 데미지 텍스트 비활성화
        return;
    }

    // 피격 상태 처리
    float deltaTime = Time::DeltaTime();
    static float animationTimer = 0.0f;
    const float hitFrameDuration = 0.1f;
    if (mIsHit) {
        mHitTimer -= deltaTime;
        if (mHitTimer <= 0.0f) {
            mIsHit = false;
            mShowDamage = false; // 피격 애니메이션 종료 시 데미지 텍스트 비활성화
        }
        animationTimer += deltaTime;
        if (animationTimer >= hitFrameDuration) {
            mCurrentHitFrame = (mCurrentHitFrame + 1) % 2;
            animationTimer = 0.0f;
        }
        // 데미지 텍스트 위로 이동
        if (mShowDamage) {
            mDamageTextY -= mDamageTextSpeed * deltaTime;
        }
     
        return;
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
            if (mCurrenAttackFrame == 1) {
                SoundManager::GetInstance()->mPlaySound("ArcherReload", false);
            }

            if (mCurrenAttackFrame >= 4)
            {
                mIsAttack = false;
                mCurrenAttackFrame = 0;
                mAttackCooldown = 3.0f;
                Arrow::FireArrow(p, mX, mY, stage);
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
            mCurrentWalkFrame = (mCurrentWalkFrame + 1) % 5;
            walkFrameTime = 0.0f;
        }
    }
    else
    {
        walkFrameTime = 0.0f;
    }
}

void Archer::LateUpdate()
{
}

void Archer::Render(HDC hdc, Player& p)
{
    // 디버그 범위 그리기
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

    int imageWidth = 141 - 60;
    int imageHeight = 145 - 60;
    int drawX = static_cast<int>(mX - imageWidth / 2.0f);
    int drawY = static_cast<int>(mY - imageHeight / 2.0f);

    currentImage->Draw(hdc, drawX, drawY, imageWidth, imageHeight);

    // 피격 이펙트 렌더링
    if (mIsHit)
    {
        CImage& effectImage = mHitEffectAnimation[mCurrentHitFrame];
        if (!effectImage.IsNull())
        {
            int effectWidth = effectImage.GetWidth() / 3.0f;
            int effectHeight = effectImage.GetHeight() / 3.0f;
            float drawX = mX - effectWidth / 2.0f + 3;
            float drawY = mY - effectHeight / 2.0f;

            // 변환 행렬 설정
            XFORM xForm = { 0 };
            xForm.eM11 = cos(mHitEffectAngle); // X 스케일 및 회전
            xForm.eM12 = sin(mHitEffectAngle); // X에 대한 Y의 기울기
            xForm.eM21 = -sin(mHitEffectAngle); // Y에 대한 X의 기울기
            xForm.eM22 = cos(mHitEffectAngle); // Y 스케일 및 회전
            xForm.eDx = mX; // 중심점 X로 이동
            xForm.eDy = mY; // 중심점 Y로 이동

            // 그래픽 모드 및 변환 적용
            SetGraphicsMode(hdc, GM_ADVANCED);
            SetWorldTransform(hdc, &xForm);

            // 이펙트 렌더링
            HDC srcDC = effectImage.GetDC();
            TransparentBlt(
                hdc,
                -effectWidth / 2, -effectHeight / 2, effectWidth, effectHeight,
                srcDC,
                0, 0, effectImage.GetWidth(), effectImage.GetHeight(),
                RGB(0, 0, 0) // 투명색
            );
            effectImage.ReleaseDC();

            // 변환 행렬 초기화
            XFORM identity = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
            SetWorldTransform(hdc, &identity);
            SetGraphicsMode(hdc, GM_COMPATIBLE);
        }
    }

    // 데미지 텍스트 렌더링
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
        Gdiplus::ImageAttributes imageAttr;
        imageAttr.SetColorKey(Gdiplus::Color(0, 0, 0), Gdiplus::Color(0, 0, 0));

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

        float bowOffset = 20.0f;
        Gdiplus::PointF startPoint(mX + directionX * bowOffset, mY + directionY * bowOffset);
        Gdiplus::PointF endPoint(mX + directionX * AttackRange * 2, mY + directionY * AttackRange * 2);
        graphics.DrawLine(&pen, startPoint, endPoint);

        int bowFrame = (std::min)(mCurrenAttackFrame, 3);
        CImage* bowImage = &mBowAttackAnimation[bowFrame];
        int bowWidth = static_cast<int>(bowImage->GetWidth() * 0.4f);
        int bowHeight = static_cast<int>(bowImage->GetHeight() * 0.4f);
        int bowDrawX = static_cast<int>(mX + directionX * bowOffset - bowWidth / 2.0f);
        int bowDrawY = static_cast<int>(mY + directionY * bowOffset - bowHeight / 2.0f);

        Gdiplus::Matrix matrix;
        matrix.RotateAt(angle, Gdiplus::PointF(mX + directionX * bowOffset, mY + directionY * bowOffset));
        graphics.SetTransform(&matrix);

        Gdiplus::Bitmap bowBitmap((HBITMAP)*bowImage, nullptr);

        graphics.DrawImage(&bowBitmap,
            Gdiplus::Rect(bowDrawX, bowDrawY, bowWidth, bowHeight),
            0, 0, bowBitmap.GetWidth(), bowBitmap.GetHeight(),
            Gdiplus::UnitPixel, &imageAttr);

        graphics.ResetTransform();
    }
}

void Archer::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
}

void Archer::TakeDamage(int d)
{
    if (mIsDead) return;
    mHitEffectAngle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
    hp -= d;
    mDamageTaken = d; // 데미지 값 저장
    mShowDamage = true; // 데미지 텍스트 표시 활성화
    mDamageTextY = mY - 50; // 초기 텍스트 위치 (캐릭터 위쪽)

    if (hp <= 0) {
        SoundManager::GetInstance()->mPlaySound("EnemyDead", false);
        hp = 0;
        mIsDead = true;
        mIsHit = false;
        mIsAttack = false;
        mIsMoving = false;
        mCurrentDeadFrame = 0;
        mShowDamage = false; // 죽을 때 데미지 텍스트 비활성화
        SceneManager::GetSharedPlayer()->PlusKillCount();
    }
    else {
        SoundManager::GetInstance()->mPlaySound("Hit", false);
        mIsHit = true;
        mHitTimer = 0.2f; // 피격 애니메이션 0.2초
        mCurrentHitFrame = 0;
        mIsAttack = false;
        mIsMoving = false;
    }
}