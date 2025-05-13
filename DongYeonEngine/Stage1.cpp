#pragma once
#include "Stage1.h"
#include "Input.h"
#include "Time.h"
Stage1::Stage1()
{
    player.SetPosition(1000, 500);
    archer.SetPosition(500, 100);
    swordman.SetPosition(700, 100);
    wizard.SetPosition(900, 100);
    swordmanAttackCooldown = 0.0f; // ��Ÿ�� �ʱ�ȭ
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
    archer.Update(player);
    wizard.Update(player);
    swordman.Update(player);

    // ��Ÿ�� ������Ʈ
    if (swordmanAttackCooldown > 0.0f) {
        swordmanAttackCooldown -= Time::DeltaTime();
    }

    // �浹 üũ
    RECT playerRect = player.GetRect(); // ����: Player::GetHitbox()�� RECT ��ȯ
    if (swordmanAttackCooldown <= 0.0f && swordman.CheckCollisionWithRect(playerRect)) 
    {
        std::cout << "asd" << std::endl;
        player.TakeDamage(swordman.GetDamage()); 
        
        swordmanAttackCooldown = 0.5f; // 0.5�� ��Ÿ��
    }

}

void Stage1::LateUpdate()
{
    Scene::LateUpdate();
}

void Stage1::Render(HDC hdc)
{
 
    wizard.Render(hdc);
    archer.Render(hdc);
    swordman.Render(hdc, player);
    player.Render(hdc);
    WCHAR swordManHpText[100];
    wsprintf(swordManHpText, L"enemy Hp : %d", swordman.GetHp());
    TextOut(hdc, 0,20, swordManHpText, lstrlen(swordManHpText));
    WCHAR PlayerHpText[100];
    wsprintf(PlayerHpText, L"player Hp : %d", player.GetHp());
    TextOut(hdc, 0,40, PlayerHpText, lstrlen(PlayerHpText));


    WCHAR Text[100];
    wsprintf(Text, L"X : %d Y : %d", (int)Input::GetMousePosition().x, (int)Input::GetMousePosition().y);
    TextOut(hdc, Input::GetMousePosition().x + 10, Input::GetMousePosition().y, Text, lstrlen(Text));
}