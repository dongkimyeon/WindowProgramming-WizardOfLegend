#include "Player_Skill_FireBall.h"
#include "Stage1.h"
#include "Time.h"
#include <cmath>

Player_Skill_FireBall::Player_Skill_FireBall(float x, float y, float dirX, float dirY)
    : mX(x), mY(y), mDirectionX(dirX), mDirectionY(dirY), speed(300.0f), mIsActive(true), damage(25),
    mCurrentFrame(0), mAnimationTimer(0.0f)
{
    for (int i = 0; i < 5; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Skill_FireBall/Fireball_%02d.png", i);
        mFireBallAnimation[i].Load(path);
    }
    UpdateHitbox();
}

Player_Skill_FireBall::~Player_Skill_FireBall()
{
    for (int i = 0; i < 5; ++i)
    {
        mFireBallAnimation[i].Destroy();
    }
}

void Player_Skill_FireBall::Update(GameObject& obj)
{
    if (!mIsActive) return;

    // 위치 업데이트
    mX += mDirectionX * speed * Time::DeltaTime();
    mY += mDirectionY * speed * Time::DeltaTime();
    UpdateHitbox();

    // 애니메이션 업데이트
    mAnimationTimer += Time::DeltaTime();
    if (mAnimationTimer >= mFrameDuration)
    {
        mCurrentFrame = (mCurrentFrame + 1) % 5;
        mAnimationTimer -= mFrameDuration;
    }

    // 맵 경계 체크 (5000x5000 맵 가정)
    const float mapWidth = 5000.0f;
    const float mapHeight = 5000.0f;
    if (mX < 0 || mX > mapWidth || mY < 0 || mY > mapHeight)
    {
        mIsActive = false;
        return;
    }

    // 충돌 체크
    if (CheckCollisionWithRect(obj.GetRect()))
    {
        obj.TakeDamage(damage);
        mIsActive = false;
        return;
    }
}

void Player_Skill_FireBall::Render(HDC hdc)
{
    if (!mIsActive) return;

    // 히트박스 그리기
    HPEN hitboxPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, hitboxPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
    Polygon(hdc, hitboxPoints, 4);
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hitboxPen);

    // 파이어볼 이미지 렌더링
    int imageWidth = mFireBallAnimation[mCurrentFrame].GetWidth();
    int imageHeight = mFireBallAnimation[mCurrentFrame].GetHeight();
    float scale = 2.0f;
    int renderWidth = static_cast<int>(imageWidth * scale);
    int renderHeight = static_cast<int>(imageHeight * scale);
    int drawX = static_cast<int>(mX - renderWidth / 2.0f);
    int drawY = static_cast<int>(mY - renderHeight / 2.0f);

    Gdiplus::Graphics graphics(hdc);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    Gdiplus::ImageAttributes imageAttr;
    imageAttr.SetColorKey(Gdiplus::Color(0, 0, 0), Gdiplus::Color(0, 0, 0));

    float angle = static_cast<float>(atan2(mDirectionY, mDirectionX) * 180.0 / 3.1415926535);
    Gdiplus::Matrix matrix;
    matrix.RotateAt(angle, Gdiplus::PointF(mX, mY));
    graphics.SetTransform(&matrix);

    Gdiplus::Bitmap arrowBitmap((HBITMAP)mFireBallAnimation[mCurrentFrame], nullptr);
    graphics.DrawImage(&arrowBitmap,
        Gdiplus::Rect(drawX, drawY, renderWidth, renderHeight),
        0, 0, imageWidth, imageHeight,
        Gdiplus::UnitPixel, &imageAttr);

    graphics.ResetTransform();
}

void Player_Skill_FireBall::UpdateHitbox()
{
    float scale = 1.5f;
    int imageWidth = static_cast<int>(mFireBallAnimation[0].GetWidth() * scale);
    int imageHeight = static_cast<int>(mFireBallAnimation[0].GetHeight() * scale);

    POINT basePoints[4] = {
        { -imageWidth / 2, -imageHeight / 2 }, // 좌상
        {  imageWidth / 2, -imageHeight / 2 }, // 우상
        {  imageWidth / 2,  imageHeight / 2 }, // 우하
        { -imageWidth / 2,  imageHeight / 2 }  // 좌하
    };

    float angle = static_cast<float>(atan2(mDirectionY, mDirectionX));
    float cosA = cos(angle);
    float sinA = sin(angle);

    for (int i = 0; i < 4; ++i)
    {
        float x = basePoints[i].x;
        float y = basePoints[i].y;
        hitboxPoints[i].x = static_cast<LONG>(mX + (x * cosA - y * sinA));
        hitboxPoints[i].y = static_cast<LONG>(mY + (x * sinA + y * cosA));
    }
}

bool Player_Skill_FireBall::CheckCollisionWithRect(const RECT& rect)
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

    for (const auto& p : rectPoints)
    {
        if (PointInPolygon(p, hitboxPoints))
        {
            return true;
        }
    }

    for (const auto& p : hitboxPoints)
    {
        if (p.x >= rect.left && p.x <= rect.right && p.y >= rect.top && p.y <= rect.bottom)
        {
            return true;
        }
    }

    return false;
}

void Player_Skill_FireBall::Active(float mX, float mY, float angle, Scene* stage)
{
  
    float dirX = cos(angle);
    float dirY = sin(angle);
    stage->AddPlayerSkillFireBall(new Player_Skill_FireBall(mX, mY, dirX, dirY));
}