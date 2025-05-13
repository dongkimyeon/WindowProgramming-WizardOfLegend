#pragma once
#include "GameObject.h"
#include "Player.h"
class Wizard: public GameObject
{
public:

	Wizard();
	enum EnemyState { LEFT, RIGHT };

	void Update(Player& p);
	void LateUpdate() override;
	void Render(HDC hdc) override;

	void SetHp(int playerDamage) { hp -= playerDamage; }
	float GetDamamge() { return damage; }

	void SetPosition(float x, float y) override;
	EnemyState GetState() { return state; }
	float GetPositionX() override { return mX; }
	float GetPositionY() override { return mY; }
	float GetSpeed() override { return speed; }
	RECT GetRect() override { return rect; }

private:
	float mX;
	float mY;
	Vector2 position;
	int hp;
	int damage;
	// Right �ִϸ��̼�
	CImage mRightIdleAnimation;
	CImage mRightAttackAnimation[4];
	CImage mRightDieAnimaion[5];
	CImage mRightHitAnimation[2];
	CImage mRightWalkAnimation[4];

	// Left �ִϸ��̼�
	CImage mLeftIdleAnimation;
	CImage mLeftAttackAnimation[4];
	CImage mLeftDieAnimaion[5];
	CImage mLeftHitAnimation[2];
	CImage mLeftWalkAnimation[4];


	EnemyState state = EnemyState::RIGHT;

	//������ ����
	bool mIsMoving;
	int mCurrentWalkFrame;
	bool mIsAttack;
	int mCurrenAttackFrame;
	bool mIsdead;
	int mCurrentDeadFrame;
	bool mIsHit;
	int mCurrentHitFrame;
	
	float PlayerDetectRange;

	float speed;
	RECT rect;
	

};

