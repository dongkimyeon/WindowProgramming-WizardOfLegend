#pragma once
#include "CommonInclude.h"

  
class Time
{
public:
    static void Initialize();
    static void Update();
    static void Render(HDC hdc);

    static float DeltaTime();

private:
    static LARGE_INTEGER CpuFrequency;
    static LARGE_INTEGER PrevFrequency;
    static LARGE_INTEGER CurrentFrequency;
    static float DeltaTimeValue;
};