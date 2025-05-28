#include "Player.h"
#include "Input.h"
#include "Time.h"
#include "SoundManager.h"


Player::Player()
{
    mX = 1920 / 2;
    mY = 1080 / 2;
    rect = { (int)(mX - 20), (int)(mY + 200), (int)(mX + 20), (int)(mY + 20) };
    speed = 200.0f;
    state = PlayerState::FRONT;
    hp = 100;
	mp = 100;
    damage = 20;
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
    mMouseClickFlag = false;
    fireDragonTriggered = false;
    mHasEffectHitbox = false;
    fireBallCooldown = 0.0f;
    fireDragonCooldown = 0.0f;
    fireBallReady = true;
    fireDragonReady = true;
    isUsingSkill = false;

    mIsTeleporting = true;          // 생성 시 텔레포트 애니메이션 활성화
    mTeleportTimer = 0.0f;          // 타이머 초기화
    mCurrentTeleportFrame = 0;      // 프레임 초기화
    mHitEffectAngle = 0.0f; // 초기화
    for (int i = 0; i < 4; ++i) mEffectHitboxPoints[i] = { 0, 0 };

    // 애니메이션 로드 (기존 코드 유지)
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

    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Attack/LSlashEffect/LSlash%02d.png", i);
        mLeftAttackEffectAnimation[i].Load(path);
        if (mLeftAttackEffectAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Attack/RSlashEffect/RSlash%02d.png", i);
        mRightAttackEffectAnimation[i].Load(path);
        if (mRightAttackEffectAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    for (int i = 0; i < 8; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Summon/SUMMON_%d.png", i);
        mTeleportEffectAnimation[i].Load(path); // 텔레포트 애니메이션 로드
        if (mTeleportEffectAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
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

void Player::Update(Scene* stage)
{

    // 텔레포트 애니메이션 처리
    if (mIsTeleporting)
    {
        const float teleportFrameDuration = 0.1f; // 프레임당 0.1초
        mTeleportTimer += Time::DeltaTime();
        if (mTeleportTimer >= teleportFrameDuration)
        {
            mCurrentTeleportFrame++;
            mTeleportTimer = 0.0f;
			mIsMoving = false; // 텔레포트 중에는 이동하지 않음
			mIsDashing = false; // 텔레포트 중에는 대쉬하지 않음
			mIsAttack = false; // 텔레포트 중에는 공격하지 않음
			mIsHit = false; // 텔레포트 중에는 피격하지 않음
			mMouseClickFlag = false; // 텔레포트 중에는 마우스 클릭 플래그 초기화

            if (mCurrentTeleportFrame >= 8) // 8프레임 후 종료
            {
                mIsTeleporting = false; // 애니메이션 종료
                mCurrentTeleportFrame = 0;
            }
        }
    }

    if (mIsDead) {
        static float dieTimer = 0.0f;
        if (mCurrentDieFrame < 6) {
            dieTimer += Time::DeltaTime();
            if (dieTimer >= 0.1f) {
                mCurrentDieFrame++;
                dieTimer = 0.0f;
            }
        }
        mIsMoving = false;
        mIsDashing = false;
        mIsAttack = false;
        mIsHit = false;
        mMouseClickFlag = false;
        rect = { 0, 0, 0, 0 };
        return;
    }

    bool isMoving = false;
    float deltaTime = Time::DeltaTime();
    static float animationTimer = 0.0f;
    const float walkFrameDuration = 0.1f;
    const float dashDistance = 150.0f;
    const float dashSpeed = 600.0f;
    const float attackDuration = 0.6f;
    const float attackFrameDuration = attackDuration / 16.0f;
    const float hitDuration = 0.2f;
    const float hitFrameDuration = hitDuration / 2.0f;
    float currentSpeed = speed;
    static int clickCounter = 0;
    static int attackEndFrame = mMouseClickFlag ? 16 : 8;
    const float skillCooldown = 3.0f; // 스킬 쿨타임 5초

    // 쿨타임 업데이트
    if (!fireBallReady) {
        fireBallCooldown -= deltaTime;
        if (fireBallCooldown <= 0.0f) {
            fireBallReady = true;
        }
    }
    if (!fireDragonReady) {
        fireDragonCooldown -= deltaTime;
        if (fireDragonCooldown <= 0.0f) {
            fireDragonReady = true;
        }
    }

    // rect 업데이트
    int imageWidth = mFrontIdleAnimation.GetWidth();
    int imageHeight = mFrontIdleAnimation.GetHeight();
    rect.left = static_cast<int>(mX - imageWidth / 2.0f) + 32;
    rect.top = static_cast<int>(mY - imageHeight / 2.0f) + 20;
    rect.right = static_cast<int>(mX + imageWidth / 2.0f) - 22;
    rect.bottom = static_cast<int>(mY + imageHeight / 2.0f);

    // 피격 상태 처리
    if (mIsHit) {
        mHitTimer -= deltaTime;
        if (mHitTimer <= 0.0f) mIsHit = false;
        animationTimer += deltaTime;
        if (animationTimer >= hitFrameDuration) {
            mCurrentHitFrame = (mCurrentHitFrame + 1) % 2;
            animationTimer = 0.0f;
        }
       
        return;
    }

    // 공격 처리
    if (!mIsAttack && Input::GetKeyDown(eKeyCode::LButton)) {
        mIsAttack = true;
        mAttackTimer = attackDuration;
        attackEndFrame = mMouseClickFlag ? 16 : 8;
        mCurrentAttackFrame = mMouseClickFlag ? 8 : 0;
        mMouseClickFlag = !mMouseClickFlag;
        isUsingSkill = false; // 기본 공격
        SoundManager::GetInstance()->mPlaySound("PlayerAttack", false);


        Vector2 mousePos = Input::GetMousePosition();
        float worldMouseX = mousePos.x + mCameraX;
        float worldMouseY = mousePos.y + mCameraY;
        float dx = worldMouseX - mX;
        float dy = worldMouseY - mY;
        attackAngle = atan2f(dy, dx);

        float angle = attackAngle * 180.0f / 3.14159f;
        if (angle >= -45.0f && angle < 45.0f) state = PlayerState::RIGHT;
        else if (angle >= 45.0f && angle < 135.0f) state = PlayerState::FRONT;
        else if (angle >= 135.0f || angle < -135.0f) state = PlayerState::LEFT;
        else state = PlayerState::BACK;



        animationTimer = 0.0f;
    }

    // FireBall 스킬 (Q)
    if (!mIsAttack && Input::GetKeyDown(eKeyCode::Q) && fireBallReady) {
        mIsAttack = true;
        mAttackTimer = attackDuration;
        attackEndFrame = mMouseClickFlag ? 16 : 8;
        mCurrentAttackFrame = mMouseClickFlag ? 8 : 0;
        mMouseClickFlag = !mMouseClickFlag;
        isUsingSkill = true; // 스킬 사용
        // 사운드 출력
        SoundManager::GetInstance()->mPlaySound("Fireball", false);


        Vector2 mousePos = Input::GetMousePosition();
        float worldMouseX = mousePos.x + mCameraX;
        float worldMouseY = mousePos.y + mCameraY;
        float dx = worldMouseX - mX;
        float dy = worldMouseY - mY;
        attackAngle = atan2f(dy, dx);

        float angle = attackAngle * 180.0f / 3.14159f;
        if (angle >= -45.0f && angle < 45.0f) state = PlayerState::RIGHT;
        else if (angle >= 45.0f && angle < 135.0f) state = PlayerState::FRONT;
        else if (angle >= 135.0f || angle < -135.0f) state = PlayerState::LEFT;
        else state = PlayerState::BACK;

        Player_Skill_FireBall::Active(mX, mY, attackAngle, stage);
        fireBallReady = false;
        fireBallCooldown = skillCooldown;
        animationTimer = 0.0f;
    }

    // FireDragon 스킬 (E)
    if (!mIsAttack && Input::GetKeyDown(eKeyCode::E) && !fireDragonTriggered && fireDragonReady) {
        mIsAttack = true;
        mAttackTimer = attackDuration;
        attackEndFrame = mMouseClickFlag ? 16 : 8;
        mCurrentAttackFrame = mMouseClickFlag ? 8 : 0;
        mMouseClickFlag = !mMouseClickFlag;
        isUsingSkill = true; // 스킬 사용

        Vector2 mousePos = Input::GetMousePosition();
        float worldMouseX = mousePos.x + mCameraX;
        float worldMouseY = mousePos.y + mCameraY;
        float dx = worldMouseX - mX;
        float dy = worldMouseY - mY;
        attackAngle = atan2f(dy, dx);

        float angle = attackAngle * 180.0f / 3.14159f;
        if (angle >= -45.0f && angle < 45.0f) state = PlayerState::RIGHT;
        else if (angle >= 45.0f && angle < 135.0f) state = PlayerState::FRONT;
        else if (angle >= 135.0f || angle < -135.0f) state = PlayerState::LEFT;
        else state = PlayerState::BACK;

        Player_Skill_FireDragon::Active(mX, mY, attackAngle, stage, this, true);
        fireDragonTriggered = true;
        fireDragonReady = false;
        fireDragonCooldown = skillCooldown;
        animationTimer = 0.0f;
    }

    // FireDragon 연속 발사
    if (fireDragonTriggered) {
        Player_Skill_FireDragon::Active(mX, mY, attackAngle, stage, this, false);
    }

    if (mIsAttack) {
        mAttackTimer -= deltaTime;
        animationTimer += deltaTime;

        if (animationTimer >= attackFrameDuration) {
            mCurrentAttackFrame++;
            animationTimer -= attackFrameDuration;
        }

        if (mCurrentAttackFrame >= attackEndFrame || mAttackTimer <= 0.0f) {
            mIsAttack = false;
            mCurrentAttackFrame = 0;
            mHasEffectHitbox = false;
            isUsingSkill = false; // 공격 종료 시 플래그 초기화
            for (int i = 0; i < 4; ++i) mEffectHitboxPoints[i] = { 0, 0 };
            animationTimer = 0.0f;
        }

        if (!isUsingSkill && ((mCurrentAttackFrame >= 4 && mCurrentAttackFrame < 8) || (mCurrentAttackFrame >= 12 && mCurrentAttackFrame < 16))) {
            int hitboxWidth = 65;
            int hitboxHeight = 70;
            float effectOffset = 27.0f;
            float centerX = mX + cos(attackAngle) * effectOffset;
            float centerY = mY + sin(attackAngle) * effectOffset;

            POINT basePoints[4] = {
                { -hitboxWidth / 2, -hitboxHeight / 2 },
                { hitboxWidth / 2, -hitboxHeight / 2 },
                { hitboxWidth / 2, hitboxHeight / 2 },
                { -hitboxWidth / 2, hitboxHeight / 2 }
            };

            for (int i = 0; i < 4; ++i) {
                float x = (float)basePoints[i].x;
                float y = (float)basePoints[i].y;
                mEffectHitboxPoints[i].x = static_cast<LONG>(centerX + (x * cos(attackAngle) - y * sin(attackAngle)));
                mEffectHitboxPoints[i].y = static_cast<LONG>(centerY + (x * sin(attackAngle) + y * cos(attackAngle)));
            }
            mHasEffectHitbox = true;
        }
        else {
            mHasEffectHitbox = false;
            for (int i = 0; i < 4; ++i) mEffectHitboxPoints[i] = { 0, 0 };
        }
        return;
    }

    // 이동 방향 벡터 계산
    Vector2 moveDirection(0.0f, 0.0f);
    if (Input::GetKey(eKeyCode::W)) moveDirection.y -= 1.0f;
    if (Input::GetKey(eKeyCode::A)) moveDirection.x -= 1.0f;
    if (Input::GetKey(eKeyCode::S)) moveDirection.y += 1.0f;
    if (Input::GetKey(eKeyCode::D)) moveDirection.x += 1.0f;

    // 대쉬 처리
    static Vector2 dashDirection(0.0f, 0.0f);
    static float dashProgress = 0.0f;
    if (!mIsDashing && Input::GetKeyDown(eKeyCode::SPACE) && moveDirection.Length() > 0.0f) {
        // 사운드 출력
        SoundManager::GetInstance()->mPlaySound("Dash", false);

        mIsDashing = true;
        dashDirection = moveDirection.Normalize();
        dashProgress = 0.0f;
        mCurrentDashFrame = 0;
        animationTimer = 0.0f;

        if (abs(dashDirection.x) > abs(dashDirection.y)) {
            state = (dashDirection.x > 0) ? PlayerState::RIGHT : PlayerState::LEFT;
        }
        else {
            state = (dashDirection.y > 0) ? PlayerState::FRONT : PlayerState::BACK;
        }
    }

    if (mIsDashing) {
        float moveDistance = dashSpeed * deltaTime;
        dashProgress += moveDistance;
        if (dashProgress >= dashDistance) {
            moveDistance -= (dashProgress - dashDistance);
            mIsDashing = false;
            dashProgress = 0.0f;
        }
        Vector2 move = dashDirection * moveDistance;
        mX += move.x;
        mY += move.y;
        isMoving = true;

        if (dashProgress < dashDistance * 0.8f) {
            mCurrentDashFrame = static_cast<int>((dashProgress / (dashDistance * 0.8f)) * 4) % 4;
        }
        else {
            mCurrentDashFrame = 4 + static_cast<int>(((dashProgress - dashDistance * 0.8f) / (dashDistance * 0.2f)) * 4) % 4;
        }
    }
    else if (moveDirection.Length() > 0.0f) {
        moveDirection = moveDirection.Normalize();
        Vector2 move = moveDirection * currentSpeed * deltaTime;
        mX += move.x;
        mY += move.y;
        isMoving = true;

        if (abs(moveDirection.x) > abs(moveDirection.y)) {
            state = (moveDirection.x > 0) ? PlayerState::RIGHT : PlayerState::LEFT;
        }
        else {
            state = (moveDirection.y > 0) ? PlayerState::FRONT : PlayerState::BACK;
        }
    }

    if (isMoving && !mIsDashing && !mIsAttack && !mIsHit) {
        animationTimer += deltaTime;
        if (animationTimer >= walkFrameDuration) {
            mCurrentWalkFrame = (mCurrentWalkFrame + 1) % 10;
            animationTimer = 0.0f;
            if(mCurrentWalkFrame % 5 == 0)
            SoundManager::GetInstance()->mPlaySound("PlayerFootstep", false);
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

    /*Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    if (mHasEffectHitbox) {
        HPEN hitboxPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
        HPEN oldPen = (HPEN)SelectObject(hdc, hitboxPen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
        Polygon(hdc, mEffectHitboxPoints, 4);
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(hitboxPen);
    }*/

    CImage* currentImage = nullptr;
    int imageWidth = 0;
    int imageHeight = 0;

    if (mIsDead) {
        currentImage = &mFrontDieAnimation[mCurrentDieFrame];
    }
    else if (mIsHit) {
        switch (state) {
        case PlayerState::FRONT: currentImage = &mFrontHitAnimation[mCurrentHitFrame]; break;
        case PlayerState::BACK: currentImage = &mBackHitAnimation[mCurrentHitFrame]; break;
        case PlayerState::LEFT: currentImage = &mLeftHitAnimation[mCurrentHitFrame]; break;
        case PlayerState::RIGHT: currentImage = &mRightHitAnimation[mCurrentHitFrame]; break;
        }
    }
    else if (mIsAttack) {
        switch (state) {
        case PlayerState::FRONT: currentImage = &mFrontAttackAnimation[mCurrentAttackFrame]; break;
        case PlayerState::BACK: currentImage = &mBackAttackAnimation[mCurrentAttackFrame]; break;
        case PlayerState::LEFT: currentImage = &mLeftAttackAnimation[mCurrentAttackFrame]; break;
        case PlayerState::RIGHT: currentImage = &mRightAttackAnimation[mCurrentAttackFrame]; break;
        }
    }
    else if (mIsMoving) {
        if (mIsDashing) {
            switch (state) {
            case PlayerState::FRONT: currentImage = &mFrontDashAnimation[mCurrentDashFrame]; break;
            case PlayerState::BACK: currentImage = &mBackDashAnimation[mCurrentDashFrame]; break;
            case PlayerState::LEFT: currentImage = &mLeftDashAnimation[mCurrentDashFrame]; break;
            case PlayerState::RIGHT: currentImage = &mRightDashAnimation[mCurrentDashFrame]; break;
            }
        }
        else {
            switch (state) {
            case PlayerState::FRONT: currentImage = &mFrontWalkAnimation[mCurrentWalkFrame]; break;
            case PlayerState::BACK: currentImage = &mBackWalkAnimation[mCurrentWalkFrame]; break;
            case PlayerState::LEFT: currentImage = &mLeftWalkAnimation[mCurrentWalkFrame]; break;
            case PlayerState::RIGHT: currentImage = &mRightWalkAnimation[mCurrentWalkFrame]; break;
            }
        }
    }
    else {
        switch (state) {
        case PlayerState::FRONT: currentImage = &mFrontIdleAnimation; break;
        case PlayerState::BACK: currentImage = &mBackIdleAnimation; break;
        case PlayerState::LEFT: currentImage = &mLeftIdleAnimation; break;
        case PlayerState::RIGHT: currentImage = &mRightIdleAnimation; break;
        }
    }

    if (currentImage != nullptr) {
        imageWidth = currentImage->GetWidth();
        imageHeight = currentImage->GetHeight();
        int drawX = static_cast<int>(mX - imageWidth / 2.0f);
        int drawY = static_cast<int>(mY - imageHeight / 2.0f);
        currentImage->Draw(hdc, drawX, drawY, imageWidth, imageHeight);
    }
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
    // 공격 이펙트 렌더링 (스킬 사용 시 제외)
    if (mIsAttack && !isUsingSkill && mCurrentAttackFrame >= 4) {
        CImage* effectImage = nullptr;
        int effectFrame = 0;
        if (mCurrentAttackFrame < 8) {
            effectFrame = (mCurrentAttackFrame - 4) * 6 / 4;
            effectImage = &mLeftAttackEffectAnimation[effectFrame % 6];
        }
        else {
            effectFrame = (mCurrentAttackFrame - 8) * 6 / 8;
            effectImage = &mRightAttackEffectAnimation[effectFrame % 6];
        }

        if (effectImage != nullptr) {
            int effectWidth = effectImage->GetWidth() + 50;
            int effectHeight = effectImage->GetHeight() + 30;
            float effectOffset = 20.0f;

            // 회전 각도 계산
            float angle = attackAngle;

            // 변환 행렬 설정
            XFORM xForm = { 0 };
            xForm.eM11 = cos(angle); // X 스케일 및 회전
            xForm.eM12 = sin(angle); // X에 대한 Y의 기울기
            xForm.eM21 = -sin(angle); // Y에 대한 X의 기울기
            xForm.eM22 = cos(angle); // Y 스케일 및 회전
            xForm.eDx = mX + cos(attackAngle) * effectOffset; // 중심점 X로 이동
            xForm.eDy = mY + sin(attackAngle) * effectOffset; // 중심점 Y로 이동

            // 그래픽 모드 및 변환 적용
            SetGraphicsMode(hdc, GM_ADVANCED);
            SetWorldTransform(hdc, &xForm);

            // 이펙트 렌더링
            HDC srcDC = effectImage->GetDC();
            TransparentBlt(
                hdc,
                -effectWidth / 2, -effectHeight / 2, effectWidth, effectHeight,
                srcDC,
                0, 0, effectImage->GetWidth(), effectImage->GetHeight(),
                RGB(0, 0, 0) // 투명색
            );
            effectImage->ReleaseDC();

            // 변환 행렬 초기화
            XFORM identity = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
            SetWorldTransform(hdc, &identity);
            SetGraphicsMode(hdc, GM_COMPATIBLE);
        }
    }

    // 텔레포트 이펙트 렌더링
    if (mIsTeleporting)
    {
        CImage& effectImage = mTeleportEffectAnimation[mCurrentTeleportFrame];
        if (!effectImage.IsNull())
        {
            int effectWidth = effectImage.GetWidth() / 3.0f;
            int effectHeight = effectImage.GetHeight() / 3.0f;
            int drawX = static_cast<int>(mX - effectWidth / 2.0f) + 3;
            int drawY = static_cast<int>(mY - effectHeight / 2.0f);

            HDC srcDC = effectImage.GetDC();
            TransparentBlt(
                hdc,
                drawX, drawY, effectWidth, effectHeight,
                srcDC,
                0, 0, effectImage.GetWidth(), effectImage.GetHeight(),
                RGB(0, 0, 0) // 투명색
            );
            effectImage.ReleaseDC();
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

void Player::TakeDamage(int d)
{
    if (mIsDead) return;
    hp -= d;
    mHitEffectAngle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
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
        mHitTimer = 0.2f;
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

bool Player::GetEffectHitbox(POINT outPoints[4])
{
    for (int i = 0; i < 4; ++i) outPoints[i] = mEffectHitboxPoints[i];
    return mHasEffectHitbox;
}

bool Player::CheckPointInPolygon(POINT& point, POINT polygon[4])
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

bool Player::CheckCollisionWithRect(RECT& otherRect)
{
    if (!mHasEffectHitbox) return false;

    POINT rectPoints[4] = {
        { otherRect.left, otherRect.top },
        { otherRect.right, otherRect.top },
        { otherRect.right, otherRect.bottom },
        { otherRect.left, otherRect.bottom }
    };

    for (auto& p : rectPoints) {
        if (CheckPointInPolygon(p, mEffectHitboxPoints)) {
            return true;
        }
    }

    for (auto& p : mEffectHitboxPoints) {
        if (p.x >= otherRect.left && p.x <= otherRect.right &&
            p.y >= otherRect.top && p.y <= otherRect.bottom) {
            return true;
        }
    }

    return false;
}