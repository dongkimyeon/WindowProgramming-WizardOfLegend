    #include "BossSkill_AquaBall.h"
    #include "Stage1.h"
    #include "Time.h"
    #include "SoundManager.h"
    #include <cmath>

BossSkill_AquaBall::BossSkill_AquaBall(float x, float y, float targetX, float targetY)
    : mX(x), mY(y), mTargetX(targetX), mTargetY(targetY + 50.0f), mInitialX(x), mInitialY(y),
    speed(700.0f), mIsActive(true), damage(10), mTime(0.0f), mHeight(200.0f),
    mTargetIndicatorFrame(0), mAquaBallFrame(0), mExplosionFrame(0),
    mAnimationTimer(0.0f), mIsTargetIndicating(true), mIsFalling(true), mIsExploding(false), mHasEffectHitbox(false)
{

    SoundManager::GetInstance()->mPlaySound("BossAquaBallStart", false);

    // 아쿠아볼 애니메이션 로드
    for (int i = 0; i < 27; ++i) {
        wchar_t path[256];
        swprintf_s(path, L"resources/Boss/AquaBall/%d.png", i);
        AquaBallAnimation[i].Load(path);
    }
    // 떨어질 지점에 표시해주는 애니메이션
    for (int i = 0; i < 25; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Boss/CastingImage/CastingCircle_%d.png", i);
        AquaBallFallingPointAnimation[i].Load(path);
    }
    // 폭발 애니메이션 로드
    for (int i = 0; i < 7; ++i) {
        wchar_t path[256];
        swprintf_s(path, L"resources/Boss/AquaBallExplosinEffect/WaterBombExplosion_%d.png", i);
        AquaBallExplosionEffectAnimation[i].Load(path);
    }
    UpdateHitbox();
}

BossSkill_AquaBall::~BossSkill_AquaBall()
{
    for (int i = 0; i < 25; i++)
    {
        AquaBallFallingPointAnimation[i].Destroy();
    }

    for (int i = 0; i < 27; ++i)
    {
        AquaBallAnimation[i].Destroy();
    }

    for (int i = 0; i < 7; ++i)
    {
        AquaBallExplosionEffectAnimation[i].Destroy();
    }
    
}

void BossSkill_AquaBall::Update(Player& player)
{
    if (!mIsActive) return;

    mTime += Time::DeltaTime();
    float flightDuration = 1.0f; // 포물선 이동 시간

    // 타겟 인디케이터 애니메이션 (떨어질 지점에서 항상 표시)
    if (mIsTargetIndicating) {
        mAnimationTimer += Time::DeltaTime();
        if (mAnimationTimer >= 0.05f) {
            mTargetIndicatorFrame = (mTargetIndicatorFrame + 1) % 25;
            mAnimationTimer = 0.0f;
        }
        if (mTime >= flightDuration && mTargetIndicatorFrame == 24)
        {
            mIsTargetIndicating = false; // 타겟 인디케이터 끄기
        }
    }

    if (mIsFalling) {
        // 포물선 이동
        float t = mTime / flightDuration;
        if (t >= 1.0f) {
            mX = mTargetX;
            mY = mTargetY;
            mIsFalling = false;
            mIsExploding = true;
            mTime = 0.0f;
            mExplosionFrame = 0;
        }
        else {
            // 선형 보간으로 X, Y 이동
            mX = mInitialX + (mTargetX - mInitialX) * t;
            mY = mInitialY + (mTargetY - mInitialY) * t - mHeight * sin(3.1415926535f * t);
            // 아쿠아볼 애니메이션 프레임 업데이트
            mAnimationTimer += Time::DeltaTime();
            if (mAnimationTimer >= 0.05f)
            {
                mAquaBallFrame = (mAquaBallFrame + 1) % 27;

                mAnimationTimer = 0.0f;
            }
        }

        UpdateHitbox();

        // 플레이어와 충돌 체크
        if(mAquaBallFrame >=15)
        { 
            if (CheckCollisionWithRect(player.GetRect())) {
                player.TakeDamage(damage);
                mIsActive = false;
                return;
            }
        }
       
    }

    else if (mIsExploding) {
        // 폭발 애니메이션
        mAnimationTimer += Time::DeltaTime();
        if (mAnimationTimer >= 0.05f) {
            mExplosionFrame++;
            mAnimationTimer = 0.0f;

            if(mExplosionFrame == 2) SoundManager::GetInstance()->mPlaySound("BossBubbleRelease", false);

            if (mExplosionFrame >= 7) {

                mIsActive = false;
                return;
            }
        }
    }
}

void BossSkill_AquaBall::Render(HDC hdc, Player& player)
{
    if (!mIsActive) return;

    // 타겟 인디케이터 애니메이션 (떨어질 지점에 표시)
    if (mIsTargetIndicating)
    {
        float scale = 1.5f;
        int indicatorWidth = AquaBallFallingPointAnimation[mTargetIndicatorFrame].GetWidth() * scale;
        int indicatorHeight = AquaBallFallingPointAnimation[mTargetIndicatorFrame].GetHeight() * scale;
        AquaBallFallingPointAnimation[mTargetIndicatorFrame].Draw(hdc,
            static_cast<int>(mTargetX - indicatorWidth / 2.0f),
            static_cast<int>(mTargetY - indicatorHeight / 2.0f),
            indicatorWidth, indicatorHeight);
    }

    // 아쿠아볼 애니메이션 (날아가는 중에만 표시)
    if (mIsFalling) {
        int imageWidth = AquaBallAnimation[mAquaBallFrame].GetWidth();
        int imageHeight = AquaBallAnimation[mAquaBallFrame].GetHeight();
        float scale = 3.0f;
        int renderWidth = static_cast<int>(imageWidth * scale);
        int renderHeight = static_cast<int>(imageHeight * scale);
        int drawX = static_cast<int>(mX - renderWidth / 2.0f);
        int drawY = static_cast<int>(mY - renderHeight / 2.0f);

        AquaBallAnimation[mAquaBallFrame].Draw(hdc, drawX, drawY, renderWidth, renderHeight);
    }

    // 폭발 애니메이션
    if (mIsExploding) {
        int imageWidth = AquaBallExplosionEffectAnimation[mExplosionFrame].GetWidth();
        int imageHeight = AquaBallExplosionEffectAnimation[mExplosionFrame].GetHeight();
        float scale = 1.3f;
        int renderWidth = static_cast<int>(imageWidth * scale);
        int renderHeight = static_cast<int>(imageHeight * scale);
        int drawX = static_cast<int>(mTargetX - renderWidth / 2.0f);
        int drawY = static_cast<int>(mTargetY - renderHeight / 2.0f);

        AquaBallExplosionEffectAnimation[mExplosionFrame].Draw(hdc, drawX, drawY, renderWidth, renderHeight);
    }

    // 히트박스 그리기 (디버깅용)
    HPEN hitboxPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, hitboxPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
    Polygon(hdc, hitboxPoints, 4);
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hitboxPen);
}

void BossSkill_AquaBall::ThrowAquaBall(Player& player, float mX, float mY, Scene* stage)
{
    float playerX = player.GetPositionX();
    float playerY = player.GetPositionY();

    // 플레이어 주변에 삼각형 형태로 3개의 아쿠아볼 생성
    float offset = 125.0f; // 플레이어 중심으로부터의 거리
    stage->AddBossSkillAquaBall(new BossSkill_AquaBall(mX, mY, playerX, playerY)); // 중심
    stage->AddBossSkillAquaBall(new BossSkill_AquaBall(mX, mY, playerX + offset, playerY + offset)); // 오른쪽 아래
    stage->AddBossSkillAquaBall(new BossSkill_AquaBall(mX, mY, playerX - offset, playerY + offset)); // 왼쪽 아래
}

void BossSkill_AquaBall::UpdateHitbox()
{
    float scale = 2.0f;
    int imageWidth = AquaBallAnimation[mAquaBallFrame].GetWidth() * scale;
    int imageHeight = AquaBallAnimation[mAquaBallFrame].GetHeight() * scale;

    POINT basePoints[4] = {
        { -imageWidth / 2, -imageHeight / 2 },
        { imageWidth / 2, -imageHeight / 2 },
        { imageWidth / 2, imageHeight / 2 },
        { -imageWidth / 2, imageHeight / 2 }
    };

    for (int i = 0; i < 4; ++i) {
        hitboxPoints[i].x = static_cast<LONG>(mX + basePoints[i].x);
        hitboxPoints[i].y = static_cast<LONG>(mY + basePoints[i].y);
    }
}

bool BossSkill_AquaBall::CheckCollisionWithRect(const RECT& rect)
{
    POINT rectPoints[4] = {
        { rect.left, rect.top },
        { rect.right, rect.top },
        { rect.right, rect.bottom },
        { rect.left, rect.bottom }
    };

    auto PointInPolygon = [](const POINT& point, const POINT polygon[4]) {
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
        };

    for (const auto& p : rectPoints) {
        if (PointInPolygon(p, hitboxPoints)) return true;
    }
    for (const auto& p : hitboxPoints) {
        if (p.x >= rect.left && p.x <= rect.right && p.y >= rect.top && p.y <= rect.bottom) return true;
    }
    return false;
}