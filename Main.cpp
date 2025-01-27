/*!
@copyright	All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
@project	Ricochet
@file       Main.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu) [~100% of code]
@role		Initial creation & loading of fonts, FMOD sound initialization & 
			Alt-Tab sminimization handling
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu)	[~15% of code]
@role		Assisted with Alt-Tab minimization handling
*//*_________________________________________________________________________*/


// ----- Include Files -----
#include "Main.h"
#include <memory>
#include <iostream>

/******************************************************************************/
/*!
	Starting point of the application
*/
/******************************************************************************/
int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);

	//// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	// Initialize the system & window
	AESysInit(instanceH, show, winWidth, winHeight, 0, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("Ricochet");

	// Set background color
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// ==============================
	// Load font of different sizes
	// ==============================
	{
		g_font12 = AEGfxCreateFont(".\\Resources\\Fonts\\Roboto-Regular.ttf", 12);
		g_font20 = AEGfxCreateFont(".\\Resources\\Fonts\\Roboto-Regular.ttf", 20);
		g_font30 = AEGfxCreateFont(".\\Resources\\Fonts\\Roboto-Regular.ttf", 30);
	}

	// Initialize chosenLevel to 0
	g_chosenLevel = 0;

	// Initialize game state manager and initialize to the given start state
	GameStateMgrInit(GS_SPLASHSCREEN);


	// ==============================
	// Create & initialize FMOD system
	// =============================={
	// Create
	if (FMOD::System_Create(&fModSys) != FMOD_OK){
		//printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return -1;
	}
	// Initialize 
	if (fModSys->init(1, FMOD_INIT_NORMAL, 0) != FMOD_OK){
		//printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return -1;
	}

	// Initialize Sounds using FMOD
	{
		fModSys->createSound(".\\Resources\\Sounds\\CRITICAL_MASS_-_Corporate_MSCCRP1_31.wav", FMOD_LOOP_NORMAL, nullptr, &mainMenuBG);
		fModSys->createSound(".\\Resources\\Sounds\\GunCartoon_CTE01_62.2.wav", FMOD_DEFAULT, nullptr, &playerShoot);
	}

	// Set fullscreen mode
	AEToogleFullScreen(toFullScreen);

	// Program Loop
	while(gGameStateCurr != GS_QUIT)
	{
		// reset the system modules
		AESysReset();

		// If not restarting, load the gamestate
		if(gGameStateCurr != GS_RESTART)
		{
			GameStateMgrUpdate();
			GameStateLoad();
		}
		else    // Update game state variables
			gGameStateNext = gGameStateCurr = gGameStatePrev;

		// Initialize the gamestate
		GameStateInit();

		// Game State Loop
		while(gGameStateCurr == gGameStateNext){
			// Call frame start
			AESysFrameStart();

			// Update input
			AEInputUpdate();

			// Update window focused mode
			winFocused = (AESysGetWindowHandle() == GetFocus());
			// Decide whether to show window
			ShowWindow(AESysGetWindowHandle(), winFocused ? SW_SHOW : SW_MINIMIZE);
			
			// Call game state's update
			GameStateUpdate();

			// Call game state's draw
			GameStateDraw();
			
			// Call frame end
			AESysFrameEnd();

			// check if forcing the application to quit
			if ((AESysDoesWindowExist() == false))
				gGameStateNext = GS_QUIT;

			// Update global delta time
			g_dt = static_cast<float>(AEFrameRateControllerGetFrameTime()) < 1.0f / 60.0f ? static_cast<float>(AEFrameRateControllerGetFrameTime()) : 1.0f / 60.0f;
			
			// Update global mouse position
			AEInputGetCursorPosition(&g_mouseX, &g_mouseY);

			// Update application time
			g_appTime += g_dt;


		}
		// Call game state's free
		GameStateFree();

		if(gGameStateNext != GS_RESTART)
			GameStateUnload();	// Call game state's unload

		gGameStatePrev = gGameStateCurr;
		gGameStateCurr = gGameStateNext;
	}

	// ==============================
	// Free fonts used
	// ==============================
	{
		AEGfxDestroyFont(g_font12);
		AEGfxDestroyFont(g_font20);
		AEGfxDestroyFont(g_font30);
	}

	// Free created FMOD system
	fModSys->release();

	// Free the system
	AESysExit();

	// Return 0
	return 0;
}