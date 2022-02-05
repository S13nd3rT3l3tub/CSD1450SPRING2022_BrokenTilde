/******************************************************************************/
/*!
\file		GameState_Asteroids.cpp
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
#include <random>
#include <iostream>

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
const unsigned int	GAME_OBJ_NUM_MAX = 32;			//The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_INST_NUM_MAX = 2048;			//The total number of different game object instances


const unsigned int	PLAYER_INITIAL_NUM = 3;			// initial number of ship lives
const float			PLAYER_SIZE = 16.0f;		// ship size
const float			PLAYER_ACCEL_FORWARD = 40.0f;		// ship forward acceleration (in m/s^2)
const float			PLAYER_ACCEL_BACKWARD = 40.0f;		// ship backward acceleration (in m/s^2)
const float			PLAYER_ROT_SPEED = (2.0f * PI);	// ship rotation speed (degree/second)

const float			BULLET_SPEED = 100.0f;		// bullet speed (m/s)

// -----------------------------------------------------------------------------
enum TYPE
{
	// list of game object types
	TYPE_PLAYER = 0,
	TYPE_BULLET,
	TYPE_PLATFORM,

	TYPE_NUM
};

// -----------------------------------------------------------------------------
// object flag definition

const unsigned long FLAG_ACTIVE = 0x00000001;

/******************************************************************************/
/*!
	Struct/Class Definitions
*/
/******************************************************************************/

//Game object structure
struct GameObj
{
	unsigned long		type;		// object type
	AEGfxVertexList* pMesh;		// This will hold the triangles which will form the shape of the object
};

// ---------------------------------------------------------------------------

//Game object instance structure
struct GameObjInst
{
	GameObj* pObject;	// pointer to the 'original' shape
	unsigned long		flag;		// bit flag or-ed together
	float				scale;		// scaling value of the object instance
	AEVec2				posCurr;	// object current position
	AEVec2				velCurr;	// object current velocity
	float				dirCurr;	// object current direction
	AABB				boundingBox;// object bouding box that encapsulates the object
	AEMtx33				transform;	// object transformation matrix: Each frame, 
									// calculate the object instance's transformation matrix and save it here

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

// pointer to the ship object
static GameObjInst* Player;										// Pointer to the "Ship" game object instance

// number of ship available (lives 0 = game over)
static long					playerLives;									// The number of lives left

// the score = number of asteroid destroyed
static unsigned long		playerScore;										// Current score

// ---------------------------------------------------------------------------

// functions to create/destroy a game object instance
GameObjInst* gameObjInstCreate(unsigned long type, float scale,
	AEVec2* pPos, AEVec2* pVel, float dir);
void					gameObjInstDestroy(GameObjInst* pInst);




/******************************************************************************/
/*!
	"Load" function of this state
*/
/******************************************************************************/
void GameStateAsteroidsLoad(void)
{
	// zero the game object array
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	sGameObjNum = 0;

	// zero the game object instance array
	memset(sGameObjInstList, 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sGameObjInstNum = 0;

	// The ship object instance hasn't been created yet, so this "spShip" pointer is initialized to 0
	Player = nullptr;

	// load/create the mesh data (game objects / Shapes)
	GameObj* pObj;

	// =====================
	// create the player shape
	// =====================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_PLAYER;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create ship object!!");


	// ----------------------------------------------------------------------------------------------------------------------------------------------
	// Change the following meshes based on our game dimensions
	// ----------------------------------------------------------------------------------------------------------------------------------------------

	// =======================
	// create the bullet shape
	// =======================
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_BULLET;
	AEGfxMeshStart(); // width = 1.0f, height = 0.25f
	AEGfxTriAdd(
		-0.5f, 0.125f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, -0.125f, 0xFFFFFF00, 0.0f, 1.0f,
		0.5f, -0.125f, 0xFFFFFF00, 1.0f, 1.0f);
	AEGfxTriAdd(
		-0.5f, 0.125f, 0xFFFFFF00, 0.0f, 0.0f,
		0.5f, 0.125f, 0xFFFFFF00, 1.0f, 0.0f,
		0.5f, -0.125f, 0xFFFFFF00, 1.0f, 1.0f);
	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create bullet object!!");

	// =========================
	// create the asteroid shape
	// =========================
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_PLATFORM;
	AEGfxMeshStart();
	AEGfxTriAdd( // width = 0.5f, height = 0.5f
		-0.25f, 0.25f, 0xFF4D5853, 0.0f, 0.0f,
		-0.25f, -0.25f, 0xFF4D5853, 0.0f, 1.0f,
		0.25f, -0.25f, 0xFF4D5853, 1.0f, 1.0f);
	AEGfxTriAdd(
		-0.25f, 0.25f, 0xFF4D5853, 0.0f, 0.0f,
		0.25f, 0.25f, 0xFF4D5853, 1.0f, 0.0f,
		0.25f, -0.25f, 0xFF4D5853, 1.0f, 1.0f);
	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create asteroid object!!");

}

/******************************************************************************/
/*!
	"Initialize" function of this state
*/
/******************************************************************************/
void GameStateAsteroidsInit(void)
{
	// create the player
	Player = gameObjInstCreate(TYPE_PLAYER, PLAYER_SIZE, nullptr, nullptr, 0.0f);
	AE_ASSERT(Player);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> sizeDistribution(20.0f, 200.0f);
	std::uniform_real_distribution<float> posXDistribution(AEGfxGetWinMinX(), AEGfxGetWinMaxX());
	std::uniform_real_distribution<float> posYDistribution(AEGfxGetWinMinY(), AEGfxGetWinMaxY());
	std::uniform_real_distribution<float> velXDistribution(0.0f, 100.0f);
	std::uniform_real_distribution<float> velYDistribution(0.0f, 100.0f);

	// CREATE THE INITIAL ASTEROIDS INSTANCES USING THE "gameObjInstCreate" FUNCTION
	for (int i(0); i < 4; ++i) {
		float rSize{ sizeDistribution(generator) },
			rPosX{ 0 }, rPosY{ 0 },
			rVelX{ velXDistribution(generator) }, rVelY{ velYDistribution(generator) };
		do {
			rPosX = posXDistribution(generator);
		} while (rPosX + 200.0f <= Player->posCurr.x);
		do {
			rPosY = posXDistribution(generator);
		} while (rPosY + 200.0f <= Player->posCurr.y);
		AEVec2 rPos{ rPosX, rPosY }, rVel{ rVelX, rVelY };
		GameObjInst* sAsteroid = gameObjInstCreate(TYPE_PLATFORM, rSize, &rPos, &rVel, 0);
		AE_ASSERT(sAsteroid);
	}

	// reset the score and the number of ships
	playerScore = 0;
	playerLives = PLAYER_INITIAL_NUM;
}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateAsteroidsUpdate(void)
{
	// =========================
	// update according to input
	// =========================

	// This input handling moves the ship without any velocity nor acceleration
	// It should be changed when implementing the Asteroids project
	//
	// Updating the velocity and position according to acceleration is 
	// done by using the following:
	// Pos1 = 1/2 * a*t*t + v0*t + Pos0
	//
	// In our case we need to divide the previous equation into two parts in order 
	// to have control over the velocity and that is done by:
	//
	// v1 = a*t + v0		//This is done when the UP or DOWN key is pressed 
	// Pos1 = v1*t + Pos0

	// ----------------------------------------------------------------------------------------------------------------------------------------------
	// Change the following input movement based on our player movement
	// ----------------------------------------------------------------------------------------------------------------------------------------------


	if (AEInputCheckCurr(AEVK_UP))
	{
		AEVec2 added;
		AEVec2Set(&added, cosf(Player->dirCurr), sinf(Player->dirCurr));
		//AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);//YOU MAY NEED TO CHANGE/REPLACE THIS LINE

		// Find the velocity according to the acceleration
		added.x *= PLAYER_ACCEL_FORWARD * AEFrameRateControllerGetFrameTime();
		added.y *= PLAYER_ACCEL_FORWARD * AEFrameRateControllerGetFrameTime();
		AEVec2Add(&Player->velCurr, &Player->velCurr, &added);
		// Limit your speed over here
		AEVec2Scale(&Player->velCurr, &Player->velCurr, 0.99f);
	}

	if (AEInputCheckCurr(AEVK_DOWN))
	{
		AEVec2 added;
		AEVec2Set(&added, -cosf(Player->dirCurr), -sinf(Player->dirCurr));
		//AEVec2Add(&spShip->posCurr, &spShip->posCurr, &added);//YOU MAY NEED TO CHANGE/REPLACE THIS LINE

		// Find the velocity according to the acceleration
		added.x *= PLAYER_ACCEL_BACKWARD * AEFrameRateControllerGetFrameTime();
		added.y *= PLAYER_ACCEL_BACKWARD * AEFrameRateControllerGetFrameTime();
		AEVec2Add(&Player->velCurr, &Player->velCurr, &added);
		// Limit your speed over here
		AEVec2Scale(&Player->velCurr, &Player->velCurr, 0.99f);
	}

	if (AEInputCheckCurr(AEVK_LEFT))
	{
		Player->dirCurr += PLAYER_ROT_SPEED * (float)(AEFrameRateControllerGetFrameTime());
		Player->dirCurr = AEWrap(Player->dirCurr, -PI, PI);
	}

	if (AEInputCheckCurr(AEVK_RIGHT))
	{
		Player->dirCurr -= PLAYER_ROT_SPEED * (float)(AEFrameRateControllerGetFrameTime());
		Player->dirCurr = AEWrap(Player->dirCurr, -PI, PI);
	}

	// ----------------------------------------------------------------------------------------------------------------------------------------------
	// Change to bullet spawning on mouse click in direction
	// ----------------------------------------------------------------------------------------------------------------------------------------------

	// Shoot a bullet if space is triggered (Create a new object instance)
	if (AEInputCheckTriggered(AEVK_SPACE))
	{
		// Get the bullet's direction according to the ship's direction
		AEVec2 dirBullet;
		AEVec2Set(&dirBullet, cosf(Player->dirCurr), sinf(Player->dirCurr));
		// Set the velocity
		dirBullet.x *= BULLET_SPEED;
		dirBullet.y *= BULLET_SPEED;
		// Create an instance
		gameObjInstCreate(TYPE_BULLET, 10.0f, &Player->posCurr, &dirBullet, Player->dirCurr);
	}

	// ======================================================
	// update physics of all active game object instances
	//  -- Get the bounding rectangle of every active instance:
	//		boundingRect_min = -BOUNDING_RECT_SIZE * instance->scale + instance->pos
	//		boundingRect_max = BOUNDING_RECT_SIZE * instance->scale + instance->pos
	//	-- Positions of the instances are updated here with the computed velocity
	// ======================================================
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		AEVec2 velDt;
		AEVec2Scale(&velDt, &pInst->velCurr, static_cast<f32>(AEFrameRateControllerGetFrameTime()));
		AEVec2Add(&pInst->posCurr, &pInst->posCurr, &velDt);

		// ----------------------------------------------------------------------------------------------------------------------------------------------
		// Change the bounding rect size based on the size used for the mesh
		// ----------------------------------------------------------------------------------------------------------------------------------------------
		
		switch (pInst->pObject->type) {
		case TYPE_PLAYER:
			pInst->boundingBox.min.x = -0.5f * 2 * pInst->scale + pInst->posCurr.x;
			pInst->boundingBox.min.y = -0.5f * 2 * pInst->scale + pInst->posCurr.y;

			pInst->boundingBox.max.x = 0.5f * 2 * pInst->scale + pInst->posCurr.x;
			pInst->boundingBox.max.y = 0.5f * 2 * pInst->scale + pInst->posCurr.y;
			break;
		case TYPE_BULLET:
			pInst->boundingBox.min.x = -0.5f * pInst->scale + pInst->posCurr.x;
			pInst->boundingBox.min.y = -0.125f * pInst->scale + pInst->posCurr.y;

			pInst->boundingBox.max.x = 0.5f * pInst->scale + pInst->posCurr.x;
			pInst->boundingBox.max.y = 0.125f * pInst->scale + pInst->posCurr.y;
			break;
		case TYPE_PLATFORM:
			pInst->boundingBox.min.x = -0.25f * pInst->scale + pInst->posCurr.x;
			pInst->boundingBox.min.y = -0.25f * pInst->scale + pInst->posCurr.y;

			pInst->boundingBox.max.x = 0.25f * pInst->scale + pInst->posCurr.x;
			pInst->boundingBox.max.y = 0.25f * pInst->scale + pInst->posCurr.y;
			break;
		}
	}

	// ====================
	// check for collision
	// ====================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		if (pInst->pObject->type == TYPE_BULLET) {
			for (unsigned long j = 0; j < GAME_OBJ_INST_NUM_MAX; j++)
			{
				GameObjInst* pOtherInst = sGameObjInstList + j;
				// skip non-active object
				if ((pOtherInst->flag & FLAG_ACTIVE) == 0)
					continue;

				switch (pOtherInst->pObject->type) {
				case TYPE_PLATFORM:
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						// ----------------------------------------------------------------------------------------------------------------------------------------------
						// Change to bullet spawning on mouse click in direction
						// ----------------------------------------------------------------------------------------------------------------------------------------------
					}
					break;
				case TYPE_PLAYER:
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {

					}
					break;
				case TYPE_BULLET:
					/*if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						gameObjInstDestroy(pInst);
						gameObjInstDestroy(pOtherInst);
					}*/
					continue;
					break;
				}

			}
		}

	}




	// ===================================
	// update active game object instances
	// Example:
	//		-- Wrap specific object instances around the world (Needed for the assignment)
	//		-- Removing the bullets as they go out of bounds (Needed for the assignment)
	//		-- If you have a homing missile for example, compute its new orientation 
	//			(Homing missiles are not required for the Asteroids project)
	//		-- Update a particle effect (Not required for the Asteroids project)
	// ===================================
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		switch (pInst->pObject->type) {
			// check if the object is a ship
		case TYPE_PLAYER:
			// warp the ship from one end of the screen to the other
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, AEGfxGetWinMinX() - PLAYER_SIZE,
				AEGfxGetWinMaxX() + PLAYER_SIZE);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, AEGfxGetWinMinY() - PLAYER_SIZE,
				AEGfxGetWinMaxY() + PLAYER_SIZE);
			break;
			// Wrap asteroids here
		case TYPE_PLATFORM:
			/*pInst->posCurr.x = AEWrap(pInst->posCurr.x, AEGfxGetWinMinX() - pInst->scale,
				AEGfxGetWinMaxX() + pInst->scale);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, AEGfxGetWinMinY() - pInst->scale,
				AEGfxGetWinMaxY() + pInst->scale);*/
			continue;
			break;
			// Remove bullets that go out of bounds
		case TYPE_BULLET:
			if (pInst->boundingBox.min.x > AEGfxGetWinMaxX() || pInst->boundingBox.max.x < AEGfxGetWinMinX() ||
				pInst->boundingBox.min.y > AEGfxGetWinMaxY() || pInst->boundingBox.max.y < AEGfxGetWinMinY())
				gameObjInstDestroy(pInst);
			break;
		}
	}




	// =====================================
	// calculate the matrix for all objects
	// =====================================

	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;
		AEMtx33		 trans, rot, scale;

		UNREFERENCED_PARAMETER(trans);
		UNREFERENCED_PARAMETER(rot);
		UNREFERENCED_PARAMETER(scale);

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// Compute the scaling matrix
		AEMtx33Scale(&scale, pInst->scale, pInst->scale);
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

*/
/******************************************************************************/
void GameStateAsteroidsDraw(void)
{
	char strBuffer[1024];

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);

	// draw all object instances in the list
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		AEGfxSetTransform(pInst->transform.m);
		// Draw the shape used by the current object instance using "AEGfxMeshDraw"
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}

	//You can replace this condition/variable by your own data.
	//The idea is to display any of these variables/strings whenever a change in their value happens
	/*static bool onValueChange = true;
	if (onValueChange)
	{
		sprintf_s(strBuffer, "Score: %d", playerScore);
		//AEGfxPrint(10, 10, (u32)-1, strBuffer);
		printf("%s \n", strBuffer);

		sprintf_s(strBuffer, "Ship Left: %d", playerLives >= 0 ? playerLives : 0);
		//AEGfxPrint(600, 10, (u32)-1, strBuffer);
		printf("%s \n", strBuffer);

		// display the game over message
		if (playerLives < 0)
		{
			//AEGfxPrint(280, 260, 0xFFFFFFFF, "       GAME OVER       ");
			printf("       GAME OVER       \n");
		}

		onValueChange = false;
	}*/
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateAsteroidsFree(void)
{
	// kill all object instances in the array using "gameObjInstDestroy"
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;

		gameObjInstDestroy(pInst);
	}
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateAsteroidsUnload(void)
{
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
GameObjInst* gameObjInstCreate(unsigned long type,
	float scale,
	AEVec2* pPos,
	AEVec2* pVel,
	float dir)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	AE_ASSERT_PARM(type < sGameObjNum);

	// loop through the object instance list to find a non-used object instance
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = sGameObjList + type;
			pInst->flag = FLAG_ACTIVE;
			pInst->scale = scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;

			// return the newly created instance
			return pInst;
		}
	}

	// cannot find empty slot => return 0
	return 0;
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void gameObjInstDestroy(GameObjInst* pInst)
{
	// if instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// zero out the flag
	pInst->flag = 0;
}