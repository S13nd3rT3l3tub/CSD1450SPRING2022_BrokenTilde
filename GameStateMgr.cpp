/******************************************************************************/
/*!
\file		GameStateMgr.cpp
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

// ---------------------------------------------------------------------------
// globals

// variables to keep track the current, previous and next game state
unsigned int	gGameStateInit;
unsigned int	gGameStateCurr;
unsigned int	gGameStatePrev;
unsigned int	gGameStateNext;

// pointer to functions for game state life cycles functions
void (*GameStateLoad)()		= 0;
void (*GameStateInit)()		= 0;
void (*GameStateUpdate)()	= 0;
void (*GameStateDraw)()		= 0;
void (*GameStateFree)()		= 0;
void (*GameStateUnload)()	= 0;

/******************************************************************************/
/*!

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

	// call the update to set the function pointers
	GameStateMgrUpdate();
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateMgrUpdate()
{
	if ((gGameStateCurr == GS_RESTART) || (gGameStateCurr == GS_QUIT))
		return;

	switch (gGameStateCurr)
	{
	case GS_MAINMENU:
		GameStateLoad = GameStateMainMenuLoad;
		GameStateInit = GameStateMainMenuInit;
		GameStateUpdate = GameStateMainMenuUpdate;
		GameStateDraw = GameStateMainMenuDraw;
		GameStateFree = GameStateMainMenuFree;
		GameStateUnload = GameStateMainMenuUnload;
		break;
	case GS_WINSCREEN:
		GameStateLoad = GameStateWinLoad;
		GameStateInit = GameStateWinInit;
		GameStateUpdate = GameStateWinUpdate;
		GameStateDraw = GameStateWinDraw;
		GameStateFree = GameStateWinFree;
		GameStateUnload = GameStateWinUnload;
		break;
	case GS_LEVEL1:
		GameStateLoad = GameStateLevel1Load;
		GameStateInit = GameStateLevel1Init;
		GameStateUpdate = GameStateLevel1Update;
		GameStateDraw = GameStateLevel1Draw;
		GameStateFree = GameStateLevel1Free;
		GameStateUnload = GameStateLevel1Unload;
		break;
	case GS_LEVELS:
		GameStateLoad = GameStateLevelsLoad;
		GameStateInit = GameStateLevelsInit;
		GameStateUpdate = GameStateLevelsUpdate;
		GameStateDraw = GameStateLevelsDraw;
		GameStateFree = GameStateLevelsFree;
		GameStateUnload = GameStateLevelsUnload;
		break;
	default:
		AE_FATAL_ERROR("invalid state!!");
	}
}
