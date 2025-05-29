#include "TutorialStage.h"
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


void TutorialStage::Initialize()
{
    //ī�޶� ����
    camera.SetTarget(SceneManager::GetSharedPlayer());

    //�÷��̾� ��ġ �ʱ�ȭ
    SceneManager::GetSharedPlayer()->SetPosition(1000, 1000);

   

    //���� �߰�
	dummies.push_back(new Dummy());
	dummies.back()->SetPosition(1000, 760);

    //Ʃ�丮�� �޼���ť
    tutorialQue.push(L"���۾ȳ�");

    tutorialQue.push(L"�����̱�");

    tutorialQue.push(L"�뽬");
    //��ó��ȯ
    tutorialQue.push(L"�⺻���� ����");
    //���� ��ȯ
    tutorialQue.push(L"��ų: ���̾ ����");
    //���� ��ȯ
    tutorialQue.push(L"��ų: ���̾�巡�� ����");
    //���ͼ�ȯ
    tutorialQue.push(L"���� �ȳ�");

}

void TutorialStage::LateUpdate()
{
}
void TutorialStage::Update()
{
    Player* player = SceneManager::GetSharedPlayer();
    player->Update(this);
    camera.Update();
    player->SetCameraX(camera.GetPositionX());
    player->SetCameraY(camera.GetPositionY());
    //��Ÿ ������Ʈ ����
    {
    for (auto* archer : archers) archer->Update(*player, this);
    for (auto* wizard : wizards) wizard->Update(*player, this);
    for (auto* swordman : swordmans) swordman->Update(*player);
    //�÷��̾� �߻�ü ������
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
                // �� �浹
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

                // �� �浹
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
                    {
                        for (auto* dummy : dummies)
                        {
                            if ((*it)->CheckCollision(*dummy))
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
                // �� �浹
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

                // �� �浹
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
                    {
                        for (auto* dummy : dummies)
                        {
                            if ((*it)->CheckCollision(*dummy))
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
        for (auto* dummy : dummies)
        {
            RECT enemyRect = dummy->GetRect();
            if (player->CheckCollisionWithRect(enemyRect) && !dummy->HasBeenHit())
            {
                dummy->TakeDamage(player->GetDamage());
                dummy->SetHitFlag(true);
            }
		}
    }
    else
    {
        for (auto* swordman : swordmans) swordman->SetHitFlag(false);
        for (auto* wizard : wizards) wizard->SetHitFlag(false);
        for (auto* archer : archers) archer->SetHitFlag(false);
		for (auto* dummy : dummies) dummy->SetHitFlag(false);
    }

    //��Ż �Ѿ �� �������ִ°�
    RECT temp;
    RECT playerRect = player->GetRect();
    RECT portalRect = portal.GetRect();
    if (IntersectRect(&temp, &playerRect, &portalRect) && Input::GetKeyDown(eKeyCode::F))
    {
        SceneManager::LoadScene(L"TitleScene");
		
    }
    //��ü���� �浹ó�� �о�°�
    HandleCollision();
    // �÷��̾� �� �浹 ó��
    if (map)
    {
        HandleCollisionMap(map, *player);
    }
    }


    // Ʃ�丮�� ����
    isStepCompleted = false;
    currentTutorialStep = L"";

    if (currentTutorialStep.empty() && !tutorialQue.empty())
    {
        currentTutorialStep = tutorialQue.front();
        
        isStepCompleted = false;
    }

    if (!currentTutorialStep.empty())
    {
        if (currentTutorialStep == L"���۾ȳ�")
        {
            // �÷��̾�� Ʃ�丮�� ���� �޽��� ǥ�� (UI�� ������)
            if (Input::GetKeyDown(eKeyCode::P)) // PŰ �Է�Ȯ��
            {
                isStepCompleted = true;
               
            }
        }
        else if (currentTutorialStep == L"�����̱�")
        {
            // WASD Ű�� �����̴��� Ȯ��
            if (Input::GetKey(eKeyCode::W) || Input::GetKey(eKeyCode::A) ||
                Input::GetKey(eKeyCode::S) || Input::GetKey(eKeyCode::D))
            {
                isStepCompleted = true;
              
            }
        }
        else if (currentTutorialStep == L"�뽬")
        {
            // �뽬 Ű (��: Shift) �Է� Ȯ��
            if (Input::GetKeyDown(eKeyCode::SPACE))
            {
                isStepCompleted = true;
                
            }
        }
        else if (currentTutorialStep == L"�⺻���� ����")
        {
            // ��ó ��ȯ
            if (archers.empty())
            {
                archers.push_back(new Archer());
                archers.back()->SetPosition(1000, 600);
            }
            // �⺻ ���� (��: ���콺 ���� Ŭ��)���� ��ó ���� Ȯ��
            if (Input::GetKeyDown(eKeyCode::LButton))
            {
                for (auto* archer : archers)
                {
                    if (archer->GetIsDead())
                    {
                        isStepCompleted = true;
                        break;
                    }
                }
            }
        }
        else if (currentTutorialStep == L"��ų: ���̾ ����")
        {
            // ���� ��ȯ
            if (swordmans.empty())
            {
                swordmans.push_back(new SwordMan());
                swordmans.back()->SetPosition(1000, 600);
            }
            // ���̾ ��ų ��� Ȯ��
            if (Input::GetKeyDown(eKeyCode::Q)) // ��: Q Ű�� ���̾
            {
                for (auto* swordMan : swordmans)
                {
                    if (swordMan->GetIsDead())
                    {
                        isStepCompleted = true;
                        break;
                    }
                }

            }
        }
        else if (currentTutorialStep == L"��ų: ���̾�巡�� ����")
        {
            // ���� ��ȯ
            if (wizards.empty())
            {
                wizards.push_back(new Wizard());
                wizards.back()->SetPosition(1000, 600);
            }
            // ���̾�巡�� ��ų ��� Ȯ��
            if (Input::GetKeyDown(eKeyCode::E)) // ��: E Ű�� ���̾�巡��
            {
                for (auto* wizard : wizards)
                {
                    if (wizard->GetIsDead())
                    {
                        isStepCompleted = true;
                        break;
                    }
                }
            }
        }
        else if (currentTutorialStep == L"���� �ȳ�")
        {
            //��Ż ��ġ ����
            portal.SetPosition(1000, 330);
            // ��Ż ��ó�� �̵� �� F Ű �Է� �ȳ�
            RECT playerRect = SceneManager::GetSharedPlayer()->GetRect();
            RECT portalRect = portal.GetRect();
            RECT temp;
            wizards.clear();
            swordmans.clear();
            archers.clear();
            dummies.clear();
            if (IntersectRect(&temp, &playerRect, &portalRect) && Input::GetKeyDown(eKeyCode::F))
            {
                isStepCompleted = true;
                SceneManager::LoadScene(L"TitleScene");
            }

        }

        // �ܰ� �Ϸ� �� ���� �ܰ��
        if (isStepCompleted && !tutorialQue.empty())
        {
            currentTutorialStep = tutorialQue.front();
            tutorialQue.pop();
            isStepCompleted = false;
        }
        else if (isStepCompleted && tutorialQue.empty())
        {
            currentTutorialStep = L"";
        }
    }

}

void TutorialStage::Render(HDC hdc)
{
    Player* player = SceneManager::GetSharedPlayer();
    int savedDC = SaveDC(hdc);
    float cameraX = camera.GetPositionX();
    float cameraY = camera.GetPositionY();
    int viewWidth = 1280;
    int viewHeight = 720;
    //��Ÿ ����

    {
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
    for(auto* dummy : dummies)
    {
        RECT rect = dummy->GetRect();
        if (rect.right >= cameraX && rect.left <= cameraX + viewWidth &&
            rect.bottom >= cameraY && rect.top <= cameraY + viewHeight)
        {
            HDC dummyDC = hdc;
            int savedDummyDC = SaveDC(dummyDC);
            OffsetViewportOrgEx(dummyDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            dummy->Render(dummyDC, *player);
            RestoreDC(dummyDC, savedDummyDC);
        }
	}
    HDC playerDC = hdc;
    int savedPlayerDC = SaveDC(playerDC);
    OffsetViewportOrgEx(playerDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
    player->Render(playerDC);
    RestoreDC(playerDC, savedPlayerDC);

    RestoreDC(hdc, savedDC);

    UI::Render(hdc);

    //�� ������������ �ؽ�Ʈ ���
    //�ؽ�Ʈ ���� 
    SetTextColor(hdc, RGB(255, 255, 255));
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    wchar_t StageIdText[20];
    swprintf_s(StageIdText, L"TutorialStage");


    SIZE textSize;
    GetTextExtentPoint32(hdc, StageIdText, wcslen(StageIdText), &textSize);

    int textX = viewWidth - textSize.cx;
    int textY = viewHeight - textSize.cy;

    TextOut(hdc, textX - 20, textY, StageIdText, wcslen(StageIdText));

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
  
    WCHAR mousePosText[100];
    float mouseWorldX = static_cast<float>(Input::GetMousePosition().x) + camera.GetPositionX();
    float mouseWorldY = static_cast<float>(Input::GetMousePosition().y) + camera.GetPositionY();
    wsprintf(mousePosText, L"���콺 ��ǥ: X = %d, Y = %d",
        static_cast<int>(mouseWorldX), static_cast<int>(mouseWorldY));
    TextOut(hdc, static_cast<int>(Input::GetMousePosition().x) + 10,
        static_cast<int>(Input::GetMousePosition().y), mousePosText, lstrlen(mousePosText));
    }

    if (!currentTutorialStep.empty())
    {
        Gdiplus::Graphics graphics(hdc);
        graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
        Gdiplus::FontFamily fontFamily(L"EXO 2");
        Gdiplus::Font font(&fontFamily, 50, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
        Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255)); // ���� �� ���� ����

        // ī�޶� ��ȯ ��� ����
        Gdiplus::Matrix transform;
        transform.Translate(-cameraX, -cameraY);
        graphics.SetTransform(&transform);

        // �÷��̾� ��ġ �������� �ؽ�Ʈ ��ġ ����
        float baseTextX = player->GetPositionX();
        float textY = player->GetRect().top - 209.0f; // �÷��̾� ���� 50�ȼ�

        // �ؽ�Ʈ ���� ������ ���� �Լ�
        auto renderTextWithOffset = [&](const wchar_t* text) {
            Gdiplus::RectF layoutRect(0, 0, 0, 0);
            Gdiplus::RectF boundingBox;
            graphics.MeasureString(text, -1, &font, layoutRect, &boundingBox);
            float textWidth = boundingBox.Width;
            float textX = baseTextX - (textWidth / 2.0f); // �ؽ�Ʈ ������ ���ݸ�ŭ �������� �̵�
            graphics.DrawString(text, -1, &font, Gdiplus::PointF(textX, textY), &brush);
            };

        if (currentTutorialStep == L"���۾ȳ�")
        {
            renderTextWithOffset(L"Press P to Start");
        }
        else if (currentTutorialStep == L"�����̱�")
        {
            renderTextWithOffset(L"Use WASD to Move");
        }
        else if (currentTutorialStep == L"�뽬")
        {
            renderTextWithOffset(L"Press Space to Dash");
        }
        else if (currentTutorialStep == L"�⺻���� ����")
        {
            renderTextWithOffset(L"Click LMB to Attack");
        }
        else if (currentTutorialStep == L"��ų: ���̾ ����")
        {
            renderTextWithOffset(L"Press Q for Fireball");
        }
        else if (currentTutorialStep == L"��ų: ���̾�巡�� ����")
        {
            renderTextWithOffset(L"Press E for FireDragon");
        }
        else if (currentTutorialStep == L"���� �ȳ�")
        {
            renderTextWithOffset(L"Press F at Portal");
        }

        // ��ȯ ��� �ʱ�ȭ
        graphics.ResetTransform();
    }
     
    
}


void TutorialStage::HandleCollision()
{
    std::vector<GameObject*> allObjects;
    allObjects.push_back(SceneManager::GetSharedPlayer());
    for (auto* swordman : swordmans) allObjects.push_back(swordman);
    for (auto* wizard : wizards) allObjects.push_back(wizard);
    for (auto* archer : archers) allObjects.push_back(archer);
	for (auto* dummies : dummies) allObjects.push_back(dummies);
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

void TutorialStage::ResolveCollision(GameObject& obj1, GameObject& obj2)
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

void TutorialStage::HandleCollisionMap(int (*map)[40], GameObject& obj)
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
            }
        }
    }
}

void TutorialStage::ResolveCollisionMap(RECT wallRect, GameObject& obj)
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