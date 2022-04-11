/*!
@copyright	All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
@project	Ricochet
@file       GameStateMgr.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Initial Creation. Added Splashscreen after creation
-------------------------------------------------------------------------------
@author     Leong Wai Kit (l.waikit@digipen.edu)
@role		Added pointers to Levels state for levels beyond the tutorial and 
			pointers to the Win state
*//*_________________________________________________________________________*/

// ----- Include Files -----
#include "Main.h"

// ---------------------------------------------------------------------------
// Game State Globals
// 
// variables to keep track the current, previous, next & inner game state
unsigned int	gGameStateInit;
unsigned int	gGameStateCurr;
unsigned int	gGameStatePrev;
unsigned int	gGameStateNext;
unsigned int	gGameStateInnerState;

// pointer to functions for game state life cycles functions
void (*GameStateLoad)()		= 0;
void (*GameStateInit)()		= 0;
void (*GameStateUpdate)()	= 0;
void (*GameStateDraw)()		= 0;
void (*GameStateFree)()		= 0;
void (*GameStateUnload)()	= 0;

/******************************************************************************/
/*!
	Game State Manager Initialize Function
*/
/******************************************************************************/
void GameStateMgrInit(unsigned int gameStateInit) 
{
	// set the initial game state
	gGameStateInit = gameStateInit;

	// reset the current, previoud and next game
	gGameStateCurr = 
	gGameStatePrev = 
	gGameStateNext = gGameStateInit;

	// Set the innerState to be play state
	gGameStateInnerState = GAME_PLAY;

	// call the update to set the function pointers
	GameStateMgrUpdate();
}

/******************************************************************************/
/*!
	Game State Manager Update Function
*/
/******************************************************************************/
void GameStateMgrUpdate() 
{
	// Check if current state is to restart or quit
	if ((gGameStateCurr == GS_RESTART) || (gGameStateCurr == GS_QUIT))
		return;

	// Switch assignment of function pointers based on current state
	switch (gGameStateCurr){
		// Splash Screen
		case GS_SPLASHSCREEN:{
			GameStateLoad = GameStateSplashScreenLoad;
			GameStateInit = GameStateSplashScreenInit;
			GameStateUpdate = GameStateSplashScreenUpdate;
			GameStateDraw = GameStateSplashScreenDraw;
			GameStateFree = GameStateSplashScreenFree;
			GameStateUnload = GameStateSplashScreenUnload;
			break;
		}
		// Main Menu
		case GS_MAINMENU: {
			GameStateLoad = GameStateMainMenuLoad;
			GameStateInit = GameStateMainMenuInit;
			GameStateUpdate = GameStateMainMenuUpdate;
			GameStateDraw = GameStateMainMenuDraw;
			GameStateFree = GameStateMainMenuFree;
			GameStateUnload = GameStateMainMenuUnload;
			break;
		}
		// Win Screen 
		case GS_WINSCREEN: {
			GameStateLoad = GameStateWinLoad;
			GameStateInit = GameStateWinInit;
			GameStateUpdate = GameStateWinUpdate;
			GameStateDraw = GameStateWinDraw;
			GameStateFree = GameStateWinFree;
			GameStateUnload = GameStateWinUnload;
			break;
		}
		// Tutorial/Introductory Level (Level 1)
		case GS_LEVEL1: {
			GameStateLoad = GameStateLevel1Load;
			GameStateInit = GameStateLevel1Init;
			GameStateUpdate = GameStateLevel1Update;
			GameStateDraw = GameStateLevel1Draw;
			GameStateFree = GameStateLevel1Free;
			GameStateUnload = GameStateLevel1Unload;
			break; 
		}
		// Any other level
		case GS_LEVELS: {
			GameStateLoad = GameStateLevelsLoad;
			GameStateInit = GameStateLevelsInit;
			GameStateUpdate = GameStateLevelsUpdate;
			GameStateDraw = GameStateLevelsDraw;
			GameStateFree = GameStateLevelsFree;
			GameStateUnload = GameStateLevelsUnload;
			break;
		}
		// None of the above
		default: {
			AE_FATAL_ERROR("invalid state!!");
			break;
		}
	}
}
