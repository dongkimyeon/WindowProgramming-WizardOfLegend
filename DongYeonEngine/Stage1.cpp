#include "Stage1.h"
#include "Input.h"
#include "Time.h"
#include "Archer.h"
#include "Arrow.h"

Stage1::Stage1()
{
    player.SetPosition(1000, 500);

    // �ʱ� �� ��ü �߰�
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(700, 100);
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(800, 200);

    wizards.push_back(new Wizard());
    wizards.back()->SetPosition(900, 100);

    archers.push_back(new Archer());
    archers.back()->SetPosition(500, 100);
}

Stage1::~Stage1()
{
    // ���� �Ҵ�� ��ü ����
    for (auto* swordman : swordmans) delete swordman;
    for (auto* wizard : wizards) delete wizard;
    for (auto* archer : archers) delete archer;
    for (auto* arrow : arrows) delete arrow;
    for (auto* fireball : fireballs) delete fireball;
}

void Stage1::Initialize()
{
}

void Stage1::Update()
{
    player.Update();

    // ��� �� ��ü ������Ʈ
    for (auto* archer : archers) archer->Update(player, this);
    for (auto* wizard : wizards) wizard->Update(player, this);
    for (auto* swordman : swordmans) swordman->Update(player);

    // ȭ�� ������Ʈ
    for (auto it = arrows.begin(); it != arrows.end();)
    {
        if ((*it)->IsActive())
        {
            (*it)->Update(player);
            ++it;
        }
        else
        {
            delete* it;
            it = arrows.erase(it);
        }
    }

    // ���̾ ������Ʈ
    for (auto it = fireballs.begin(); it != fireballs.end();)
    {
        if ((*it)->IsActive())
        {
            (*it)->Update(player);
            ++it;
        }
        else
        {
            delete* it;
            it = fireballs.erase(it);
        }
    }

    // �÷��̾� ���� ó��
    POINT effectHitboxPoints[4];
    bool hasEffectHitbox = player.GetEffectHitbox(effectHitboxPoints);

    //�ǰ� ó��
    if (hasEffectHitbox)
    {
        for (auto* swordman : swordmans)
        {
            RECT enemyRect = swordman->GetRect();
            if (player.CheckCollisionWithRect(enemyRect) && !swordman->HasBeenHit())
            {
                swordman->TakeDamage(player.GetDamage());
                swordman->SetHitFlag(true); // �ǰ� �÷��� ����
            }
        }
        for (auto* wizard : wizards)
        {
            RECT enemyRect = wizard->GetRect();
            if (player.CheckCollisionWithRect(enemyRect) && !wizard->HasBeenHit())
            {
                wizard->TakeDamage(player.GetDamage());
                wizard->SetHitFlag(true); // �ǰ� �÷��� ����
            }
        }
        for (auto* archer : archers)
        {
            RECT enemyRect = archer->GetRect();
            if (player.CheckCollisionWithRect(enemyRect) && !archer->HasBeenHit())
            {
                archer->TakeDamage(player.GetDamage());
                archer->SetHitFlag(true); // �ǰ� �÷��� ����
            }
        }
    }
    else
    {
        // ������ ������ �� ��� ���� �ǰ� �÷��� ����
        for (auto* swordman : swordmans) swordman->SetHitFlag(false);
        for (auto* wizard : wizards) wizard->SetHitFlag(false);
        for (auto* archer : archers) archer->SetHitFlag(false);
    }

	// �� ��ü�鳢�� ��ġ���ʰ� ó��
    HandleCollision();

}

void Stage1::LateUpdate()
{
    Scene::LateUpdate();
}

void Stage1::Render(HDC hdc)
{
    // ���� ��� �׸���
    int gridSize = 50; // ���� �� ĭ�� ũ�� (�ȼ�)
    int screenWidth = 1920; // ȭ�� �ʺ�
    int screenHeight = 1080; // ȭ�� ����

    // ����� �ܻ����� ä��� (���� ����)
    RECT screenRect = { 0, 0, screenWidth, screenHeight };
    HBRUSH bgBrush = CreateSolidBrush(RGB(200, 200, 200)); // ȸ�� ���
    FillRect(hdc, &screenRect, bgBrush);
    DeleteObject(bgBrush);

    // ���� �� �׸���
    HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150)); // ���� �� ���� (���� ȸ��)
    HPEN oldPen = (HPEN)SelectObject(hdc, gridPen);

    // ���� �� �׸���
    for (int x = 0; x <= screenWidth; x += gridSize)
    {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, screenHeight);
    }

    // ���� �� �׸���
    for (int y = 0; y <= screenHeight; y += gridSize)
    {
        MoveToEx(hdc, 0, y, NULL);
        LineTo(hdc, screenWidth, y);
    }

    // �� ��ü ���� �� ����
    SelectObject(hdc, oldPen);
    DeleteObject(gridPen);

    // ��� ���� �÷��̾� ������
    for (auto* wizard : wizards) wizard->Render(hdc, player);
    for (auto* archer : archers) archer->Render(hdc, player);
    for (auto* swordman : swordmans) swordman->Render(hdc, player);
    player.Render(hdc);

    for (auto* arrow : arrows) arrow->Render(hdc);
    for (auto* fireball : fireballs) fireball->Render(hdc);

    // UI �ؽ�Ʈ ���
    if (!swordmans.empty())
    {
        WCHAR swordManHpText[100];
        wsprintf(swordManHpText, L"enemy Hp : %d", swordmans[0]->GetHp());
        TextOut(hdc, 0, 20, swordManHpText, lstrlen(swordManHpText));
    }
    WCHAR PlayerHpText[100];
    wsprintf(PlayerHpText, L"player Hp : %d", player.GetHp());
    TextOut(hdc, 0, 40, PlayerHpText, lstrlen(PlayerHpText));

    WCHAR Text[100];
    wsprintf(Text, L"X : %d Y : %d", static_cast<int>(Input::GetMousePosition().x), static_cast<int>(Input::GetMousePosition().y));
    TextOut(hdc, static_cast<int>(Input::GetMousePosition().x) + 10, static_cast<int>(Input::GetMousePosition().y), Text, lstrlen(Text));
}

void Stage1::HandleCollision()
{
    // ��� ��ü�� �ϳ��� ���Ϳ� ����
    std::vector<GameObject*> allObjects;
    allObjects.push_back(&player); // �÷��̾� �߰�
    for (auto* swordman : swordmans) allObjects.push_back(swordman);
    for (auto* wizard : wizards) allObjects.push_back(wizard);
    for (auto* archer : archers) allObjects.push_back(archer);

    // ��� ��ü �ֿ� ���� �浹 Ȯ��
    for (size_t i = 0; i < allObjects.size(); ++i)
    {
        for (size_t j = i + 1; j < allObjects.size(); ++j)
        {
            if (allObjects[i]->IsCollidingWith(allObjects[j]))
            {
                ResolveCollision(*allObjects[i], *allObjects[j]);
            }
        }
    }
	
}

void Stage1::ResolveCollision(GameObject& obj1, GameObject& obj2)
{

    RECT rect1 = obj1.GetRect();
    RECT rect2 = obj2.GetRect();
    RECT intersect;

    // �浹 Ȯ��
    if (IntersectRect(&intersect, &rect1, &rect2))
    {
        int overlapX = min(rect1.right, rect2.right) - max(rect1.left, rect2.left);
        int overlapY = min(rect1.bottom, rect2.bottom) - max(rect1.top, rect2.top);

        if (overlapX < overlapY)
        {
            // x�� �о��
            if (rect1.left < rect2.left)
            {
                obj1.SetPosition(obj1.GetPositionX() - overlapX, obj1.GetPositionY());
                obj2.SetPosition(obj2.GetPositionX() + overlapX, obj2.GetPositionY());
            }
            else
            {
                obj1.SetPosition(obj1.GetPositionX() + overlapX, obj1.GetPositionY());
                obj2.SetPosition(obj2.GetPositionX() - overlapX, obj2.GetPositionY());
            }
        }
        else
        {
            // y�� �о��
            if (rect1.top < rect2.top)
            {
                obj1.SetPosition(obj1.GetPositionX(), obj1.GetPositionY() - overlapY);
                obj2.SetPosition(obj2.GetPositionX(), obj2.GetPositionY() + overlapY);
            }
            else
            {
                obj1.SetPosition(obj1.GetPositionX(), obj1.GetPositionY() + overlapY);
                obj2.SetPosition(obj2.GetPositionX(), obj2.GetPositionY() - overlapY);
            }
        }
    }
}
