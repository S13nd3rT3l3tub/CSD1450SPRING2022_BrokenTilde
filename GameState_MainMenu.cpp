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

#include "main.h"

#define WINDOW_W 1400
#define WINDOW_H 750

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
const unsigned int	GAME_OBJ_NUM_MAX = 16;					//The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_INST_NUM_MAX = 1028;			//The total number of different game object instances
AEGfxVertexList* bgMesh;
AEGfxTexture* backgroundTexture;

AEGfxTexture* buttonTexture_START;
AEGfxTexture* buttonTexture_QUIT;
AEGfxTexture* digipenlogo;

int overlay;
//AEGfxTexture* Texture1;
//AEGfxTexture* Texture2;

AEVec2		BUTTON_MESHSIZE = { 500.0f, 100.0f };
AEVec2		BUTTON_SCALE = { 1.0f, 1.0f };
AEVec2		SPLASH_MESHSIZE = {2125, 800};//{ 1525, 445 };

static float worldMouseX{ 0 }, worldMouseY{ 0 };

// -----------------------------------------------------------------------------
// object flag definition
const unsigned int	FLAG_ACTIVE = 0x00000001;
const unsigned int	FLAG_VISIBLE = 0x00000002;
const unsigned int	FLAG_NON_COLLIDABLE = 0x00000004;

/******************************************************************************/
/*!
	Enums/Struct/Class Definitions
*/
/******************************************************************************/
enum TYPE
{
	TYPE_BUTTON = 0,
	TYPE_SPLASH
};

enum BUTTON_TYPE {
	START_GAME = 1,


	EXIT_GAME
};

//Game object structure
struct GameObj
{
	unsigned long		type;		// object type
	AEGfxVertexList* pMesh;		// This will hold the triangles which will form the shape of the object
	AEVec2 meshSize;
};

// ---------------------------------------------------------------------------

//Game object instance structure
struct GameObjInst
{
	GameObj* pObject;	// pointer to the 'original' shape
	unsigned long		flag;		// bit flag or-ed together
	AEVec2				scale;		// scaling value of the object instance
	AEVec2				posCurr;	// object current position
	AEVec2				velCurr;	// object current velocity
	float				dirCurr;	// object current direction
	AABB				boundingBox;// object bouding box that encapsulates the object
	AEMtx33				transform;	// object transformation matrix: Each frame, 
									// calculate the object instance's transformation matrix and save it here
	unsigned long		sub_type;	// to differentiate between button objects

	// pointer to custom data specific for each object type
	void* pUserData;

	//void				(*pfUpdate)(void);
	//void				(*pfDraw)(void);
};

/******************************************************************************/
/*!
	Static Variables
*/
/******************************************************************************/
// list of original object
static GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
static unsigned long		sGameObjNum;								// The number of defined game objects

// list of object instances
static GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
static unsigned long		sGameObjInstNum;							// The number of used game object instances

static GameObjInst* ButtonInstance_START;
static GameObjInst* ButtonInstance_QUIT;
static GameObjInst* splashcreen;
static float splashscreentimer = 4.f;


/******************************************************************************/
/*!
	Helper Functions
*/
/******************************************************************************/

// ---------------------------------------------------------------------------

// functions to create/destroy a game object instance
GameObjInst* gameObjInstCreate(unsigned int type, AEVec2* scale,
							   AEVec2* pPos, AEVec2* pVel,
							   float dir, unsigned int sub_type);

void					gameObjInstDestroy(GameObjInst* pInst);

//	function to check if mouse clicking button
int Collision_Mouse_Button(const AABB& aabb, float MouseX, float MouseY);


/******************************************************************************/
/*!
	"Load" function of this state
*/
/******************************************************************************/
void GameStateMainMenuLoad() {
	std::cout << "Menu:load\n";
	backgroundTexture = AEGfxTextureLoad(".\\Resources\\Assets\\background.png");
	AE_ASSERT_MESG(backgroundTexture, "failed to create background texture");

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
	// create the Button Shape
	// =========================
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
// create the Splashscreen Shape
// =========================
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_SPLASH;
	AEGfxMeshStart();
	AEGfxTriAdd(
		-SPLASH_MESHSIZE.x / 2, -SPLASH_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 1.0f,
		SPLASH_MESHSIZE.x / 2, -SPLASH_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 1.0f,
		-SPLASH_MESHSIZE.x / 2, SPLASH_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f);

	AEGfxTriAdd(
		SPLASH_MESHSIZE.x / 2, -SPLASH_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 1.0f,
		SPLASH_MESHSIZE.x / 2, SPLASH_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 0.0f,
		-SPLASH_MESHSIZE.x / 2, SPLASH_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f);
	pObj->pMesh = AEGfxMeshEnd();
	pObj->meshSize = AEVec2{ SPLASH_MESHSIZE.x, SPLASH_MESHSIZE.y };
	AE_ASSERT_MESG(pObj->pMesh, "fail to create SPLASH object!!");

	// =========================
	// load the Button texture
	// =========================
	buttonTexture_START = AEGfxTextureLoad(".\\Resources\\Assets\\start_button.png");
	AE_ASSERT_MESG(buttonTexture_START, "failed to create start button texture");

	buttonTexture_QUIT = AEGfxTextureLoad(".\\Resources\\Assets\\exit_button.png");
	AE_ASSERT_MESG(buttonTexture_QUIT, "failed to create quit button texture");

	digipenlogo = AEGfxTextureLoad(".\\Resources\\Assets\\DigiPen.png");
	AE_ASSERT_MESG(digipenlogo, "failed to create quit button texture");


	// Move camera to 0,0 in event menu is loaded after game
	AEGfxSetCamPosition(0.0f, 0.0f);
}

/******************************************************************************/
/*!
	"InIt" function of this state
*/
/******************************************************************************/
void GameStateMainMenuInit() {

	AEToogleFullScreen(true);
	AEGfxSetBackgroundColor(0.2f, 0.2f, 0.2f);

	//Load mesh 
	AEGfxMeshStart();
	AEGfxTriAdd(
		-WINDOW_W / 2, -WINDOW_H / 2, 0x00FFFFFF, 0.0f, 1.0f,
		WINDOW_W / 2, -WINDOW_H / 2, 0x00FFFFFF, 1.0f, 1.0f,
		-WINDOW_W / 2, WINDOW_H / 2, 0x00FFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		WINDOW_W / 2, -WINDOW_H / 2, 0x00FFFFFF, 1.0f, 1.0f,
		WINDOW_W / 2, WINDOW_H / 2, 0x00FFFFFF, 1.0f, 0.0f,
		-WINDOW_W / 2, WINDOW_H / 2, 0x00FFFFFF, 0.0f, 0.0f);
	bgMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(bgMesh, "Failed to create beltMesh!!");

	g_chosenLevel = 0;
	std::cout << "Main Menu: " << g_chosenLevel << std::endl;
	
	overlay = main; 

	//	Create Button
	AEVec2 Start_Button = { 0,0 };
	ButtonInstance_START = gameObjInstCreate(TYPE_BUTTON, &BUTTON_SCALE, &Start_Button, 0, 0.0f, START_GAME);

	AEVec2 Quit_Button = { 0,-200 };
	ButtonInstance_QUIT = gameObjInstCreate(TYPE_BUTTON, &BUTTON_SCALE, &Quit_Button, 0, 0.0f, EXIT_GAME);

	AEVec2 logoscale = {0.6f,0.5f};
	AEVec2 logopos = { 0, -10.f };
	splashcreen = gameObjInstCreate(TYPE_SPLASH, &logoscale, &logopos, 0, 0.0f, EXIT_GAME);
}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateMainMenuUpdate() {

	if (splashscreentimer > 0)
	{
		splashscreentimer -= g_dt;
	}
	else
	{
		gameObjInstDestroy(splashcreen);
	}

	//	if number key 1 is pressed
	if (AEInputCheckCurr(AEVK_1))
	{
		//	load level 1
		g_chosenLevel = 1;
		gGameStateNext = GS_LEVEL1;
	}

	//	if number key 2 is pressed
	if (AEInputCheckCurr(AEVK_2))
	{
		//	load level 2
		g_chosenLevel = 2;
		gGameStateNext = GS_LEVELS;
	}

	//	if number key 2 is pressed
	if (AEInputCheckCurr(AEVK_Q))
	{
		gGameStateNext = GS_QUIT;
	}

	//	if left mouse click
	if (AEInputCheckReleased(VK_LBUTTON))
	{
		if (Collision_Mouse_Button(ButtonInstance_START->boundingBox, worldMouseX, worldMouseY))
		{
			//	load level 1
			g_chosenLevel = 1;
			gGameStateNext = GS_LEVEL1;
		}

		if (Collision_Mouse_Button(ButtonInstance_QUIT->boundingBox, worldMouseX, worldMouseY))
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

		switch (pInst->pObject->type) {
		default:
			break;
		}
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
		AEMtx33Concat(&pInst->transform, &trans, &rot);
		AEMtx33Concat(&pInst->transform, &pInst->transform, &scale);
	}

	// =====================================
	// Mouse Input
	// =====================================
	float cameraX, cameraY;
	AEGfxGetCamPosition(&cameraX, &cameraY);
	worldMouseX = cameraX + (static_cast<float>(g_mouseX) - static_cast<float>(AEGetWindowWidth()) / 2);
	worldMouseY = cameraY + (-1) * (static_cast<float>(g_mouseY) - static_cast<float>(AEGetWindowHeight()) / 2);
	//std::cout << "Mouse World Pos: (" << worldMouseX << ", " << worldMouseY << ")\n";
}

/******************************************************************************/
/*!
	"Draw" function of this state
*/
/******************************************************************************/
void GameStateMainMenuDraw() {
	//char strBuffer[1024];

	// =====================================
	//		DRAW BACKGROUND
	// =====================================
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0.0f, 0.0f);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);
	//AEGfxTextureSet(NULL, 0, 0);
	switch (overlay)
	{
	case main:
		if (splashscreentimer < 0)
		{
			AEGfxTextureSet(backgroundTexture, 0.0f, 0.0f);
		}
		else
		{
			AEGfxTextureSet(NULL, 0.0f, 0.0f);
		}
		
		break;
	default:
		break;
	}
	AEGfxMeshDraw(bgMesh, AE_GFX_MDM_TRIANGLES);

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

		if (pInst->pObject->type == TYPE_SPLASH)
		{
			AEGfxSetBackgroundColor(1, 1, 1);
			AEGfxTextureSet(digipenlogo, 0.0f, 0.0f);
			AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		}

	}

	/*AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0.0f, 0.0f);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.0f);
	AEGfxTextureSet(buttonTexture, 0.0f, 0.0f);
	AEGfxMeshDraw(ButtonInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);*/
	
	/*
	memset(strBuf, 0, 1000 * sizeof(char));
	sprintf_s(strBuf, "Ricochet");
	AEGfxPrint(g_font20, strBuf, -0.2f, 0.4f, 1.0f, 1.0f, 0.0f, 1.0f);

	sprintf_s(strBuf, "Press 1 for LEVEL 1");
	AEGfxPrint(g_font20, strBuf, -0.2f, 0.2f, 1.0f, 1.0f, 1.0f, 1.0f);

	sprintf_s(strBuf, "Press 2 for LEVEL 2");
	AEGfxPrint(g_font20, strBuf, -0.2f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f);

	sprintf_s(strBuf, "Press Q for EXIT");
	AEGfxPrint(g_font20, strBuf, -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	*/
}

/******************************************************************************/
/*!
	"Free" function of this state
*/
/******************************************************************************/
void GameStateMainMenuFree() {
	
	if (gGameStateNext == GS_QUIT)
	{
		AEGfxTextureUnload(backgroundTexture);
		AEGfxTextureUnload(buttonTexture_START);
		AEGfxTextureUnload(buttonTexture_QUIT);
		AEGfxMeshFree(bgMesh);
	}

	// kill all object instances in the array using "gameObjInstDestroy"
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;

		gameObjInstDestroy(pInst);
	}

	//AEGfxMeshFree(bgMesh);
}

/******************************************************************************/
/*!
	"Unload" function of this state
*/
/******************************************************************************/
void GameStateMainMenuUnload() {
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned long i = 0; i < sGameObjNum; i++) {
		GameObj* pObj = sGameObjList + i;
		AEGfxMeshFree(pObj->pMesh);
	}
}
/******************************************************************************/
/*!

*/
/******************************************************************************/
GameObjInst* gameObjInstCreate(unsigned int type, AEVec2* scale,
							   AEVec2* pPos, AEVec2* pVel,
							   float dir, unsigned int sub_type)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	AE_ASSERT_PARM(type < sGameObjNum);

	// loop through the object instance list to find a non-used object instance
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = sGameObjList + type;
			pInst->flag = FLAG_ACTIVE | FLAG_VISIBLE;
			pInst->scale = *scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;
			pInst->pUserData = 0;
			pInst->sub_type = sub_type;


			// return the newly created instance
			return pInst;
		}
	}

	return 0;
}


/******************************************************************************/
/*!
	This function returns true if the mouse is within boundary of button
*/
/******************************************************************************/
int Collision_Mouse_Button(const AABB& aabb, float MouseX, float MouseY)
{
	//if (worldMouseX - ButtonInstance->boundingBox.min.x >= 0 )	//	mouse touch from left
	//if (worldMouseX - ButtonInstance->boundingBox.max.x >= 0 )	//	mouse touch from right
	//if (worldMouseY - ButtonInstance->boundingBox.max.y <= 0)		// mouse touch from top
	//if (worldMouseY - ButtonInstance->boundingBox.min.y >= 0)		// mouse touch from bottom
	if ((worldMouseX - aabb.min.x >= 0 && worldMouseX - aabb.max.x <= 0) &&
		(worldMouseY - aabb.max.y <= 0 && worldMouseY - aabb.min.y >= 0))
		return 1;
	else
		return 0;
}
