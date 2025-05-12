#pragma once
#include "PlayScene.h"
#include "Input.h"

PlayScene::PlayScene()
{
    player.SetPosition(1920 / 2, 1080 / 2);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
    
}

void PlayScene::Update()
{
    player.Update();


}

void PlayScene::LateUpdate()
{
    Scene::LateUpdate();
}

void PlayScene::Render(HDC hdc)
{
    player.Render(hdc);
 
    WCHAR Text[100];
    wsprintf(Text, L"X : %d Y : %d", (int)Input::GetMousePosition().x, (int)Input::GetMousePosition().y);
    TextOut(hdc, Input::GetMousePosition().x + 10, Input::GetMousePosition().y, Text, lstrlen(Text));
}