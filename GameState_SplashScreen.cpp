/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       GameState_SplashScreen.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		
-------------------------------------------------------------------------------
@author		Mohamed Zafir (m.zafir@digipen.edu)
@role		
*//*_________________________________________________________________________*/

// ----- Include Files ------
#include "Main.h"

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
AEVec2		SPLASH_MESHSIZE{ 1225.0f, 380.0f };		// Splashscreen mesh size

/******************************************************************************/
/*!
	Enums/Struct/Class Definitions
*/
/******************************************************************************/

/******************************************************************************/
/*!
	(Static) Variables
*/
/******************************************************************************/
AEGfxTexture*		digipenLogoTexture{};	// Digipen splashscreen texture
AEGfxVertexList*	splashMesh{};			// Mesh for splashscreen
float				timer{};				// Timer for the state

/******************************************************************************/
/*!
	"Load" function of this state
*/
/******************************************************************************/
void GameStateSplashScreenLoad(void) {
	// Load splash screen texture and assert if it was successful
	digipenLogoTexture = AEGfxTextureLoad(".\\Resources\\Assets\\DigiPen.png");
	AE_ASSERT_MESG(digipenLogoTexture, "failed to load digipen logo texture");
	
	// =========================
	// Create the Splashscreen mesh
	// =========================
	AEGfxMeshStart();
	AEGfxTriAdd(
		-SPLASH_MESHSIZE.x / 2, -SPLASH_MESHSIZE.y / 2, 0x00FFFFFF, 0.0f, 1.0f,
		SPLASH_MESHSIZE.x / 2, -SPLASH_MESHSIZE.y / 2, 0x00FFFFFF, 1.0f, 1.0f,
		-SPLASH_MESHSIZE.x / 2, SPLASH_MESHSIZE.y / 2, 0x00FFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		SPLASH_MESHSIZE.x / 2, -SPLASH_MESHSIZE.y / 2, 0x00FFFFFF, 1.0f, 1.0f,
		SPLASH_MESHSIZE.x / 2, SPLASH_MESHSIZE.y / 2, 0x00FFFFFF, 1.0f, 0.0f,
		-SPLASH_MESHSIZE.x / 2, SPLASH_MESHSIZE.y / 2, 0x00FFFFFF, 0.0f, 0.0f);
	splashMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(splashMesh, "fail to create Splashscreen object!!");

	// Move camera to 0,0
	AEGfxSetCamPosition(0.0f, 0.0f);
}

/******************************************************************************/
/*!
	"Initialize" function of this state
*/
/******************************************************************************/
void GameStateSplashScreenInit(void) {
	// Initialize timer to 3 seconds
	timer = 3.0f;
}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateSplashScreenUpdate(void) {
	// Switch logic based on current InnerState
	switch (gGameStateInnerState) {
		// State Pause
		case GAME_PAUSE: {
			// Check if window is focused
			if (winFocused)
				gGameStateInnerState = GAME_PLAY;	// Update InnerState to Play state

			break;
		}
		// State Play
		case GAME_PLAY: {
			// Check if window is unfocused
			if (winFocused == false) {
				gGameStateInnerState = GAME_PAUSE;		// Update InnerState to Pause state
				break;									// Break out of switch case
			}

			// Decrement timer counter
			timer -= g_dt;

			// Check if certain keys was pressed to skip the splash screen
			if (AEInputCheckReleased(VK_LBUTTON) || AEInputCheckReleased(VK_RBUTTON) || AEInputCheckReleased(AEVK_RETURN) || AEInputCheckReleased(AEVK_SPACE) || AEInputCheckReleased(AEVK_ESCAPE))
				timer = -1.0f;	// Set timer counter to negative

			// Check if timer is negative
			if (timer < 0.0f)
				gGameStateNext = GS_MAINMENU;	// Update next state to Main Menu

			// Update sound system
			fModSys->update();
			break;
		}
	}
}

/******************************************************************************/
/*!
	"Draw" function of this state
*/
/******************************************************************************/
void GameStateSplashScreenDraw(void) {
	// ----- Set render settings ------
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);			// Background color
	AEGfxSetBlendMode(AE_GFX_BM_NONE);					// Blend mode
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);				// Render mode
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);			// Tint color settings
	AEGfxSetPosition(0.0f, 0.0f);						// Render at 0,0
	AEGfxTextureSet(digipenLogoTexture, 0.0f, 0.0f);	// Set texture to be digipen logo splash
	AEGfxMeshDraw(splashMesh, AE_GFX_MDM_TRIANGLES);	// Render the mesh

	// Set blend mode back
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// ----- Draw Copyright Text ------
	char strBuffer[100];
	float textWidth{}, textHeight{};
	memset(strBuffer, 0, 100 * sizeof(char));
	sprintf_s(strBuffer, "All Content Copyright 2022 DigiPen Institute of Technology Singapore, All Rights Reserved.");
	AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, textWidth, textHeight);
	AEGfxPrint(g_font12, strBuffer, 0.0f - textWidth / 2, -0.9f - textHeight / 2, 1.0f, 1.f, 1.f, 1.f);
}

/******************************************************************************/
/*!
	"Free" function of this state
*/
/******************************************************************************/
void GameStateSplashScreenFree(void) {

}

/******************************************************************************/
/*!
	"Unload" function of this state
*/
/******************************************************************************/
void GameStateSplashScreenUnload(void) {
	// Free digipen logo splash
	AEGfxTextureUnload(digipenLogoTexture);
	
	// Free splash mesh
	AEGfxMeshFree(splashMesh);
}
