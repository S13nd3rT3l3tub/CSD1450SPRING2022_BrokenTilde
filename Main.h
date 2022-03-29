/******************************************************************************/
/*!
\file		Main.h
\author 	DigiPen
\par    	email: digipen\@digipen.edu
\date   	January 01, 20xx
\brief		ToDo: give a brief explanation here

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/


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











