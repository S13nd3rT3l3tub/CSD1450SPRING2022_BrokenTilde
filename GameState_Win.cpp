/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       GameState_Win.cpp
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

static double fireworktimer{};
static double textblinktimer{};

/******************************************************************************/
/*!
	"Load" function of this state
*/
/******************************************************************************/
void GameStateWinLoad() {
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

	// =====================
	// create the Particle1 shape
	// =====================
	particleObjIndex = sGameObjNum;
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_PARTICLE1;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-PLAYER_MESHSIZE.x / 5, -PLAYER_MESHSIZE.y / 5, 0xFFFF0000, 0.0f, 0.0f,
		PLAYER_MESHSIZE.x / 5, -PLAYER_MESHSIZE.y / 5, 0xFFFF0000, 0.0f, 0.0f,
		-PLAYER_MESHSIZE.x / 5, PLAYER_MESHSIZE.y / 5, 0xFFFF0000, 0.0f, 0.0f);

	AEGfxTriAdd(
		PLAYER_MESHSIZE.x / 5, -PLAYER_MESHSIZE.y / 5, 0xFFFF0000, 0.0f, 0.0f,
		PLAYER_MESHSIZE.x / 5, PLAYER_MESHSIZE.y / 5, 0xFFFF0000, 0.0f, 0.0f,
		-PLAYER_MESHSIZE.x / 5, PLAYER_MESHSIZE.y / 5, 0xFFFF0000, 0.0f, 0.0f);
	pObj->pMesh = AEGfxMeshEnd();
	pObj->meshSize = AEVec2{ PLAYER_MESHSIZE.x / 5, PLAYER_MESHSIZE.y / 5 };
	AE_ASSERT_MESG(pObj->pMesh, "fail to create TYPE_PARTICLE1 object!!");

	fModSys->playSound(mainMenuBG, nullptr, false, &soundChannel);
	if (soundVolumeLevel)
		soundChannel->setVolume(0.7f);
	else
		soundChannel->setVolume(0.0f);
	
}

/******************************************************************************/
/*!
	"Init" function of this state
*/
/******************************************************************************/
void GameStateWinInit() {


}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateWinUpdate() {

	switch (gGameStateInnerState) {
	case GAME_PAUSE:
		soundChannel->setPaused(true);

		if (winFocused)
			gGameStateInnerState = GAME_PLAY;

		break;
	case GAME_PLAY:
		if (winFocused == false)
			gGameStateInnerState = GAME_PAUSE;

		bool result;
		soundChannel->getPaused(&result);
		if (result)
			soundChannel->setPaused(false);

		fModSys->update();

		// =========================
		// update according to input
		// =========================
		if (AEInputCheckCurr(VK_SPACE) && g_chosenLevel == 2)
			gGameStateNext = GS_LEVELS;
		else if (AEInputCheckCurr(VK_SPACE) && g_chosenLevel > 2)
			gGameStateNext = GS_MAINMENU;


		if (fireworktimer < 0)
		{
			AEVec2 particleVel;
			for (double x = -1.0f; x < 20.0f; x += ((1.f + rand() % 50) / 100.f)) // spawn particles on left side of screen
			{
				AEVec2 particlespawn = { 10.f, static_cast<float>(x) };
				if (rand() % 2) // randomize polarity of particleVel.x
				{
					particleVel = { rand() % 20 / -10.0f, rand() % 20 / 10.f };
					if (rand() % 2)
					{
						gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_NONE); // red particles
					}
					else
					{
						gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_ALERT); // orange particles
					}

				}
				else
				{
					particleVel = { rand() % 20 / 10.f, rand() % 20 / 10.f };
					if (rand() % 2)
					{
						gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_NONE); // red particles
					}
					else
					{
						gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_ALERT); // oraange particles
					}
				}
			}
			for (double x = -1.0f; x < 20.0f; x += ((1.f + rand() % 50) / 100.f)) // spawn particles on right side of screen
			{
				AEVec2 particlespawn = { 30.f, static_cast<float>(x) };
				if (rand() % 2) // randomize polarity of particleVel.x
				{
					particleVel = { rand() % 20 / -10.0f, rand() % 20 / 10.f };
					if (rand() % 2)
					{
						gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_NONE); // red particle
					}
					else
					{
						gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_ALERT); //orange particle
					}

				}
				else
				{
					particleVel = { rand() % 20 / 10.f, rand() % 20 / 10.f };
					if (rand() % 2)
					{
						gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_NONE); // red particle
					}
					else
					{
						gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_ALERT); // orange particle
					}
				}
			}
			fireworktimer = 0.2f; //set firework period
		}
		else
		{
			fireworktimer -= g_dt;
		}

		int i{};
		GameObjInst* pInst;

		//Update object instances positions
		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
		{
			pInst = sGameObjInstList + i;

			// skip non-active object
			if (0 == (pInst->flag & FLAG_ACTIVE))
				continue;

			/*******************************
			update the position of instances
			*******************************/

			// ----- Update Position -----
			pInst->posCurr.x += pInst->velCurr.x * g_dt;
			pInst->posCurr.y += pInst->velCurr.y * g_dt;

			//// ----- Update Bounding Box -----
			//pInst->boundingBox.min.x = -(pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
			//pInst->boundingBox.min.y = -(pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;

			//pInst->boundingBox.max.x = (pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
			//pInst->boundingBox.max.y = (pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;
		}

		// =====================================
		// calculate the matrix for all objects
		// =====================================

		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
		{
			pInst = sGameObjInstList + i;
			AEMtx33		 trans, rot, scale;

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
			AEMtx33Concat(&pInst->transform, &trans, &rot);
			AEMtx33Concat(&pInst->transform, &pInst->transform, &scale);
		}
		break;
	}
}

/******************************************************************************/
/*!
	"Draw" function of this state
*/
/******************************************************************************/
void GameStateWinDraw() {

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);

	// draw all object instances in the list
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		AEMtx33 cellTranslation, cellFinalTransformation;
		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE) || 0 == (pInst->flag & FLAG_VISIBLE))
			continue;

		AEMtx33Concat(&cellFinalTransformation, &MapTransform, &pInst->transform);
		AEGfxSetTransform(cellFinalTransformation.m);

		// Particle transparancy handler
		if (pInst->pObject->type == TYPE_PARTICLE1)
		{
			AEGfxSetTransparency(pInst->dirCurr);
			if (pInst->dirCurr <= 0) //if fully transparent, destroy particle
			{
				gameObjInstDestroy(pInst);
			}

			if (pInst->state == STATE_ALERT) //Orange particles
			{
				AEGfxSetBlendColor(1.0f, 0.35f, 0.0f, 1.f);
			}

			pInst->dirCurr -= g_dt; //deplete transparency of particles
		}

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxTextureSet(NULL, 0.0f, 0.0f);
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
		AEGfxSetTransparency(1.f);
	}

	//	Render text for Win Screen
	f32 TextWidth = 1.0f;
	f32 TextHeight = 1.0f;
	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	//Mission completed text
	sprintf_s(strBuffer, "Mission %d completed!", g_chosenLevel - 1);
	AEGfxGetPrintSize(g_font30, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font30, strBuffer, 0.0f - TextWidth / 2, 0.30f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	//Text displaying the time taken to complete level
	sprintf_s(strBuffer, "Time taken: %.2f seconds", levelTime); 
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, 0.0f - TextWidth / 2, 0.05f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	if (textblinktimer > 0.5f) // render the text white in color
	{
		sprintf_s(strBuffer, "Press spacebar to continue");
		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font20, strBuffer, 0.0f - TextWidth / 2, -0.20f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
	}
	else // render the text green in color
	{
		sprintf_s(strBuffer, "Press spacebar to continue");
		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font20, strBuffer, 0.0f - TextWidth / 2, -0.20f - TextHeight / 2, 1.0f, 0.f, 1.f, 0.f);
	}

	//control for text blinker timer
	if (textblinktimer > 0) 
	{
		textblinktimer -= g_dt;
	}
	else
	{
		textblinktimer = 1.0f;
	}
}

/******************************************************************************/
/*!
	"Free" function of this state
*/
/******************************************************************************/
void GameStateWinFree() {
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
void GameStateWinUnload() {
	soundChannel->stop(); //stop background music
}
