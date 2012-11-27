#ifndef _MYPS2APPLICATION_H
#define _MYPS2APPLICATION_H

#include "primitives.h"
#include "font.h"
#include "ps2matrix4x4.h"
#include "MorrisManager.h"
#ifndef _BOARD_H_
#include "Board.h"
#endif

class MyPS2Application
{
public:
	MyPS2Application();
	void Run();
	static void set_quitting(bool quitting) { quitting_ = quitting; }
	static bool quitting() { return quitting_; }
private:
	void Init();
	void CleanUp();
	void Update();
	void Render();
	
	//  game loop boolean
	static bool quitting_;
	int pads;
	
	// Font Class
	CFont font_;

	// declare a sprite for use.
	MorrisManager gameManager;	
};

#endif // _MYPS2APPLICATION_H