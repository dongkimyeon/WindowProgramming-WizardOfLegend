#include "Dummy.h"
#include "Time.h"
#include "SoundManager.h"

Dummy::Dummy()
{
    DummyImage.Load(L"resources/Dummy.png");

    mX = 0.0f;
    mY = 0.0f;
    rect = { (int)(mX - 22), (int)(mY - 38), (int)(mX + 20), (int)(mY + 38) };
    mScale = 1.8f;
    mDamageTaken = 0;
    mDamageTextY = 0.0f;
    mDamageTextSpeed = 50.0f; // 텍스트 상승 속도 (초당 픽셀)
    mShowDamage = false;
    mHitTimer = 0.0f;
}

Dummy::~Dummy()
{
}

void Dummy::Render(HDC hdc, Player& p)
{
    // 더미 이미지 렌더링
    int imageWidth = DummyImage.GetWidth();
    int imageHeight = DummyImage.GetHeight();
    int scaledWidth = static_cast<int>(imageWidth * mScale);
    int scaledHeight = static_cast<int>(imageHeight * mScale);
    int drawX = static_cast<int>(mX - scaledWidth / 2.0f);
    int drawY = static_cast<int>(mY - scaledHeight / 2.0f);
    DummyImage.Draw(hdc, drawX, drawY, scaledWidth, scaledHeight);
    // 피격 시 데미지 텍스트 렌더링
    if (mShowDamage)
    {
        // 텍스트 위로 이동
        mDamageTextY -= mDamageTextSpeed * Time::DeltaTime();
        mHitTimer -= Time::DeltaTime();
        if (mHitTimer <= 0.0f)
        {
            mShowDamage = false;
            mHasBeenHit = false;
        }

        SetTextColor(hdc, RGB(255, 255, 255));
        HFONT hFont = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
        wchar_t damageText[32];
        swprintf_s(damageText, L"%d", mDamageTaken);

        TextOut(hdc, static_cast<int>(mX) - 40, static_cast<int>(mDamageTextY), damageText, wcslen(damageText));
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
    }

    // 충돌 사각형 디버깅용
    HBRUSH nullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, nullBrush);
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    SelectObject(hdc, oldBrush);
}

void Dummy::TakeDamage(int d)
{
    SoundManager::GetInstance()->mPlaySound("Hit", false);
    mDamageTaken = d; // 데미지 값 저장
    mShowDamage = true; // 데미지 텍스트 표시 활성화
    mDamageTextY = mY - 50; // 텍스트 초기 위치 (더미 위쪽)
    mHasBeenHit = true; // 피격 플래그 설정
    mHitTimer = 0.2f; // 데미지 텍스트 0.2초 표시
}

void Dummy::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
    rect = { (int)(mX - 22), (int)(mY - 38), (int)(mX + 20), (int)(mY + 38) };
}