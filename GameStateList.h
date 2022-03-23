/******************************************************************************/
/*!
\file		GameStateList.h
\author 	DigiPen
\par    	email: digipen\@digipen.edu
\date   	January 01, 20xx
\brief		ToDo: give a brief explanation here

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/

#ifndef GAME_STATE_LIST_H_
#define GAME_STATE_LIST_H_

// ---------------------------------------------------------------------------
// game state list

enum
{
	// list of all game states 
	GS_MAINMENU = 0,
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