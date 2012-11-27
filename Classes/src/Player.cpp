#include "Player.h"
#include <iostream>


Player::Player(Piece::PieceColour Colour, Cursor *C, Board *board)
{
	for (int i = 0; i < 9; i++) {
		pieces[i].Initialise(Colour, 0.0f, 0.0f);
	}
	
	cursor = C;
	
	selected = NULL;
	gameBoard = board;
	
	piecesRemaining = 9;
	piecesPlaced = 0;
	
	colour = Colour;
	
	canTakeAPiece = false;
	
	frameRotation = 0;
	
	turn = 0;
	turnLastCycle = 0;
	
	hasLost = false;
	
	actuatorFrame = 0;
	
	phase = OPENING;
	
	if (colour == Piece::WHITE) {
		playerColour = Position::WHITE;
	} else {
		playerColour = Position::BLACK;
	}

}

Player::~Player()
{
}

void Player::SetPad(Controller PadUsed, int pads)
//  Set the primary and secondary pad points to the correct value for the player.
//  if two pads then one pad each, if one pad then both on same pad.
//  Turn enable the actuators!
{	
	if (pads == 2) {
		if (PadUsed == PAD0) {
			m_pad = &pad[0];
			s_pad = &pad[1];
			enable_actuator(0,1,1);			
		} else {
			m_pad = &pad[1];
			s_pad = &pad[0];
			enable_actuator(1,1,1);			
		}	
	} else {
		if (PadUsed == PAD0) {
			m_pad = &pad[0];
			s_pad = &pad[0];
			enable_actuator(0,1,1);				
		} else {
			m_pad = &pad[1];
			s_pad = &pad[1];
			enable_actuator(1,1,1);				
		}
	}
	padUsed = PadUsed;	
}

void Player::CheckActuators()
//  How long have the actuators been running?
//  Turn them off if have been on for too long
{
	if (actuatorFrame > 0) {
		set_actuator(padUsed, 1, actuatorFrame++);
	}
	if (actuatorFrame == 180) {
		set_actuator(padUsed, 0, 0);
		actuatorFrame = 0;
	}	
}

void Player::TakeTurn()
//  Main take turn function, calls other functions for different types of turn
{
	frameRotation = 0;		//  No rotation during this frame so far
	
	if (playerColour == Position::BLACK) {
		cursor->SetColourToBlack();
	} else {										//  Set the cursor to the right colour for the player
		cursor->SetColourToWhite();
	}
	
	if(m_pad->axes[0])
		cursor->MoveDeltaX(m_pad->axes[0]*10.0);			//  check for cursor movement and board rotation
	if(m_pad->axes[1])
		cursor->MoveDeltaZ(m_pad->axes[1]*-10.0);	
	if(m_pad->axes[2])
		frameRotation = m_pad->axes[2]/5.0;		
	
	if (turnLastCycle < turn) {				//  If it's the beginning of the players turn
		if (!CheckIfCanMove()){				//  check if they have any legal moves available
			hasLost = true;					//  if not then they lose.
		}
		turnLastCycle = turn;
	}

	if (piecesPlaced < 9) {
		phase = OPENING;
		TakeStartingTurn();					//  Less than 9 pieces placed so in opening phase
	} else if (piecesRemaining == 3) {
		phase = FLYING;
		TakeFlyingTurn();					//  3 pieces remaining so in flying phase
	} else {
		phase = MOVING;
		TakeMovingTurn();					//  Otherwise in movement phase
	}
}

bool Player::CheckIfCanMove()
//  Check all adjacent positions for all pieces, if all are used then the player cannot move
//  and thus loses.
//  Mills used to find which spots are adjacent as they are adjacent in the mill arrays
{
	for (int i = 0; i < 9; i++) {			//  for all pieces
		if (pieces[i].GetState() == Piece::PLACED) {			//  Check it's been placed
			for (int j = 0; j < 24; j++) {						//  Check all positions
				if (gameBoard->boardpositions[j].GetState() == Position::UNUSED && IsPositionAdjacentToPiece(i, j)) {		//  is it unused AND next to the piece?
					return true;						//  if yes then player can move, return true.
				}			
			}
		} else if (pieces[i].GetState() == Piece::UNPLACED) {		//  If any pieces are unplaced then player can obviously move
			return true;
		}
	}
	return false;			//  If no moves found then player can't move.
}

void Player::Render()
{
	for (int i = 0; i < 9; i++) {
		if (pieces[i].GetState() == Piece::PLACED) {
			pieces[i].Render();
		}
	}

}

void Player::SetPerspective(Matrix4x4 m)
{
	for (int i = 0; i < 9; i++) {
		pieces[i].SetPerspective(m);
	}
}

void Player::SetTransformation(Matrix4x4 m)
{
	for (int i = 0; i < 9; i++) {
		pieces[i].SetTransformation(m);
	}	
}

void Player::WorldToScreen()
{
	for (int i = 0; i < 9; i++) {
		pieces[i].WorldToScreen();
	}		

}

bool Player::TakePiece()
//  Player pretends to be opposiing player for the purposes of removing a piece from the board.
//  Players cannot see each other and thus must take their own pieces.
//  Basically similar to normal turn functions, but with colours reversed.
{	
	frameRotation = 0;
	phase = TAKING;
	if (playerColour == Position::WHITE) {
		cursor->SetColourToBlack();
	} else {
		cursor->SetColourToWhite();
	}
	
	if(s_pad->axes[0])
		cursor->MoveDeltaX(s_pad->axes[0]*10.0);
	if(s_pad->axes[1])
		cursor->MoveDeltaZ(s_pad->axes[1]*-10.0);	
	if(s_pad->axes[2])
		frameRotation = s_pad->axes[2]/5.0;		


	if (s_pad->pressed & PAD_CROSS) {					//  Beginning of main logic, when player presses button
		int piecesInMills = 0;
		int piecesOnBoard = 0;
		int p = 0;
		bool canTakeMillPiece = false;
		for (int i = 0; i < 9; i++) {						//  first check all pieces on board
			if (pieces[i].GetState() == Piece::PLACED) {	//  if all pieces are in mills
				piecesOnBoard++;							//  then player needs to be able to take a piece in
				p = GetPositionOfPiece(i);					
				if (CheckIfPieceIsInMill(p)) {
					piecesInMills++;
				}				
			}
		}
		if (piecesInMills == piecesOnBoard) {
			canTakeMillPiece = true;				//  Compare the number of pieces on the board to the number in mills
		}											//  if the number is the same then player can take pieces in mills
	
		for (int i = 0; i < 9; i++) {
			float dx = pieces[i].GetX() - cursor->GetCentreX();			
			float dz = pieces[i].GetZ() - cursor->GetCentreZ();
			if ((40*40) > (dx*dx+dz*dz)) {								//  find which piece is being clicked on
				p = GetPositionOfPiece(i);								//  find where that piece is on the board
				if (!CheckIfPieceIsInMill(p) || canTakeMillPiece) {			//  is the player allowed to take it?
					pieces[i].Take();										//  if yes then take piece, set state
					gameBoard->boardpositions[p].SetState(Position::UNUSED);		//  vibrate other pad
					pieces[i].MoveTo(0.0f, 0.0f, 0.0f);								//  and break out of the loop
					i = 9;
					actuatorFrame += 120;
					set_actuator(padUsed, 1, actuatorFrame);
					piecesRemaining--;
					return true;											//  return true to let opponent know a piece was taken
				}
			}
		}
	}
	return false;			//  No piece was taken
}

bool Player::CheckIfPieceIsInMill(int p)
//  Used in the TakePiece() function to check which pieces are in mills
{
	int mill1 = FindFirstMillOfPosition(p);			//  Every piece is in two mills
	int mill2 = FindSecondMillOfPosition(p);		//  find both of the mills
	
	if (gameBoard->mills[mill1][0]->GetState() == gameBoard->mills[mill1][1]->GetState()		//  First check that the possible mill is an actual mill
		&& gameBoard->mills[mill1][1]->GetState() == gameBoard->mills[mill1][2]->GetState()		//  Check if the state of all 3 is the same (and not unused)
		&& gameBoard->mills[mill1][0]->GetState() != Position::UNUSED)
	{	
		return true;							//  If this is true then the piece is in at least one mill and thus return true
	}
	if (gameBoard->mills[mill2][0]->GetState() == gameBoard->mills[mill2][1]->GetState()			//  Check the second mill in the same way
		&& gameBoard->mills[mill2][1]->GetState() == gameBoard->mills[mill2][2]->GetState()
		&& gameBoard->mills[mill2][0]->GetState() != Position::UNUSED)
	{	
		return true;
	}
	
	
	return false;			//  If we have reached here then the piece is not in mill, so false.
}

void Player::TakingPiece(bool didTake)
//  Waiting to take a piece
{
	if (didTake)
		canTakeAPiece = false;
	else
		canTakeAPiece = true;
}

void Player::TakeStartingTurn()
//  Takes turn during opening placement phase of game
{
	if (m_pad->pressed & PAD_CROSS) {		//  if button pressed
		if (PlacePiece()) {					//  Try to place a piece
			turn++;							//  if piece successfully placed then end turn
			CheckTurnMillCreation();		//  and check if a new mill was made
		} 
	}
}

bool Player::PlacePiece()
//  Place a new piece on the board, used during opening placement phase
{
	for (int i = 0; i < 24; i++) {															//  For all positions
		float dx = gameBoard->boardpositions[i].GetX() - cursor->GetCentreX();
		float dz = gameBoard->boardpositions[i].GetZ() - cursor->GetCentreZ();		
		if ((25*25) > (dx*dx+dz*dz) && (gameBoard->boardpositions[i].GetState() == Position::UNUSED)) {		//  find if cursor is over the position
			for (int j = 0; j < 9; j++) {												//  find first unplaced piece in players pieces array
				if (pieces[j].GetState() == Piece::UNPLACED) {
					pieces[j].Place(gameBoard->boardpositions[i].GetX(), gameBoard->boardpositions[i].GetZ());				//  and place it on position
					piecesPlaced++;
					gameBoard->boardpositions[i].SetState(playerColour);
					firstMillUsed = FindFirstMillOfPosition(i);			//  Take note of the mills it was placed in for later check.
					secondMillUsed = FindSecondMillOfPosition(i);		
					return true;										//  if a piece was place return true (turn will be over)
				}
			}

		}
	}
	return false;			//  if you clicked somewhere where no position was, then no piece gets placed, turn does not increment
}

bool Player::SelectPiece()
//  Find the piece under the cursor and select it for movement later.
{
	if (selected != NULL) {
		selected->MakeUnselected();		//  First unselect old selection
	}
	
	for (int i = 0; i < 9; i++) {					//  Check for cursor location
		float dx = pieces[i].GetX() - cursor->GetCentreX();
		float dz = pieces[i].GetZ() - cursor->GetCentreZ();
		
		if ((40*40) > (dx*dx+dz*dz) && pieces[i].GetState() == Piece::PLACED) {		//  compare with placed pieces
			selected = &pieces[i];							//  if within range then set piece to selected and make selected a pointer to the piece
			selected->MakeSelected();
			return true;						//  and say a piece was selected by returning true
		}
	}	
	return false;				//  No piece selected
}

void Player::TakeMovingTurn()
//  function for taking turn during movement phase
{
	if (m_pad->pressed & PAD_CROSS) {		//  when player hits button
		if (SelectPiece()) {				//  try to select a piece (deselects if nothing selected)
		}
		if (selected != NULL) {				//  if something is selected then try to move it
			if (MovePiece()) {
				turn++;							//  if successfully moved then end turn and deselect piece
				selected->MakeUnselected();
				selected = NULL;
				CheckTurnMillCreation();			//  also check for any new mills created
			}
		}
	}	
}

void Player::TakeFlyingTurn()
//  function for taking turn during Flying phase (3 piece remaining)
//  Same as TakeMovingTurn() but with no check on adjacency
{
	if (m_pad->pressed & PAD_CROSS) {
		if (SelectPiece()) {
		}
		if (selected != NULL) {
			if (FlyPiece()) {
				turn++;
				selected = NULL;
				CheckTurnMillCreation();
			}
		}
	}
}

bool Player::FlyPiece()
//  Special case MovePiece function for when piece can move to anywhere, not just adjacent spots
{
	int selectedPieceNumber = -1;
	for (int j = 0; j < 9; j++) {
		if (&pieces[j] == selected) {		//  first find out which piece (in the array) is referenced by the selected pointer
			selectedPieceNumber = j;
			j = 9;
		}
	}	
	
	for (int i = 0; i < 24; i++) {
		float dx = gameBoard->boardpositions[i].GetX() - cursor->GetCentreX();
		float dz = gameBoard->boardpositions[i].GetZ() - cursor->GetCentreZ();
		
		if ((40*40) > (dx*dx+dz*dz) && gameBoard->boardpositions[i].GetState() == Position::UNUSED) {		//  Then attempt to move the piece to
			gameBoard->boardpositions[GetPositionOfPiece(selectedPieceNumber)].SetState(Position::UNUSED);	//  the position clicked on
			gameBoard->boardpositions[i].SetState(playerColour);											//  no limits on distance can move
			selected->Place(gameBoard->boardpositions[i].GetX(), gameBoard->boardpositions[i].GetZ());
			firstMillUsed = FindFirstMillOfPosition(i);
			secondMillUsed = FindSecondMillOfPosition(i);				
			return true;										//  successful move, true
		}
	}
	return false;					//  No move made, false
}

bool Player::MovePiece()
//  Move a selected piece to an adjacent selected position
{
	int selectedPieceNumber = -1;
	for (int j = 0; j < 9; j++) {
		if (&pieces[j] == selected) {
			selectedPieceNumber = j;			//  find which piece is referenced by the selected pointer
			j = 9;
		}
	}	
	
	for (int i = 0; i < 24; i++) {					//  for all positions find the one the player clicked on
		float dx = gameBoard->boardpositions[i].GetX() - cursor->GetCentreX();
		float dz = gameBoard->boardpositions[i].GetZ() - cursor->GetCentreZ();
		
		if ((40*40) > (dx*dx+dz*dz) && gameBoard->boardpositions[i].GetState() == Position::UNUSED && IsPositionAdjacentToPiece(selectedPieceNumber, i)) {
			gameBoard->boardpositions[GetPositionOfPiece(selectedPieceNumber)].SetState(Position::UNUSED);
			gameBoard->boardpositions[i].SetState(playerColour);											//  Position must be unused, under the cursor AND
			selected->Place(gameBoard->boardpositions[i].GetX(), gameBoard->boardpositions[i].GetZ());		//  Must be adjacent to the piece
			firstMillUsed = FindFirstMillOfPosition(i);														//  if all this is ok, then move the piece as normal
			secondMillUsed = FindSecondMillOfPosition(i);				
			return true;										//  Piece successfully moved
		}
	}
	return false;											//  Piece not moved.
}

bool Player::IsPositionAdjacentToPiece(int pieceNumber, int targetPositionNumber)
//  Check if a piece (entered as it's index in the player's pieces array) is adjacent to a position
//  (entered as the index in the Board's positions array)
{
	int piecePosition = GetPositionOfPiece(pieceNumber);		//  First find which position the piece is in
	if (piecePosition < 0) {						//  If piece is not in a legal position then return false
		return false;								//  Something went wrong
	}
	
	firstMillUsed = FindFirstMillOfPosition(piecePosition);		//  Find which mills the piece is in
	secondMillUsed = FindSecondMillOfPosition(piecePosition);
		
	int mill = firstMillUsed;			//  First check the firts mill
	int piecePosInMill = -1;			//  the index the position the piece is occupying in the mill of interest
	int positionPosInMill = -3;			//  the index the position to move to is occupying in the mill of interest
	
	for (int i = 0; i < 3; i++) {			//  for all three mill positions
		if (gameBoard->mills[mill][i] == &gameBoard->boardpositions[piecePosition]) {		//  check if the piece is in that position
			piecePosInMill = i;					//  if it is then remember the index of the position
		}
	}
	
	for (int i = 0; i < 3; i++) {			//  for all three mill positions
		if (gameBoard->mills[mill][i] == &gameBoard->boardpositions[targetPositionNumber]) {	//  check if position to move to is in the mill
			positionPosInMill = i;				//  if it is then remember the index of the position
		}
	}

	if (positionPosInMill >= 0) {			//  If the position to move to WAS in the mill then check index distance
		int diff = positionPosInMill - piecePosInMill;
		if (diff == 1 || diff == -1) {			//  if the distance is 1 or -1 then they are adjacent in the mill array
			return true;						//  and thus adjacent on the board, return true.
		}
	}
	
	mill = secondMillUsed;			//  repeat process for second mill
	piecePosInMill = -1;
	positionPosInMill = -3;	
	
	for (int i = 0; i < 3; i++) {
		if (gameBoard->mills[mill][i] == &gameBoard->boardpositions[piecePosition]) {
			piecePosInMill = i;
		}
	}
	for (int i = 0; i < 3; i++) {
		if (gameBoard->mills[mill][i] == &gameBoard->boardpositions[targetPositionNumber]) {
			positionPosInMill = i;
		}
	}
	

	if (positionPosInMill >= 0) {
		int diff = positionPosInMill - piecePosInMill;
		if (diff == 1 || diff == -1) {
			return true;
		} else {
			return false;		//  If we reached here then target position is not adjacent
		}
	}
	return false;				//  If we reached here then target position is not adjacent
}

int Player::GetPositionOfPiece(int p)
//  Find which position (in Board.Positions[]) a given piece is in.  Piece passed by it's array index in
//  player.pieces[] 
{
	if (p >= 9) 
		return -1;		//  impossible piece index value, broken stuff happened.

	for (int i = 0; i < 24; i++) {												//  for all positions				
		float dx = gameBoard->boardpositions[i].GetX() - pieces[p].GetX();		//  compare x and z coords
		float dz = gameBoard->boardpositions[i].GetZ() - pieces[p].GetZ();	
		if (dx == 0.0f && dz == 0.0f) {											//  if they're identical then the piece is in that position
			return i;								//  so return the index of the position found
		}
	}	
	return -2;					//  Or else couldn't find it.  Broken stuff happens again.
}

int Player::FindFirstMillOfPosition(int p)
//  Find the first mill a position can be found in
//  position passed in as index of Positions array
{
	for (int i = 0; i < 16; i++) {					//  for all mills
		for (int j = 0; j < 3; j++) {				//  Check all positions in mill
			if (gameBoard->mills[i][j] == &gameBoard->boardpositions[p]) {		//  if mill position points to position to check then 
				return i;													//  found, return the index of the mill
			}			
		}
	}
	return -1;				//  broken stuff!
}

int Player::FindSecondMillOfPosition(int p)
//  Find the second mill a position can be found in
//  position passed in as index of Positions array 
{
	for (int i = 15; i >= 0; i--) {					//  Do same as firstmill, but reverse iterate
		for (int j = 0; j < 3; j++) {
			if (gameBoard->mills[i][j] == &gameBoard->boardpositions[p]) {
				return i;				// found, return index of mill
			}			
		}
	}
	return -1;				//  brokit!
}

bool Player::CheckTurnMillCreation()
//  Check to see if a new mill has been made
{
	if (firstMillUsed < 0 || secondMillUsed < 0) {
		return false;				//  Catch errors
	}
	
	//  Uses first and secondMillUsed only check the mills of the newly placed piece

	if (gameBoard->mills[firstMillUsed][0]->GetState() == gameBoard->mills[firstMillUsed][1]->GetState()			//  Check first mill to find it it is 
		&& gameBoard->mills[firstMillUsed][1]->GetState() == gameBoard->mills[firstMillUsed][2]->GetState())		//  a potential mill (all positions have same
	{																												//  state (may be UNUSED)

		if (playerColour == gameBoard->mills[firstMillUsed][0]->GetState()){					//  Check that the common state of the mill matches the players
			firstMillUsed = -1;																	//  colour property
			secondMillUsed = -1;											//  mill found, don't check further, set to -1 to stop repeat checks
			canTakeAPiece = true;
			return true;					//  Made a mill, and can take a piece next turn!
		}	
	}
	
	//  Do the same for the second mill
	
	if (gameBoard->mills[secondMillUsed][0]->GetState() == gameBoard->mills[secondMillUsed][1]->GetState()
		&& gameBoard->mills[secondMillUsed][1]->GetState() == gameBoard->mills[secondMillUsed][2]->GetState())
	{	
	
		if ((colour == Piece::WHITE && gameBoard->mills[secondMillUsed][0]->GetState() == Position::WHITE)
			|| (colour == Piece::BLACK && gameBoard->mills[secondMillUsed][0]->GetState() == Position::BLACK))
		{
			firstMillUsed = -1;
			secondMillUsed = -1;
			canTakeAPiece = true;
			return true;
		}	
	}
	
	
	canTakeAPiece = false;			//  If we reached here then no mill has been made, so cannot take a piece
	return false;
}

