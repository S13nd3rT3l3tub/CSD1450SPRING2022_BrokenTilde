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



/******************************************************************************/
/*!
	Enums/Struct/Class Definitions
*/
/******************************************************************************/
enum BUTTON_TYPE {
	START_GAME = 1,
	OPTIONS,
	CREDITS,
	EXIT_GAME
};


/******************************************************************************/
/*!
	(Static) Variables
*/
/******************************************************************************/
AEGfxTexture* backgroundTexture;
AEGfxTexture* buttonTexture_START;
AEGfxTexture* buttonTexture_QUIT;

AEVec2		BUTTON_MESHSIZE = { 500.0f, 100.0f };
AEVec2		BUTTON_SCALE	= { 1.0f, 1.0f };



static GameObjInst* ButtonInstance_START;
static GameObjInst* ButtonInstance_QUIT;


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
	buttonTexture_START = AEGfxTextureLoad(".\\Resources\\Assets\\start_button.png");
	AE_ASSERT_MESG(buttonTexture_START, "failed to create start button texture");

	buttonTexture_QUIT = AEGfxTextureLoad(".\\Resources\\Assets\\exit_button.png");
	AE_ASSERT_MESG(buttonTexture_QUIT, "failed to create quit button texture");

	backgroundTexture = AEGfxTextureLoad(".\\Resources\\Assets\\background.png");
	AE_ASSERT_MESG(backgroundTexture, "failed to create background texture");


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
}

/******************************************************************************/
/*!
	"InIt" function of this state
*/
/******************************************************************************/
void GameStateMainMenuInit() {
	g_chosenLevel = 0;

	AEGfxSetBackgroundColor(0.2f, 0.2f, 0.2f);

	AEVec2 scale{ 1.0f, 1.0f }, pos{0.0f, 0.0f};
	gameObjInstCreate(&sGameObjList[bgObjIndex], &scale, &pos,0, 0.0f, STATE_NONE);

	//	Create Button
	scale = { 1.0f, 1.0f };
	pos = { 0.0f, 0.0f };
	ButtonInstance_START = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_START->sub_type = START_GAME;

	scale = { 1.0f, 1.0f };
	pos = { 0,-200 };
	ButtonInstance_QUIT = gameObjInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_QUIT->sub_type = EXIT_GAME;
}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateMainMenuUpdate() {
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
			if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_START->boundingBox))
			{
				//	load level 1
				g_chosenLevel = 1;
				gGameStateNext = GS_LEVEL1;
			}

			if (CollisionIntersection_PointRect(worldMouseX, worldMouseY, ButtonInstance_QUIT->boundingBox))
				gGameStateNext = GS_QUIT;
		}


		// =========================
		// update according to input
		// =========================


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

		if (pInst->pObject->type == TYPE_BG) {
			AEGfxTextureSet(backgroundTexture, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}
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
	
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned long i = 0; i < sGameObjNum; i++) {
		GameObj* pObj = sGameObjList + i;
		if (pObj->pMesh != nullptr)
			AEGfxMeshFree(pObj->pMesh);
	}
}
