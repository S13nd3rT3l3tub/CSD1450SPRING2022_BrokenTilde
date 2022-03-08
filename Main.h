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
#include "AEEngine.h"
#include "Math.h"
#include <string>
#include <iostream>
#include <fstream>


#include "GameStateMgr.h"
#include "GameState_MainMenu.h"
#include "GameState_Level1.h"
#include "Collision.h"
//#include "BinaryMap.h"

//------------------------------------
// Globals

extern float	g_dt;
extern double	g_appTime;
extern s8		g_font12;
extern s8		g_font20;
extern int		g_mouseX;
extern int		g_mouseY;
extern int		g_chosenLevel;

#endif











