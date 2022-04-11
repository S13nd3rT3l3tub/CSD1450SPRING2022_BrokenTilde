/*!
@copyright	All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
@project	Ricochet
@file       GameStateList.h
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Initial creation. Added splashscreen after creation
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu)
@role		Added state for levels and win screen
*//*_________________________________________________________________________*/

// ----- Include Guard ------
#ifndef GAME_STATE_LIST_H_
#define GAME_STATE_LIST_H_

// ---------------------------------------------------------------------------
// game state list

enum 
{
	// list of all game states 
	GS_SPLASHSCREEN,		// Splash screen
	GS_MAINMENU,			// Main menu
	GS_LEVEL1,				// Tutorial/Introductory evel (a.k.a. Level 1)
	GS_LEVELS,				// Other levels
	GS_WINSCREEN,			// Win screen

	// special game state. Do not change
	GS_RESTART,
	GS_QUIT, 
	GS_NONE
};

#endif // GAME_STATE_LIST_H_