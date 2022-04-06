/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       GameState_MainMenu.cpp
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

#include "Main.h"

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/


/******************************************************************************/
/*!
	Enums/Struct/Class Definitions
*/
/******************************************************************************/
enum BUTTON_TYPE {
	START_GAME = 1,
	OPTIONS,
	CREDITS,
	EXIT_GAME,
	RETURN,
	TOGGLE_FS,
	YES,
	NO,
};

enum SCREEN_TYPE {
	MAIN_SCREEN = 0,
	CREDIT_SCREEN,
	OPTION_SCREEN,
	EXIT_SCREEN
};


/******************************************************************************/
/*!
	(Static) Variables
*/
/******************************************************************************/
AEGfxTexture* backgroundTexture;
AEGfxTexture* buttonTexture_START;
AEGfxTexture* buttonTexture_QUIT;
AEGfxTexture* buttonTexture_OPTIONS;
AEGfxTexture* buttonTexture_CREDITS;
AEGfxTexture* buttonTexture_TOGGLE_FS;
AEGfxTexture* buttonTexture_RETURN;
AEGfxTexture* buttonTexture_YES;
AEGfxTexture* buttonTexture_NO;


AEVec2		BUTTON_MESHSIZE = { 500.0f, 100.0f };
AEVec2		BUTTON_SCALE	= { 1.0f, 1.0f };


static GameObjInst* ButtonInstance_START;
static GameObjInst* ButtonInstance_QUIT;
static GameObjInst* ButtonInstance_OPTIONS;
static GameObjInst* ButtonInstance_CREDITS;
static GameObjInst* ButtonInstance_TOGGLE_FS;
static GameObjInst* ButtonInstance_RETURN;
static GameObjInst* ButtonInstance_YES;
static GameObjInst* ButtonInstance_NO;


static int			screen;
static bool			toggle_state;

/******************************************************************************/
/*!
	"Load" function of this state
*/
/******************************************************************************/
void GameStateMainMenuLoad() {
	
	// zero the game object array
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	sGameObjNum = 0;

	// zero the game object instance array
	memset(sGameObjInstList, 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sGameObjInstNum = 0;



	// load/create the mesh data (game objects / Shapes)
	GameObj* pObj;

	// =========================
	// Load textures
	// =========================
	backgroundTexture = AEGfxTextureLoad(".\\Resources\\Assets\\background.png");
	AE_ASSERT_MESG(backgroundTexture, "failed to create background texture");

	buttonTexture_START = AEGfxTextureLoad(".\\Resources\\Assets\\start_button.png");
	AE_ASSERT_MESG(buttonTexture_START, "failed to create start button texture");

	buttonTexture_OPTIONS = AEGfxTextureLoad(".\\Resources\\Assets\\option_button.png");
	AE_ASSERT_MESG(buttonTexture_OPTIONS, "failed to create option button texture");

	buttonTexture_CREDITS = AEGfxTextureLoad(".\\Resources\\Assets\\credit_button.png");
	AE_ASSERT_MESG(buttonTexture_CREDITS, "failed to create credit button texture");

	buttonTexture_QUIT = AEGfxTextureLoad(".\\Resources\\Assets\\exit_button.png");
	AE_ASSERT_MESG(buttonTexture_QUIT, "failed to create quit button texture");

	buttonTexture_TOGGLE_FS = AEGfxTextureLoad(".\\Resources\\Assets\\toggle_fs.png");
	AE_ASSERT_MESG(buttonTexture_TOGGLE_FS, "failed to create toggle fullscreen button texture");

	buttonTexture_RETURN = AEGfxTextureLoad(".\\Resources\\Assets\\return_button.png");
	AE_ASSERT_MESG(buttonTexture_TOGGLE_FS, "failed to create toggle fullscreen button texture");

	buttonTexture_YES = AEGfxTextureLoad(".\\Resources\\Assets\\yes_button.png");
	AE_ASSERT_MESG(buttonTexture_YES, "failed to create toggle fullscreen button texture");

	buttonTexture_NO = AEGfxTextureLoad(".\\Resources\\Assets\\no_button.png");
	AE_ASSERT_MESG(buttonTexture_NO, "failed to create toggle fullscreen button texture");


	// =========================
	// create the Button Shape
	// =========================
	buttonObjIndex = sGameObjNum;
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_BUTTON;
	AEGfxMeshStart();
	AEGfxTriAdd(
		-BUTTON_MESHSIZE.x / 2, -BUTTON_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 1.0f,
		BUTTON_MESHSIZE.x / 2, -BUTTON_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 1.0f,
		-BUTTON_MESHSIZE.x / 2, BUTTON_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f);

	AEGfxTriAdd(
		BUTTON_MESHSIZE.x / 2, -BUTTON_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 1.0f,
		BUTTON_MESHSIZE.x / 2, BUTTON_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 0.0f,
		-BUTTON_MESHSIZE.x / 2, BUTTON_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f);
	pObj->pMesh = AEGfxMeshEnd();
	pObj->meshSize = AEVec2{ BUTTON_MESHSIZE.x, BUTTON_MESHSIZE.y };
	AE_ASSERT_MESG(pObj->pMesh, "fail to create BUTTON object!!");

	// =========================
	// create the background mesh
	// =========================
	bgObjIndex = sGameObjNum;
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_BG;
	//Load mesh 
	AEGfxMeshStart();
	AEGfxTriAdd(
		-static_cast<float>(winWidth) / 2, -static_cast<float>(winHeight) / 2, 0x00FFFFFF, 0.0f, 1.0f,
		static_cast<float>(winWidth) / 2, -static_cast<float>(winHeight) / 2, 0x00FFFFFF, 1.0f, 1.0f,
		-static_cast<float>(winWidth) / 2, static_cast<float>(winHeight) / 2, 0x00FFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		static_cast<float>(winWidth) / 2, -static_cast<float>(winHeight) / 2, 0x00FFFFFF, 1.0f, 1.0f,
		static_cast<float>(winWidth) / 2, static_cast<float>(winHeight) / 2, 0x00FFFFFF, 1.0f, 0.0f,
		-static_cast<float>(winWidth) / 2, static_cast<float>(winHeight) / 2, 0x00FFFFFF, 0.0f, 0.0f);
	pObj->pMesh = AEGfxMeshEnd();
	pObj->meshSize = AEVec2{ 1.0f, 1.0f };
	AE_ASSERT_MESG(pObj->pMesh, "Failed to create bgMesh!!");

	
	// Move camera to 0,0 in event menu is loaded after game
	AEGfxSetCamPosition(0.0f, 0.0f);

	fmodSys->playSound(mainMenuBG, nullptr, false, &soundChannel);
	
	screen = MAIN_SCREEN;
}

/******************************************************************************/
/*!
	"InIt" function of this state
*/
/******************************************************************************/
void GameStateMainMenuInit() {
	//g_chosenLevel = 0;

	AEGfxSetBackgroundColor(0.2f, 0.2f, 0.2f);

	AEVec2 scaling{ 1.0f, 1.0f }, pos{0.0f, 0.0f};
	gameObjInstCreate(&sGameObjList[bgObjIndex], &scaling, &pos,0, 0.0f, STATE_NONE);

	//	Create Button
	scaling = { 1.0f, 1.0f };
	pos = { 0.0f, 0.0f };
	ButtonInstance_START = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_START->sub_type = START_GAME;

	scaling = { 1.0f, 1.0f };
	pos = { 0.0f, -100.0f };
	ButtonInstance_OPTIONS = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_OPTIONS->sub_type = OPTIONS;

	scaling = { 1.0f, 1.0f };
	pos = { 0.0f, -200.0f };
	ButtonInstance_CREDITS = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_CREDITS->sub_type = CREDITS;

	scaling = { 1.0f, 1.0f };
	pos = { 0,-300.0F };
	ButtonInstance_QUIT = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_QUIT->sub_type = EXIT_GAME;
}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateMainMenuUpdate() {

	AEVec2 scaling{ 1.0f, 1.0f }, pos{ 0.0f, 0.0f };
	switch (currInnerState) {
	case GAME_PLAY:
		//	if number key 1 is pressed
		//if (AEInputCheckCurr(AEVK_1))
		//{
		//	//	load level 1
		//	g_chosenLevel = 1;
		//	gGameStateNext = GS_LEVEL1;
		//}

		////	if number key 2 is pressed
		//if (AEInputCheckCurr(AEVK_2))
		//{
		//	//	load level 2
		//	g_chosenLevel = 2;
		//	gGameStateNext = GS_LEVELS;
		//}

		////	if number key 2 is pressed
		//if (AEInputCheckCurr(AEVK_Q))
		//{
		//	gGameStateNext = GS_QUIT;
		//}

		//	if left mouse click
		if (AEInputCheckReleased(VK_LBUTTON))
		{
			switch (screen)
			{
				case MAIN_SCREEN:
					if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_START->boundingBox))
					{
						//	load level 1
						//g_chosenLevel = 1;
						gGameStateNext = GS_LEVEL1;
					}

					if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_OPTIONS->boundingBox))
					{
						//	load option
						screen = OPTION_SCREEN;
						gameObjInstCreate(&sGameObjList[bgObjIndex], &scaling, &pos, 0, 0.0f, STATE_NONE);

						//	Create Button (toggle fullscreen)
						scaling = { 1.0f, 1.0f };
						pos = { 0.0f, 0.0f };
						ButtonInstance_TOGGLE_FS = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
						ButtonInstance_TOGGLE_FS->sub_type = TOGGLE_FS;

						//	create button (return)
						scaling = { 1.0f, 1.0f };
						pos = { 0,-300.0F };
						ButtonInstance_RETURN = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
						ButtonInstance_RETURN->sub_type = RETURN;
					}

					if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_CREDITS->boundingBox))
					{
						// load credit
						screen = CREDIT_SCREEN;
						gameObjInstCreate(&sGameObjList[bgObjIndex], &scaling, &pos, 0, 0.0f, STATE_NONE);

						//	create button (return)
						scaling = { 1.0f, 1.0f };
						pos = { 0,-300.0F };
						ButtonInstance_RETURN = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
						ButtonInstance_RETURN->sub_type = RETURN;
					}

					//	QUIT GAME
					if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_QUIT->boundingBox))
					{
						//	exit confirmation
						screen = EXIT_SCREEN;
						gameObjInstCreate(&sGameObjList[bgObjIndex], &scaling, &pos, 0, 0.0f, STATE_NONE);

						//	create button (Yes)
						scaling = { 0.5f, 0.5f };
						pos = { -300.0f,-300.0f };
						ButtonInstance_YES = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
						ButtonInstance_YES->sub_type = YES;

						//	create button (No)
						scaling = { 0.5f, 0.5f };
						pos = { 300.0f ,-300.0f };
						ButtonInstance_NO = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
						ButtonInstance_NO->sub_type = NO;
						//gGameStateNext = GS_QUIT;
					}
					break;

				case OPTION_SCREEN:
					//	TOGGLE FULL SCREEN
					if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_TOGGLE_FS->boundingBox))
					{
						//if (toFullScreen)
						//{
						//	toFullScreen = false;
						//	AEToogleFullScreen(toFullScreen);
						//}
						//else
						//{
						//	toFullScreen = true;
						//	AEToogleFullScreen(toFullScreen);
						//}
						toFullScreen = !toFullScreen;
						AEToogleFullScreen(toFullScreen);
					}

					// Toggle Sound
					/*if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_TOGGLE_SOUND->boundingBox)){
						soundVolumeLevel = !soundVolumeLevel;
						if (soundVolumeLevel)
							soundChannel->setVolume(1.0f);
						else
							soundChannel->setVolume(0.0f);
					}*/

					//	RETURN TO MAIN MENU
					if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_RETURN->boundingBox))
					{
						screen = MAIN_SCREEN;
						gGameStateNext = GS_RESTART;
					}
					break;

				case CREDIT_SCREEN:
					//	RETURN TO MAIN MENU
					if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_RETURN->boundingBox))
					{
						screen = MAIN_SCREEN;
						gGameStateNext = GS_RESTART;
					}
					break;

				case EXIT_SCREEN:
					//	YES to exit game
					if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_YES->boundingBox))
						gGameStateNext = GS_QUIT;
				
					// NO to return to main menu
					if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_NO->boundingBox))
					{
						screen = MAIN_SCREEN;
						gGameStateNext = GS_RESTART;
					}

					break;
			}
		}

		fmodSys->update();

		int i{};
		GameObjInst* pInst;

		//Update object instances physics and behavior
		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
		{
			pInst = sGameObjInstList + i;

			// skip non-active object
			if (0 == (pInst->flag & FLAG_ACTIVE))
				continue;

		}

		//Update object instances positions
		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
		{
			pInst = sGameObjInstList + i;

			// skip non-active object
			if (0 == (pInst->flag & FLAG_ACTIVE))
				continue;

			/**********
			update the position using: P1 = V1*dt + P0
			Get the bouding rectangle of every active instance:
				boundingRect_min = -BOUNDING_RECT_SIZE * instance->scale + instance->pos
				boundingRect_max = BOUNDING_RECT_SIZE * instance->scale + instance->pos
			**********/

			// ----- Update Position -----
			pInst->posCurr.x += pInst->velCurr.x * g_dt;
			pInst->posCurr.y += pInst->velCurr.y * g_dt;

			// ----- Update Bounding Box -----
			pInst->boundingBox.min.x = -(pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
			pInst->boundingBox.min.y = -(pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;

			pInst->boundingBox.max.x = (pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
			pInst->boundingBox.max.y = (pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;
		}

		// ====================
		// check for collision
		// ====================

		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		{
			pInst = sGameObjInstList + i;

			// skip non-active object
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			/*switch (pInst->pObject->type) {
			default:
				break;
			}*/
		}


		// =====================================
		// calculate the matrix for all objects
		// =====================================

		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		{
			pInst = sGameObjInstList + i;
			AEMtx33		 trans, rot, scale;

			//UNREFERENCED_PARAMETER(trans);
			//UNREFERENCED_PARAMETER(rot);
			//UNREFERENCED_PARAMETER(scale);

			// skip non-active object
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			// Compute the scaling matrix
			AEMtx33Scale(&scale, pInst->scale.x, pInst->scale.y);
			// Compute the rotation matrix 	
			AEMtx33Rot(&rot, pInst->dirCurr);
			// Compute the translation matrix
			AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
			// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
			AEMtx33Concat(&pInst->transform, &rot, &scale);
			AEMtx33Concat(&pInst->transform, &trans, &pInst->transform);
		}

		// =====================================
		// Mouse Input
		// =====================================
		float cameraX, cameraY;
		AEGfxGetCamPosition(&cameraX, &cameraY);
		worldMouseX = cameraX + (static_cast<float>(g_mouseX) - static_cast<float>(AEGetWindowWidth()) / 2);
		worldMouseY = cameraY + (-1) * (static_cast<float>(g_mouseY) - static_cast<float>(AEGetWindowHeight()) / 2);
		//std::cout << "Mouse World Pos: (" << worldMouseX << ", " << worldMouseY << ")\n";

		break;
	}
}

/******************************************************************************/
/*!
	"Draw" function of this state
*/
/******************************************************************************/
void GameStateMainMenuDraw() {
	//char strBuffer[1024];

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);


	// =====================================
	//		DRAW BACKGROUND
	// =====================================
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);

	// draw all object instances in the list
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{

		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE) || 0 == (pInst->flag & FLAG_VISIBLE))
			continue;

		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		AEGfxSetTransform(pInst->transform.m);
		// Draw the shape used by the current object instance using "AEGfxMeshDraw"
		// =====================================
		//		DRAW BUTTON
		// =====================================
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);


		if (pInst->sub_type == START_GAME)
		{
			AEGfxTextureSet(buttonTexture_START, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

		if (pInst->sub_type == EXIT_GAME)
		{
			AEGfxTextureSet(buttonTexture_QUIT, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

		if (pInst->sub_type == OPTIONS)
		{
			AEGfxTextureSet(buttonTexture_OPTIONS, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

		if (pInst->sub_type == CREDITS)
		{
			AEGfxTextureSet(buttonTexture_CREDITS, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

		if (pInst->sub_type == TOGGLE_FS)
		{
			AEGfxTextureSet(buttonTexture_TOGGLE_FS, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

		if (pInst->sub_type == RETURN)
		{
			AEGfxTextureSet(buttonTexture_RETURN, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

		if (pInst->sub_type == YES)
		{
			AEGfxTextureSet(buttonTexture_YES, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

		if (pInst->sub_type == NO)
		{
			AEGfxTextureSet(buttonTexture_NO, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

		if (pInst->pObject->type == TYPE_BG) {
			AEGfxTextureSet(backgroundTexture, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	//	Drawing for Font for all states
	char strBuffer[100];
	f32 TextWidth = 1.0f;
	f32 TextHeight = 1.0f;

	memset(strBuffer, 0, 100 * sizeof(char));
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	/*sprintf_s(strBuffer, "Current Time : %.2f", levelTime);
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, 0.8f - TextWidth / 2, 0.9f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);*/
	switch (screen)
	{
	case CREDIT_SCREEN:
		sprintf_s(strBuffer, "Broken Tilde");
		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font20, strBuffer, -0.6f, 0.2f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
		sprintf_s(strBuffer, "Team Members :");
		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font20, strBuffer, -0.9f, 0.1f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Mohamed Zafir");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, -0.6f, 0.1f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
		sprintf_s(strBuffer, "Lee Hsien Wei, Joachim");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, -0.6f, 0.0f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
		sprintf_s(strBuffer, "Desmond Too Wei Kang");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, -0.6f, -0.1f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
		sprintf_s(strBuffer, "Leong Wai Kit");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, -0.6f, -0.2f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Instructors :");
		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font20, strBuffer, -0.83f, -0.4f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
		sprintf_s(strBuffer, "Cheng Ding Xiang");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, -0.6f, -0.4f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
		sprintf_s(strBuffer, "Gerald Wong Han Feng");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, -0.6f, -0.5f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "All content copyright 2022 DigiPen Institute of Technology Singapore, all rights reserved.");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, -0.6f, -0.65f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "President :");
		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font20, strBuffer, 0.05f, 0.1f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Claude COMAIR");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, 0.28f, 0.1f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Executives :");
		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font20, strBuffer, 0.035f, -0.05f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Jason CHU");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, 0.28f, -0.05f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Christopher COMAIR");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, 0.28f, -0.15f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Michael GATS");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, 0.28f, -0.25f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Michele COMAIR");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, 0.28f, -0.35f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Raymond YAN");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, 0.28f, -0.45f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Samir ABOU SAMRA");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, 0.28f, -0.55f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Prasanna GHALI");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, 0.52f, -0.05f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "John BAUER");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, 0.52f, -0.15f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Michael GATS");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, 0.52f, -0.25f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		sprintf_s(strBuffer, "Michael GATS");
		AEGfxGetPrintSize(g_font12, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font12, strBuffer, 0.52f, -0.35f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		break;

	case EXIT_SCREEN:
		sprintf_s(strBuffer, "Are you sure you want to exit?");
		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font20, strBuffer, -0.2f, -0.3f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

		break;
	}
}

/******************************************************************************/
/*!
	"Free" function of this state
*/
/******************************************************************************/
void GameStateMainMenuFree() {
	// kill all object instances in the array using "gameObjInstDestroy"
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;

		gameObjInstDestroy(pInst);
	}
}

/******************************************************************************/
/*!
	"Unload" function of this state
*/
/******************************************************************************/
void GameStateMainMenuUnload() {
	AEGfxTextureUnload(backgroundTexture);
	AEGfxTextureUnload(buttonTexture_START);
	AEGfxTextureUnload(buttonTexture_QUIT);
	AEGfxTextureUnload(buttonTexture_CREDITS);
	AEGfxTextureUnload(buttonTexture_OPTIONS);
	AEGfxTextureUnload(buttonTexture_TOGGLE_FS);
	AEGfxTextureUnload(buttonTexture_RETURN);
	AEGfxTextureUnload(buttonTexture_YES);
	AEGfxTextureUnload(buttonTexture_NO);

	soundChannel->stop();
		
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned long i = 0; i < sGameObjNum; i++) {
		GameObj* pObj = sGameObjList + i;
		if (pObj->pMesh != nullptr)
			AEGfxMeshFree(pObj->pMesh);
	}
}
