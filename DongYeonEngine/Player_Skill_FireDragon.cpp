#include "Player_Skill_FireDragon.h"
#include "Stage1.h"
#include "Time.h"
#include <cmath>


Player_Skill_FireDragon::Player_Skill_FireDragon(float x, float y, float dirX, float dirY)
    : mX(x), mY(y), mDirectionX(dirX), mDirectionY(dirY), speed(300.0f), mIsActive(true), damage(25),
    mCurrentFrame(0), mAnimationTimer(0.0f), mWaveTime(0.0f)
{
    // ���̾� �巡�� �̹��� �ε�
    mFireDragonLeftImage.Load(L"resources/Player/Player_Skill_FireDragonL/SKILL_FIREDRAGON_00.png");
    mFireDragonRightImage.Load(L"resources/Player/Player_Skill_FireDragonR/SKILL_FIREDRAGON_00.png");

    UpdateHitbox();
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
    mX += mDirectionX * speed * Time::DeltaTime();
    mY += mDirectionY * speed * Time::DeltaTime();

    // ���� ȿ�� (�����ĸ� �̿��� y�� �̵�)
    float waveAmplitude = 60.0f; // ���� ũ�� ����
    float waveFrequency = 4.0f;  // ���� �ӵ� ����
    mY += sin(mWaveTime * waveFrequency) * waveAmplitude * Time::DeltaTime();

    UpdateHitbox();

    // �ִϸ��̼� ������Ʈ (�������� ���Ƿ� ����, ���� �̹����� ���� �̹��� ���)
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
    CImage& currentImage = (mDirectionX >= 0) ? mFireDragonRightImage : mFireDragonLeftImage;

    int imageWidth = currentImage.GetWidth();
    int imageHeight = currentImage.GetHeight();
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

    Gdiplus::Bitmap dragonBitmap((HBITMAP)currentImage, nullptr);
    graphics.DrawImage(&dragonBitmap,
        Gdiplus::Rect(drawX, drawY, renderWidth, renderHeight),
        0, 0, imageWidth, imageHeight,
        Gdiplus::UnitPixel, &imageAttr);

    graphics.ResetTransform();
}

void Player_Skill_FireDragon::UpdateHitbox()
{
    float scale = 1.5f;
    int imageWidth = static_cast<int>(mFireDragonLeftImage.GetWidth() * scale);
    int imageHeight = static_cast<int>(mFireDragonLeftImage.GetHeight() * scale);

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
            -WaveTime * waveFrequency)* waveAmplitude* Time::DeltaTime();
        }

        UpdateHitbox();

        // �ִϸ��̼� ������ ������Ʈ (���� �̹����� ����ϹǷ� �������� �������� ����)
        mAnimationTimer += Time::DeltaTime();
        if (mAnimationTimer >= mFrameDuration)
        {
            mCurrentFrame = (mCurrentFrame + 1) % 5; // ������ ���� ���Ƿ� 5�� ����
            mAnimationTimer -= mFrameDuration;
        }

        // �� ��� üũ
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

void Player_Skill_FireDragon::Active(float mX, float mY, float angle, Scene* stage)
{
    const int numDragons = 10; // ������ ���̾� �巡�� ��
    const float spawnInterval = 0.08f; // ���̾� �巡�� �� ���� (�ð�)

    for (int i = 0; i < numDragons; ++i)
    {
        float dirX = cos(angle);
        float dirY = sin(angle);

        // �ð� ���̸� �ξ� ����ó�� ���̰� �ʱ� ��ġ ����
        float offsetX = dirX * speed * spawnInterval * i;
        float offsetY = dirY * speed * spawnInterval * i;

        stage->AddPlayerSkillFireDragon(new Player_Skill_FireDragon(mX + offsetX, mY + offsetY, dirX, dirY));
    }
}