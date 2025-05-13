#include "Archer.h"
#include "Time.h"
Archer::Archer()
{
    mX = 1920 / 2;
    mY = 1080 / 2;
    rect = { (int)(mX - 20), (int)(mY + 200), (int)(mX + 20), (int)(mY + 20) };
    speed = 100.0f;
    state = EnemyState::RIGHT;

    // Right 애니메이션 로드
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

    // Left 애니메이션 로드
    mLeftIdleAnimation.Load(L"resources/Monster/ARCHER/ArcherLeft/ArcherIdle/ARCHER_LEFT_00.png");
    if (mLeftIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/ARCHER/ArcherLeft/ArcherIdle/ARCHER_LEFT_00 לגדול.png\n");
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
}

void Archer::Update(Player& p)
{
    // rect 업데이트 (이미지 중심에 맞게)
    int imageWidth = mRightIdleAnimation.GetWidth(); // 기본적으로 Idle 이미지 크기 기준
    int imageHeight = mRightIdleAnimation.GetHeight();
    rect.left = static_cast<int>(mX - imageWidth / 2.0f) + 45;
    rect.top = static_cast<int>(mY - imageHeight / 2.0f) + 40;
    rect.right = static_cast<int>(mX + imageWidth / 2.0f) - 54;
    rect.bottom = static_cast<int>(mY + imageHeight / 2.0f) - 33;

    // 애니메이션 프레임 업데이트
    static float frameTime = 0.0f;
    frameTime += Time::DeltaTime(); // Time.h에서 제공되는 DeltaTime 사용

    if (mIsdead)
    {
        if (frameTime >= 0.1f)
        {
            mCurrentDeadFrame = (mCurrentDeadFrame + 1) % 6; // Die: 6프레임
            frameTime = 0.0f;
        }
    }
    else if (mIsHit)
    {
        if (frameTime >= 0.1f)
        {
            mCurrentHitFrame = (mCurrentHitFrame + 1) % 2; // Hit: 2프레임
            frameTime = 0.0f;
        }
    }
    else if (mIsAttack)
    {
        if (frameTime >= 0.1f)
        {
            mCurrenAttackFrame = (mCurrenAttackFrame + 1) % 3; // Attack: 3프레임
            frameTime = 0.0f;
        }
    }
    else if (mIsMoving)
    {
        if (frameTime >= 0.1f)
        {
            mCurrentWalkFrame = (mCurrentWalkFrame + 1) % 6; // Walk: 6프레임
            frameTime = 0.0f;
        }
    }
}

void Archer::LateUpdate()
{
}

void Archer::Render(HDC hdc)
{
    // 충돌 영역 사각형
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    CImage* currentImage = nullptr;

    // 상태에 따라 렌더링할 이미지 선택
    if (mIsdead)
    {
        if (state == EnemyState::RIGHT)
            currentImage = &mRightDieAnimaion[mCurrentDeadFrame];
        else
            currentImage = &mLeftDieAnimaion[mCurrentDeadFrame];
    }
    else if (mIsHit)
    {
        if (state == EnemyState::RIGHT)
            currentImage = &mRightHitAnimation[mCurrentHitFrame];
        else
            currentImage = &mLeftHitAnimation[mCurrentHitFrame];
    }
    else if (mIsAttack)
    {
        if (state == EnemyState::RIGHT)
            currentImage = &mRightAttackAnimation[mCurrenAttackFrame];
        else
            currentImage = &mLeftAttackAnimation[mCurrenAttackFrame];
    }
    else if (mIsMoving)
    {
        if (state == EnemyState::RIGHT)
            currentImage = &mRightWalkAnimation[mCurrentWalkFrame];
        else
            currentImage = &mLeftWalkAnimation[mCurrentWalkFrame];
    }
    else
    {
        if (state == EnemyState::RIGHT)
            currentImage = &mRightIdleAnimation;
        else
            currentImage = &mLeftIdleAnimation;
    }

    // 이미지 크기 가져오기
    int imageWidth = 141 - 60;
    int imageHeight = 145 - 60;

    // 중심을 mX, mY에 맞추기 위해 왼쪽 상단 좌표 계산
    int drawX = static_cast<int>(mX - imageWidth / 2.0f);
    int drawY = static_cast<int>(mY - imageHeight / 2.0f);

    // 캐릭터 이미지 그리기
    currentImage->Draw(hdc, drawX, drawY, imageWidth, imageHeight);

    
}

void Archer::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
}
