/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       BinaryMap.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Authored Functions
*//*_________________________________________________________________________*/

#ifndef GAME_STATE_MAINMENU_H_
#define GAME_STATE_MAINMENU_H_

// ---------------------------------------------------------------------------

void GameStateMainMenuLoad(void);
void GameStateMainMenuInit(void);
void GameStateMainMenuUpdate(void);
void GameStateMainMenuDraw(void);
void GameStateMainMenuFree(void);
void GameStateMainMenuUnload(void);

enum overlay
{
	main = 0
};

// ---------------------------------------------------------------------------

#endif 


