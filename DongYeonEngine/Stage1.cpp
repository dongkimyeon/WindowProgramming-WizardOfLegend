#include "Stage1.h"
#include "Input.h"
#include "Time.h"
#include "Archer.h"
#include "Arrow.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "UI.h"


#define MAP_COLS 40
#define MAP_ROWS 40
#define TILE_SIZE 50

Stage1::Stage1()
{
    camera.SetTarget(SceneManager::GetSharedPlayer());
   
    SceneManager::GetSharedPlayer()->SetPosition(1000, 1200);


    //middle room
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(800, 900);
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(950, 900);
    wizards.push_back(new Wizard());
    wizards.back()->SetPosition(1100, 900);
    archers.push_back(new Archer());
    archers.back()->SetPosition(1250, 900);

    // middle under room
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(800, 1700);
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(1200, 1700);
    wizards.push_back(new Wizard());
    wizards.back()->SetPosition(1000, 1700);

    //middle Up room
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(800, 400);
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(1200, 400);
    archers.push_back(new Archer());
    archers.back()->SetPosition(1000, 200);

    //Left Long room
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(500, 400);
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(500, 600);
    archers.push_back(new Archer());
    archers.back()->SetPosition(250, 500);
    wizards.push_back(new Wizard());
    wizards.back()->SetPosition(350, 1000);
    wizards.push_back(new Wizard());
    wizards.back()->SetPosition(250, 1400);
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(500, 1500);
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(350, 1600);
    
    //right up room
	portal.SetPosition(1700, 450);
}

Stage1::~Stage1()
{
    for (auto* swordman : swordmans) delete swordman;
    for (auto* wizard : wizards) delete wizard;
    for (auto* archer : archers) delete archer;
    for (auto* arrow : arrows) delete arrow;
    for (auto* fireball : fireballs) delete fireball;
    for (auto* playerFireball : playerFireballs) delete playerFireball;
    for (auto* fireDragon : playerFireDragon) delete fireDragon;
}

void Stage1::Initialize()
{
	UI::Initialize();
    MapManager::GetInstance()->Initialize();
    camera.Update();
}

void Stage1::Update()
{
    Player* player = SceneManager::GetSharedPlayer();
    player->Update(this);
    camera.Update();
    player->SetCameraX(camera.GetPositionX());
    player->SetCameraY(camera.GetPositionY());

    for (auto* archer : archers) archer->Update(*player, this);
    for (auto* wizard : wizards) wizard->Update(*player, this);
    for (auto* swordman : swordmans) swordman->Update(*player);

    // �߻�ü �� �浹 üũ
    auto map = MapManager::GetInstance()->GetMap();
    if (map)
    {
        // Arrows
        for (auto it = arrows.begin(); it != arrows.end();)
        {
            if ((*it)->IsActive())
            {
                POINT* points = (*it)->GetHitboxPoints();
                // POINT[4]���� RECT ����
                LONG minX = points[0].x, maxX = points[0].x, minY = points[0].y, maxY = points[0].y;
                for (int k = 1; k < 4; ++k)
                {
                    minX = min(minX, points[k].x);
                    maxX = max(maxX, points[k].x);
                    minY = min(minY, points[k].y);
                    maxY = max(maxY, points[k].y);
                }
                RECT projectileRect = { minX, minY, maxX, maxY };
                bool collided = false;
                for (int i = 0; i < MAP_ROWS && !collided; ++i)
                {
                    for (int j = 0; j < MAP_COLS && !collided; ++j)
                    {
                        if (map[i][j] == 1)
                        {
                            RECT wallRect = {
                                j * TILE_SIZE,
                                i * TILE_SIZE,
                                (j + 1) * TILE_SIZE,
                                (i + 1) * TILE_SIZE
                            };
                            RECT intersect;
                            if (IntersectRect(&intersect, &wallRect, &projectileRect))
                            {
								printf("Arrow collided with wall at tile [%d, %d] - Wall RECT: (%ld, %ld, %ld, %ld), Arrow Points: [(%ld,%ld), (%ld,%ld), (%ld,%ld), (%ld,%ld)]\n",
									i, j,
									wallRect.left, wallRect.top, wallRect.right, wallRect.bottom,
									points[0].x, points[0].y, points[1].x, points[1].y,
									points[2].x, points[2].y, points[3].x, points[3].y);
								(*it)->SetActive(false);
                                collided = true;
                            }
                        }
                    }
                }
                if (!collided)
                {
                    (*it)->Update(*player);
                    ++it;
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                delete* it;
                it = arrows.erase(it);
            }
        }

        // Fireballs
        for (auto it = fireballs.begin(); it != fireballs.end();)
        {
            if ((*it)->IsActive())
            {
                POINT* points = (*it)->GetHitboxPoints();
                LONG minX = points[0].x, maxX = points[0].x, minY = points[0].y, maxY = points[0].y;
                for (int k = 1; k < 4; ++k)
                {
                    minX = min(minX, points[k].x);
                    maxX = max(maxX, points[k].x);
                    minY = min(minY, points[k].y);
                    maxY = max(maxY, points[k].y);
                }
                RECT projectileRect = { minX, minY, maxX, maxY };
                bool collided = false;
                for (int i = 0; i < MAP_ROWS && !collided; ++i)
                {
                    for (int j = 0; j < MAP_COLS && !collided; ++j)
                    {
                        if (map[i][j] == 1)
                        {
                            RECT wallRect = {
                                j * TILE_SIZE,
                                i * TILE_SIZE,
                                (j + 1) * TILE_SIZE,
                                (i + 1) * TILE_SIZE
                            };
                            RECT intersect;
                            if (IntersectRect(&intersect, &wallRect, &projectileRect))
                            {
                                printf("Fireball collided with wall at tile [%d, %d] - Wall RECT: (%ld, %ld, %ld, %ld), Fireball Points: [(%ld,%ld), (%ld,%ld), (%ld,%ld), (%ld,%ld)]\n",
                                    i, j,
                                    wallRect.left, wallRect.top, wallRect.right, wallRect.bottom,
                                    points[0].x, points[0].y, points[1].x, points[1].y,
                                    points[2].x, points[2].y, points[3].x, points[3].y);
                                (*it)->SetActive(false);
                                collided = true;
                            }
                        }
                    }
                }
                if (!collided)
                {
                    (*it)->Update(*player);
                    ++it;
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                delete* it;
                it = fireballs.erase(it);
            }
        }

        // Player Fireballs
        for (auto it = playerFireballs.begin(); it != playerFireballs.end();)
        {
            if ((*it)->IsActive())
            {
                POINT* points = (*it)->GetHitboxPoints();
                LONG minX = points[0].x, maxX = points[0].x, minY = points[0].y, maxY = points[0].y;
                for (int k = 1; k < 4; ++k)
                {
                    minX = min(minX, points[k].x);
                    maxX = max(maxX, points[k].x);
                    minY = min(minY, points[k].y);
                    maxY = max(maxY, points[k].y);
                }
                RECT projectileRect = { minX, minY, maxX, maxY };
                bool collided = false;
                for (int i = 0; i < MAP_ROWS && !collided; ++i)
                {
                    for (int j = 0; j < MAP_COLS && !collided; ++j)
                    {
                        if (map[i][j] == 1)
                        {
                            RECT wallRect = {
                                j * TILE_SIZE,
                                i * TILE_SIZE,
                                (j + 1) * TILE_SIZE,
                                (i + 1) * TILE_SIZE
                            };
                            RECT intersect;
                            if (IntersectRect(&intersect, &wallRect, &projectileRect))
                            {
                                printf("PlayerFireball collided with wall at tile [%d, %d] - Wall RECT: (%ld, %ld, %ld, %ld), PlayerFireball Points: [(%ld,%ld), (%ld,%ld), (%ld,%ld), (%ld,%ld)]\n",
                                    i, j,
                                    wallRect.left, wallRect.top, wallRect.right, wallRect.bottom,
                                    points[0].x, points[0].y, points[1].x, points[1].y,
                                    points[2].x, points[2].y, points[3].x, points[3].y);
                                (*it)->SetActive(false);
                                collided = true;
                            }
                        }
                    }
                }
                if (!collided)
                {
                    bool enemyCollided = false;
                    for (auto* swordman : swordmans)
                    {
                        (*it)->Update(*swordman);
                        if (!(*it)->IsActive())
                        {
                            enemyCollided = true;
                            break;
                        }
                    }
                    if (!enemyCollided)
                    {
                        for (auto* wizard : wizards)
                        {
                            (*it)->Update(*wizard);
                            if (!(*it)->IsActive())
                            {
                                enemyCollided = true;
                                break;
                            }
                        }
                    }
                    if (!enemyCollided)
                    {
                        for (auto* archer : archers)
                        {
                            (*it)->Update(*archer);
                            if (!(*it)->IsActive())
                            {
                                enemyCollided = true;
                                break;
                            }
                        }
                    }
                    if (!(*it)->IsActive())
                    {
                        ++it;
                    }
                    else
                    {
                        ++it;
                    }
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                delete* it;
                it = playerFireballs.erase(it);
            }
        }

        // Player FireDragons
        for (auto it = playerFireDragon.begin(); it != playerFireDragon.end();)
        {
            if ((*it)->IsActive())
            {
                POINT* points = (*it)->GetHitboxPoints();
                LONG minX = points[0].x, maxX = points[0].x, minY = points[0].y, maxY = points[0].y;
                for (int k = 1; k < 4; ++k)
                {
                    minX = min(minX, points[k].x);
                    maxX = max(maxX, points[k].x);
                    minY = min(minY, points[k].y);
                    maxY = max(maxY, points[k].y);
                }
                RECT projectileRect = { minX, minY, maxX, maxY };
                bool collided = false;
                for (int i = 0; i < MAP_ROWS && !collided; ++i)
                {
                    for (int j = 0; j < MAP_COLS && !collided; ++j)
                    {
                        if (map[i][j] == 1)
                        {
                            RECT wallRect = {
                                j * TILE_SIZE,
                                i * TILE_SIZE,
                                (j + 1) * TILE_SIZE,
                                (i + 1) * TILE_SIZE
                            };
                            RECT intersect;
                            if (IntersectRect(&intersect, &wallRect, &projectileRect))
                            {
                                printf("FireDragon collided with wall at tile [%d, %d] - Wall RECT: (%ld, %ld, %ld, %ld), FireDragon Points: [(%ld,%ld), (%ld,%ld), (%ld,%ld), (%ld,%ld)]\n",
                                    i, j,
                                    wallRect.left, wallRect.top, wallRect.right, wallRect.bottom,
                                    points[0].x, points[0].y, points[1].x, points[1].y,
                                    points[2].x, points[2].y, points[3].x, points[3].y);
                                (*it)->SetActive(false);
                                collided = true;
                            }
                        }
                    }
                }
                if (!collided)
                {
                    bool enemyCollided = false;
                    for (auto* swordman : swordmans)
                    {
                        (*it)->Update(*swordman);
                        if (!(*it)->IsActive())
                        {
                            enemyCollided = true;
                            break;
                        }
                    }
                    if (!enemyCollided)
                    {
                        for (auto* wizard : wizards)
                        {
                            (*it)->Update(*wizard);
                            if (!(*it)->IsActive())
                            {
                                enemyCollided = true;
                                break;
                            }
                        }
                    }
                    if (!enemyCollided)
                    {
                        for (auto* archer : archers)
                        {
                            (*it)->Update(*archer);
                            if (!(*it)->IsActive())
                            {
                                enemyCollided = true;
                                break;
                            }
                        }
                    }
                    if (!(*it)->IsActive())
                    {
                        ++it;
                    }
                    else
                    {
                        ++it;
                    }
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                delete* it;
                it = playerFireDragon.erase(it);
            }
        }
    }

    POINT effectHitboxPoints[4];
    bool hasEffectHitbox = player->GetEffectHitbox(effectHitboxPoints);
    if (hasEffectHitbox)
    {
        for (auto* swordman : swordmans)
        {
            RECT enemyRect = swordman->GetRect();
            if (player->CheckCollisionWithRect(enemyRect) && !swordman->HasBeenHit())
            {
                swordman->TakeDamage(player->GetDamage());
                swordman->SetHitFlag(true);
            }
        }
        for (auto* wizard : wizards)
        {
            RECT enemyRect = wizard->GetRect();
            if (player->CheckCollisionWithRect(enemyRect) && !wizard->HasBeenHit())
            {
                wizard->TakeDamage(player->GetDamage());
                wizard->SetHitFlag(true);
            }
        }
        for (auto* archer : archers)
        {
            RECT enemyRect = archer->GetRect();
            if (player->CheckCollisionWithRect(enemyRect) && !archer->HasBeenHit())
            {
                archer->TakeDamage(player->GetDamage());
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

    RECT temp;
	RECT playerRect = player->GetRect();
	RECT portalRect = portal.GetRect();
    if(IntersectRect(&temp, &playerRect, &portalRect) && Input::GetKeyDown(eKeyCode::F))
    {
		SceneManager::LoadScene(L"Stage2");
		SceneManager::GetSharedPlayer()->SetPosition(1200, 1200);
	}
    //��ü���� �浹ó�� �о�°�
	HandleCollision();

    // �÷��̾� �� �浹 ó��
    if (map)
    {
        HandleCollisionMap(map, *player);
    }
}
void Stage1::LateUpdate()
{
    Scene::LateUpdate();
}

void Stage1::Render(HDC hdc)
{
    Player* player = SceneManager::GetSharedPlayer();
    int savedDC = SaveDC(hdc);
    float cameraX = camera.GetPositionX();
    float cameraY = camera.GetPositionY();
    int viewWidth = 1280;
    int viewHeight = 720;

    MapManager::GetInstance()->Render(hdc, cameraX, cameraY);

    HDC portalDC = hdc;
    int savedPortalDC = SaveDC(portalDC);
    OffsetViewportOrgEx(portalDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
    portal.Render(portalDC);
    RestoreDC(portalDC, savedPortalDC);
  

    for (auto* wizard : wizards)
    {
        RECT rect = wizard->GetRect();
        if (rect.right >= cameraX && rect.left <= cameraX + viewWidth &&
            rect.bottom >= cameraY && rect.top <= cameraY + viewHeight)
        {
            HDC wizardDC = hdc;
            int savedWizardDC = SaveDC(wizardDC);
            OffsetViewportOrgEx(wizardDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            wizard->Render(wizardDC, *player);
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
            archer->Render(archerDC, *player);
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
            swordman->Render(swordmanDC, *player);
            RestoreDC(swordmanDC, savedSwordmanDC);
        }
    }
    
    for (auto* arrow : arrows)
    {
        POINT* points = arrow->GetHitboxPoints();
        bool inView = false;
        for (int i = 0; i < 4; ++i)
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
        for (int i = 0; i < 4; ++i)
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
    for (auto* playerFireBall : playerFireballs)
    {
        POINT* points = playerFireBall->GetHitboxPoints();
        bool inView = false;
        for (int i = 0; i < 4; ++i)
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
            playerFireBall->Render(fireballDC);
            RestoreDC(fireballDC, savedFireballDC);
        }
    }
    for (auto* FireDragon : playerFireDragon)
    {
        POINT* points = FireDragon->GetHitboxPoints();
        bool inView = false;
        for (int i = 0; i < 4; ++i)
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
            HDC FireDragonDC = hdc;
            int saveFireDragonDC = SaveDC(FireDragonDC);
            OffsetViewportOrgEx(FireDragonDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            FireDragon->Render(FireDragonDC);
            RestoreDC(FireDragonDC, saveFireDragonDC);
        }
    }

    HDC playerDC = hdc;
    int savedPlayerDC = SaveDC(playerDC);
    OffsetViewportOrgEx(playerDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
    player->Render(playerDC);
    RestoreDC(playerDC, savedPlayerDC);

    RestoreDC(hdc, savedDC);

	UI::Render(hdc);


   WCHAR playerPosText[100];
    wsprintf(playerPosText, L"�÷��̾� ��ǥ: X = %d, Y = %d",
        static_cast<int>(player->GetPositionX()), static_cast<int>(player->GetPositionY()));
    TextOut(hdc, 0, 60, playerPosText, lstrlen(playerPosText));

    WCHAR mousePosText[100];
    float mouseWorldX = static_cast<float>(Input::GetMousePosition().x) + camera.GetPositionX();
    float mouseWorldY = static_cast<float>(Input::GetMousePosition().y) + camera.GetPositionY();
    wsprintf(mousePosText, L"���콺 ��ǥ: X = %d, Y = %d",
        static_cast<int>(mouseWorldX), static_cast<int>(mouseWorldY));
    TextOut(hdc, static_cast<int>(Input::GetMousePosition().x) + 10,
        static_cast<int>(Input::GetMousePosition().y), mousePosText, lstrlen(mousePosText));

    /*int arrowTextOffsetY = 80;
    int arrowIndex = 0;
    for (const auto* arrow : arrows)
    {
        if (arrow->IsActive())
        {
            WCHAR arrowPosText[100];
            wsprintf(arrowPosText, L"ȭ�� %d ��ǥ: X = %d, Y = %d",
                arrowIndex, static_cast<int>(arrow->GetPositionX()),
                static_cast<int>(arrow->GetPositionY()));
            TextOut(hdc, 0, arrowTextOffsetY, arrowPosText, lstrlen(arrowPosText));
            arrowTextOffsetY += 20;
            ++arrowIndex;
        }
    }*/
}



void Stage1::HandleCollision()
{
    std::vector<GameObject*> allObjects;
    allObjects.push_back(SceneManager::GetSharedPlayer());
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
        float overlapX = static_cast<float>(min(rect1.right, rect2.right) - max(rect1.left, rect2.left));
        float overlapY = static_cast<float>(min(rect1.bottom, rect2.bottom) - max(rect1.top, rect2.top));
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

void Stage1::HandleCollisionMap(int (*map)[40], GameObject& obj)
{
    Player* player = SceneManager::GetSharedPlayer();
    if (&obj != player) return;

    for (int i = 0; i < MAP_ROWS; ++i)
    {
        for (int j = 0; j < MAP_COLS; ++j)
        {
            if (map[i][j] == 1)
            {
                RECT wallRect = {
                    j * TILE_SIZE,
                    i * TILE_SIZE,
                    (j + 1) * TILE_SIZE,
                    (i + 1) * TILE_SIZE
                };
                RECT playerRect = player->GetRect();
                RECT intersect;
                if (IntersectRect(&intersect, &wallRect, &playerRect))
                {
                    printf("Player collided with wall at tile [%d, %d] - Wall RECT: (%ld, %ld, %ld, %ld), Player RECT: (%ld, %ld, %ld, %ld)\n",
                        i, j,
                        wallRect.left, wallRect.top, wallRect.right, wallRect.bottom,
                        playerRect.left, playerRect.top, playerRect.right, playerRect.bottom);
                    ResolveCollisionMap(wallRect, *player);
                }
                for (auto* swordman : swordmans)
                {
                    RECT enemyRect = swordman->GetRect();
                    if (IntersectRect(&intersect, &wallRect, &enemyRect))
                    {
                        printf("Swordman collided with wall at tile [%d, %d] - Wall RECT: (%ld, %ld, %ld, %ld), Swordman RECT: (%ld, %ld, %ld, %ld)\n",
                            i, j,
                            wallRect.left, wallRect.top, wallRect.right, wallRect.bottom,
                            enemyRect.left, enemyRect.top, enemyRect.right, enemyRect.bottom);
                        ResolveCollisionMap(wallRect, *swordman);
                    }
                }
                for (auto* wizard : wizards)
                {
                    RECT enemyRect = wizard->GetRect();
                    if (IntersectRect(&intersect, &wallRect, &enemyRect))
                    {
                        printf("Wizard collided with wall at tile [%d, %d] - Wall RECT: (%ld, %ld, %ld, %ld), Wizard RECT: (%ld, %ld, %ld, %ld)\n",
                            i, j,
                            wallRect.left, wallRect.top, wallRect.right, wallRect.bottom,
                            enemyRect.left, enemyRect.top, enemyRect.right, enemyRect.bottom);
                        ResolveCollisionMap(wallRect, *wizard);
                    }
				}
                for (auto* archer : archers)
                {
                    RECT enemyRect = archer->GetRect();
                    if (IntersectRect(&intersect, &wallRect, &enemyRect))
                    {
                        printf("Archer collided with wall at tile [%d, %d] - Wall RECT: (%ld, %ld, %ld, %ld), Archer RECT: (%ld, %ld, %ld, %ld)\n",
                            i, j,
                            wallRect.left, wallRect.top, wallRect.right, wallRect.bottom,
                            enemyRect.left, enemyRect.top, enemyRect.right, enemyRect.bottom);
                        ResolveCollisionMap(wallRect, *archer);
                    }
                }

            }
        }       
    }
}

void Stage1::ResolveCollisionMap(RECT wallRect, GameObject& obj)
{
    RECT objRect = obj.GetRect();
    RECT intersect;

    if (IntersectRect(&intersect, &wallRect, &objRect))
    {
        float overlapX = static_cast<float>(min(wallRect.right, objRect.right) - max(wallRect.left, objRect.left));
        float overlapY = static_cast<float>(min(wallRect.bottom, objRect.bottom) - max(wallRect.top, objRect.top));

        if (overlapX < overlapY)
        {
            if (objRect.left < wallRect.left)
            {
                obj.SetPosition(obj.GetPositionX() - overlapX, obj.GetPositionY());
            }
            else
            {
                obj.SetPosition(obj.GetPositionX() + overlapX, obj.GetPositionY());
            }
        }
        else
        {
            if (objRect.top < wallRect.top)
            {
                obj.SetPosition(obj.GetPositionX(), obj.GetPositionY() - overlapY);
            }
            else
            {
                obj.SetPosition(obj.GetPositionX(), obj.GetPositionY() + overlapY);
            }
        }
    }
}