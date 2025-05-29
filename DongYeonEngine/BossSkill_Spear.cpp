#include "BossSkill_Spear.h"
#include "Stage1.h"
#include "Time.h"
#include <cmath>


BossSkill_Spear::BossSkill_Spear(float x, float y, float dirX, float dirY)
    : mX(x), mY(y), mDirectionX(dirX), mDirectionY(dirY), speed(600.0f), mIsActive(true), damage(0)
{

    float offsetDistance = 20.0f;
    mX += dirX * offsetDistance;
    mY += dirY * offsetDistance;


    mSpearImage.Load(L"resources/Boss/SpearSkill/IceSpear.png");

    UpdateHitbox();

}

BossSkill_Spear::~BossSkill_Spear()
{
}

void BossSkill_Spear::Update(Player& player)
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

void BossSkill_Spear::Render(HDC hdc)
{
    if (!mIsActive) return;

    HPEN hitboxPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, hitboxPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));

    // 히트박스 그리기
    Polygon(hdc, hitboxPoints, 4);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hitboxPen);

    int imageWidth = mSpearImage.GetWidth();
    int imageHeight = mSpearImage.GetHeight();
    float scale = 1.3f;
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

    Gdiplus::Bitmap arrowBitmap((HBITMAP)mSpearImage, nullptr);
    graphics.DrawImage(&arrowBitmap,
        Gdiplus::Rect(drawX, drawY, renderWidth, renderHeight),
        0, 0, imageWidth, imageHeight,
        Gdiplus::UnitPixel, &imageAttr);

    graphics.ResetTransform();
}

void BossSkill_Spear::ThrowSpear(Player& player, float mX, float mY, Scene* stage)
{
    float playerX = player.GetPositionX();
    float playerY = player.GetPositionY();

    // 보스가 플레이어를 향하는 방향 계산
    float distance = sqrt(pow(mX - playerX, 2) + pow(mY - playerY, 2));
    float baseDirX = (distance > 0.0f) ? (playerX - mX) / distance : 1.0f;
    float baseDirY = (distance > 0.0f) ? (playerY - mY) / distance : 0.0f;

    // 보스의 방향에 수직인 벡터 계산 (창 간격을 위해)
    float perpDirX = -baseDirY; // 방향 벡터에 수직인 벡터 (90도 회전)
    float perpDirY = baseDirX;

    // 창 간격 설정: 더 넓은 간격
    float offset = 60.0f; // 창 간 수직 간격 (기존 30.0f에서 60.0f로 증가)

    // 창 3개의 시작 위치
    float startX[3] = { mX, mX + perpDirX * offset, mX - perpDirX * offset };
    float startY[3] = { mY, mY + perpDirY * offset, mY - perpDirY * offset };

    // 각 창에 대해 플레이어를 향하는 방향 계산
    for (int i = 0; i < 3; ++i) {
        float spearDistance = sqrt(pow(startX[i] - playerX, 2) + pow(startY[i] - playerY, 2));
        float dirX = (spearDistance > 0.0f) ? (playerX - startX[i]) / spearDistance : 1.0f;
        float dirY = (spearDistance > 0.0f) ? (playerY - startY[i]) / spearDistance : 0.0f;

        // 창 추가
        stage->AddBossSkillIceSpear(new BossSkill_Spear(startX[i], startY[i], dirX, dirY));
    }
}
void BossSkill_Spear::UpdateHitbox()
{
    float scale = 1.3f;
    int imageWidth = static_cast<int>(mSpearImage.GetWidth() * scale);
    int imageHeight = static_cast<int>(mSpearImage.GetHeight() * scale);

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

bool BossSkill_Spear::CheckCollisionWithRect(const RECT& rect)
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