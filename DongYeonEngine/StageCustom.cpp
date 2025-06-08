#include "StageCustom.h"
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

void StageCustom::ObjectDestroy()
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
void StageCustom::Initialize()
{
	camera.SetTarget(SceneManager::GetSharedPlayer());
	
	// 파티클 이미지 로드
	for (int i = 0; i < 20; ++i)
	{
		wchar_t path[256];
		swprintf_s(path, L"resources/Player/FireEffect/FIRE_PARTICLE_%02d.png", i);
		if (mFireParticleImage[i].Load(path) != S_OK) {
			std::cout << "Failed to load particle image: " << i << std::endl;
		}
	}
	mPlayerSpawnPoint.Load(L"resources/TELEPORT.png");
	// 파티클 관련 변수 초기화
	mParticleTimer = 0.0f;
	mParticleSpawnInterval = 0.1f;
}

void StageCustom::ObjectInitialize()
{
	LoadObject(L"StageCustomObject.txt");
	
}

// 파티클 생성 함수
void StageCustom::CreateFireParticles(std::vector<Particle>& particles, float x, float y)
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

void StageCustom::Update()
{
	Player* player = SceneManager::GetSharedPlayer();
	player->Update(this);
	camera.Update();
	player->SetCameraX(camera.GetPositionX());
	player->SetCameraY(camera.GetPositionY());
	SoundManager::GetInstance()->Update();

	for (auto* archer : archers) archer->Update(*player, this);
	for (auto* wizard : wizards) wizard->Update(*player, this);
	for (auto* swordman : swordmans) swordman->Update(*player);
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

	auto map = MapManager::GetInstance()->GetMap();
	if (map)
	{
		// Arrows
		for (auto it = arrows.begin(); it != arrows.end();)
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
	


	HandleCollision();

	if (map)
	{
		HandleCollisionMap(map, *player);
	}
}

void StageCustom::LateUpdate()
{
	Scene::LateUpdate();
}

void StageCustom::Render(HDC hdc)
{
	Player* player = SceneManager::GetSharedPlayer();
	int savedDC = SaveDC(hdc);
	float cameraX = camera.GetPositionX();
	float cameraY = camera.GetPositionY();
	int viewWidth = 1280;
	int viewHeight = 720;

	MapManager::GetInstance()->Render(hdc, cameraX, cameraY);


	//// 플레이어 스폰포인트 렌더링
	//HDC spawnDC = hdc;
	//int savedSpawnDC = SaveDC(spawnDC);
	//OffsetViewportOrgEx(spawnDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
	//float spawnScale = 1.5f; // 스케일 값 (기본값 1.0, 필요 시 조정 가능)
	//int spawnWidth = 100; // 기본 이미지 너비 (예시, 실제 이미지 크기에 맞게 조정)
	//int spawnHeight = 100; // 기본 이미지 높이 (예시, 실제 이미지 크기에 맞게 조정)
	//int scaledWidth = static_cast<int>(spawnWidth * spawnScale);
	//int scaledHeight = static_cast<int>(spawnHeight * spawnScale);
	//int spawnX = static_cast<int>(183 - scaledWidth / 2.0f); // 중심을 (183, 304)로 설정
	//int spawnY = static_cast<int>(334 - scaledHeight / 2.0f);
	//mPlayerSpawnPoint.Draw(spawnDC, spawnX, spawnY, scaledWidth, scaledHeight);
	//RestoreDC(spawnDC, savedSpawnDC);





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




	HDC playerDC = hdc;
	int savedPlayerDC = SaveDC(playerDC);
	OffsetViewportOrgEx(playerDC, -static_cast<int>(cameraX), -static_cast<int>(cameraY), nullptr);
	player->Render(playerDC);
	RestoreDC(playerDC, savedPlayerDC);

	RestoreDC(hdc, savedDC);

	UI::Render(hdc);

	// 몇 스테이지인지 텍스트 출력
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

	wchar_t StageIdText[20];
	swprintf_s(StageIdText, L"CustomStage");

	SIZE textSize;
	GetTextExtentPoint32(hdc, StageIdText, wcslen(StageIdText), &textSize);

	int textX = viewWidth - textSize.cx;
	int textY = viewHeight - textSize.cy;

	TextOut(hdc, textX - 20, textY, StageIdText, wcslen(StageIdText));

	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);

}

void StageCustom::HandleCollision()
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

void StageCustom::ResolveCollision(GameObject& obj1, GameObject& obj2)
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

void StageCustom::HandleCollisionMap(int (*map)[40], GameObject& obj)
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

void StageCustom::ResolveCollisionMap(RECT wallRect, GameObject& obj)
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

void StageCustom::LoadObject(const std::wstring& name) {
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
							archers.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
						}
						else if (Object == "SwordMan")
						{
							std::cout << Object << "\n";
							swordmans.push_back(new SwordMan());
							swordmans.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
						}
						else if (Object == "Wizard")
						{
							std::cout << Object << "\n";
							wizards.push_back(new Wizard());
							wizards.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
						}
						else if (Object == "Candle")
						{
							std::cout << Object << "\n";
							mCandle.push_back(new Candle());
							mCandle.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
						}
						else if (Object == "IceChunk0")
						{
							std::cout << Object << "\n";
							mIceBigChunk.push_back(new IceBigChunk());
							mIceBigChunk.back()->SetImageNum(0);
							mIceBigChunk.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
						}
						else if (Object == "IceChunk1")
						{
							std::cout << Object << "\n";
							mIceBigChunk.push_back(new IceBigChunk());
							mIceBigChunk.back()->SetImageNum(1);
							mIceBigChunk.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
						}
						else if (Object == "IceFlag")
						{
							std::cout << Object << "\n";
							mIceFlag.push_back(new IceFlag());
							mIceFlag.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
						}
						else if (Object == "IceSmallChunk")
						{
							std::cout << Object << "\n";
							mIceSmallChunk.push_back(new IceSmallChunk());
							mIceSmallChunk.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
						}
						else if (Object == "IceWindow0")
						{
							std::cout << Object << "\n";
							mWindow.push_back(new IceWindow());
							mWindow.back()->SetImageNum(0);
							mWindow.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
						}
						else if (Object == "IceWindow1")
						{
							std::cout << Object << "\n";
							mWindow.push_back(new IceWindow());
							mWindow.back()->SetImageNum(1);
							mWindow.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
						}
						else if (Object == "IceWindow2")
						{
							std::cout << Object << "\n";
							mWindow.push_back(new IceWindow());
							mWindow.back()->SetImageNum(2);
							mWindow.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
						}
						else if (Object == "Jar")
						{
							std::cout << Object << "\n";
							mJar.push_back(new Jar());
							mJar.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
						}
						else if (Object == "Statue")
						{
							std::cout << Object << "\n";
							mStatue.push_back(new Statue());
							mStatue.back()->SetPosition(j * TILE_SIZE + 25, i * TILE_SIZE + 25);
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