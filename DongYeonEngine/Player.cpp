#include "Player.h"
#include "Input.h"
#include "Time.h"

using namespace Gdiplus;

Player::Player()
{
    mX = 1920 / 2;
    mY = 1080 / 2;
    rect = { (int)(mX - 20), (int)(mY + 200), (int)(mX + 20), (int)(mY + 20) };
    color = RGB(0, 0, 255);
    speed = 200.0f;
    state = FRONT;

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
    bool isMoving = false;
    float deltaTime = Time::DeltaTime();
    static float animationTimer = 0.0f;
    static int walkFrame = 0;
    static int dashFrame = 0;
    const float walkFrameDuration = 0.1f;  // Walk 애니메이션 프레임 지속 시간
    const float dashDuration = 0.4f;       // 대쉬 지속 시간 (0.5초)
    const float dashFrameDuration = dashDuration / 8.0f; // Dash 애니메이션 프레임당 지속 시간
    float currentSpeed = speed;            // 기본 속도

    // 대쉬 중이 아닌 경우에만 대쉬 시작 가능
    if (!mIsDashing && Input::GetKeyDown(eKeyCode::SPACE))
    {
        mIsDashing = true;
        mDashTimer = dashDuration;
        dashFrame = 0; // 대쉬 시작 시 프레임 리셋
        animationTimer = 0.0f; // 타이머 리셋
    }

    // 대쉬 처리
    if (mIsDashing)
    {
        currentSpeed = 400.0f; // 대쉬 속도는 6배
        mDashTimer -= deltaTime;
        if (mDashTimer <= 0.0f)
        {
            mIsDashing = false; // 대쉬 종료
        }
    }

    // 이동 로직
    if (mIsDashing)
    {
        // 대쉬 중에는 현재 state 방향으로만 이동
        switch (state)
        {
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
        default:
            break;
        }
        isMoving = true;
    }
    else
    {
        // 대쉬 중이 아닐 때만 이동 입력 처리
        if (Input::GetKey(eKeyCode::W))
        {
            state = PlayerState::BACK;
            mY -= currentSpeed * deltaTime;
            isMoving = true;
        }
        if (Input::GetKey(eKeyCode::A))
        {
            state = PlayerState::LEFT;
            mX -= currentSpeed * deltaTime;
            isMoving = true;
        }
        if (Input::GetKey(eKeyCode::S))
        {
            state = PlayerState::FRONT;
            mY += currentSpeed * deltaTime;
            isMoving = true;
        }
        if (Input::GetKey(eKeyCode::D))
        {
            state = PlayerState::RIGHT;
            mX += currentSpeed * deltaTime;
            isMoving = true;
        }
    }

    // rect 업데이트
    rect = { (int)(mX + 28), (int)(mY)+11, (int)(mX + 63), (int)(mY + 80) };

    // 애니메이션 프레임 업데이트
    if (isMoving)
    {
        animationTimer += deltaTime;
        if (mIsDashing)
        {
            if (animationTimer >= dashFrameDuration)
            {
                dashFrame = (dashFrame + 1) % 8; // Dash 애니메이션은 8프레임
                animationTimer -= dashFrameDuration;
            }
        }
        else
        {
            if (animationTimer >= walkFrameDuration)
            {
                walkFrame = (walkFrame + 1) % 10; // Walk 애니메이션은 10프레임
                animationTimer -= walkFrameDuration;
            }
        }
    }
    else
    {
        walkFrame = 0; // 이동하지 않을 때는 첫 프레임으로 리셋
        dashFrame = 0;
        animationTimer = 0.0f;
    }

    // 현재 프레임 및 상태 저장
    mCurrentWalkFrame = walkFrame;
    mCurrentDashFrame = dashFrame;
    mIsMoving = isMoving;
}

void Player::LateUpdate()
{
}

void Player::Render(HDC hdc)
{
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    if (mIsMoving)
    {
        if (mIsDashing)
        {
            switch (state)
            {
            case PlayerState::FRONT:
                mFrontDashAnimation[mCurrentDashFrame].Draw(hdc, mX, mY, mFrontDashAnimation[mCurrentDashFrame].GetWidth(), mFrontDashAnimation[mCurrentDashFrame].GetHeight());
                break;
            case PlayerState::BACK:
                mBackDashAnimation[mCurrentDashFrame].Draw(hdc, mX, mY, mBackDashAnimation[mCurrentDashFrame].GetWidth(), mBackDashAnimation[mCurrentDashFrame].GetHeight());
                break;
            case PlayerState::LEFT:
                mLeftDashAnimation[mCurrentDashFrame].Draw(hdc, mX, mY, mLeftDashAnimation[mCurrentDashFrame].GetWidth(), mLeftDashAnimation[mCurrentDashFrame].GetHeight());
                break;
            case PlayerState::RIGHT:
                mRightDashAnimation[mCurrentDashFrame].Draw(hdc, mX, mY, mRightDashAnimation[mCurrentDashFrame].GetWidth(), mRightDashAnimation[mCurrentDashFrame].GetHeight());
                break;
            default:
                break;
            }
        }
        else
        {
            switch (state)
            {
            case PlayerState::FRONT:
                mFrontWalkAnimation[mCurrentWalkFrame].Draw(hdc, mX, mY, mFrontWalkAnimation[mCurrentWalkFrame].GetWidth(), mFrontWalkAnimation[mCurrentWalkFrame].GetHeight());
                break;
            case PlayerState::BACK:
                mBackWalkAnimation[mCurrentWalkFrame].Draw(hdc, mX, mY, mBackWalkAnimation[mCurrentWalkFrame].GetWidth(), mBackWalkAnimation[mCurrentWalkFrame].GetHeight());
                break;
            case PlayerState::LEFT:
                mLeftWalkAnimation[mCurrentWalkFrame].Draw(hdc, mX, mY, mLeftWalkAnimation[mCurrentWalkFrame].GetWidth(), mLeftWalkAnimation[mCurrentWalkFrame].GetHeight());
                break;
            case PlayerState::RIGHT:
                mRightWalkAnimation[mCurrentWalkFrame].Draw(hdc, mX, mY, mRightWalkAnimation[mCurrentWalkFrame].GetWidth(), mRightWalkAnimation[mCurrentWalkFrame].GetHeight());
                break;
            default:
                break;
            }
        }
    }
    else
    {
        switch (state)
        {
        case PlayerState::FRONT:
            mFrontIdleAnimation.Draw(hdc, mX, mY, mFrontIdleAnimation.GetWidth(), mFrontIdleAnimation.GetHeight());
            break;
        case PlayerState::BACK:
            mBackIdleAnimation.Draw(hdc, mX, mY, mBackIdleAnimation.GetWidth(), mBackIdleAnimation.GetHeight());
            break;
        case PlayerState::LEFT:
            mLeftIdleAnimation.Draw(hdc, mX, mY, mLeftIdleAnimation.GetWidth(), mLeftIdleAnimation.GetHeight());
            break;
        case PlayerState::RIGHT:
            mRightIdleAnimation.Draw(hdc, mX, mY, mRightIdleAnimation.GetWidth(), mRightIdleAnimation.GetHeight());
            break;
        default:
            break;
        }
    }
}

void Player::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
    position.x = x;
    position.y = y;
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