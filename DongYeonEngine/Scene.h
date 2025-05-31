#pragma once
#include "Entity.h"
#include "GameObject.h"	
#include "Player_Skill_FireBall.h"	
#include "Arrow.h"
#include "FireBall.h"
#include "Player_Skill_FireDragon.h"
#include "BossSkill_Spear.h"	
#include "BossSkill_AquaBall.h"


class Player_Skill_FireBall;
class Arrow;
class FireBall;
class Player_Skill_FireDragon;
class BossSkill_Spear;
class BossSkill_AquaBall;

class Scene : public Entity
{
public:
	Scene();
	~Scene();

	virtual void Initialize();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(HDC hdc);
	virtual void ObjectInitialize();
	

	virtual void AddPlayerSkillFireBall(Player_Skill_FireBall* fireball);
	virtual void AddArrow(Arrow* arrow);
	virtual void AddFireBall(FireBall* fireball);
	virtual void AddPlayerSkillFireDragon(Player_Skill_FireDragon* fireDragon);
	virtual void AddBossSkillIceSpear(BossSkill_Spear* spears);
	virtual void AddBossSkillAquaBall(BossSkill_AquaBall* aquaball);
private:
	
};