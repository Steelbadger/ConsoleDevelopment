#include "Piece.h"
#include <iostream>


Piece::Piece()
{
}

Piece::~Piece()
{
}


void Piece::Initialise(PieceColour c, float xpos, float zpos)
{
	state = UNPLACED;
	colour = c;
	
	x = xpos;
	z = zpos;
	float rad = 40.0f;
	float ybase = 800;			//  base y-depth of the board
	
	top.Initialise(rad);			//  Initialise the two sides of the piece.
	bottom.Initialise(rad);

	
	if (c == BLACK) {
		Vector4 temp1(0,0,0,127);
		Vector4 temp2(20, 20, 20, 127);			//  Setup colours, different for black/white pieces.
		Vector4 temp3(30, 30, 30, 127);
		top.SetColours(temp1, temp2);
		bottom.SetColours(temp3, temp3);
	} else if (c == WHITE) {
		Vector4 temp1(115,115,115,127);
		Vector4 temp2(100, 100, 100, 127);
		Vector4 temp3(120, 120, 120, 127);
		top.SetColours(temp1, temp2);
		bottom.SetColours(temp3, temp3);
	}
	MoveTo(x, ybase, z);	//  Move it to a starting position
}

void Piece::MoveTo(float xpos, float ypos, float zpos)
{
	x = xpos;
	y = ypos;
	z = zpos;
	
	top.MoveTo(x, y-25, z);
	bottom.MoveTo(x, y, z);
	top.WorldToScreen();
	bottom.WorldToScreen();
	
}

void Piece::WorldToScreen()
{
	top.WorldToScreen();
	bottom.WorldToScreen();
}

void Piece::SetPerspective(Matrix4x4 m)
{
	top.SetPerspective(m);
	bottom.SetPerspective(m);
}

void Piece::SetTransformation(Matrix4x4 m)
{
	top.SetTransformation(m);
	bottom.SetTransformation(m);
}

void Piece::MakeSelected()
//  Change the colour of the piece to show that it has been selected for movement
{
	if (colour == BLACK)
	{
		Vector4 temp1(40,0,0,127);
		Vector4 temp2(60, 20, 20, 127);
		Vector4 temp3(70, 30, 30, 127);
		top.SetColours(temp1, temp2);
		bottom.SetColours(temp3, temp3);
	} else {
		Vector4 temp1(75,75,115,127);
		Vector4 temp2(60, 60, 60, 127);
		Vector4 temp3(80, 80, 120, 127);
		top.SetColours(temp1, temp2);
		bottom.SetColours(temp3, temp3);
	}
}

void Piece::MakeUnselected()
//  Return piece to original colour once unselected
{
	if (colour == BLACK) {
		Vector4 temp1(0,0,0,127);
		Vector4 temp2(20, 20, 20, 127);
		Vector4 temp3(30, 30, 30, 127);
		top.SetColours(temp1, temp2);
		bottom.SetColours(temp3, temp3);
	} else if (colour == WHITE) {
		Vector4 temp1(115,115,115,127);
		Vector4 temp2(100, 100, 100, 127);
		Vector4 temp3(120, 120, 120, 127);
		top.SetColours(temp1, temp2);
		bottom.SetColours(temp3, temp3);
	}	
}

void Piece::Render()
//  Only draw a piece if it is placed on the board
{
	if (state == PLACED) {
		top.Render();
		bottom.Render();
	}
}

void Piece::Place(float xpos, float zpos)
//  Special case MoveTo, constrained to y = 800 plane and changes piece state to placed.
{
	x = xpos;
	y = 800.0f;
	z = zpos;
	
	state = PLACED;
	MoveTo(x, y, z);
}

float Piece::GetX()
{
	return bottom.GetCentreX();
}

float Piece::GetZ()
{
	return bottom.GetCentreZ();
}

void Piece::Take()
{
	state = TAKEN;
}
