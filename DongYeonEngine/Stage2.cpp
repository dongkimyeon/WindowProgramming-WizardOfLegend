#include "Stage2.h"
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


void Stage2::Initialize()
{
    //카메라 설정
    camera.SetTarget(SceneManager::GetSharedPlayer());
  
    //몬스터 추가
    archers.push_back(new Archer());
    archers.back()->SetPosition(1800, 1700);
    //포탈 위치 설정
    portal.SetPosition(1650, 1750);
}

void Stage2::LateUpdate()
{

}
void Stage2::Update()
{
    Player* player = SceneManager::GetSharedPlayer();
    player->Update(this);
    camera.Update();
    player->SetCameraX(camera.GetPositionX());
    player->SetCameraY(camera.GetPositionY());

    for (auto* archer : archers) archer->Update(*player, this);
    for (auto* wizard : wizards) wizard->Update(*player, this);
    for (auto* swordman : swordmans) swordman->Update(*player);
    //플레이어 발사체 움직임
    for (auto* fireDragon : playerFireDragon)
    {
        if (fireDragon->IsActive())
            fireDragon->Move();
    }
    for (auto* fireball : playerFireballs)
    {
        if (fireball->IsActive())
            fireball->Move();
    }

    // 발사체 벽 충돌 체크
    auto map = MapManager::GetInstance()->GetMap();
    if (map)
    {
        // Arrows
        for (auto it = arrows.begin(); it != arrows.end();)
        {
            if ((*it)->IsActive())
            {
                POINT* points = (*it)->GetHitboxPoints();
                // POINT[4]에서 RECT 생성
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
                // 벽 충돌
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
                                (*it)->SetActive(false);
                                collided = true;
                            }
                        }
                    }
                }

                // 적 충돌
                if (!collided)
                {
                    bool enemyCollided = false;
                    for (auto* swordman : swordmans)
                    {
                        if ((*it)->CheckCollision(*swordman))
                        {
                            enemyCollided = true;
                            break;
                        }
                    }
                    if (!enemyCollided)
                    {
                        for (auto* wizard : wizards)
                        {
                            if ((*it)->CheckCollision(*wizard))
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
                            if ((*it)->CheckCollision(*archer))
                            {
                                enemyCollided = true;
                                break;
                            }
                        }
                    }
                    if (!enemyCollided)
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
                it = playerFireballs.erase(it);
            }
        }

        // Player FireDragons
        for (auto it = playerFireDragon.begin(); it != playerFireDragon.end();)
        {
            if ((*it)->IsActive())
            {
                // 벽 충돌
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
                               
                                (*it)->SetActive(false);
                                collided = true;
                            }
                        }
                    }
                }

                // 적 충돌
                if (!collided)
                {
                    bool enemyCollided = false;
                    for (auto* swordman : swordmans)
                    {
                        if ((*it)->CheckCollision(*swordman))
                        {
                            enemyCollided = true;
                            break;
                        }
                    }
                    if (!enemyCollided)
                    {
                        for (auto* wizard : wizards)
                        {
                            if ((*it)->CheckCollision(*wizard))
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
                            if ((*it)->CheckCollision(*archer))
                            {
                                enemyCollided = true;
                                break;
                            }
                        }
                    }
                    if (!enemyCollided)
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
    if (IntersectRect(&temp, &playerRect, &portalRect) && Input::GetKeyDown(eKeyCode::F))
    {
        SceneManager::LoadScene(L"Stage1");
		MapManager::GetInstance()->LoadMap(L"Stage1.txt");
        SceneManager::GetSharedPlayer()->SetPosition(180, 270);
        SceneManager::GetSharedPlayer()->SetTelporting(true);
    }
    //객체간에 충돌처리 밀어내는거
    HandleCollision();

    // 플레이어 벽 충돌 처리
    if (map)
    {
        HandleCollisionMap(map, *player);
    }
}

void Stage2::Render(HDC hdc)
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

    
    //몇 스테이지인지 텍스트 출력
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    wchar_t StageIdText[20];
    swprintf_s(StageIdText, L"Stage2");

 
    SIZE textSize;
    GetTextExtentPoint32(hdc, StageIdText, wcslen(StageIdText), &textSize);

    int textX = viewWidth - textSize.cx; 
    int textY = viewHeight - textSize.cy; 

    TextOut(hdc, textX - 20, textY, StageIdText, wcslen(StageIdText));

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}


void Stage2::HandleCollision()
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

void Stage2::ResolveCollision(GameObject& obj1, GameObject& obj2)
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

void Stage2::HandleCollisionMap(int (*map)[40], GameObject& obj)
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

                    ResolveCollisionMap(wallRect, *player);
                }
                for (auto* swordman : swordmans)
                {
                    RECT enemyRect = swordman->GetRect();
                    if (IntersectRect(&intersect, &wallRect, &enemyRect))
                    {

                        ResolveCollisionMap(wallRect, *swordman);
                    }
                }
                for (auto* wizard : wizards)
                {
                    RECT enemyRect = wizard->GetRect();
                    if (IntersectRect(&intersect, &wallRect, &enemyRect))
                    {

                        ResolveCollisionMap(wallRect, *wizard);
                    }
                }
                for (auto* archer : archers)
                {
                    RECT enemyRect = archer->GetRect();
                    if (IntersectRect(&intersect, &wallRect, &enemyRect))
                    {

                        ResolveCollisionMap(wallRect, *archer);
                    }
                }

            }
        }
    }
}

void Stage2::ResolveCollisionMap(RECT wallRect, GameObject& obj)
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