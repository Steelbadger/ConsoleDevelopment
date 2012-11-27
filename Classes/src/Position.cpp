#include "Position.h"
#include <iostream>


Position::Position():
	sprite(25)
{
	Initialise(0, 1000);
}

Position::~Position()
{

}

void Position::Initialise(float x, float z)
//  Move the sprite to a spot on the y = 800 plane.
{
	sprite.MoveTo(x, 800, z);
}

void Position::SetPerspective(Matrix4x4 p)
{
	sprite.SetPerspective(p);
}

void Position::SetTransformation(Matrix4x4 m)
{
	sprite.SetTransformation(m);
}

void Position::WorldToScreen()
{
	sprite.WorldToScreen();
}

void Position::Render() 
{
	sprite.Render();
}

float Position::GetX()
{
	return sprite.GetCentreX();
}

float Position::GetZ()
{
	return sprite.GetCentreZ();
}


