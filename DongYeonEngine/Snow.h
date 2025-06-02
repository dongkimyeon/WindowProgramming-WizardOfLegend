#pragma once
#include "CommonInclude.h"
#include "Time.h"

class Snow
{
public:
    Snow()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> disX(0, 1279);
        std::uniform_int_distribution<int> disPhase(0, 359);
        std::uniform_int_distribution<int> disImage(0, 3);
        std::uniform_real_distribution<float> disSpeed(100, 200);
        mSpeed = disSpeed(gen);
        mX = static_cast<float>(disX(gen));
        mY = 0.0f;
        mPhase = static_cast<float>(disPhase(gen));
        for (int i = 0; i < 4; ++i)
        {
            wchar_t path[256];
            swprintf_s(path, L"resources/Snow/Snow%d.png", i);
            mSnowImage[i].Load(path);
        }
        mCurSnowImage = &mSnowImage[disImage(gen)];
    }

    ~Snow()
    {
        for (int i = 0; i < 4; ++i)
        {
            mSnowImage[i].Destroy(); // 배열 요소별로 Destroy 호출
        }
    }

    void Update()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> disX(0, 1279);
        std::uniform_int_distribution<int> disPhase(0, 359);
        std::uniform_int_distribution<int> disImage(0, 3);

        mY += mSpeed * Time::DeltaTime();
        mPhase += 2.0f * Time::DeltaTime();
        mX += sinf(mPhase) * 20.0f * Time::DeltaTime();
        if (mY > 720)
        {
            mY = 0.0f;
            mX = static_cast<float>(disX(gen));
            mPhase = static_cast<float>(disPhase(gen));
            mCurSnowImage = &mSnowImage[disImage(gen)];
        }
    }


    void Render(HDC hdc)
    {
        if (mCurSnowImage)
        {
            // 이미지 원본 크기로 렌더링
            int width = mCurSnowImage->GetWidth() + 3;
            int height = mCurSnowImage->GetHeight() + 3;
            mCurSnowImage->Draw(hdc, static_cast<int>(mX), static_cast<int>(mY), width, height);
        }
    }

private:
    CImage mSnowImage[4];
    CImage* mCurSnowImage;
    float mX;
    float mY;
    float mSpeed;
    float mPhase; // 사인파 위상 (좌우 흔들림용)
};