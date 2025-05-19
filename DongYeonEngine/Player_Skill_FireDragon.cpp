#include "Player_Skill_FireDragon.h"
#include "Stage1.h"
#include "Time.h"
#include <cmath>
#include <iostream>
#include <random>

Player_Skill_FireDragon::Player_Skill_FireDragon(float x, float y, float dirX, float dirY, float phaseOffset)
    : mX(x), mY(y), mDirectionX(dirX), mDirectionY(dirY), speed(200.0f), mIsActive(true), damage(25),
    mCurrentFrame(0), mAnimationTimer(0.0f), mWaveTime(0.0f), phaseOffset(phaseOffset),
    mInstantDirX(0.0f), mInstantDirY(0.0f), mParticleTimer(0.0f), mParticleSpawnInterval(0.1f),
    mFrameDuration(0.1f)
{
    mFireDragonLeftImage.Load(L"resources/Player/Player_Skill_FireDragonL/SKILL_FIREDRAGON_00.png");
    mFireDragonRightImage.Load(L"resources/Player/Player_Skill_FireDragonR/SKILL_FIREDRAGON_00.png");

    for (int i = 0; i < 20; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/FireEffect/FIRE_PARTICLE_%02d.png", i);
        if (mFireParticleImage[i].Load(path) != S_OK) {
            std::cout << "Failed to load particle image: " << i << std::endl;
        }
        else {
            std::cout << "Loaded particle image: " << i << std::endl;
        }
    }

    UpdateHitbox();
    std::cout << "FireDragon created at (" << mX << ", " << mY << ")" << std::endl;
}

Player_Skill_FireDragon::~Player_Skill_FireDragon()
{
    mFireDragonLeftImage.Destroy();
    mFireDragonRightImage.Destroy();
    for (int i = 0; i < 20; ++i) {
        mFireParticleImage[i].Destroy();
    }
}

void Player_Skill_FireDragon::Update(GameObject& obj)
{
    if (!mIsActive) return;

    mWaveTime += Time::DeltaTime();

    float baseVelX = mDirectionX * speed;
    float baseVelY = mDirectionY * speed;

    float waveAmplitude = 60.0f;
    float waveFrequency = 2.0f;

    float perpX = mDirectionY;
    float perpY = -mDirectionX;

    float waveVel = waveAmplitude * waveFrequency * cos(waveFrequency * mWaveTime + phaseOffset);

    mInstantDirX = baseVelX + perpX * waveVel;
    mInstantDirY = baseVelY + perpY * waveVel;

    mX += mInstantDirX * Time::DeltaTime();
    mY += mInstantDirY * Time::DeltaTime();

    UpdateHitbox();
    UpdateParticles();

    mAnimationTimer += Time::DeltaTime();
    if (mAnimationTimer >= mFrameDuration)
    {
        mCurrentFrame = (mCurrentFrame + 1) % 5;
        mAnimationTimer -= mFrameDuration;
    }

    const float mapWidth = 5000.0f;
    const float mapHeight = 5000.0f;
    if (mX < 0 || mX > mapWidth || mY < 0 || mY > mapHeight)
    {
        mIsActive = false;
        return;
    }

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

    HPEN hitboxPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, hitboxPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
    Polygon(hdc, hitboxPoints, 4);
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hitboxPen);

    CImage& currentImage = (mInstantDirX >= 0) ? mFireDragonRightImage : mFireDragonLeftImage;

    int imageWidth = currentImage.GetWidth();
    int imageHeight = currentImage.GetHeight();
    float scale = 0.6f;
    int renderWidth = static_cast<int>(imageWidth * scale);
    int renderHeight = static_cast<int>(imageHeight * scale);
    int drawX = static_cast<int>(mX - renderWidth / 2.0f);
    int drawY = static_cast<int>(mY - renderHeight / 2.0f);

    Gdiplus::Graphics graphics(hdc);
    Gdiplus::ImageAttributes imageAttr;
    //imageAttr.SetColorKey(Gdiplus::Color(0, 0, 0), Gdiplus::Color(0, 0, 0), Gdiplus::ColorAdjustTypeBitmap);
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

    std::cout << "Particle count: " << mParticles.size() << std::endl;
    for (const auto& particle : mParticles) {
        CImage& particleImage = mFireParticleImage[particle.frame];
        int pWidth = particleImage.GetWidth();
        int pHeight = particleImage.GetHeight();
        float pScale = 0.7f;
        int pRenderWidth = static_cast<int>(pWidth * pScale);
        int pRenderHeight = static_cast<int>(pHeight * pScale);
        int pDrawX = static_cast<int>(particle.x - pRenderWidth / 2.0f);
        int pDrawY = static_cast<int>(particle.y - pRenderHeight / 2.0f);

        std::cout << "Rendering particle at (" << pDrawX << ", " << pDrawY << ") with frame " << particle.frame << std::endl;

        Gdiplus::Bitmap particleBitmap((HBITMAP)particleImage, nullptr);
        graphics.DrawImage(&particleBitmap,
            Gdiplus::Rect(pDrawX, pDrawY, pRenderWidth, pRenderHeight),
            0, 0, pWidth, pHeight,
            Gdiplus::UnitPixel, &imageAttr);
    }
}

void Player_Skill_FireDragon::UpdateHitbox()
{
    float scale = 0.5f;
    int imageWidth = static_cast<int>(mFireDragonLeftImage.GetWidth() * scale);
    int imageHeight = static_cast<int>(mFireDragonLeftImage.GetHeight() * scale);

    POINT basePoints[4] = {
        { -imageWidth / 2, -imageHeight / 2 },
        { imageWidth / 2, -imageHeight / 2 },
        { imageWidth / 2, imageHeight / 2 },
        { -imageWidth / 2, imageHeight / 2 }
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

void Player_Skill_FireDragon::Active(float mX, float mY, float angle, Scene* stage, Player* player, bool triggerFire)
{
    static bool isFiring = false;
    static float accumulatedTime = 0.0f;
    static int dragonsSpawned = 0;
    static float savedMX = 0.0f;
    static float savedMY = 0.0f;
    static float savedAngle = 0.0f;
    const int numDragons = 10;
    const float spawnInterval = 0.07f;

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

    if (isFiring)
    {
        accumulatedTime += Time::DeltaTime();
        std::cout << "Active called: accumulatedTime = " << accumulatedTime << ", dragonsSpawned = " << dragonsSpawned << std::endl;

        if (dragonsSpawned < numDragons && accumulatedTime >= spawnInterval)
        {
            float dirX = cos(savedAngle);
            float dirY = sin(savedAngle);
            float phaseOffset = (dragonsSpawned % 2 == 0) ? 0.0f : 3.1415926535f;

            std::cout << "Spawning FireDragon #" << dragonsSpawned + 1 << " at (" << savedMX << ", " << savedMY << ") with phaseOffset = " << phaseOffset << std::endl;

            stage->AddPlayerSkillFireDragon(new Player_Skill_FireDragon(savedMX, savedMY, dirX, dirY, phaseOffset));

            dragonsSpawned++;
            accumulatedTime = 0.0f;
        }

        if (dragonsSpawned >= numDragons)
        {
            std::cout << "All " << numDragons << " FireDragons spawned, stopping firing." << std::endl;
            isFiring = false;
            dragonsSpawned = 0;
            accumulatedTime = 0.0f;
            if (player != nullptr)
            {
                player->ResetFireDragonTriggered();
            }
        }
    }
}

void Player_Skill_FireDragon::SpawnParticle()
{
   
    float magnitude = std::sqrt(mInstantDirX * mInstantDirX + mInstantDirY * mInstantDirY);
    float normDirX = (magnitude > 0) ? mInstantDirX / magnitude : 0.0f;
    float normDirY = (magnitude > 0) ? mInstantDirY / magnitude : 0.0f;

    float tailOffset = -30.0f;
    float spawnX = mX + tailOffset * normDirX;
    float spawnY = mY + tailOffset * normDirY;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> velDist(-5.0f, 5.0f); 
    std::uniform_real_distribution<float> lifeDist(0.5f, 1.0f);

    Particle particle;
    particle.x = spawnX;
    particle.y = spawnY;
    particle.velX = mDirectionX * speed * 0.5f + velDist(gen); 
    particle.velY = mDirectionY * speed * 0.5f + velDist(gen);
    particle.lifetime = lifeDist(gen);
    particle.initialLifetime = particle.lifetime;
    particle.frame = 0;

    mParticles.emplace_back(particle);
    std::cout << "Spawned particle at (" << spawnX << ", " << spawnY << ")" << std::endl;
}

void Player_Skill_FireDragon::UpdateParticles()
{
    mParticleTimer += Time::DeltaTime();
    if (mParticleTimer >= mParticleSpawnInterval) {
        SpawnParticle();
        mParticleTimer = 0.0f;
    }

    for (auto it = mParticles.begin(); it != mParticles.end();) {
        it->lifetime -= Time::DeltaTime();
        if (it->lifetime <= 0) {
            it = mParticles.erase(it);
            continue;
        }

        it->x += it->velX * Time::DeltaTime();
        it->y += it->velY * Time::DeltaTime();
        it->frame = static_cast<int>(20 * (1.0f - it->lifetime / it->initialLifetime)) % 20;
        ++it;
    }
}