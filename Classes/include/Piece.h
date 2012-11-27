#ifndef _PIECE_H_
#define _PIECE_H_
#include "ps2matrix4x4.h"
#include "ps2vector4.h"
#include "3dCircleSprite.h"

class Piece
{
public:
	enum PieceColour {WHITE, BLACK};
	enum State {UNPLACED, PLACED, TAKEN};
	Piece();
	~Piece();
	
	void Initialise(PieceColour Colour, float x, float z);
	
	void MoveTo(float x, float y, float z);
	
	void SetPerspective(Matrix4x4 PerspectiveMatrix);			//  Pass on matrices and prepare for drawing
	void WorldToScreen();
	void SetTransformation(Matrix4x4 TransformationMatrix);
	
	void MakeSelected();			//  For changing the colour of the piece when it is selected for movement
	void MakeUnselected();
	
	void Render();
	
	void Place(float x, float z);		//  A special case move to function
										//  constrained to the board plane and alters te state of the piece to PLACED
	
	void Take();						//  Changes piece state to TAKEN
	
	State GetState(){return state;}	
	float GetX();
	float GetZ();

private:
	
	PS2CircleSprite3D top;
	PS2CircleSprite3D bottom;
	
	State state;
	PieceColour colour;
	
	float x, y, z;
};
#endif