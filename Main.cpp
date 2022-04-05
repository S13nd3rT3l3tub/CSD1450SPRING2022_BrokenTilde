/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       Main.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Added font loading and free-ing for game usage
*//*_________________________________________________________________________*/

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

	//int * pi = new int;
	////delete pi;


	// Initialize the system
	//AESysInit (instanceH, show, AEGetWindowWidth(), AEGetWindowHeight(), 1, 60, false, NULL);
	AESysInit(instanceH, show, winWidth, winHeight, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("Ricochet");

	//set background color
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Load font
	g_font12 = AEGfxCreateFont(".\\Resources\\Fonts\\Roboto-Regular.ttf", 12);
	g_font20 = AEGfxCreateFont(".\\Resources\\Fonts\\Roboto-Regular.ttf", 20);
	g_font30 = AEGfxCreateFont(".\\Resources\\Fonts\\Roboto-Regular.ttf", 30);

	g_chosenLevel = 0;

	GameStateMgrInit(GS_SPLASHSCREEN);

	FMOD_RESULT result{};
	result = FMOD::System_Create(&fmodSys);      // Create the main system object.
	if (result != FMOD_OK){
		//printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return -1;
	}
	result = fmodSys->init(5, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (result != FMOD_OK){
		//printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return -1;
	}

	fmodSys->createSound(".\\Resources\\Sounds\\Change.wav", FMOD_LOOP_NORMAL, nullptr, &mainMenuBG);
	//fmodSys->playSound(mainMenuBG, nullptr, false, nullptr);

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
		else
			gGameStateNext = gGameStateCurr = gGameStatePrev;

		// Initialize the gamestate
		GameStateInit();

		while(gGameStateCurr == gGameStateNext)
		{
			AESysFrameStart();

			AEInputUpdate();

			GameStateUpdate();

			GameStateDraw();
			
			AESysFrameEnd();

			// check if forcing the application to quit
			if ((AESysDoesWindowExist() == false))
				gGameStateNext = GS_QUIT;

			g_dt = static_cast<float>(AEFrameRateControllerGetFrameTime()) < 1.0f / 60.0f ? static_cast<float>(AEFrameRateControllerGetFrameTime()) : 1.0f / 60.0f;
			AEInputGetCursorPosition(&g_mouseX, &g_mouseY);
			g_appTime += g_dt;
		}
		
		GameStateFree();

		if(gGameStateNext != GS_RESTART)
			GameStateUnload();

		gGameStatePrev = gGameStateCurr;
		gGameStateCurr = gGameStateNext;
	}

	// Free font
	AEGfxDestroyFont(g_font12);
	AEGfxDestroyFont(g_font20);
	AEGfxDestroyFont(g_font30);

	// Free the fMod Sys
	fmodSys->release();

	// free the system
	AESysExit();
}