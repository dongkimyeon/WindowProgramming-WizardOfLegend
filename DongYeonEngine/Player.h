#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	
	Player();
	void Update() override;
	void LateUpdate() override;
	void Render(HDC hdc) override;

	void SetPosition(float x, float y) override;

	PlayerState GetState() { return state; }
	float GetPositionX() override;
	float GetPositionY() override;
	float GetSpeed() override;
	RECT GetRect() override;

private:
	float mX;
	float mY;
	Vector2 position;

	// Front 애니메이션
	CImage mFrontIdleAnimation;
	CImage mFrontAttackAnimation[16];
	CImage mFrontDashAnimation[8];
	CImage mFrontDieAnimation[7];
	CImage mFrontHitAnimation[2];
	CImage mFrontWalkAnimation[10];

	// Right 애니메이션
	CImage mRightIdleAnimation;
	CImage mRightAttackAnimation[16];
	CImage mRightDashAnimation[8];
	CImage mRightHitAnimation[2];
	CImage mRightWalkAnimation[10];

	// Left 애니메이션
	CImage mLeftIdleAnimation;
	CImage mLeftAttackAnimation[16];
	CImage mLeftDashAnimation[8];
	CImage mLeftHitAnimation[2];
	CImage mLeftWalkAnimation[10];

	// Back 애니메이션
	CImage mBackIdleAnimation;
	CImage mBackAttackAnimation[16];
	CImage mBackDashAnimation[8];
	CImage mBackHitAnimation[2];
	CImage mBackWalkAnimation[10];

	PlayerState state = PlayerState::FRONT;

	//움직임 변수
	bool mIsMoving;
	int mCurrentWalkFrame;

	//대쉬 변수
	bool mIsDashing;        // 대쉬 상태 여부
	int mCurrentDashFrame;  // 현재 Dash 애니메이션 프레임
	float mDashTimer;

	float speed;
	RECT rect;
	

};

