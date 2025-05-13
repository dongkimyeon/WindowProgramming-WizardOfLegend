#pragma once
#include "GameObject.h"

class SwordMan : public GameObject
{
public:

	SwordMan();
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

	// Right 애니메이션
	CImage mRightIdleAnimation;
	CImage mRightAttackAnimation[3];
	CImage mRightDieAnimaion[6];
	CImage mRightHitAnimation[2];
	CImage mRightWalkAnimation[6];

	// Left 애니메이션
	CImage mLeftIdleAnimation;
	CImage mLeftAttackAnimation[3];
	CImage mLeftDieAnimaion[6];
	CImage mLeftHitAnimation[2];
	CImage mLeftWalkAnimation[6];

	CImage mAttackEffectAnimation[4];
	EnemyState state = EnemyState::RIGHT;

	//움직임 변수
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

