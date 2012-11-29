#ifndef _MORRIS_MANAGER_H_
#define _MORRIS_MANAGER_H_
#include "Board.h"
#include "ps2vector4.h"
#include "ps2matrix4x4.h"
#include "Piece.h"
#include "Player.h"
#include "font.h"
#include "Cursor.h"

class MorrisManager
{
public:
	enum GameState {START, WHITETURN, BLACKTURN, WHITEWIN, BLACKWIN};
	
	MorrisManager();
	~MorrisManager();
	
	void Update();
	void Render();
	void SetPads(int NumberofPads);						//  Correctly detects and sets the pads to the players
														//  Can assign two pads or just one for hotseat play.
														
	void SetupText(CFont &font);						//  For passing a reference to the font class in
														//  myApplication to the Morris Manager for use
														//  in DisplayText.  No need to copy the object.
	
private:
	void CreateViewport(float CameraX, float CameraY, float CameraZ,									//  Creates a perspective transformation matrix
						float ViewPlaneA, float ViewPlaneB, float ViewPlaneC, float ViewPlaneD);		//  corresponding to a camera at (x, y, z) with 
																										//  image on plane ax + by + cz = d.
	
	void RotateWorldByAngle(float Angle);					//  Creates a transformation matrix for the rotation
															//  which is then passed to all 3D sprites to generate 
															//  rotation.  Then recalculates screen coordinates of
															//  all vertices.
	
	
	void DisplayText();										//  Display relevant text, dependant on game State and Phase
	
	Player PlayerWhite;								
	Player PlayerBlack;
	GameState runState;
	Player::GamePhase phase;

	CFont *font_;
	
	Board boardGraphics;				//  An object that handles drawing and transformation of the board.
										//  Also can be used to find the current positions of all pieces.
										
	Cursor cursor;
	
	Matrix4x4 viewportMatrix;	
};

#endif