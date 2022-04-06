/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       GameState_SplashScreen.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Authored Functions
*//*_________________________________________________________________________*/

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

	fmodSys->update();
}

void GameStateSplashScreenDraw(void) {
	

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);
	AEGfxSetPosition(0.0f, 0.0f);
	AEGfxTextureSet(digipenLogo, 0.0f, 0.0f);
	AEGfxMeshDraw(splashMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	
	char strBuffer[100];
	float textWidth{}, textHeight{};
	memset(strBuffer, 0, 100 * sizeof(char));

	sprintf_s(strBuffer, "All Content Copyright 2022 DigiPen Institute of Technology Singapore, All Rights Reserved.");
	AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, textWidth, textHeight);
	AEGfxPrint(g_font12, strBuffer, 0.0f - textWidth / 2, -0.9f - textHeight / 2, 1.0f, 1.f, 1.f, 1.f);
}

void GameStateSplashScreenFree(void) {

}

void GameStateSplashScreenUnload(void) {
	AEGfxTextureUnload(digipenLogo);
	AEGfxMeshFree(splashMesh);
}
