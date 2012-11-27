#include "Board.h"

Board::Board()
{
	Initialise();
}

Board::~Board()
{
}

void Board::Initialise()
{
	float zbase = 400;							//  Variable to store how deep in the z axis the object is based.
	
	
	Vector4 temp1(-600, 800, 1200 + zbase, 1);
	Vector4 temp2(600, 800, 1200 + zbase, 1);
	Vector4 temp3(600, 800, 0 + zbase, 1);
	Vector4 temp4(-600,800,0 + zbase,1);
	top.Initialise(temp1, temp2, temp3, temp4);		//  Set individual vertex locations for the top face of the board
	
	Vector4 col1(70,50,4,1);
	Vector4 col2(92,67,5,1);
	Vector4 col3(102,75,6,1);
	Vector4 col4(119,77,0,1);
	Vector4 col5(69, 25, 18, 1);
	top.SetColours(col1, col2, col2, col3);			//  Set individual colour values of each of those vertices
	
	
	
	Vector4 temp5(-600, 800, 0 + zbase, 1);
	Vector4 temp6(600, 800, 0 + zbase, 1);
	Vector4 temp7(600, 1000, 0 + zbase, 1);
	Vector4 temp8(-600,1000, 0 + zbase,1);			//  Individual vertices for the front face
	
	Vector4 temp9(-800, 1000, 1200 + zbase + 300, 1);
	Vector4 temp10(800, 1000, 1200 + zbase + 300, 1);
	Vector4 temp11(800, 1000, 100, 1);
	Vector4 temp12(-800, 1000, 100,1);				//  Individual vertices for the base face
	table.Initialise(temp9, temp10, temp11, temp12);
	table.SetColours(col5, col5, col5, col5);		//  Set to one mat colour, no shading.
	
	Vector4 temp13(-600, 800, 1200 + zbase, 1);
	Vector4 temp14(-600, 800, 0 + zbase, 1);
	Vector4 temp15(-600, 1000, 0 + zbase, 1);
	Vector4 temp16(-600, 1000, 1200 + zbase, 1);	//  Set individual vertices for the left face
	
	Vector4 temp17(600, 800, 0 + zbase, 1);
	Vector4 temp18(600, 800, 1200 + zbase, 1);
	Vector4 temp19(600, 1000, 1200 + zbase, 1);
	Vector4 temp20(600, 1000, 0 + zbase, 1);		//  Set individual vertices for the right face

	sides[0].Initialise(temp5, temp6, temp7, temp8);
	sides[1].Initialise(temp13, temp14, temp15, temp16);
	sides[2].Initialise(temp17, temp18, temp19, temp20);
	sides[3].Initialise(temp2, temp1, temp16, temp19);		//  Use known vertices to initialise the side faces
	
	sides[0].SetColours(col3, col4, col4, col2);
	sides[1].SetColours(col1, col2, col2, col1);
	sides[2].SetColours(col2, col1, col1, col2);
	sides[3].SetColours(col4, col4, col4, col4);			//  Then set the colours
	
	
	float positionLocations[24][2] =   {{-450, 1050 + zbase},		//1					An array holding the centre locations
										{-300,  900 + zbase},		//2					of each of the positions where pieces
										{-150,  750 + zbase},		//3					can be placed.
										{  0 ,  750 + zbase},		//4
										{ 150,  750 + zbase},		//5
										{ 300,  900 + zbase},		//6
										{ 450, 1050 + zbase},		//7
										{  0 ,  900 + zbase},		//8
										{  0 , 1050 + zbase},		//9
										{-450,  600 + zbase},		//10
										{-300,  600 + zbase},		//11
										{-150,  600 + zbase},		//12
										{-450,  150 + zbase},		//13
										{-300,  300 + zbase},		//14
										{-150,  450 + zbase},		//15
										{  0 ,  150 + zbase},		//16
										{  0 ,  300 + zbase},		//17
										{  0 ,  450 + zbase},		//18
										{ 450,  150 + zbase},		//19
										{ 300,  300 + zbase},		//20
										{ 150,  450 + zbase},		//21
										{ 450,  600 + zbase},		//22
										{ 300,  600 + zbase},		//23
										{ 150,  600 + zbase}};		//24
	for (int i = 0; i < 24; i++) {									
		boardpositions[i].Initialise(positionLocations[i][0], positionLocations[i][1]);			//  Create the individual graphical representations
	}																							//  (class Position) and put in array for easy drawing.
	InitialiseLines();				//  Ugly function to draw each and every line on the
									//  board with individual hand-placed vertices.
									
	SetupMills();					//  Set up the mills (groups of three) with pointers to the positions
									//  as alternate referencing option
	
}

void Board::InitialiseLines()
//  Oh dear.
{

	float zbase = 400;
	{
		Vector4 temp1(-390, 800, 1060 + zbase, 1);
		Vector4 temp2(-60, 800, 1060 + zbase, 1);
		Vector4 temp3(-60, 800, 1040 + zbase, 1);
		Vector4 temp4(-390, 800, 1040 + zbase, 1);
		boardlines[0].Initialise(temp1, temp2, temp3, temp4);
	}
	{
		Vector4 temp1(60, 800, 1060 + zbase, 1);
		Vector4 temp2(390, 800, 1060 + zbase, 1);
		Vector4 temp3(390, 800, 1040 + zbase, 1);
		Vector4 temp4(60, 800, 1040 + zbase, 1);
		boardlines[1].Initialise(temp1, temp2, temp3, temp4);	
	}
	{
		Vector4 temp1(460, 800, 990 + zbase, 1);
		Vector4 temp2(460, 800, 660 + zbase, 1);
		Vector4 temp3(440, 800, 660 + zbase, 1);
		Vector4 temp4(440, 800, 990 + zbase, 1);
		boardlines[2].Initialise(temp1, temp2, temp3, temp4);	
	}
	{
		Vector4 temp1(460, 800, 540 + zbase, 1);
		Vector4 temp2(460, 800, 210 + zbase, 1);
		Vector4 temp3(440, 800, 210 + zbase, 1);
		Vector4 temp4(440, 800, 540 + zbase, 1);
		boardlines[3].Initialise(temp1, temp2, temp3, temp4);	
	}
	{
		Vector4 temp1(60, 800, 160 + zbase, 1);
		Vector4 temp2(390, 800, 160 + zbase, 1);
		Vector4 temp3(390, 800, 140 + zbase, 1);
		Vector4 temp4(60, 800, 140 + zbase, 1);
		boardlines[4].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(-390, 800, 160 + zbase, 1);
		Vector4 temp2(-60, 800, 160 + zbase, 1);
		Vector4 temp3(-60, 800, 140 + zbase, 1);
		Vector4 temp4(-390, 800, 140 + zbase, 1);
		boardlines[5].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(-460, 800, 540 + zbase, 1);
		Vector4 temp2(-440, 800, 540 + zbase, 1);
		Vector4 temp3(-440, 800, 210 + zbase, 1);
		Vector4 temp4(-460, 800, 210 + zbase, 1);
		boardlines[6].Initialise(temp1, temp2, temp3, temp4);	
	}
	{
		Vector4 temp1(-460, 800, 990 + zbase, 1);
		Vector4 temp2(-440, 800, 990 + zbase, 1);
		Vector4 temp3(-440, 800, 660 + zbase, 1);
		Vector4 temp4(-460, 800, 660 + zbase, 1);
		boardlines[7].Initialise(temp1, temp2, temp3, temp4);	
	}
	{
		Vector4 temp1(-240, 800, 910 + zbase, 1);
		Vector4 temp2(-60, 800, 910 + zbase, 1);
		Vector4 temp3(-60, 800, 890 + zbase, 1);
		Vector4 temp4(-240, 800, 890 + zbase, 1);
		boardlines[8].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(60, 800, 910 + zbase, 1);
		Vector4 temp2(240, 800, 910 + zbase, 1);
		Vector4 temp3(240, 800, 890 + zbase, 1);
		Vector4 temp4(60, 800, 890 + zbase, 1);
		boardlines[9].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(290, 800, 840 + zbase, 1);
		Vector4 temp2(310, 800, 840 + zbase, 1);
		Vector4 temp3(310, 800, 660 + zbase, 1);
		Vector4 temp4(290, 800, 660 + zbase, 1);
		boardlines[10].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(290, 800, 540 + zbase, 1);
		Vector4 temp2(310, 800, 540 + zbase, 1);
		Vector4 temp3(310, 800, 360 + zbase, 1);
		Vector4 temp4(290, 800, 360 + zbase, 1);
		boardlines[11].Initialise(temp1, temp2, temp3, temp4);	
	}		
	{
		Vector4 temp1(60, 800, 310 + zbase, 1);
		Vector4 temp2(240, 800, 310 + zbase, 1);
		Vector4 temp3(240, 800, 290 + zbase, 1);
		Vector4 temp4(60, 800, 290 + zbase, 1);
		boardlines[12].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(-240, 800, 310 + zbase, 1);
		Vector4 temp2(-60, 800, 310 + zbase, 1);
		Vector4 temp3(-60, 800, 290 + zbase, 1);
		Vector4 temp4(-240, 800, 290 + zbase, 1);
		boardlines[13].Initialise(temp1, temp2, temp3, temp4);	
	}		
	{
		Vector4 temp1(-290, 800, 540 + zbase, 1);
		Vector4 temp2(-310, 800, 540 + zbase, 1);
		Vector4 temp3(-310, 800, 360 + zbase, 1);
		Vector4 temp4(-290, 800, 360 + zbase, 1);
		boardlines[14].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(-290, 800, 840 + zbase, 1);
		Vector4 temp2(-310, 800, 840 + zbase, 1);
		Vector4 temp3(-310, 800, 660 + zbase, 1);
		Vector4 temp4(-290, 800, 660 + zbase, 1);
		boardlines[15].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(-90, 800, 760 + zbase, 1);
		Vector4 temp2(-60, 800, 760 + zbase, 1);
		Vector4 temp3(-60, 800, 740 + zbase, 1);
		Vector4 temp4(-90, 800, 740 + zbase, 1);
		boardlines[16].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(60, 800, 760 + zbase, 1);
		Vector4 temp2(90, 800, 760 + zbase, 1);
		Vector4 temp3(90, 800, 740 + zbase, 1);
		Vector4 temp4(60, 800, 740 + zbase, 1);
		boardlines[17].Initialise(temp1, temp2, temp3, temp4);	
	}		
	{
		Vector4 temp1(140, 800, 690 + zbase, 1);
		Vector4 temp2(150, 800, 690 + zbase, 1);
		Vector4 temp3(150, 800, 660 + zbase, 1);
		Vector4 temp4(140, 800, 660 + zbase, 1);
		boardlines[18].Initialise(temp1, temp2, temp3, temp4);	
	}		
	{
		Vector4 temp1(140, 800, 540 + zbase, 1);
		Vector4 temp2(150, 800, 540 + zbase, 1);
		Vector4 temp3(150, 800, 510 + zbase, 1);
		Vector4 temp4(140, 800, 510 + zbase, 1);
		boardlines[19].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(60, 800, 460 + zbase, 1);
		Vector4 temp2(90, 800, 460 + zbase, 1);
		Vector4 temp3(90, 800, 440 + zbase, 1);
		Vector4 temp4(60, 800, 440 + zbase, 1);
		boardlines[20].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(-90, 800, 460 + zbase, 1);
		Vector4 temp2(-60, 800, 460 + zbase, 1);
		Vector4 temp3(-60, 800, 440 + zbase, 1);
		Vector4 temp4(-90, 800, 440 + zbase, 1);
		boardlines[21].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(-160, 800, 540 + zbase, 1);
		Vector4 temp2(-140, 800, 540 + zbase, 1);
		Vector4 temp3(-140, 800, 510 + zbase, 1);
		Vector4 temp4(-160, 800, 510 + zbase, 1);
		boardlines[22].Initialise(temp1, temp2, temp3, temp4);	
	}
	{
		Vector4 temp1(-160, 800, 690 + zbase, 1);
		Vector4 temp2(-140, 800, 690 + zbase, 1);
		Vector4 temp3(-140, 800, 660 + zbase, 1);
		Vector4 temp4(-160, 800, 660 + zbase, 1);
		boardlines[23].Initialise(temp1, temp2, temp3, temp4);	
	}
	{
		Vector4 temp1(-10, 800, 840 + zbase, 1);
		Vector4 temp2(10, 800, 840 + zbase, 1);
		Vector4 temp3(10, 800, 810 + zbase, 1);
		Vector4 temp4(-10, 800, 810 + zbase, 1);
		boardlines[24].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(-10, 800, 990 + zbase, 1);
		Vector4 temp2(10, 800, 990 + zbase, 1);
		Vector4 temp3(10, 800, 960 + zbase, 1);
		Vector4 temp4(-10, 800, 960 + zbase, 1);
		boardlines[25].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(210, 800, 610 + zbase, 1);
		Vector4 temp2(240, 800, 610 + zbase, 1);
		Vector4 temp3(240, 800, 590 + zbase, 1);
		Vector4 temp4(210, 800, 590 + zbase, 1);
		boardlines[26].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(360, 800, 610 + zbase, 1);
		Vector4 temp2(390, 800, 610 + zbase, 1);
		Vector4 temp3(390, 800, 590 + zbase, 1);
		Vector4 temp4(360, 800, 590 + zbase, 1);
		boardlines[27].Initialise(temp1, temp2, temp3, temp4);	
	}
	{
		Vector4 temp1(-10, 800, 390 + zbase, 1);
		Vector4 temp2(10, 800, 390 + zbase, 1);
		Vector4 temp3(10, 800, 360 + zbase, 1);
		Vector4 temp4(-10, 800, 360 + zbase, 1);
		boardlines[28].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(-10, 800, 240 + zbase, 1);
		Vector4 temp2(10, 800, 240 + zbase, 1);
		Vector4 temp3(10, 800, 210 + zbase, 1);
		Vector4 temp4(-10, 800, 210 + zbase, 1);
		boardlines[29].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(-390, 800, 610 + zbase, 1);
		Vector4 temp2(-360, 800, 610 + zbase, 1);
		Vector4 temp3(-360, 800, 590 + zbase, 1);
		Vector4 temp4(-390, 800, 590 + zbase, 1);
		boardlines[30].Initialise(temp1, temp2, temp3, temp4);	
	}	
	{
		Vector4 temp1(-240, 800, 610 + zbase, 1);
		Vector4 temp2(-210, 800, 610 + zbase, 1);
		Vector4 temp3(-210, 800, 590 + zbase, 1);
		Vector4 temp4(-240, 800, 590 + zbase, 1);
		boardlines[31].Initialise(temp1, temp2, temp3, temp4);	
	}	
	
}

void Board::SetupMills()
//  Record which position is in which group of three.  This can be used to find neighbouring 
//  positions and can be used to check for any formed mills.
{
	mills[0][0] = &boardpositions[0];
	mills[0][1] = &boardpositions[8];
	mills[0][2] = &boardpositions[6];
	
	mills[1][0] = &boardpositions[6];
	mills[1][1] = &boardpositions[21];
	mills[1][2] = &boardpositions[18];
	
	mills[2][0] = &boardpositions[18];
	mills[2][1] = &boardpositions[15];
	mills[2][2] = &boardpositions[12];
	
	mills[3][0] = &boardpositions[12];
	mills[3][1] = &boardpositions[9];
	mills[3][2] = &boardpositions[0];
	
	mills[4][0] = &boardpositions[1];
	mills[4][1] = &boardpositions[7];
	mills[4][2] = &boardpositions[5];
	
	mills[5][0] = &boardpositions[5];
	mills[5][1] = &boardpositions[22];
	mills[5][2] = &boardpositions[19];
	
	mills[6][0] = &boardpositions[19];
	mills[6][1] = &boardpositions[16];
	mills[6][2] = &boardpositions[13];
	
	mills[7][0] = &boardpositions[13];
	mills[7][1] = &boardpositions[10];
	mills[7][2] = &boardpositions[1];
	
	mills[8][0] = &boardpositions[2];
	mills[8][1] = &boardpositions[3];
	mills[8][2] = &boardpositions[4];
	
	mills[9][0] = &boardpositions[4];
	mills[9][1] = &boardpositions[23];
	mills[9][2] = &boardpositions[20];
	
	mills[10][0] = &boardpositions[20];
	mills[10][1] = &boardpositions[17];
	mills[10][2] = &boardpositions[14];
	
	mills[11][0] = &boardpositions[14];
	mills[11][1] = &boardpositions[11];
	mills[11][2] = &boardpositions[2];
	
	mills[12][0] = &boardpositions[8];
	mills[12][1] = &boardpositions[7];
	mills[12][2] = &boardpositions[3];
	
	mills[13][0] = &boardpositions[21];
	mills[13][1] = &boardpositions[22];
	mills[13][2] = &boardpositions[23];
	
	mills[14][0] = &boardpositions[15];
	mills[14][1] = &boardpositions[16];
	mills[14][2] = &boardpositions[17];
	
	mills[15][0] = &boardpositions[9];
	mills[15][1] = &boardpositions[10];
	mills[15][2] = &boardpositions[11];	
}


void Board::SetPerspective(Matrix4x4 PerspectiveMatrix)
//  Recieve is perspective matrix from the parent class and hand it to the children
{
	top.SetPerspective(PerspectiveMatrix);
	table.SetPerspective(PerspectiveMatrix);
	
	for (int i = 0; i < 4; i++) {
		sides[i].SetPerspective(PerspectiveMatrix);
	}
	
	for (int i = 0; i < 24; i++) {									
		boardpositions[i].SetPerspective(PerspectiveMatrix);
	}
	for (int i = 0; i < 32; i++) {
		boardlines[i].SetPerspective(PerspectiveMatrix);
	}
}

void Board::SetTransformation(Matrix4x4 m)
//  Recieve the transformation matrix from the parent class and hand it to the children
{
	top.SetTransformation(m);
	table.SetTransformation(m);
	for (int i = 0; i < 4; i++) {
		sides[i].SetTransformation(m);
	}	
	for (int i = 0; i < 24; i++) {									
		boardpositions[i].SetTransformation(m);
	}	
	for (int i = 0; i < 32; i++) {
		boardlines[i].SetTransformation(m);
	}	
}

void Board::WorldToScreen()
//  Tell all child objects to convert their 3D vertices to 2D vertices.
{
	top.WorldToScreen();
	table.WorldToScreen();
	for (int i = 0; i < 4; i++) {
		sides[i].WorldToScreen();
	}	
	
	for (int i = 0; i < 24; i++) {									
		boardpositions[i].WorldToScreen();
	}	
	for (int i = 0; i < 32; i++) {
		boardlines[i].WorldToScreen();
	}
	
}

void Board::Render()
//  Draw all the objects to the screen
{
	for (int i = 0; i < 24; i++) {									
		boardpositions[i].Render();
	}
	for (int i = 0; i < 32; i++) {
		boardlines[i].Render();
	}	
	top.Render();

	SortSides();			//  Sort the sides to ensure right draw order.
	
	for (int i = 0; i < 4; i++)
	{
		sides[i].Render();
	}
	table.Render();
}

void Board::SortSides()
//  Sort sides by z-depth to ensure that they get draw in an order that causes no
//  visible overlap.
{
	for(int i=0;i<4;i++)
    {
		for(int j=0;j<i;j++)
        {
			if(sides[i].GetCentreZ()<sides[j].GetCentreZ())
            {
                PS2Sprite3D temp = sides[i]; //swap 
                sides[i]=sides[j];
                sides[j]=temp;
            }
        }
    }
}