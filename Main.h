/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       Main.h
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Added font variables for game usage
-------------------------------------------------------------------------------
@author
@role
*//*_________________________________________________________________________*/


#ifndef _MAIN_H_
#define _MAIN_H_

// ---------------------------------------------------------------------------
// includes
//#include <SFML>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include "AEEngine.h"
#include "Math.h"

#include "GameStateMgr.h"
#include "Collision.h"
#include "Globals.h"
#include "BinaryMap.h"
#include "GameState_MainMenu.h"
#include "GameState_Level1.h"
#include "GameState_Levels.h"
#include "GameState_Win.h"


//------------------------------------
// Globals

extern float	g_dt;
extern double	g_appTime;
extern double	levelTime;
extern s8		g_font12;
extern s8		g_font20;
extern int		g_mouseX;
extern int		g_mouseY;
extern int		g_chosenLevel;

#endif











