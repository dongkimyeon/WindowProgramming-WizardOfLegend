#include "Stage1.h"
#include "Input.h"
#include "Time.h"
#include "Archer.h"
#include "Arrow.h"

Stage1::Stage1()
{
    player.SetPosition(2500, 2500);
    camera.SetTarget(&player);
	player.SetCameraX(camera.GetPositionX());
	player.SetCameraY(camera.GetPositionY());

    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(2700, 2300);
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(2800, 2400);
    wizards.push_back(new Wizard());
    wizards.back()->SetPosition(2900, 2300);
    archers.push_back(new Archer());
    archers.back()->SetPosition(2300, 2300);
}

Stage1::~Stage1()
{
    for (auto* swordman : swordmans) delete swordman;
    for (auto* wizard : wizards) delete wizard;
    for (auto* archer : archers) delete archer;
    for (auto* arrow : arrows) delete arrow;
    for (auto* fireball : fireballs) delete fireball;
}

void Stage1::Initialize()
{
    MapManager::GetInstance()->Initialize();
    camera.Update();
}

void Stage1::Update()
{
    player.Update();
    camera.Update();
    player.SetCameraX(camera.GetPositionX());
    player.SetCameraY(camera.GetPositionY());
    for (auto* archer : archers) archer->Update(player, this);
    for (auto* wizard : wizards) wizard->Update(player, this);
    for (auto* swordman : swordmans) swordman->Update(player);
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
    POINT effectHitboxPoints[4];
    bool hasEffectHitbox = player.GetEffectHitbox(effectHitboxPoints);
    if (hasEffectHitbox)
    {
        for (auto* swordman : swordmans)
        {
            RECT enemyRect = swordman->GetRect();
            if (player.CheckCollisionWithRect(enemyRect) && !swordman->HasBeenHit())
            {
                swordman->TakeDamage(player.GetDamage());
                swordman->SetHitFlag(true);
            }
        }
        for (auto* wizard : wizards)
        {
            RECT enemyRect = wizard->GetRect();
            if (player.CheckCollisionWithRect(enemyRect) && !wizard->HasBeenHit())
            {
                wizard->TakeDamage(player.GetDamage());
                wizard->SetHitFlag(true);
            }
        }
        for (auto* archer : archers)
        {
            RECT enemyRect = archer->GetRect();
            if (player.CheckCollisionWithRect(enemyRect) && !archer->HasBeenHit())
            {
                archer->TakeDamage(player.GetDamage());
                archer->SetHitFlag(true);
            }
        }
    }
    else
    {
        for (auto* swordman : swordmans) swordman->SetHitFlag(false);
        for (auto* wizard : wizards) wizard->SetHitFlag(false);
        for (auto* archer : archers) archer->SetHitFlag(false);
    }
    HandleCollision();
}

void Stage1::LateUpdate()
{
    Scene::LateUpdate();
}

void Stage1::Render(HDC hdc)
{
    int savedDC = SaveDC(hdc);
    float cameraX = camera.GetPositionX();
    float cameraY = camera.GetPositionY();
    int viewWidth = 1280;
    int viewHeight = 720;

    // Render map
    MapManager::GetInstance()->Render(hdc, cameraX, cameraY);

    // Apply camera offset for objects
    for (auto* wizard : wizards)
    {
        RECT rect = wizard->GetRect();
        if (rect.right >= cameraX && rect.left <= cameraX + viewWidth &&
            rect.bottom >= cameraY && rect.top <= cameraY + viewHeight)
        {
            // Adjust rendering position
            HDC wizardDC = hdc;
            int savedWizardDC = SaveDC(wizardDC);
            OffsetViewportOrgEx(wizardDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            wizard->Render(wizardDC, player);
            RestoreDC(wizardDC, savedWizardDC);
        }
    }
    for (auto* archer : archers)
    {
        RECT rect = archer->GetRect();
        if (rect.right >= cameraX && rect.left <= cameraX + viewWidth &&
            rect.bottom >= cameraY && rect.top <= cameraY + viewHeight)
        {
            HDC archerDC = hdc;
            int savedArcherDC = SaveDC(archerDC);
            OffsetViewportOrgEx(archerDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            archer->Render(archerDC, player);
            RestoreDC(archerDC, savedArcherDC);
        }
    }
    for (auto* swordman : swordmans)
    {
        RECT rect = swordman->GetRect();
        if (rect.right >= cameraX && rect.left <= cameraX + viewWidth &&
            rect.bottom >= cameraY && rect.top <= cameraY + viewHeight)
        {
            HDC swordmanDC = hdc;
            int savedSwordmanDC = SaveDC(swordmanDC);
            OffsetViewportOrgEx(swordmanDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            swordman->Render(swordmanDC, player);
            RestoreDC(swordmanDC, savedSwordmanDC);
        }
    }
    // Render player
    {
        HDC playerDC = hdc;
        int savedPlayerDC = SaveDC(playerDC);
        OffsetViewportOrgEx(playerDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
        player.Render(playerDC);
        RestoreDC(playerDC, savedPlayerDC);
    }
    for (auto* arrow : arrows)
    {
        POINT* points = arrow->GetHitboxPoints();
        bool inView = false;
        for (int i = 0; i < 4; ++i) // Assuming 4 points for hitbox
        {
            if (points[i].x >= cameraX && points[i].x <= cameraX + viewWidth &&
                points[i].y >= cameraY && points[i].y <= cameraY + viewHeight)
            {
                inView = true;
                break;
            }
        }
        if (inView)
        {
            HDC arrowDC = hdc;
            int savedArrowDC = SaveDC(arrowDC);
            OffsetViewportOrgEx(arrowDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            arrow->Render(arrowDC);
            RestoreDC(arrowDC, savedArrowDC);
        }
    }
    for (auto* fireball : fireballs)
    {
        POINT* points = fireball->GetHitboxPoints();
        bool inView = false;
        for (int i = 0; i < 4; ++i) // Assuming 4 points for hitbox
        {
            if (points[i].x >= cameraX && points[i].x <= cameraX + viewWidth &&
                points[i].y >= cameraY && points[i].y <= cameraY + viewHeight)
            {
                inView = true;
                break;
            }
        }
        if (inView)
        {
            HDC fireballDC = hdc;
            int savedFireballDC = SaveDC(fireballDC);
            OffsetViewportOrgEx(fireballDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            fireball->Render(fireballDC);
            RestoreDC(fireballDC, savedFireballDC);
        }
    }

    RestoreDC(hdc, savedDC);
    // µð¹ö±ë ÅØ½ºÆ®: ÇÃ·¹ÀÌ¾î ÁÂÇ¥
    WCHAR playerPosText[100];
    wsprintf(playerPosText, L"ÇÃ·¹ÀÌ¾î ÁÂÇ¥: X = %d, Y = %d", static_cast<int>(player.GetPositionX()), static_cast<int>(player.GetPositionY()));
    TextOut(hdc, 0, 60, playerPosText, lstrlen(playerPosText));

    //// µð¹ö±ë ÅØ½ºÆ®: °Ë»ç ÁÂÇ¥
    //int swordmanIndex = 0;
    //for (auto* swordman : swordmans)
    //{
    //    WCHAR swordmanPosText[100];
    //    wsprintf(swordmanPosText, L"°Ë»ç %d ÁÂÇ¥: X = %d, Y = %d", swordmanIndex, static_cast<int>(swordman->GetPositionX()), static_cast<int>(swordman->GetPositionY()));
    //    TextOut(hdc, 0, 80 + swordmanIndex * 20, swordmanPosText, lstrlen(swordmanPosText));
    //    swordmanIndex++;
    //}

    //// µð¹ö±ë ÅØ½ºÆ®: ¸¶¹ý»ç ÁÂÇ¥
    //int wizardIndex = 0;
    //for (auto* wizard : wizards)
    //{
    //    WCHAR wizardPosText[100];
    //    wsprintf(wizardPosText, L"¸¶¹ý»ç %d ÁÂÇ¥: X = %d, Y = %d", wizardIndex, static_cast<int>(wizard->GetPositionX()), static_cast<int>(wizard->GetPositionY()));
    //    TextOut(hdc, 0, 100 + swordmanIndex * 20 + wizardIndex * 20, wizardPosText, lstrlen(wizardPosText));
    //    wizardIndex++;
    //}

    //// µð¹ö±ë ÅØ½ºÆ®: ±Ã¼ö ÁÂÇ¥
    //int archerIndex = 0;
    //for (auto* archer : archers)
    //{
    //    WCHAR archerPosText[100];
    //    wsprintf(archerPosText, L"±Ã¼ö %d ÁÂÇ¥: X = %d, Y = %d", archerIndex, static_cast<int>(archer->GetPositionX()), static_cast<int>(archer->GetPositionY()));
    //    TextOut(hdc, 0, 120 + swordmanIndex * 20 + wizardIndex * 20 + archerIndex * 20, archerPosText, lstrlen(archerPosText));
    //    archerIndex++;
    //}

    
    WCHAR mousePosText[100];
    float mouseWorldX = static_cast<float>(Input::GetMousePosition().x) + camera.GetPositionX();
    float mouseWorldY = static_cast<float>(Input::GetMousePosition().y) + camera.GetPositionY();
    wsprintf(mousePosText, L"¸¶¿ì½º ÁÂÇ¥: X = %d, Y = %d", static_cast<int>(mouseWorldX), static_cast<int>(mouseWorldY));
    TextOut(hdc, static_cast<int>(Input::GetMousePosition().x) + 10, static_cast<int>(Input::GetMousePosition().y), mousePosText, lstrlen(mousePosText));
}

void Stage1::HandleCollision()
{
    std::vector<GameObject*> allObjects;
    allObjects.push_back(&player);
    for (auto* swordman : swordmans) allObjects.push_back(swordman);
    for (auto* wizard : wizards) allObjects.push_back(wizard);
    for (auto* archer : archers) allObjects.push_back(archer);
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
    if (IntersectRect(&intersect, &rect1, &rect2))
    {
        int overlapX = min(rect1.right, rect2.right) - max(rect1.left, rect2.left);
        int overlapY = min(rect1.bottom, rect2.bottom) - max(rect1.top, rect2.top);
        if (overlapX < overlapY)
        {
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