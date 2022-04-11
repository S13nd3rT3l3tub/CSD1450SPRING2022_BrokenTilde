/*!
@copyright	All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
@project	Ricochet
@file       GameStateMgr.h
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Initial creation 
*//*_________________________________________________________________________*/

// ----- Include Guard -----
#ifndef GAME_STATE_MGR_H_
#define GAME_STATE_MGR_H_

// ----- Include Files -----
#include "AEEngine.h"
#include "GameStateList.h"	// include the list of game states

// ---------------------------------------------------------------------------
// Game State Globals
extern unsigned int gGameStateInit;
extern unsigned int gGameStateCurr;
extern unsigned int gGameStatePrev;
extern unsigned int gGameStateNext;
extern unsigned int gGameStateInnerState;

// ---------------------------------------------------------------------------
// Game State Function Pointers
extern void (*GameStateLoad)();
extern void (*GameStateInit)();
extern void (*GameStateUpdate)();
extern void (*GameStateDraw)();
extern void (*GameStateFree)();
extern void (*GameStateUnload)();

// ---------------------------------------------------------------------------
// Function prototypes

// call this at the beginning and AFTER all game states are added to the manager
void GameStateMgrInit(unsigned int gameStateInit);

// update is used to set the function pointers
void GameStateMgrUpdate();

// ---------------------------------------------------------------------------

#endif