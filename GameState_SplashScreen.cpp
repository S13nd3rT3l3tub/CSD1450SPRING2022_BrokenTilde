/******************************************************************************/
/*!
\file		GameState_MainMenu.cpp
\author 	DigiPen
\par    	email: digipen\@digipen.edu
\date   	January 01, 20xx
\brief		ToDo: give a brief explanation here

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Main.h"

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
AEVec2		SPLASH_MESHSIZE = { 1225.0f, 380.0f };

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
float timer;

AEGfxTexture* digipenLogo;
AEGfxVertexList* splashMesh;



/******************************************************************************/
/*!
	Helper Functions
*/
/******************************************************************************/

/******************************************************************************/
/*!
	"Load" function of this state
*/
/******************************************************************************/

void GameStateSplashScreenLoad(void) {
	digipenLogo = AEGfxTextureLoad(".\\Resources\\Assets\\DigiPen.png");
	AE_ASSERT_MESG(digipenLogo, "failed to load digipen logo texture");

	// =========================
	// create the Splashscreen mesh
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
	AE_ASSERT_MESG(splashMesh, "fail to create SPLASH object!!");

	// Move camera to 0,0
	AEGfxSetCamPosition(0.0f, 0.0f);


	// Set fullscreen to true
	AEToogleFullScreen(toFullScreen);
}

void GameStateSplashScreenInit(void) {
	// Set fullscreen to true
	AEToogleFullScreen(toFullScreen);

	// Initialize timer
	timer = 3.0f;
}
void GameStateSplashScreenUpdate(void) {
	timer -= g_dt;
	
	if (AEInputCheckReleased(VK_LBUTTON) || AEInputCheckReleased(VK_RBUTTON))
		timer = -1.0f;

	if (AEInputCheckCurr(AEVK_LALT) || AEInputCheckCurr(AEVK_RALT))
		std::cout << "Issue key\n";

	if (timer < 0.0f)
		gGameStateNext = GS_MAINMENU;
}

void GameStateSplashScreenDraw(void) {
	//char strBuffer[1024];

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);
	AEGfxSetPosition(0.0f, 0.0f);
	AEGfxTextureSet(digipenLogo, 0.0f, 0.0f);
	AEGfxMeshDraw(splashMesh, AE_GFX_MDM_TRIANGLES);
}

void GameStateSplashScreenFree(void) {

}

void GameStateSplashScreenUnload(void) {
	AEGfxTextureUnload(digipenLogo);
	AEGfxMeshFree(splashMesh);
}
