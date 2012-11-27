#ifndef _BOARD_H_
#define _BOARD_H_
#include "3dSprite.h"
#include "ps2matrix4x4.h"
#include "Position.h"

class Board
{
public:
	Board();
	~Board();
	
	void Initialise();
	void SetPerspective(Matrix4x4 PerspectiveMatrix);
	void WorldToScreen();
	void SetTransformation(Matrix4x4 TransformationMatrix);
	
	void Render();
	
	Position boardpositions[24];
	Position *mills[16][3];
	
private:
	void SortSides();			//  sort sides by z depth to ensure correct draw order
	
	void InitialiseLines();		//  Function to pull out large by-hand per vertex drawing process.
	void SetupMills();			//  Function to pull out large by-hand creation of look-up array for the
								//  cartesian lines on the board (the mills).
	
	PS2Sprite3D boardlines[32];
	PS2Sprite3D top;
	PS2Sprite3D table;
	
	PS2Sprite3D sides[4];
};

#endif