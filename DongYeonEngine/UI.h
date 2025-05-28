#pragma once
#include "CommonInclude.h"

class UI {
public:
    UI();
     ~UI();
    static void Initialize(); 
    static void Update();    
    static void Render(HDC hdc); 
private:
    static CImage UI_HPBAR;
    static CImage UI_MPBAR;
    static CImage UI_PLAYERBAR;
    static CImage UI_SKILLBAR;
    static CImage UI_BOSSBAR;
    static CImage UI_BOSS_HPBAR;

   
};