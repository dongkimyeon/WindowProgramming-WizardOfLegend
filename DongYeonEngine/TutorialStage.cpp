#include "TutorialStage.h"
#include "Input.h"
#include "Time.h"
#include "Archer.h"
#include "Arrow.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "SoundManager.h"
#include "UI.h"

#define MAP_COLS 40
#define MAP_ROWS 40
#define TILE_SIZE 50


void TutorialStage::ObjectDestroy() 
{
    // Release Wizards
    for (auto* wizard : wizards)
    {
        delete wizard;
    }
    wizards.clear();

    // Release SwordMans
    for (auto* swordman : swordmans)
    {
        delete swordman;
    }
    swordmans.clear();

    // Release Archers
    for (auto* archer : archers)
    {
        delete archer;
    }
    archers.clear();

    // Release Arrows
    for (auto* arrow : arrows)
    {
        delete arrow;
    }
    arrows.clear();

    // Release FireBalls
    for (auto* fireball : fireballs)
    {
        delete fireball;
    }
    fireballs.clear();

    // Release Player FireBalls
    for (auto* playerFireball : playerFireballs)
    {
        delete playerFireball;
    }
    playerFireballs.clear();

    // Release Player FireDragons
    for (auto* fireDragon : playerFireDragon)
    {
        delete fireDragon;
    }
    playerFireDragon.clear();
    // Release IceBigChunks
    for (auto* iceBigChunk : mIceBigChunk)
    {
        delete iceBigChunk;
    }
    mIceBigChunk.clear();

    // Release IceSmallChunks
    for (auto* iceSmallChunk : mIceSmallChunk)
    {
        delete iceSmallChunk;
    }
    mIceSmallChunk.clear();

    // Release Statues
    for (auto* statue : mStatue)
    {
        delete statue;
    }
    mStatue.clear();

    // Release Candles
    for (auto* candle : mCandle)
    {
        delete candle;
    }
    mCandle.clear();

    // Release IceFlags
    for (auto* iceFlag : mIceFlag)
    {
        delete iceFlag;
    }
    mIceFlag.clear();

    // Release IceWindows
    for (auto* window : mWindow)
    {
        delete window;
    }
    mWindow.clear();

    // Release Jars
    for (auto* jar : mJar)
    {
        delete jar;
    }
    mJar.clear();
    // Clear particles


    mParticles.clear();
}
// 파티클 생성 함수
void TutorialStage::CreateFireParticles(std::vector<Particle>& particles, float x, float y)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f); // 360도 랜덤 각도
    std::uniform_real_distribution<float> speedDist(100.0f, 150.0f); // 속도 범위
    std::uniform_real_distribution<float> lifeDist(0.3f, 0.5f);

    for (int i = 0; i < 20; ++i) {
        Particle p;
        p.x = x;
        p.y = y;
        float angle = angleDist(gen);
        float speed = speedDist(gen);
        p.velX = cosf(angle) * speed;
        p.velY = sinf(angle) * speed;
        p.lifetime = lifeDist(gen);
        p.initialLifetime = p.lifetime;
        p.frame = 0;
        particles.push_back(p);
    }
}

void TutorialStage::Initialize()
{
    // 카메라 설정
    camera.SetTarget(SceneManager::GetSharedPlayer());

    // 몬스터 추가
    dummies.push_back(new Dummy());
    dummies.back()->SetPosition(736, 942);
    dummies.push_back(new Dummy());
    dummies.back()->SetPosition(1136, 942);
    dummies.push_back(new Dummy());
    dummies.back()->SetPosition(1136, 1342);
    dummies.push_back(new Dummy());
    dummies.back()->SetPosition(736, 1342);

    // 튜토리얼 메세지 큐 (완료 메시지 제외)
    tutorialQue.push(L"시작안내");
    tutorialQue.push(L"움직이기");
    tutorialQue.push(L"대쉬");
    tutorialQue.push(L"기본공격 연습");
    tutorialQue.push(L"스킬: 파이어볼 연습");
    tutorialQue.push(L"스킬: 파이어드래곤 연습");
    tutorialQue.push(L"종료 안내");

    // 초기화
    isStepCompleted = false;
    isShowingCompletion = false;
    completionMessageTimer = 0.0f;
    completionMessageAlpha = 255.0f;
    currentTutorialStep = L"";
    // 파티클 이미지 로드
    for (int i = 0; i < 20; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/FireEffect/FIRE_PARTICLE_%02d.png", i);
        if (mFireParticleImage[i].Load(path) != S_OK) {
            std::cout << "Failed to load particle image: " << i << std::endl;
        }
    }
    // 파티클 관련 변수 초기화
    mParticleTimer = 0.0f;
    mParticleSpawnInterval = 0.1f;
}

void TutorialStage::LateUpdate()
{
}
void TutorialStage::Update()
{
    Player* player = SceneManager::GetSharedPlayer();
    player->Update(this);
    camera.Update();
    SoundManager::GetInstance()->Update();
    player->SetCameraX(camera.GetPositionX());
    player->SetCameraY(camera.GetPositionY());
    //기타 업데이트 로직
    {
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
    // 파티클 업데이트
    for (auto it = mParticles.begin(); it != mParticles.end();)
    {
        it->lifetime -= Time::DeltaTime();
        if (it->lifetime <= 0) {
            it = mParticles.erase(it);
        }
        else {
            it->x += it->velX * Time::DeltaTime();
            it->y += it->velY * Time::DeltaTime();
            it->frame = static_cast<int>((1.0f - it->lifetime / it->initialLifetime) * 20);
            if (it->frame >= 20) it->frame = 19;
            ++it;
        }
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
                                CreateFireParticles(mParticles, (minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
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
                            CreateFireParticles(mParticles, (minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
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
                                CreateFireParticles(mParticles, (minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
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
                                CreateFireParticles(mParticles, (minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
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
                                CreateFireParticles(mParticles, (minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
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
                                CreateFireParticles(mParticles, (minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
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
                            CreateFireParticles(mParticles, (minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
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
                                CreateFireParticles(mParticles, (minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
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
                                CreateFireParticles(mParticles, (minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
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
                                CreateFireParticles(mParticles, (minX + maxX) / 2.0f, (minY + maxY) / 2.0f);
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

    //포탈 넘어갈 때 설정해주는거
    RECT temp;
    RECT playerRect = player->GetRect();
    RECT portalRect = portal.GetRect();
    if (IntersectRect(&temp, &playerRect, &portalRect) && Input::GetKeyDown(eKeyCode::F))
    {
        SoundManager::GetInstance()->mPlaySound("ExitPortal", false);
        SceneManager::LoadScene(L"TitleScene");
        SoundManager::GetInstance()->mPlaySound("TitleScreen", true);
		
    }
    //객체간에 충돌처리 밀어내는거
    HandleCollision();
    // 플레이어 벽 충돌 처리
    if (map)
    {
        HandleCollisionMap(map, *player);
    }
    }


    // 튜토리얼 로직
    if (currentTutorialStep.empty() && !tutorialQue.empty())
    {
        currentTutorialStep = tutorialQue.front();
        tutorialQue.pop();
        isStepCompleted = false;
        isShowingCompletion = false;
        completionMessageTimer = 0.0f;
        completionMessageAlpha = 255.0f;
    }

    if (!currentTutorialStep.empty())
    {
        // 완료 메시지 표시 중일 때
        if (isShowingCompletion)
        {
            completionMessageTimer += Time::DeltaTime();
            completionMessageAlpha = 255.0f * (1.0f - completionMessageTimer / 1.0f); // 1초간 페이드 아웃
            if (completionMessageAlpha <= 0.0f)
            {
                completionMessageAlpha = 0.0f;
                isShowingCompletion = false;
                currentTutorialStep = tutorialQue.empty() ? L"" : tutorialQue.front();
                if (!tutorialQue.empty()) tutorialQue.pop();
                isStepCompleted = false;
                completionMessageTimer = 0.0f;
                completionMessageAlpha = 255.0f;
            }
        }
        // 튜토리얼 단계 처리
        else if (currentTutorialStep == L"시작안내")
        {
            if (Input::GetKeyDown(eKeyCode::P))
            {
                isStepCompleted = true;
            }
        }
        else if (currentTutorialStep == L"움직이기")
        {
            TutorialMoveTimer += Time::DeltaTime();
            if (TutorialMoveTimer >= 5.0f)
            {
                isStepCompleted = true;
                TutorialMoveTimer = 0;
            }
         
            
        }
        else if (currentTutorialStep == L"대쉬")
        {
            if (Input::GetKeyDown(eKeyCode::SPACE))
            {
                isStepCompleted = true;
            }
        }
        else if (currentTutorialStep == L"기본공격 연습")
        {
            if (archers.empty())
            {
                archers.push_back(new Archer());
                archers.back()->SetPosition(949, 1024-50);
            }
           
            for (auto* archer : archers)
            {
                if (archer->GetIsDead())
                {
                    isStepCompleted = true;
                    break;
                }
            }
            
        }
        else if (currentTutorialStep == L"스킬: 파이어볼 연습")
        {
            if (swordmans.empty())
            {
                swordmans.push_back(new SwordMan());
                swordmans.back()->SetPosition(949, 1024 - 50);
            }

            for (auto* swordman : swordmans)
            {
                if (swordman->GetIsDead())
                {
                    isStepCompleted = true;
                    break;
                }
            }
            
        }
        else if (currentTutorialStep == L"스킬: 파이어드래곤 연습")
        {
            if (wizards.empty())
            {
                wizards.push_back(new Wizard());
                wizards.back()->SetPosition(949, 1024 - 50);
            }
           
            for (auto* wizard : wizards)
            {
                if (wizard->GetIsDead())
                {
                    isStepCompleted = true;
                    break;
                }
            }
            
        }
        else if (currentTutorialStep == L"종료 안내")
        {
            portal.SetPosition(949, 483);
            RECT playerRect = SceneManager::GetSharedPlayer()->GetRect();
            RECT portalRect = portal.GetRect();
            RECT temp;
            wizards.clear();
            swordmans.clear();
            archers.clear();
            dummies.clear();
            SceneManager::GetSharedPlayer()->SetHp(300);
            if (IntersectRect(&temp, &playerRect, &portalRect) && Input::GetKeyDown(eKeyCode::F))
            {
                isStepCompleted = true;
            }
        }

        // 단계 완료 시 완료 메시지 표시
        if (isStepCompleted && !isShowingCompletion)
        {
            isShowingCompletion = true;
            completionMessageTimer = 0.0f;
            completionMessageAlpha = 255.0f;
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
    //기타 렌더

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
        for (auto* dummy : dummies)
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
        // 파티클 렌더링
        for (const auto& particle : mParticles)
        {
            if (particle.x >= cameraX && particle.x <= cameraX + viewWidth &&
                particle.y >= cameraY && particle.y <= cameraY + viewHeight)
            {
                HDC particleDC = hdc;
                int savedParticleDC = SaveDC(particleDC);
                OffsetViewportOrgEx(particleDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
                mFireParticleImage[particle.frame].Draw(particleDC,
                    static_cast<int>(particle.x - 25),
                    static_cast<int>(particle.y - 25),
                    50, 50);
                RestoreDC(particleDC, savedParticleDC);
            }
        }

        // map object
        for (auto& Candle : mCandle) {
            HDC CandleDC = hdc;
            int savedCandleDC = SaveDC(CandleDC);
            OffsetViewportOrgEx(CandleDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            Candle->Render(CandleDC);
            RestoreDC(CandleDC, savedCandleDC);
        }
        for (auto& IceBigChunk : mIceBigChunk) {
            HDC IceChunkDC = hdc;
            int savedIceChunkDC = SaveDC(IceChunkDC);
            OffsetViewportOrgEx(IceChunkDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            IceBigChunk->Render(IceChunkDC);
            RestoreDC(IceChunkDC, savedIceChunkDC);
        }
        for (auto& IceSmallChunk : mIceSmallChunk) {
            HDC IceChunkDC = hdc;
            int savedIceChunkDC = SaveDC(IceChunkDC);
            OffsetViewportOrgEx(IceChunkDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            IceSmallChunk->Render(IceChunkDC);
            RestoreDC(IceChunkDC, savedIceChunkDC);
        }
        for (auto& MapObject : mIceFlag) {
            HDC MapObDC = hdc;
            int savedMapObDC = SaveDC(MapObDC);
            OffsetViewportOrgEx(MapObDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            MapObject->Render(MapObDC);
            RestoreDC(MapObDC, savedMapObDC);
        }
        for (auto& MapObject : mJar) {
            HDC MapObDC = hdc;
            int savedMapObDC = SaveDC(MapObDC);
            OffsetViewportOrgEx(MapObDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            MapObject->Render(MapObDC);
            RestoreDC(MapObDC, savedMapObDC);
        }
        for (auto& MapObject : mWindow) {
            HDC MapObDC = hdc;
            int savedMapObDC = SaveDC(MapObDC);
            OffsetViewportOrgEx(MapObDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            MapObject->Render(MapObDC);
            RestoreDC(MapObDC, savedMapObDC);
        }
        for (auto& MapObject : mStatue) {
            HDC MapObDC = hdc;
            int savedMapObDC = SaveDC(MapObDC);
            OffsetViewportOrgEx(MapObDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
            MapObject->Render(MapObDC);
            RestoreDC(MapObDC, savedMapObDC);
        }



        UI::Render(hdc);

        //몇 스테이지인지 텍스트 출력
        //텍스트 설정 
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
    }



    // 튜토리얼 텍스트 렌더링
    if (!currentTutorialStep.empty() || isShowingCompletion)
    {
        Gdiplus::Graphics graphics(hdc);
        graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
        Gdiplus::FontFamily fontFamily(L"EXO 2");
        Gdiplus::Font font(&fontFamily, 50, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
        Gdiplus::SolidBrush brush(Gdiplus::Color(static_cast<BYTE>(isShowingCompletion ? completionMessageAlpha : 255), 255, 255, 255));

        // 카메라 변환 행렬 적용
        Gdiplus::Matrix transform;
        transform.Translate(-cameraX, -cameraY);
        graphics.SetTransform(&transform);

        // 플레이어 위치 기준으로 텍스트 위치 설정
        float baseTextX = player->GetPositionX();
        float textY = player->GetRect().top - 209.0f;

        // 텍스트 길이 측정 및 렌더링 함수
        auto renderTextWithOffset = [&](const wchar_t* text) {
            Gdiplus::RectF layoutRect(0, 0, 0, 0);
            Gdiplus::RectF boundingBox;
            graphics.MeasureString(text, -1, &font, layoutRect, &boundingBox);
            float textWidth = boundingBox.Width;
            float textX = baseTextX - (textWidth / 2.0f);
            graphics.DrawString(text, -1, &font, Gdiplus::PointF(textX, textY), &brush);
            };

        // 완료 메시지 표시
        if (isShowingCompletion)
        {
            renderTextWithOffset(L"잘하셨습니다.");
        }
        // 튜토리얼 단계별 안내 메시지
        else if (currentTutorialStep == L"시작안내")
        {
            renderTextWithOffset(L"P키를 눌러 튜토리얼을 시작하세요.");
        }
        else if (currentTutorialStep == L"움직이기")
        {
            renderTextWithOffset(L"WASD를 사용해 움직일 수 있습니다.");
        }
        else if (currentTutorialStep == L"대쉬")
        {
            renderTextWithOffset(L"스페이스바를 눌러 대쉬할 수 있습니다.");
        }
        else if (currentTutorialStep == L"기본공격 연습")
        {
            renderTextWithOffset(L"좌클릭으로 기본공격을 할 수 있습니다.");
        }
        else if (currentTutorialStep == L"스킬: 파이어볼 연습")
        {
            renderTextWithOffset(L"Q키를 눌러 파이어볼을 사용할 수 있습니다.");
        }
        else if (currentTutorialStep == L"스킬: 파이어드래곤 연습")
        {
            renderTextWithOffset(L"E키를 눌러 파이어드래곤을 사용할 수 있습니다.");
        }
        else if (currentTutorialStep == L"종료 안내")
        {
            renderTextWithOffset(L"F키를 눌러 포탈을 타세요.");
        }

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

void TutorialStage::LoadObject(const std::wstring& name) {
    FILE* fp;
    fp = _wfopen(name.c_str(), L"r");

    if (fp != NULL) {
        for (int i = 0; i < 40; i++) {
            for (int j = 0; j < 40; j++) {
                char tile[15];
                if (fscanf(fp, "%s", tile) == 1) {
                    std::string Object = std::string(tile);
                    if (Object != "empty") {

                        if (Object == "Archer")
                        {
                            std::cout << Object << "\n";
                            archers.push_back(new Archer());
                            archers.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                        else if (Object == "SwordMan")
                        {
                            std::cout << Object << "\n";
                            swordmans.push_back(new SwordMan());
                            swordmans.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                        else if (Object == "Wizard")
                        {
                            std::cout << Object << "\n";
                            wizards.push_back(new Wizard());
                            wizards.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                        else if (Object == "Candle")
                        {
                            std::cout << Object << "\n";
                            mCandle.push_back(new Candle());
                            mCandle.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                        else if (Object == "IceChunk0")
                        {
                            std::cout << Object << "\n";
                            mIceBigChunk.push_back(new IceBigChunk());
                            mIceBigChunk.back()->SetImageNum(0);
                            mIceBigChunk.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                        else if (Object == "IceChunk1")
                        {
                            std::cout << Object << "\n";
                            mIceBigChunk.push_back(new IceBigChunk());
                            mIceBigChunk.back()->SetImageNum(1);
                            mIceBigChunk.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                        else if (Object == "IceFlag")
                        {
                            std::cout << Object << "\n";
                            mIceFlag.push_back(new IceFlag());
                            mIceFlag.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                        else if (Object == "IceSmallChunk")
                        {
                            std::cout << Object << "\n";
                            mIceSmallChunk.push_back(new IceSmallChunk());
                            mIceSmallChunk.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                        else if (Object == "IceWindow0")
                        {
                            std::cout << Object << "\n";
                            mWindow.push_back(new IceWindow());
                            mWindow.back()->SetImageNum(0);
                            mWindow.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                        else if (Object == "IceWindow1")
                        {
                            std::cout << Object << "\n";
                            mWindow.push_back(new IceWindow());
                            mWindow.back()->SetImageNum(1);
                            mWindow.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                        else if (Object == "IceWindow2")
                        {
                            std::cout << Object << "\n";
                            mWindow.push_back(new IceWindow());
                            mWindow.back()->SetImageNum(2);
                            mWindow.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                        else if (Object == "Jar")
                        {
                            std::cout << Object << "\n";
                            mJar.push_back(new Jar());
                            mJar.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                        else if (Object == "Statue")
                        {
                            std::cout << Object << "\n";
                            mStatue.push_back(new Statue());
                            mStatue.back()->SetPosition(j * TILE_SIZE, i * TILE_SIZE);
                        }
                    }
                }
            }
        }
        fclose(fp);
    }
    else
        std::cout << "File error";
}
