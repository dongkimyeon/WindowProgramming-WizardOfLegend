#pragma once
#include "GameObject.h"


class Enemy : public GameObject
{
public:
	Enemy();
	void Update() override;
	void LateUpdate() override;
	void Render(HDC hdc) override;

	void SetPosition(float x, float y) override;

	float GetPositionX() override;
	float GetPositionY() override;
	float GetSpeed() override;
	COLORREF GetColor() override;
	float GetRadius() override;
	RECT GetRect() override;
private:
	float mX;
	float mY;
	float radius;
	float speed;
	RECT rect;
	COLORREF color;


};

