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
 
	
}

void Stage2::LateUpdate()
{

}
void Stage2::Update()
{
	UI::Update();

	
	SceneManager::GetSharedPlayer()->Update(this);
}

void Stage2::Render(HDC hdc)
{
   
	SceneManager::GetSharedPlayer()->Render(hdc);

	

    UI::Render(hdc);
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
                    printf("Player collided with wall at tile [%d, %d] - Wall RECT: (%ld, %ld, %ld, %ld), Player RECT: (%ld, %ld, %ld, %ld)\n",
                        i, j,
                        wallRect.left, wallRect.top, wallRect.right, wallRect.bottom,
                        playerRect.left, playerRect.top, playerRect.right, playerRect.bottom);
                    ResolveCollisionMap(wallRect, *player);
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