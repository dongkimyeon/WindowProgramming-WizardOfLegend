#pragma once
#include "GameObject.h"

class Tile 
{
private:
	CImage tileImage;
	RECT range;

public:
	Tile();
	~Tile();

	void Update();
	void Render(HDC hdc);

	void SetPosition(float x, float y);
};

