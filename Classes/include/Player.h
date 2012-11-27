#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Piece.h"
#include "pad.h"
#include "Board.h"
#include "ps2matrix4x4.h"
#include "ps2maths.h"
#include "font.h"
#include "Cursor.h"

class Player
{
public:
	enum Controller {PAD0, PAD1};
	enum GamePhase {OPENING, MOVING, FLYING, TAKING, CAPTUREWIN, TRAPWIN};	
	Player(Piece::PieceColour Colour, Cursor *cursor, Board *board);
	~Player();
	
	void SetPad(Controller PadUsed, int pads);		//  Sets the player to use the pad
	
	void TakeTurn();			//  Main function for taking a turn, calls smaller functions for various types of turn
	
	bool TakePiece();							//  Function used when opponent is taking a piece.  Player takes OWN piece.
	void TakingPiece(bool doesTakePiece);		//  Function used while taking a piece.  Waits while opponent takes OWN piece.
	
	void Render();
	
	void SetPerspective(Matrix4x4 PerspectiveMatrix);
	void WorldToScreen();										//  Pass through matrices for piece transformation and viewing
	void SetTransformation(Matrix4x4 TransformationMatrix);
	
	bool CheckTurnMillCreation();					//  Checks if the player has formed a new 3
	
	int PiecesPlaced(){return piecesPlaced;}
	int PiecesRemaining(){return piecesRemaining;}
	
	int GetTurn(){return turn;}		//  What turn number the player is on
	
	bool WaitingToTakePiece(){return canTakeAPiece;}		//  Returns true if the player is waiting for the opponent to take their OWN piece
	
	float GetFrameRotation(){return frameRotation;}		//  Return rotation that player has created using right analogue stick
	
	bool HasLost(){return hasLost;}			
	
	void CheckActuators();			//  turns off actuators after a certain time.
	
	GamePhase GetGamePhase(){return phase;}
	

private:

	Piece::PieceColour colour;
	Position::State playerColour;			//  The piece colour of the player, stored in two different forms for different uses
	
	void TakeStartingTurn();			//  function that takes turn during placement phase
	void TakeMovingTurn();				//  function that takes turn during moving phase
	void TakeFlyingTurn();				//  function that takes turn while player is flying
	
	bool IsPositionAdjacentToPiece(int pieceNumber, int targetPositionNumber);		//  Check if a piece is adjacent to a position for movement legality
	int GetPositionOfPiece(int PieceNumber);			//  Finds what position a given piece is in
	
	int FindFirstMillOfPosition(int PositionNumber);		//  Find which mills any given position is in
	int FindSecondMillOfPosition(int PositionNumber);
	
	bool CheckIfPieceIsInMill(int Positionnumber);		//  Find if piece is part of an active groupf of three.
	
	bool CheckIfCanMove();			//  Check if there are ANY moves that the player can make
	
	bool PlacePiece();
	bool MovePiece();		//  functions for moving the pieces in corresponding turn phase
	bool FlyPiece();
	
	bool SelectPiece();		//  Makes a piece selected.
	
	Controller padUsed;		//  enum holding the pad the player is assigned
	
	GamePhase phase;		//  what phase of the game is the player in
	
	int firstMillUsed, secondMillUsed;		//  holding values for mill check logic
	float frameRotation;
	
	int piecesPlaced;
	int piecesRemaining;
	
	int actuatorFrame;			//  How long has the actuator been on, in frames.
	
	bool canTakeAPiece;
	bool hasLost;
	int turnLastCycle;		//  a lagging value of turn, to allow for some turn change logic
	
	Piece pieces[9];		//  The nine pieces the player has
	
	padinfo_t *m_pad;		//  pointer the the player's primary pad
	padinfo_t *s_pad;		//  pointer to the opposing player's pad
	
	Cursor *cursor;
	
	int turn;
	
	Piece *selected;		//  Pointer to the piece that has been selected most recently
	Board *gameBoard;		//  pointer to the board so that mills and positions can be checked.

};
#endif