#include "Arrow.h"
#include "Stage1.h"
#include "Time.h"
#include <cmath>
#include "SoundManager.h"

Arrow::Arrow(float x, float y, float dirX, float dirY)
    : mX(x), mY(y), mDirectionX(dirX), mDirectionY(dirY), speed(800.0f), mIsActive(true), damage(25)
{
    
    float offsetDistance = 20.0f;
    mX += dirX * offsetDistance;
    mY += dirY * offsetDistance;


    mArrowImage.Load(L"resources/Monster/ARCHER/ARCHER_BOW/Attack/ARCHER_BOW_0.png");
    if (mArrowImage.IsNull()) wprintf(L"Failed to load: resources/Monster/ARCHER/ARCHER_BOW/Attack/ARCHER_BOW_0.png\n");
    UpdateHitbox();

    SoundManager::GetInstance()->mPlaySound("ArcherShoot", false);
}

Arrow::~Arrow()
{
    SoundManager::GetInstance()->mPlaySound("ArcherRelease", false);
    mArrowImage.Destroy();
}

void Arrow::Update(Player& player)
{
    if (!mIsActive) return;

    mX += mDirectionX * speed * Time::DeltaTime();
    mY += mDirectionY * speed * Time::DeltaTime();
    UpdateHitbox();

    if (CheckCollisionWithRect(player.GetRect()))
    {
        player.TakeDamage(damage);
        mIsActive = false;
        return;
    }

}

void Arrow::Render(HDC hdc)
{
    if (!mIsActive) return;

 

    int imageWidth = mArrowImage.GetWidth();
    int imageHeight = mArrowImage.GetHeight();
    float scale = 0.6f;
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

    Gdiplus::Bitmap arrowBitmap((HBITMAP)mArrowImage, nullptr);
    graphics.DrawImage(&arrowBitmap,
        Gdiplus::Rect(drawX, drawY, renderWidth, renderHeight),
        0, 0, imageWidth, imageHeight,
        Gdiplus::UnitPixel, &imageAttr);

    graphics.ResetTransform();
}

void Arrow::FireArrow(Player& player, float mX, float mY, Scene* stage)
{
    float playerX = player.GetPositionX();
    float playerY = player.GetPositionY();
    float distance = sqrt(pow(mX - playerX, 2) + pow(mY - playerY, 2));
    float dirX = (distance > 0.0f) ? (playerX - mX) / distance : 1.0f;
    float dirY = (distance > 0.0f) ? (playerY - mY) / distance : 0.0f;

    stage->AddArrow(new Arrow(mX, mY, dirX, dirY));
}

void Arrow::UpdateHitbox()
{
    float scale = 0.6f;
    int imageWidth = static_cast<int>(mArrowImage.GetWidth() * scale);
    int imageHeight = static_cast<int>(mArrowImage.GetHeight() * scale);

    POINT basePoints[4] = {
        { -imageWidth / 2, -imageHeight / 2 }, // �»�
        {  imageWidth / 2, -imageHeight / 2 }, // ���
        {  imageWidth / 2,  imageHeight / 2 }, // ����
        { -imageWidth / 2,  imageHeight / 2 }  // ����
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

bool Arrow::CheckCollisionWithRect(const RECT& rect)
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