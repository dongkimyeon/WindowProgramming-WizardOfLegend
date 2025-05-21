#include "Player_Skill_FireDragon.h"
#include "Stage1.h"
#include "Time.h"
#include <cmath>
#include <iostream> // ������ ���

Player_Skill_FireDragon::Player_Skill_FireDragon(float x, float y, float dirX, float dirY, bool useSineWave)
    : mX(x), mY(y), mDirectionX(dirX), mDirectionY(dirY), speed(300.0f), mIsActive(true), damage(25),
    mCurrentFrame(0), mAnimationTimer(0.0f), mWaveTime(0.0f), useSineWave(useSineWave), mInstantDirX(0.0f), mInstantDirY(0.0f)
{
    // ���̾� �巡�� �̹��� �ε�
    mFireDragonLeftImage.Load(L"resources/Player/Player_Skill_FireDragonL/SKILL_FIREDRAGON_00.png");
    mFireDragonRightImage.Load(L"resources/Player/Player_Skill_FireDragonR/SKILL_FIREDRAGON_00.png");

    UpdateHitbox();
    std::cout << "FireDragon created at (" << mX << ", " << mY << ")" << std::endl; // ����� ���
}

Player_Skill_FireDragon::~Player_Skill_FireDragon()
{
    mFireDragonLeftImage.Destroy();
    mFireDragonRightImage.Destroy();
}

void Player_Skill_FireDragon::Update(GameObject& obj)
{
    if (!mIsActive) return;

    // �ð��� ���� ���� ȿ�� ������Ʈ
    mWaveTime += Time::DeltaTime();

    // �⺻ �̵�
    float baseVelX = mDirectionX * speed;
    float baseVelY = mDirectionY * speed;

    // ���� ȿ���� ���� y�� �ӵ� ��ȭ
    float waveAmplitude = 100.0f; // ����
    float waveFrequency = 2.0f;  // ���ļ�
    float waveVelY;
    if (useSineWave)
    {
        waveVelY = waveAmplitude * waveFrequency * cos(mWaveTime * waveFrequency); // �������� �̺�
    }
    else
    {
        waveVelY = -waveAmplitude * waveFrequency * sin(mWaveTime * waveFrequency); // �ڻ������� �̺�
    }

    // �������� �ӵ� ���� ���
    mInstantDirX = baseVelX;
    mInstantDirY = baseVelY + waveVelY;

    // ��ġ ������Ʈ
    mX += mInstantDirX * Time::DeltaTime();
    mY += mInstantDirY * Time::DeltaTime();

    UpdateHitbox();

    // �ִϸ��̼� ������Ʈ (���� �̹����� ����ϹǷ� �������� �������� ����)
    mAnimationTimer += Time::DeltaTime();
    if (mAnimationTimer >= mFrameDuration)
    {
        mCurrentFrame = (mCurrentFrame + 1) % 5; // ������ ���� ���Ƿ� 5�� ����
        mAnimationTimer -= mFrameDuration;
    }

    // �� ��� üũ (5000x5000 ��)
    const float mapWidth = 5000.0f;
    const float mapHeight = 5000.0f;
    if (mX < 0 || mX > mapWidth || mY < 0 || mY > mapHeight)
    {
        mIsActive = false;
        return;
    }

    // �浹 üũ
    if (CheckCollisionWithRect(obj.GetRect()))
    {
        obj.TakeDamage(damage);
        mIsActive = false;
        return;
    }
}

void Player_Skill_FireDragon::Render(HDC hdc)
{
    if (!mIsActive) return;

    // ��Ʈ�ڽ� �׸���
    HPEN hitboxPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, hitboxPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
    Polygon(hdc, hitboxPoints, 4);
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hitboxPen);

    // ���̾� �巡�� �̹��� ������ (���⿡ ���� ����)
    CImage& currentImage = (mInstantDirX >= 0) ? mFireDragonRightImage : mFireDragonLeftImage;

    int imageWidth = currentImage.GetWidth();
    int imageHeight = currentImage.GetHeight();
    float scale = 0.6f;
    int renderWidth = static_cast<int>(imageWidth * scale);
    int renderHeight = static_cast<int>(imageHeight * scale);
    int drawX = static_cast<int>(mX - renderWidth / 2.0f);
    int drawY = static_cast<int>(mY - renderHeight / 2.0f);

    Gdiplus::Graphics graphics(hdc);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    Gdiplus::ImageAttributes imageAttr;

    // ��� ����� �����ϰ� ó�� (RGB 245-255 ����)
    imageAttr.SetColorKey(Gdiplus::Color(245, 245, 245), Gdiplus::Color(0, 0, 0), Gdiplus::ColorAdjustTypeBitmap);

    // �������� �ӵ� ���͸� ������� ȸ�� ���� ���
    float angle = static_cast<float>(atan2(mInstantDirY, mInstantDirX) * 180.0 / 3.1415926535);

    Gdiplus::Matrix matrix;
    matrix.RotateAt(angle, Gdiplus::PointF(mX, mY));
    graphics.SetTransform(&matrix);

    Gdiplus::Bitmap dragonBitmap((HBITMAP)currentImage, nullptr);
    graphics.DrawImage(&dragonBitmap,
        Gdiplus::Rect(drawX, drawY, renderWidth, renderHeight),
        0, 0, imageWidth, imageHeight,
        Gdiplus::UnitPixel, &imageAttr);

    graphics.ResetTransform();
}

void Player_Skill_FireDragon::UpdateHitbox()
{
    float scale = 1.0f;
    int imageWidth = static_cast<int>(mFireDragonLeftImage.GetWidth() * scale);
    int imageHeight = static_cast<int>(mFireDragonLeftImage.GetHeight() * scale);

    POINT basePoints[4] = {
        { -imageWidth / 2, -imageHeight / 2 }, // �»�
        {  imageWidth / 2, -imageHeight / 2 }, // ���
        {  imageWidth / 2,  imageHeight / 2 }, // ����
        { -imageWidth / 2,  imageHeight / 2 }  // ����
    };

    float angle = static_cast<float>(atan2(mInstantDirY, mInstantDirX));
    float cosA = cos(angle);
    float sinA = sin(angle);

    for (int i = 0; i < 4; ++i)
    {
        float x = static_cast<float>(basePoints[i].x);
        float y = static_cast<float>(basePoints[i].y);
        hitboxPoints[i].x = static_cast<LONG>(mX + (x * cosA - y * sinA));
        hitboxPoints[i].y = static_cast<LONG>(mY + (x * sinA + y * cosA));
    }
}

bool Player_Skill_FireDragon::CheckCollisionWithRect(const RECT& rect)
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

    // �簢�� ���� �巡�� ��Ʈ�ڽ� �ȿ� �ִ��� Ȯ��
    for (const auto& p : rectPoints)
    {
        if (PointInPolygon(p, hitboxPoints))
        {
            return true;
        }
    }

    // �巡�� ��Ʈ�ڽ� ���� �簢�� �ȿ� �ִ��� Ȯ��
    for (const auto& p : hitboxPoints)
    {
        if (p.x >= rect.left && p.x <= rect.right && p.y >= rect.top && p.y <= rect.bottom)
        {
            return true;
        }
    }

    return false;
}

void Player_Skill_FireDragon::Active(float mX, float mY, float angle, Scene* stage, bool triggerFire)
{
    static bool isFiring = false;
    static float accumulatedTime = 0.0f;
    static int dragonsSpawned = 0;
    static float savedMX = 0.0f;
    static float savedMY = 0.0f;
    static float savedAngle = 0.0f;
    const int numDragons = 10;
    const float spawnInterval = 0.05f;

    // �߻� ����
    if (triggerFire && !isFiring)
    {
        isFiring = true;
        accumulatedTime = 0.0f;
        dragonsSpawned = 0;
        savedMX = mX;
        savedMY = mY;
        savedAngle = angle;
        std::cout << "Firing started at (" << savedMX << ", " << savedMY << ") with angle " << savedAngle << std::endl;
    }

    // �߻� ���� �� �ð� ���� �� �巡�� ����
    if (isFiring)
    {
        accumulatedTime += Time::DeltaTime();
        std::cout << "Active called: accumulatedTime = " << accumulatedTime << ", dragonsSpawned = " << dragonsSpawned << std::endl;

        if (dragonsSpawned < numDragons && accumulatedTime >= spawnInterval)
        {
            float dirX = cos(savedAngle);
            float dirY = sin(savedAngle);

            // �����Ŀ� �ڻ� drills �� ������ ���
            bool useSine = (dragonsSpawned % 2 == 0); // ¦��: ����, Ȧ��: �ڻ���

            std::cout << "Spawning FireDragon #" << dragonsSpawned + 1 << " at (" << savedMX << ", " << savedMY << ") with useSine = " << useSine << std::endl;

            stage->AddPlayerSkillFireDragon(new Player_Skill_FireDragon(savedMX, savedMY, dirX, dirY, useSine));

            dragonsSpawned++;
            accumulatedTime = 0.0f;
        }

        // ��� �巡�� �߻� �Ϸ�
        if (dragonsSpawned >= numDragons)
        {
            std::cout << "All " << numDragons << " FireDragons spawned, stopping firing." << std::endl;
            isFiring = false;
            dragonsSpawned = 0;
            accumulatedTime = 0.0f;

        }
    }
}