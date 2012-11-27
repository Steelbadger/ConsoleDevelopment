#include "MyPS2Application.h"
#include <stdio.h>
#include <sps2tags.h>
#include <sps2regstructs.h>
#include <signal.h>
#include "PS2Defines.h"
#include "pad.h"
#include "sps2wrap.h"
#include "dma.h"
#include "texture.h"
#include "MorrisManager.h"

bool MyPS2Application::quitting_ = false;

void sig_handle(int sig)
{
	MyPS2Application::set_quitting(true);
}

MyPS2Application::MyPS2Application()
{
}

void MyPS2Application::Run()
{
	Init();
	while(!quitting_)
	{
		Update();
		Render();	
	}
	CleanUp();
}

void MyPS2Application::Init()
{
	// Initialise control pad 0
	if(!pad_init(PAD_0, PAD_INIT_LOCK | PAD_INIT_ANALOGUE | PAD_INIT_PRESSURE))
	{
		printf("Failed to initialise control pad\n");
		pad_cleanup(PAD_0);
		exit(0);
	}
	pads = 2;
	if(!pad_init(PAD_1, PAD_INIT_LOCK | PAD_INIT_ANALOGUE | PAD_INIT_PRESSURE))
	{
		printf("Failed to initialise control pad2\n");
		pad_cleanup(PAD_1);
		pads = 1;
	}		
	
	
	// Allocate memory for the graphics data
	SPS2Manager.Initialise(1024);	// 1024 * 4K Pages = 4MB Total
	VIFStaticDMA.Initialise(512);	// 512 * 4K Pages = 2MB Static DMA
	VIFDynamicDMA.Initialise(256);	// 256 * 4K Pages * 2 Buffers =
									// 2MB Dynamic DMA
																	
	// Register our signal function for every possible signal (e.g. ctrl + c)
	for(int sig=0; sig<128; sig++)
		signal(sig, sig_handle);

	// Define the clear screen colour. We want to clear to blueish.
	SPS2Manager.InitScreenClear(0, 0, 0);
	
	// Load the font bitmap and data
	CTexture font_tex;
	if(!font_.Load("PS2Framework/font.dat", true))
	{
		quitting_ = true;
		printf("Can't load font.dat\n");
	}
	if(!font_tex.LoadBitmap("PS2Framework/font.bmp", false, true))
	{
		printf("Can't load font.bmp\n");
		quitting_ = false;
	}

	// Upload these once to VRAM since
	// they are not going to change
	font_tex.Upload(TEXBUF496);
	font_tex.Select();
	gameManager.SetPads(pads);
	gameManager.SetupText(font_);
}

void MyPS2Application::CleanUp()
{
	SPS2Manager.CleanUp();
	set_actuator(PAD_0, 0, 0);
	pad_cleanup(PAD_0);
	if (pads == 2) {
		pad_cleanup(PAD_1);
		set_actuator(PAD_1, 0, 0);
	}
}

void MyPS2Application::Update()
{
	// Tell DMAC to send previous graphics data to Graphics Synthesiser (GS)
	VIFDynamicDMA.Fire();
	
	// Read the control pad into data buffer
	pad_update(PAD_0);
	pad_update(PAD_1);
	
	// Check for exit condition
	if((pad[0].buttons & PAD_START)&&(pad[0].buttons & PAD_SELECT)) {
		quitting_ = true;
	}
	if (pads == 2) {
		if((pad[1].buttons & PAD_START)&&(pad[1].buttons & PAD_SELECT)) {
			quitting_ = true;
		}	
	}
	
	gameManager.Update();
	
	
}

void MyPS2Application::Render()
{
	// All drawing commands should be between BeginScene and EndScene
	SPS2Manager.BeginScene();
	gameManager.Render();
	
	
//	font_.printfL(  -300, -240, 127, 127, 127, 127, "My Second PS2 Graphics Program!");
//	font_.printfR(	text_x, text_y, 127, 0, 127, 127, "X: %f  Y: %f", sprite_.GetX(), sprite_.GetY());	

	SPS2Manager.EndScene();			
}