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

	// Front �ִϸ��̼�
	CImage mFrontIdleAnimation;
	CImage mFrontAttackAnimation[16];
	CImage mFrontDashAnimation[8];
	CImage mFrontDieAnimation[7];
	CImage mFrontHitAnimation[2];
	CImage mFrontWalkAnimation[10];

	// Right �ִϸ��̼�
	CImage mRightIdleAnimation;
	CImage mRightAttackAnimation[16];
	CImage mRightDashAnimation[8];
	CImage mRightHitAnimation[2];
	CImage mRightWalkAnimation[10];

	// Left �ִϸ��̼�
	CImage mLeftIdleAnimation;
	CImage mLeftAttackAnimation[16];
	CImage mLeftDashAnimation[8];
	CImage mLeftHitAnimation[2];
	CImage mLeftWalkAnimation[10];

	// Back �ִϸ��̼�
	CImage mBackIdleAnimation;
	CImage mBackAttackAnimation[16];
	CImage mBackDashAnimation[8];
	CImage mBackHitAnimation[2];
	CImage mBackWalkAnimation[10];

	PlayerState state = PlayerState::FRONT;

	//������ ����
	bool mIsMoving;
	int mCurrentWalkFrame;

	//�뽬 ����
	bool mIsDashing;        // �뽬 ���� ����
	int mCurrentDashFrame;  // ���� Dash �ִϸ��̼� ������
	float mDashTimer;

	float speed;
	RECT rect;
	

};

