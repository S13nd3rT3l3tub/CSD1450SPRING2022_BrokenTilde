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

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
const unsigned int	GAME_OBJ_NUM_MAX = 16;			//The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_INST_NUM_MAX = 1028;			//The total number of different game object instances


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
enum class BUTTON_TYPE {
	START_GAME = 0,


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


/******************************************************************************/
/*!
	Helper Functions
*/
/******************************************************************************/

// ---------------------------------------------------------------------------

// functions to create/destroy a game object instance
GameObjInst* gameObjInstCreate(unsigned int type, AEVec2* scale,
	AEVec2* pPos, AEVec2* pVel,
	float dir);
void					gameObjInstDestroy(GameObjInst* pInst);


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

}

/******************************************************************************/
/*!
	"InIt" function of this state
*/
/******************************************************************************/
void GameStateMainMenuInit() {
	g_chosenLevel = 1;
	std::cout << "Main Menu: " << g_chosenLevel << std::endl;

}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateMainMenuUpdate() {
	gGameStateNext = GS_LEVEL1;

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

}

/******************************************************************************/
/*!
	"Draw" function of this state
*/
/******************************************************************************/
void GameStateMainMenuDraw() {
	//char strBuffer[1024];

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
	
	
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
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
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

	FreeMapData();
}

/******************************************************************************/
/*!
	"Unload" function of this state
*/
/******************************************************************************/
void GameStateMainMenuUnload() {

}

/******************************************************************************/
/*!

*/
/******************************************************************************/
GameObjInst* gameObjInstCreate(unsigned int type, AEVec2* scale,
	AEVec2* pPos, AEVec2* pVel,
	float dir)
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

			// return the newly created instance
			return pInst;
		}
	}

	return 0;
}

