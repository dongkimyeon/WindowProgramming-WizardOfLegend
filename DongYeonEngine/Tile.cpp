#include "Tile.h"


Tile::Tile()
{

}

Tile::~Tile()
{
}

void Tile::Update()
{
}	

void Tile::Render(HDC hdc)
{
	
}

void Tile::SetPosition(float x, float y)
{
	range.left = x;
	range.right = y;
	//rect = { (int)(mX - 20), (int)(mY + 20), (int)(mX + 20), (int)(mY - 20) };
}