#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "3dCircleSprite.h"
#include "ps2matrix4x4.h"

class Cursor
{
public:
	Cursor();
	~Cursor();
	
	void Initialise();
	
	void DrawBase();			//  Two separate render functions so that the cursor
	void DrawSpire();			//  appears to pass through pieces in 3D space (bottom goes under
								//  and top goes over).

	void SetPerspective(Matrix4x4 PerspectiveMatrix);			//  Pass various matrices through to primitives
	void WorldToScreen();										//  for the perspective drawing
	void SetTransformation(Matrix4x4 TransformationMatrix);
	
	
	float GetCentreX(){return sprites[0].GetCentreX();}			//  Get the centre of the base of the cursor for collision
	float GetCentreY(){return sprites[0].GetCentreY();}
	float GetCentreZ(){return sprites[0].GetCentreZ();}
	
	void SetColourToWhite();					//  Changle the colour of the cursor
	void SetColourToBlack();
	
	void MoveDeltaX(float dx);					//  Nudge cursor by a distance, need to call world to screen to see changes.
	void MoveDeltaY(float dy);
	void MoveDeltaZ(float dz);
	
	void MoveTo(float x, float y, float z); 		//  Jump to a location in 3D space
	
private:
	PS2CircleSprite3D sprites[5];
};

#endif