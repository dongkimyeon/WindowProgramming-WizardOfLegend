#pragma once
#include "Stage1.h"
#include "Input.h"

Stage1::Stage1()
{
    player.SetPosition(1920 / 2, 1080 / 2);
    archer.SetPosition(100, 100);
    swordman.SetPosition(200, 100);
    wizard.SetPosition(300, 100);
}

Stage1::~Stage1()
{
}

void Stage1::Initialize()
{
    
}

void Stage1::Update()
{
    player.Update();


}

void Stage1::LateUpdate()
{
    Scene::LateUpdate();
}

void Stage1::Render(HDC hdc)
{
    player.Render(hdc);
 
    WCHAR Text[100];
    wsprintf(Text, L"X : %d Y : %d", (int)Input::GetMousePosition().x, (int)Input::GetMousePosition().y);
    TextOut(hdc, Input::GetMousePosition().x + 10, Input::GetMousePosition().y, Text, lstrlen(Text));
}