/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       BinaryMap.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Authored Functions
-------------------------------------------------------------------------------
@author		Mohamed Zafir (m.zafir@digipen.edu)
@role		Authored Functions
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu)
@role		Authored Functions
*//*_________________________________________________________________________*/

#ifndef GAME_STATE_LIST_H_
#define GAME_STATE_LIST_H_

// ---------------------------------------------------------------------------
// game state list

enum
{
	// list of all game states 
	GS_SPLASHSCREEN,
	GS_MAINMENU,
	GS_LEVEL1, 
	GS_LEVELS,
	GS_WINSCREEN,
	// special game state. Do not change
	GS_RESTART,
	GS_QUIT, 
	GS_NONE
};

// ---------------------------------------------------------------------------

#endif // GAME_STATE_LIST_H_