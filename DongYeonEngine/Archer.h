#pragma once
#include "GameObject.h"

class Archer : public GameObject
{
public:

	Archer();
	enum EnemyState { LEFT, RIGHT };
	void Update() override;
	void LateUpdate() override;
	void Render(HDC hdc) override;

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

	// Right �ִϸ��̼�
	CImage mRightIdleAnimation;
	CImage mRightAttackAnimation[4];
	CImage mRightDieAnimaion[6];
	CImage mRightHitAnimation[2];
	CImage mRightWalkAnimation[5];

	// Left �ִϸ��̼�
	CImage mLeftIdleAnimation;
	CImage mLeftAttackAnimation[4];
	CImage mLeftDieAnimaion[6];
	CImage mLeftHitAnimation[2];
	CImage mLeftWalkAnimation[5];


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

	float speed;
	RECT rect;

};

