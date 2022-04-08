/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       Main.h
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu)
@role
*//*_________________________________________________________________________*/

// ----- Include Guard -----
#ifndef _MAIN_H_
#define _MAIN_H_

// ---------------------------------------------------------------------------
// File Includes
#include <windowsx.h>
#include <fmod.hpp>
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
#include "GameState_SplashScreen.h"
#include "GameState_MainMenu.h"
#include "GameState_Level1.h"
#include "GameState_Levels.h"
#include "GameState_Win.h"

/******************************************************************************/
/*!
	Main Globals
*/
/******************************************************************************/
// ----- Time -----
extern float	g_dt;				// Delta time
extern double	g_appTime;			// Application time
extern double	levelTime;			// Level time

// ----- Font -----
extern s8		g_font12;			// Font size 12
extern s8		g_font20;			// Font size 20
extern s8		g_font30;			// Font size 30

// ----- Mouse -----
extern int		g_mouseX;			// Mouse window pos X
extern int		g_mouseY;			// Mouse window pos Y
extern float	worldMouseX;		// Mouse world pos X
extern float	worldMouseY;		// Mouse world pos Y

// ----- Window -----
extern bool		winFocused;			// Window focused flag variable
extern bool		toFullScreen;		// Window fullscreen flag variable
const int		winWidth{ 1280 };	// Window width
const int		winHeight{ 720 };	// Window height

// ----- Sound -----
extern bool				soundVolumeLevel;	// Sound volume flag variable
extern FMOD::System* fModSys;			// FMOD system
extern FMOD::Sound* mainMenuBG;			// Background sound pointer
extern FMOD::Sound* playerShoot;		// Projectile shooting sound pointer 
extern FMOD::Channel* soundChannel;		// FMOD sound channel pointer


#endif











