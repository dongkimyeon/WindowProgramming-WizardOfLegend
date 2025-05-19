#pragma once
#include "Entity.h"
#include "GameObject.h"	
#include "Player_Skill_FireBall.h"	
#include "Arrow.h"
#include "FireBall.h"

class Player_Skill_FireBall;
class Arrow;
class FireBall;

class Scene : public Entity
{
public:
	Scene();
	~Scene();

	virtual void Initialize();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(HDC hdc);

	

	virtual void AddPlayerSkillFireBall(Player_Skill_FireBall* fireball);
	virtual void AddArrow(Arrow* arrow);
	virtual void AddFireBall(FireBall* fireball);

private:
	
};