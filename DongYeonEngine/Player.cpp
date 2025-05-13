#include "Player.h"
#include "Input.h"
#include "Time.h"

Player::Player()
{
    mX = 1920 / 2;
    mY = 1080 / 2;
    rect = { (int)(mX - 20), (int)(mY + 200), (int)(mX + 20), (int)(mY + 20) };
    speed = 200.0f;
    state = PlayerState::FRONT;
    hp = 100;
    mIsMoving = false;
    mIsDashing = false;
    mIsAttack = false;
    mIsHit = false;
    mIsDead = false;
    mCurrentWalkFrame = 0;
    mCurrentDashFrame = 0;
    mCurrentAttackFrame = 0;
    mCurrentHitFrame = 0;
    mCurrentDieFrame = 0;
    mDashTimer = 0.0f;
    mAttackTimer = 0.0f;
    mHitTimer = 0.0f;

    // Front 애니메이션 로드
    mFrontIdleAnimation.Load(L"resources/Player/Front/Idle/FRONT_COMPLETE_00.png");
    if (mFrontIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Player/Front/Idle/FRONT_COMPLETE_00.png\n");
    for (int i = 0; i < 16; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Front/Attack/FRONT_COMPLETE_%02d.png", i);
        mFrontAttackAnimation[i].Load(path);
        if (mFrontAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 8; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Front/Dash/FRONT_COMPLETE_%02d.png", i);
        mFrontDashAnimation[i].Load(path);
        if (mFrontDashAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 7; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Front/Die/FRONT_COMPLETE_%02d.png", i);
        mFrontDieAnimation[i].Load(path);
        if (mFrontDieAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Front/Hit/FRONT_COMPLETE_%02d.png", i);
        mFrontHitAnimation[i].Load(path);
        if (mFrontHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 10; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Front/Walk/FRONT_COMPLETE_%02d.png", i);
        mFrontWalkAnimation[i].Load(path);
        if (mFrontWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    // Back 애니메이션 로드
    mBackIdleAnimation.Load(L"resources/Player/Back/Idle/BACK_COMPLETE_00.png");
    if (mBackIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Player/Back/Idle/BACK_COMPLETE_00.png\n");
    for (int i = 0; i < 16; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Back/Attack/BACK_COMPLETE_%02d.png", i);
        mBackAttackAnimation[i].Load(path);
        if (mBackAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 8; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Back/Dash/BACK_COMPLETE_%02d.png", i);
        mBackDashAnimation[i].Load(path);
        if (mBackDashAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Back/Hit/BACK_COMPLETE_%02d.png", i);
        mBackHitAnimation[i].Load(path);
        if (mBackHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 10; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Back/Walk/BACK_COMPLETE_%02d.png", i);
        mBackWalkAnimation[i].Load(path);
        if (mBackWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    // Right 애니메이션 로드
    mRightIdleAnimation.Load(L"resources/Player/Right/Idle/RIGHT_COMPLETE_00.png");
    if (mRightIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Player/Right/Idle/RIGHT_COMPLETE_00.png\n");
    for (int i = 0; i < 16; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Right/Attack/RIGHT_COMPLETE_%02d.png", i);
        mRightAttackAnimation[i].Load(path);
        if (mRightAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 8; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Right/Dash/RIGHT_COMPLETE_%02d.png", i);
        mRightDashAnimation[i].Load(path);
        if (mRightDashAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Right/Hit/RIGHT_COMPLETE_%02d.png", i);
        mRightHitAnimation[i].Load(path);
        if (mRightHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 10; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Right/Walk/RIGHT_COMPLETE_%02d.png", i);
        mRightWalkAnimation[i].Load(path);
        if (mRightWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    // Left 애니메이션 로드
    mLeftIdleAnimation.Load(L"resources/Player/Left/Idle/LEFT_COMPLETE_00.png");
    if (mLeftIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Player/Left/Idle/LEFT_COMPLETE_00.png\n");
    for (int i = 0; i < 16; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Left/Attack/LEFT_COMPLETE_%02d.png", i);
        mLeftAttackAnimation[i].Load(path);
        if (mLeftAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 8; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Left/Dash/LEFT_COMPLETE_%02d.png", i);
        mLeftDashAnimation[i].Load(path);
        if (mLeftDashAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Left/Hit/LEFT_COMPLETE_%02d.png", i);
        mLeftHitAnimation[i].Load(path);
        if (mLeftHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 10; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Left/Walk/LEFT_COMPLETE_%02d.png", i);
        mLeftWalkAnimation[i].Load(path);
        if (mLeftWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
}

void Player::Update()
{
    if (mIsDead) {
        // 사망 상태: Die 애니메이션 업데이트 (한 번만 재생)
        static float dieTimer = 0.0f;
        if (mCurrentDieFrame < 6) { // 마지막 프레임(6)까지 재생
            dieTimer += Time::DeltaTime();
            if (dieTimer >= 0.1f) {
                mCurrentDieFrame++; // 프레임 증가
                dieTimer = 0.0f;
            }
        }
        return; // 사망 시 다른 로직 실행 안 함
    }

    bool isMoving = false;
    float deltaTime = Time::DeltaTime();
    static float animationTimer = 0.0f;
    const float walkFrameDuration = 0.1f;  // Walk 애니메이션 프레임 지속 시간
    const float dashDuration = 0.4f;       // 대쉬 지속 시간
    const float dashFrameDuration = dashDuration / 8.0f; // Dash 프레임당 지속 시간
    const float attackDuration = 0.8f;     // 공격 지속 시간 (16프레임)
    const float attackFrameDuration = attackDuration / 16.0f; // 공격 프레임당 지속 시간
    const float hitDuration = 0.2f;        // 피격 지속 시간 (2프레임)
    const float hitFrameDuration = hitDuration / 2.0f; // 피격 프레임당 지속 시간
    float currentSpeed = speed;

    // rect 업데이트
    int imageWidth = mFrontIdleAnimation.GetWidth();
    int imageHeight = mFrontIdleAnimation.GetHeight();
    rect.left = static_cast<int>(mX - imageWidth / 2.0f) + 30;
    rect.top = static_cast<int>(mY - imageHeight / 2.0f) + 10;
    rect.right = static_cast<int>(mX + imageWidth / 2.0f) - 18;
    rect.bottom = static_cast<int>(mY + imageHeight / 2.0f);

    // 피격 상태 처리
    if (mIsHit) {
        mHitTimer -= deltaTime;
        if (mHitTimer <= 0.0f) {
            mIsHit = false;
        }
        animationTimer += deltaTime;
        if (animationTimer >= hitFrameDuration) {
            mCurrentHitFrame = (mCurrentHitFrame + 1) % 2; // 2프레임
            animationTimer = 0.0f;
        }
        return; // 피격 중에는 다른 입력 처리 안 함
    }

    // 공격 처리
    if (!mIsAttack && Input::GetKeyDown(eKeyCode::LButton)) {
        mIsAttack = true;
        mAttackTimer = attackDuration;
        mCurrentAttackFrame = 0;
        animationTimer = 0.0f;
    }
    if (mIsAttack) {
        mAttackTimer -= deltaTime;
        if (mAttackTimer <= 0.0f) {
            mIsAttack = false;
        }
        animationTimer += deltaTime;
        if (animationTimer >= attackFrameDuration) {
            mCurrentAttackFrame = (mCurrentAttackFrame + 1) % 16; // 16프레임
            animationTimer = 0.0f;
        }
        return; // 공격 중에는 이동 처리 안 함
    }

    // 대쉬 처리
    if (!mIsDashing && Input::GetKeyDown(eKeyCode::SPACE)) {
        mIsDashing = true;
        mDashTimer = dashDuration;
        mCurrentDashFrame = 0;
        animationTimer = 0.0f;
    }
    if (mIsDashing) {
        currentSpeed = 400.0f;
        mDashTimer -= deltaTime;
        if (mDashTimer <= 0.0f) {
            mIsDashing = false;
        }
        animationTimer += deltaTime;
        if (animationTimer >= dashFrameDuration) {
            mCurrentDashFrame = (mCurrentDashFrame + 1) % 8; // 8프레임
            animationTimer = 0.0f;
        }
    }

    // 이동 로직
    if (mIsDashing) {
        // 대쉬 중에는 현재 state 방향으로만 이동
        switch (state) {
        case PlayerState::BACK:
            mY -= currentSpeed * deltaTime;
            break;
        case PlayerState::LEFT:
            mX -= currentSpeed * deltaTime;
            break;
        case PlayerState::FRONT:
            mY += currentSpeed * deltaTime;
            break;
        case PlayerState::RIGHT:
            mX += currentSpeed * deltaTime;
            break;
        }
        isMoving = true;
    }
    else {
        // 대쉬 중이 아닐 때 입력 처리
        if (Input::GetKey(eKeyCode::W)) {
            state = PlayerState::BACK;
            mY -= currentSpeed * deltaTime;
            isMoving = true;
        }
        if (Input::GetKey(eKeyCode::A)) {
            state = PlayerState::LEFT;
            mX -= currentSpeed * deltaTime;
            isMoving = true;
        }
        if (Input::GetKey(eKeyCode::S)) {
            state = PlayerState::FRONT;
            mY += currentSpeed * deltaTime;
            isMoving = true;
        }
        if (Input::GetKey(eKeyCode::D)) {
            state = PlayerState::RIGHT;
            mX += currentSpeed * deltaTime;
            isMoving = true;
        }
    }

    // 이동 애니메이션 (대쉬/공격/피격이 아닐 때)
    if (isMoving && !mIsDashing && !mIsAttack && !mIsHit) {
        animationTimer += deltaTime;
        if (animationTimer >= walkFrameDuration) {
            mCurrentWalkFrame = (mCurrentWalkFrame + 1) % 10; // 10프레임
            animationTimer = 0.0f;
        }
    }
    else if (!mIsDashing && !mIsAttack && !mIsHit) {
        mCurrentWalkFrame = 0;
        animationTimer = 0.0f;
    }

    mIsMoving = isMoving;
}

void Player::LateUpdate()
{
}

void Player::Render(HDC hdc)
{
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    CImage* currentImage = nullptr;
    int imageWidth = 0;
    int imageHeight = 0;

    // 상태에 따라 이미지 선택
    if (mIsDead) {
        currentImage = &mFrontDieAnimation[mCurrentDieFrame]; // Die는 Front만 사용
    }
    else if (mIsHit) {
        switch (state) {
        case PlayerState::FRONT:
            currentImage = &mFrontHitAnimation[mCurrentHitFrame];
            break;
        case PlayerState::BACK:
            currentImage = &mBackHitAnimation[mCurrentHitFrame];
            break;
        case PlayerState::LEFT:
            currentImage = &mLeftHitAnimation[mCurrentHitFrame];
            break;
        case PlayerState::RIGHT:
            currentImage = &mRightHitAnimation[mCurrentHitFrame];
            break;
        }
    }
    else if (mIsAttack) {
        switch (state) {
        case PlayerState::FRONT:
            currentImage = &mFrontAttackAnimation[mCurrentAttackFrame];
            break;
        case PlayerState::BACK:
            currentImage = &mBackAttackAnimation[mCurrentAttackFrame];
            break;
        case PlayerState::LEFT:
            currentImage = &mLeftAttackAnimation[mCurrentAttackFrame];
            break;
        case PlayerState::RIGHT:
            currentImage = &mRightAttackAnimation[mCurrentAttackFrame];
            break;
        }
    }
    else if (mIsMoving) {
        if (mIsDashing) {
            switch (state) {
            case PlayerState::FRONT:
                currentImage = &mFrontDashAnimation[mCurrentDashFrame];
                break;
            case PlayerState::BACK:
                currentImage = &mBackDashAnimation[mCurrentDashFrame];
                break;
            case PlayerState::LEFT:
                currentImage = &mLeftDashAnimation[mCurrentDashFrame];
                break;
            case PlayerState::RIGHT:
                currentImage = &mRightDashAnimation[mCurrentDashFrame];
                break;
            }
        }
        else {
            switch (state) {
            case PlayerState::FRONT:
                currentImage = &mFrontWalkAnimation[mCurrentWalkFrame];
                break;
            case PlayerState::BACK:
                currentImage = &mBackWalkAnimation[mCurrentWalkFrame];
                break;
            case PlayerState::LEFT:
                currentImage = &mLeftWalkAnimation[mCurrentWalkFrame];
                break;
            case PlayerState::RIGHT:
                currentImage = &mRightWalkAnimation[mCurrentWalkFrame];
                break;
            }
        }
    }
    else {
        switch (state) {
        case PlayerState::FRONT:
            currentImage = &mFrontIdleAnimation;
            break;
        case PlayerState::BACK:
            currentImage = &mBackIdleAnimation;
            break;
        case PlayerState::LEFT:
            currentImage = &mLeftIdleAnimation;
            break;
        case PlayerState::RIGHT:
            currentImage = &mRightIdleAnimation;
            break;
        }
    }

    // 이미지 렌더링
    if (currentImage != nullptr) {
        imageWidth = currentImage->GetWidth();
        imageHeight = currentImage->GetHeight();
        int drawX = static_cast<int>(mX - imageWidth / 2.0f);
        int drawY = static_cast<int>(mY - imageHeight / 2.0f);
        currentImage->Draw(hdc, drawX, drawY, imageWidth, imageHeight);
    }
}

void Player::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
    position.x = x;
    position.y = y;
}

void Player::TakeDamage(int d)
{
    if (mIsDead) return;
    hp -= d;
    if (hp <= 0) {
        hp = 0;
        mIsDead = true;
        mIsHit = false;
        mIsAttack = false;
        mIsDashing = false;
        mIsMoving = false;
        mCurrentDieFrame = 0;
    }
    else {
        mIsHit = true;
        mHitTimer = 0.2f; // 피격 애니메이션 0.2초
        mCurrentHitFrame = 0;
        mIsAttack = false;
        mIsDashing = false;
    }
}

float Player::GetPositionX()
{
    return mX;
}

float Player::GetPositionY()
{
    return mY;
}

float Player::GetSpeed()
{
    return speed;
}

RECT Player::GetRect()
{
    return rect;
}