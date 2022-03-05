/******************************************************************************/
/*!
\file		Main.cpp
\author 	DigiPen
\par    	email: digipen\@digipen.edu
\date   	January 01, 20xx
\brief		ToDo: give a brief explanation here

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/

#include "main.h"
#include <memory>
#include <iostream>

// ---------------------------------------------------------------------------
// Globals
float	 g_dt;
double	 g_appTime;
s8		 g_font12;
s8		 g_font20;
int		 g_mouseX;
int		 g_mouseY;

int winWidth{ 1280 }, winHeight{ 720 };

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
	//#if defined(DEBUG) | defined(_DEBUG)
	//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//#endif

	//int * pi = new int;
	////delete pi;


	// Initialize the system
	//AESysInit (instanceH, show, AEGetWindowWidth(), AEGetWindowHeight(), 1, 60, false, NULL);
	AESysInit(instanceH, show, winWidth, winHeight, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("Broken Tilde Prototype");

	//set background color
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Load font
	g_font12 = AEGfxCreateFont("Roboto-Regular.ttf", 12);
	g_font20 = AEGfxCreateFont("Roboto-Regular.ttf", 20);

	GameStateMgrInit(GS_LEVEL1);

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
			if ((AESysDoesWindowExist() == false) || AEInputCheckTriggered(AEVK_ESCAPE))
				gGameStateNext = GS_QUIT;

			g_dt = (f32)AEFrameRateControllerGetFrameTime();
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

	// free the system
	AESysExit();
}