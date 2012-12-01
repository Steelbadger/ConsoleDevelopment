#include "MorrisManager.h"
#include <iostream>


MorrisManager::~MorrisManager()
{
}

MorrisManager::MorrisManager():
	PlayerWhite(Piece::WHITE, &cursor, &boardGraphics),	
	PlayerBlack(Piece::BLACK, &cursor, &boardGraphics),				
	viewportMatrix(Matrix4x4::IDENTITY)
{
	Vector4 from(0,-300,0,1);								//  Set the camera's position using a temporary vector.
	CreateViewport(from.x, from.y, from.z, 0,-1,-1,-200);	//  Create the viewport for later use.
	
	boardGraphics.SetPerspective(viewportMatrix);
	boardGraphics.WorldToScreen();
	
	PlayerWhite.SetPerspective(viewportMatrix);
	PlayerWhite.WorldToScreen();
	
	PlayerBlack.SetPerspective(viewportMatrix);			//  Make sure all graphical objects are using 
	PlayerBlack.WorldToScreen();						//  the viewport matrix for their transformations.

	cursor.Initialise();
	cursor.SetPerspective(viewportMatrix);
	cursor.WorldToScreen();
	
	runState = START;						//  Begin the game!
}

void MorrisManager::SetupText(CFont &font)
{
									//  This function is for passing in the font class
	font_ = &font;					//  once it has been created in myApplication.
									//  Don't need to build a whole new font class.
}

void MorrisManager::SetPads(int Number)
{
	if (Number == 1) {
		PlayerWhite.SetPad(Player::PAD0, Number);			//  If only 1 pad detected then assign both players to said pad
		PlayerBlack.SetPad(Player::PAD0, Number);
	} else if (Number == 2) {
		PlayerWhite.SetPad(Player::PAD0, Number);			//  If two pads detected then assign white to pad 1 and black to pad 2
		PlayerBlack.SetPad(Player::PAD1, Number);
	}

}

void MorrisManager::DisplayText()
//  Display any necessary text here, currently draws turns, victories and hints describing what is happening and why.
{
	if (runState == WHITETURN) {
		font_->printfL(  -300, -240, 127, 127, 127, 127, "White's Turn");		//  Display text relevant to state
	} else if (runState == BLACKTURN) {											//  player who's turn it is and 
		font_->printfR(	300, -240, 127, 127, 127, 127, "Black's Turn");			//  notify of winner if there is one.
	} else if (runState == WHITEWIN) {
		font_->printfC(0,0,127,127,127,127,"White Wins!");
	} else if (runState == BLACKWIN) {
		font_->printfC(0,0,127,127,127,127,"Black Wins!");	
	}
	
	switch(phase)
	{
		case Player::OPENING:
			font_->printfC(0,-230,100,100,127,127,"Placement Phase");			
			font_->printfC(0,-205,100,100,127,127,"Place your pieces anywhere on the board");			//  This block draws extra info to
			break;																						//  inform the player of the state
		case Player::MOVING:																			//  and phase of the game.
			font_->printfC(0,-230,100,100,127,127,"Movement Phase");			
			font_->printfC(0,-205,100,100,127,127,"Move any of your pieces to open adjacent spots");
			break;
		case Player::FLYING:
			font_->printfC(0,-230,100,100,127,127,"You can Fly!");			
			font_->printfC(0,-205,100,100,127,127,"Because you have 3 pieces you no longer need to");
			font_->printfC(0,-180,100,100,127,127,"move to adjacent spots, the whole board is open to you");
			break;
		case Player::TAKING:
			font_->printfC(0,-230,100,100,127,127,"Taking a Piece!");			
			font_->printfC(0,-205,100,100,127,127,"Because you formed a Mill(three in a row) you can");
			font_->printfC(0,-180,100,100,127,127,"take ONE of your opponents pieces");	
			break;
		case Player::CAPTUREWIN:
			font_->printfC(0,-230,100,100,127,127,"Capture Win");			
			font_->printfC(0,-205,100,100,127,127,"You won by reducing your opponent to just 2 pieces");
			break;			
		case Player::TRAPWIN:
			font_->printfC(0,-230,100,100,127,127,"Trapping Win");			
			font_->printfC(0,-205,100,100,127,127,"You won by trapping your opponent so that they cannot move");
			break;					
	}
}

void MorrisManager::Update()
//  Main loop logic function.  Players take their turns in here.
{
	if(PlayerBlack.GetFrameRotation() != 0)
		RotateWorldByAngle(PlayerBlack.GetFrameRotation());			//  Is the player trying to rotate the board
	if(PlayerWhite.GetFrameRotation() != 0)							//  If yes then do so by rotating the world.
		RotateWorldByAngle(PlayerWhite.GetFrameRotation());
	
	if ((pad[0].buttons & PAD_L1) && (pad[0].buttons & PAD_R1)) {			//  Pad 1 override incase pad 2 is not working properly.
		cout << "SetPads to use just Pad0" << endl;							//  Sets controls to hotseat mode even if there are two pads.
		SetPads(1);
	}
	if ((pad[0].buttons & PAD_L2) && (pad[0].buttons & PAD_R2)) {			//  Take pretty pictures.
		cout << "Pretty Picture incoming!" << endl;							
		SPS2Manager.ScreenShot();
	}	
	PlayerBlack.CheckActuators();				//  Check the the actuators of the player's active pad
	PlayerWhite.CheckActuators();				//  This handles turning the actuators off after a set time.
		
	if (PlayerBlack.PiecesRemaining() < 3 || PlayerBlack.HasLost()) {			//  Check win conditions for black.
		runState = WHITEWIN;
		if (PlayerBlack.PiecesRemaining() < 3) {
			phase = Player::CAPTUREWIN;						
		} else {
			phase = Player::TRAPWIN;
		}
	} else if (PlayerWhite.PiecesRemaining() < 3 || PlayerWhite.HasLost()) {		//  Check win conditions for white.
		runState = BLACKWIN;
		if (PlayerWhite.PiecesRemaining() < 3) {
			phase = Player::CAPTUREWIN;
		} else {
			phase = Player::TRAPWIN;
		}
	} else {
		if (PlayerWhite.GetTurn() <= PlayerBlack.GetTurn()) {			//  Make players take their turns
			if (!PlayerBlack.WaitingToTakePiece()) {					//  White always goes first
				runState = WHITETURN;									//  extra state to allow a player to take
				PlayerWhite.TakeTurn();									//  a piece if they have formed a mill (three in a row).
				phase = PlayerWhite.GetGamePhase();
			} else {
				runState = BLACKTURN;
				PlayerBlack.TakingPiece(PlayerWhite.TakePiece());		//  should be white's turn but black is allowed to take a piece
				phase = PlayerWhite.GetGamePhase();						//  black takes piece by asking white to do it for them
			}															//  players do not have access to the pieces of the opposition.
		} else {
			if (!PlayerWhite.WaitingToTakePiece()) {
				runState = BLACKTURN;
				PlayerBlack.TakeTurn();
				phase = PlayerBlack.GetGamePhase();				
			} else {
				runState = WHITETURN;
				PlayerWhite.TakingPiece(PlayerBlack.TakePiece());
				phase = PlayerBlack.GetGamePhase();				
			}
		}
	}
}

void MorrisManager::Render()
//  Draw everything, closest/topmost first.
{
	DisplayText();
	cursor.DrawSpire();
	PlayerWhite.Render();
	PlayerBlack.Render();
	cursor.DrawBase();
	boardGraphics.Render();
}

void MorrisManager::CreateViewport(float x, float y, float z, float a, float b, float c, float d)
//  Create the transformation matrix corresponding to a camera position at (x, y, z) on the plane
//  ax + by + cz = d.
{
	Matrix4x4 temp		(d-(b*y+c*z),	a*y,	a*z,		a,
						   b*x,	   d-(a*x+c*z), b*z,		b,
						   c*x,			c*y,  d-(a*x+b*y), 	c,
						  -d*x, 	   -d*y,   -d*z, -(a*x+b*y+c*z));
						  
	viewportMatrix = temp;
}

void MorrisManager::RotateWorldByAngle(float a)
//  Rotate all vertices in scene by given angle around a line parallel to the y-axis that
//  passes through the point (0,800,1000).
{
	Matrix4x4 temp1(Matrix4x4::IDENTITY);		//  Make some temporary matrices for calculation
	Matrix4x4 temp2(Matrix4x4::IDENTITY);
	
	temp2.Translation(0,-800,-1000);			//  Translate one to place board at origin
	temp1 = temp1 * temp2;						//  Store in temp 1
	temp2.RotationY(a);							//  Rotate world by the angle
	temp1 = temp1 * temp2;						//  multiply with translation matrix to get resultant matrix
	temp2.Translation(0,800,1000);				//  Transform back to where the board was originally
	temp1 = temp1 * temp2;						//  multiply again to get the final transformation matrix
	
	boardGraphics.SetTransformation(temp1);
	cursor.SetTransformation(temp1);			//  Set new transformation for all objects
	PlayerWhite.SetTransformation(temp1);
	PlayerBlack.SetTransformation(temp1);
	
	boardGraphics.SetPerspective(viewportMatrix);
	cursor.SetPerspective(viewportMatrix);				//  Set their perspective matrices for the new positions
	PlayerWhite.SetPerspective(viewportMatrix);
	PlayerBlack.SetPerspective(viewportMatrix);
	
	boardGraphics.WorldToScreen();
	cursor.WorldToScreen();								//  recalculate the 2d screen coordinates of the objects after the transformation
	PlayerWhite.WorldToScreen();
	PlayerBlack.WorldToScreen();
}

