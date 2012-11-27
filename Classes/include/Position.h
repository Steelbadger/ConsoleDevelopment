#ifndef _POSITION_H_
#define _POSITION_H_
#include "ps2matrix4x4.h"
#include "3dCircleSprite.h"

class Position
{
public:
	enum State {UNUSED, WHITE, BLACK};
	Position();
	~Position();
	
	void Initialise(float x, float z);
	
	void SetPerspective(Matrix4x4 PerspectiveMatrix);
	void SetTransformation(Matrix4x4 TransformationMatrix);		//  Pass various matrices to child sprite
	void WorldToScreen();
	
	void Render();
	
	State GetState(){return state;}
	void SetState(State s){state = s;}
	
	float GetX();		//  Returns the centre of the sprite.
	float GetZ();

private:
	PS2CircleSprite3D sprite;
	State state;
};

#endif