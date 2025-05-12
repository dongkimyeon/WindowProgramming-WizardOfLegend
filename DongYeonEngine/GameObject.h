#pragma once
#include "CommonInclude.h"

class GameObject
{
public:
	GameObject();
	~GameObject();
	enum PlayerState { FRONT, BACK, LEFT, RIGHT };
	 virtual void Update();
	 virtual void LateUpdate();
	 virtual void Render(HDC hdc);

	 virtual void SetPosition(float x, float y)
	 {
		mX = x;
		mY = y;
	 }
	 virtual void Setradius(float r)
	 {
		 radius = r;
	 }
	 virtual void SetSpeed(float s)
	 {
		 speed = s;
	 }
	 virtual void SetColor(COLORREF c)
	 {
		 color = c;
	 }
	 virtual float GetPositionX() { return mX; }
	 virtual float GetPositionY() { return mY; }
	 virtual float GetSpeed() { return speed; }
	 virtual COLORREF GetColor() { return color; }
	 virtual float GetRadius() { return radius; }
	 virtual RECT GetRect() 
	 {
		 return rect;
	 }

protected:
	float mX;
	float mY;
	float radius;
	float speed;
	RECT rect;
	COLORREF color;
	
};
