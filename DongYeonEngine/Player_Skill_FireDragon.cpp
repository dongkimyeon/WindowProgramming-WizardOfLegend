#include "Player_Skill_FireDragon.h"
#include "Stage1.h"
#include "Time.h"
#include <cmath>
#include <random>
#include <iostream>
#include "SoundManager.h"

Player_Skill_FireDragon::Player_Skill_FireDragon(float x, float y, float dirX, float dirY, float phaseOffset)
    : mX(x), mY(y), mDirectionX(dirX), mDirectionY(dirY), speed(800.0f), mIsActive(true), damage(8),
    mCurrentFrame(0), mAnimationTimer(0.0f), mWaveTime(0.0f), phaseOffset(phaseOffset),
    mInstantDirX(0.0f), mInstantDirY(0.0f), mParticleTimer(0.0f), mParticleSpawnInterval(0.05f),
    mFrameDuration(0.1f)
{
    float offsetDistance = 30.0f;
    mX += dirX * offsetDistance;
    mY += dirY * offsetDistance;

    mFireDragonLeftImage.Load(L"resources/Player/Player_Skill_FireDragonL/SKILL_FIREDRAGON_00.png");
    mFireDragonRightImage.Load(L"resources/Player/Player_Skill_FireDragonR/SKILL_FIREDRAGON_00.png");

    for (int i = 0; i < 20; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/FireEffect/FIRE_PARTICLE_%02d.png", i);
        if (mFireParticleImage[i].Load(path) != S_OK) {
            std::cout << "Failed to load particle image: " << i << std::endl;
        }
    }

    SoundManager::GetInstance()->mPlaySound("FireBlastStart", false);

    UpdateHitbox();
}

Player_Skill_FireDragon::~Player_Skill_FireDragon()
{
    mFireDragonLeftImage.Destroy();
    mFireDragonRightImage.Destroy();
    for (int i = 0; i < 20; ++i) {
        mFireParticleImage[i].Destroy();
    }
    SoundManager::GetInstance()->mPlaySound("FireBlastEnd", false);
}

void Player_Skill_FireDragon::Move()
{
    if (!mIsActive) return;

    // 물결 효과 계산
    mWaveTime += Time::DeltaTime();

    float baseVelX = mDirectionX * speed;
    float baseVelY = mDirectionY * speed;

    float waveAmplitude = 60.0f;
	float waveFrequency = 10.0f; // 주파수

    float perpX = mDirectionY;
    float perpY = -mDirectionX;

    float waveVel = waveAmplitude * waveFrequency * cos(waveFrequency * mWaveTime + phaseOffset);

    mInstantDirX = baseVelX + perpX * waveVel;
    mInstantDirY = baseVelY + perpY * waveVel;

    // 위치 업데이트
    mX += mInstantDirX * Time::DeltaTime();
    mY += mInstantDirY * Time::DeltaTime();

    UpdateHitbox();
    UpdateParticles();

    // 애니메이션 업데이트
    mAnimationTimer += Time::DeltaTime();
    if (mAnimationTimer >= mFrameDuration)
    {
        mCurrentFrame = (mCurrentFrame + 1) % 5;
        mAnimationTimer -= mFrameDuration;
    }

    // 맵 경계 체크
    const float mapWidth = 5000.0f;
    const float mapHeight = 5000.0f;
    if (mX < 0 || mX > mapWidth || mY < 0 || mY > mapHeight)
    {
        mIsActive = false;
    }
}

bool Player_Skill_FireDragon::CheckCollision(GameObject& obj)
{
    if (!mIsActive || obj.GetIsDead()) return false;

    if (CheckCollisionWithRect(obj.GetRect()))
    {
        obj.TakeDamage(damage);
        mIsActive = false;
        return true;
    }
    return false;
}

void Player_Skill_FireDragon::Render(HDC hdc)
{
    if (!mIsActive) return;

    CImage& currentImage = (mInstantDirX >= 0) ? mFireDragonRightImage : mFireDragonLeftImage;

    const int imageWidth = 179;  // 원본 가로
    const int imageHeight = 117; // 원본 세로
    float scale = 0.75f;
    int renderWidth = static_cast<int>(imageWidth * scale);
    int renderHeight = static_cast<int>(imageHeight * scale);

    float angle = static_cast<float>(atan2(mInstantDirY, mInstantDirX));

    XFORM xForm = { 0 };
    xForm.eM11 = cos(angle);
    xForm.eM12 = sin(angle);
    xForm.eM21 = -sin(angle);
    xForm.eM22 = cos(angle);
    xForm.eDx = mX;
    xForm.eDy = mY;

    SetGraphicsMode(hdc, GM_ADVANCED);
    SetWorldTransform(hdc, &xForm);

    HDC srcDC = currentImage.GetDC();
    TransparentBlt(
        hdc,
        -renderWidth / 2, -renderHeight / 2, renderWidth, renderHeight,
        srcDC,
        0, 0, imageWidth, imageHeight,
        RGB(0, 0, 0)
    );
    currentImage.ReleaseDC();

    XFORM identity = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
    SetWorldTransform(hdc, &identity);
    SetGraphicsMode(hdc, GM_COMPATIBLE);

    // 파티클 렌더링
    for (const auto& particle : mParticles)
    {
        CImage& particleImage = mFireParticleImage[particle.frame];
        int pWidth = particleImage.GetWidth();
        int pHeight = particleImage.GetHeight();
        float pScale = 0.7f;
        int pRenderWidth = static_cast<int>(pWidth * pScale);
        int pRenderHeight = static_cast<int>(pHeight * pScale);
        int pDrawX = static_cast<int>(particle.x - pRenderWidth / 2.0f);
        int pDrawY = static_cast<int>(particle.y - pRenderHeight / 2.0f);

        HDC particleDC = particleImage.GetDC();
        TransparentBlt(
            hdc,
            pDrawX, pDrawY, pRenderWidth, pRenderHeight,
            particleDC,
            0, 0, pWidth, pHeight,
            RGB(0, 0, 0)
        );
        particleImage.ReleaseDC();
    }

    // 히트박스 디버깅
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    MoveToEx(hdc, hitboxPoints[0].x, hitboxPoints[0].y, nullptr);
    for (int i = 1; i < 4; ++i) {
        LineTo(hdc, hitboxPoints[i].x, hitboxPoints[i].y);
    }
    LineTo(hdc, hitboxPoints[0].x, hitboxPoints[0].y);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void Player_Skill_FireDragon::UpdateHitbox()
{
    float scale = 0.5f;
    int imageWidth = static_cast<int>(mFireDragonLeftImage.GetWidth() * scale) - 50;
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

    // 마나에 따라 드래곤 수와 발사 패턴 조정
    int numDragons = (player != nullptr && player->GetMp() >= 100.0f) ? 15 : 10; // 마나 100 이상 시 15개, 아니면 10개
    const float spawnInterval = 0.07f;
    const float angleSpread = 0.5f; // 부채꼴 각도 범위 (라디안, 약 28.6도)

    if (triggerFire && !isFiring)
    {
        isFiring = true;
        accumulatedTime = 0.0f;
        dragonsSpawned = 0;
        savedMX = mX;
        savedMY = mY;
        savedAngle = angle;
    }

    if (isFiring)
    {
        accumulatedTime += Time::DeltaTime();

        if (dragonsSpawned < numDragons && accumulatedTime >= spawnInterval)
        {
            // 부채꼴 패턴으로 여러 갈래 발사
            int dragonsPerWave = (player != nullptr && player->GetMp() >= 100.0f) ? 3 : 1; // 마나 100 이상 시 한 번에 3개 발사
            for (int i = 0; i < dragonsPerWave; ++i)
            {
                // 각 드래곤의 발사 각도 계산
                float spreadOffset = (dragonsPerWave > 1) ? (i - (dragonsPerWave - 1) / 2.0f) * (angleSpread / (dragonsPerWave - 1)) : 0.0f;
                float currentAngle = savedAngle + spreadOffset;

                float dirX = cos(currentAngle);
                float dirY = sin(currentAngle);
                float phaseOffset = (dragonsSpawned % 2 == 0) ? 0.0f : 3.1415926535f;

                stage->AddPlayerSkillFireDragon(new Player_Skill_FireDragon(savedMX, savedMY, dirX, dirY, phaseOffset));
            }

            dragonsSpawned += dragonsPerWave;
            accumulatedTime = 0.0f;
        }

        if (dragonsSpawned >= numDragons)
        {
            isFiring = false;
            dragonsSpawned = 0;
            accumulatedTime = 0.0f;
            if (player != nullptr)
            {
                player->ResetFireDragonTriggered();
                // 마나 소모 (예: 마나 100일 때 추가 소모)
                if (player->GetMp() >= 100.0f)
                {
                    player->ConsumeMana(100); // 마나 소모량은 게임 밸런스에 맞게 조정
                }
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
}

void Player_Skill_FireDragon::UpdateParticles()
{
    mParticleTimer += Time::DeltaTime();
    if (mParticleTimer >= mParticleSpawnInterval) {
        SpawnParticle();
        mParticleTimer = 0.0f;
    }
    //a

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