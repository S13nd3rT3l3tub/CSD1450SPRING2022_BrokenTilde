/******************************************************************************/
/*!
\file		GameState_Level1.cpp
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
#include <iostream>

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
const unsigned int	GAME_OBJ_NUM_MAX = 32;			//The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_INST_NUM_MAX = 2048;			//The total number of different game object instances


const unsigned int	PLAYER_INITIAL_NUM = 3;			// initial number of player lives
static AEVec2		PLAYER_SIZE = {70.0f, 30.0f};		// player size
static AEVec2		GUN_SIZE = { 80.0f, 15.0f };		// gun size

const float			PLAYER_ACCEL_FORWARD = 250.0f;		// player forward acceleration (in m/s^2)
const float			PLAYER_ACCEL_BACKWARD = 250.0f;		// player backward acceleration (in m/s^2)
const float			GRAVITY = 9.8f;

static AEVec2		BULLET_SIZE = { 7.5f, 7.5f };
const float			BULLET_SPEED = 300.0f;		// bullet speed (m/s)

// -----------------------------------------------------------------------------
enum TYPE
{
	// list of game object types
	TYPE_PLAYER = 0,
	TYPE_PLAYERGUN,
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
	AEVec2				scale;		// scaling value of the object instance
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

// pointer to the PlayerBody and gun object
static GameObjInst* PlayerBody;										
static GameObjInst* PlayerGun;
// number of player lives available (lives 0 = game over)
static long					playerLives;									// The number of lives left

// the score = number of enemies destroyed
static unsigned long		playerScore;										// Current score

static signed int mouseX{ 0 }, mouseY{ 0 };

// ---------------------------------------------------------------------------

// functions to create/destroy a game object instance
GameObjInst* gameObjInstCreate(unsigned long type, AEVec2* pScale,
	AEVec2* pPos, AEVec2* pVel, float dir);
void					gameObjInstDestroy(GameObjInst* pInst);


/******************************************************************************/
/*!
	"Load" function of this state
*/
/******************************************************************************/
void GameStateLevel1Load(void)
{
	// zero the game object array
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	sGameObjNum = 0;

	// zero the game object instance array
	memset(sGameObjInstList, 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sGameObjInstNum = 0;

	
	PlayerBody = nullptr;

	// load/create the mesh data (game objects / Shapes)
	GameObj* pObj;

	// =====================
	// create the player shape
	// =====================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_PLAYER;

	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create player object!!");

	// =====================
	// create the player gun shape
	// =====================
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_PLAYERGUN;
	AEGfxMeshStart();
	AEGfxTriAdd(
		0.0f, 0.25f, 0xFFFF0000, 0.0f, 0.0f,
		0.0f, -0.25f, 0xFFFF0000, 0.0f, 1.0f,
		0.5f, -0.25f, 0xFFFFFFFF, 1.0f, 1.0f);
	AEGfxTriAdd(
		0.0f, 0.25f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.25f, 0xFFFF0000, 1.0f, 0.0f,
		0.5f, -0.25f, 0xFFFFFFFF, 1.0f, 1.0f);
	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create player gun object!!");

	// =======================
	// create the bullet shape
	// =======================
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_BULLET;
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFF00, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f);
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFF00, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFF00, 1.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f);

	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create bullet object!!");

	// =========================
	// create the platform shape
	// =========================
	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_PLATFORM;
	AEGfxMeshStart();
	AEGfxTriAdd( 
		-0.5f, 0.5f, 0xFF4D5853, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFF4D5853, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF4D5853, 1.0f, 1.0f);
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF4D5853, 0.0f, 0.0f,
		0.5f, 0.5f, 0xFF4D5853, 1.0f, 0.0f,
		0.5f, -0.5f, 0xFF4D5853, 1.0f, 1.0f);
	pObj->pMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pObj->pMesh, "fail to create platform object!!");
}

/******************************************************************************/
/*!
	"Initialize" function of this state
*/
/******************************************************************************/
void GameStateLevel1Init(void)
{
	// create the player
	AEVec2 spawnpoint = { 0,100 };
	PlayerBody = gameObjInstCreate(TYPE_PLAYER, &PLAYER_SIZE, &spawnpoint, nullptr, 0.0f);
	AE_ASSERT(PlayerBody);

	PlayerGun = gameObjInstCreate(TYPE_PLAYERGUN, &GUN_SIZE, nullptr, nullptr, 0.0f);
	AE_ASSERT(PlayerGun);

	AEVec2 platVel, platPos, platScale;
	platVel = { 0, 0 };
	platScale = { 100, 600 };
	platPos = { AEGfxGetWinMinX(), 0 };
	gameObjInstCreate(TYPE_PLATFORM, &platScale, &platPos, &platVel, PI/2); // left wall
	platPos = { 0,AEGfxGetWinMinY() };
	platScale = { 700, 100 };
	gameObjInstCreate(TYPE_PLATFORM, &platScale, &platPos, &platVel, 0.0f); //floor
	platPos = { AEGfxGetWinMaxX(), 0 };
	platScale = { 100, 600 };
	gameObjInstCreate(TYPE_PLATFORM, &platScale, &platPos, &platVel, PI/2); // right wall
	platPos = { 0,AEGfxGetWinMaxY() };
	platScale = { 700, 100 };
	gameObjInstCreate(TYPE_PLATFORM, &platScale, &platPos, &platVel, 0.0f); // ceiling

	platPos = { AEGfxGetWinMinX() + 100, AEGfxGetWinMinY() + 150 };
	platScale = { 150, 50 };
	gameObjInstCreate(TYPE_PLATFORM, &platScale, &platPos, &platVel, 0.0f); // platform1

	//platPos = { 0,AEGfxGetWinMaxY() };
	//platScale = { 700, 100 };
	//gameObjInstCreate(TYPE_PLATFORM, &platScale, &platPos, &platVel, 0.0f); // platform2

	// reset the score and the number of lives
	playerScore = 0;
	playerLives = PLAYER_INITIAL_NUM;
}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateLevel1Update(void)
{
	// =========================
	// update according to input
	// =========================

	// ----------------------------------------------------------------------------------------------------------------------------------------------
	// Change the following input movement based on our player movement
	// ----------------------------------------------------------------------------------------------------------------------------------------------


	AEInputGetCursorPosition(&mouseX, &mouseY);
	// Coordinate conversion
	mouseX -= (AEGfxGetWinMaxX() - AEGfxGetWinMinX()) / 2;
	mouseY -= (AEGfxGetWinMaxY() - AEGfxGetWinMinY()) / 2;
	mouseY = -mouseY;
	float dotProduct = atan2(mouseY - PlayerBody->posCurr.y, mouseX - PlayerBody->posCurr.x);
	PlayerGun->dirCurr = dotProduct;

	if (AEInputCheckCurr(AEVK_UP)) // DEV TOOL, Delete all bullet on screen.
	{
		for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
			GameObjInst* pInst = sGameObjInstList + i;

			// skip non-active object
			if ((pInst->flag & FLAG_ACTIVE) == 0)
				continue;

			if (pInst->pObject->type == TYPE_BULLET)
				gameObjInstDestroy(pInst);

		}
	}

	if (AEInputCheckTriggered(AEVK_W)) // JUMP - right now, can infinitely jump. Need to implement hotspot at bottom of tank to detect
	{									// whether the tank is touching the ground or not.
		AEVec2 added;
		AEVec2Set(&added, 0.f, 1.f);

		// Find the velocity according to the acceleration
		added.x *= 1;//PLAYER_ACCEL_FORWARD * g_dt;
		added.y *= 29000 * g_dt;
		AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);
		// Limit your speed over here
		AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.99f);
	}

	if (AEInputCheckCurr(AEVK_A)) // Move left
	{
		AEVec2 added;
		AEVec2Set(&added, cosf(PlayerBody->dirCurr), sinf(PlayerBody->dirCurr));

		// Find the velocity according to the acceleration
		added.x *= -PLAYER_ACCEL_BACKWARD * g_dt;
		added.y *= -PLAYER_ACCEL_BACKWARD * g_dt;
		AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);
		// Limit your speed over here
		AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.99f);
	}

	if (AEInputCheckCurr(AEVK_D))
	{
		//Player->dirCurr -= PLAYER_ROT_SPEED * (float)(g_dt);
		//Player->dirCurr = AEWrap(Player->dirCurr, -PI, PI);

		AEVec2 added;
		AEVec2Set(&added, cosf(PlayerBody->dirCurr), sinf(PlayerBody->dirCurr));

		// Find the velocity according to the acceleration
		added.x *= PLAYER_ACCEL_FORWARD * g_dt;
		added.y *= PLAYER_ACCEL_FORWARD * g_dt;
		AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);
		// Limit your speed over here
		AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.99f);
	}

	// ----------------------------------------------------------------------------------------------------------------------------------------------
	// Change to bullet spawning on mouse click in direction
	// ----------------------------------------------------------------------------------------------------------------------------------------------

	// Shoot a bullet if left mouse button is triggered (Create a new object instance)
	if (AEInputCheckTriggered(VK_LBUTTON))
	{
		// Get the bullet's direction according to the player's direction
		AEVec2 dirBullet;
		AEVec2Set(&dirBullet, cosf(PlayerGun->dirCurr), sinf(PlayerGun->dirCurr));
		// Set the velocity
		dirBullet.x *= BULLET_SPEED;
		dirBullet.y *= BULLET_SPEED;
		// Calculate end of gun barrel position
		// 
		// 
		// Create an instance
		gameObjInstCreate(TYPE_BULLET, &BULLET_SIZE, &PlayerGun->posCurr, &dirBullet, PlayerGun->dirCurr);
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
		AEVec2Scale(&velDt, &pInst->velCurr, static_cast<f32>(g_dt));
		AEVec2Add(&pInst->posCurr, &pInst->posCurr, &velDt);

		if (pInst == PlayerBody) 
		{
			pInst->velCurr.x *= 0.98f; // brakes tank when no longer holding the arrow keys.
			//pInst->velCurr.y *= 0.98f;
			//if (pInst->posCurr.y != -255)
			//{
			//	pInst->posCurr.y = -255;
			//}
			pInst->velCurr.y -= GRAVITY;	// enforce gravity
		}
		if (pInst == PlayerGun) // attach turret to body
		{
			pInst->posCurr = PlayerBody->posCurr;
		}

		// ----------------------------------------------------------------------------------------------------------------------------------------------
		// Change the bounding rect size based on the size used for the mesh
		// ----------------------------------------------------------------------------------------------------------------------------------------------
		pInst->boundingBox.min.x = -0.5f * pInst->scale.x + pInst->posCurr.x;
		pInst->boundingBox.min.y = -0.5f * pInst->scale.y + pInst->posCurr.y;

		pInst->boundingBox.max.x = 0.5f * pInst->scale.x + pInst->posCurr.x;
		pInst->boundingBox.max.y = 0.5f * pInst->scale.y + pInst->posCurr.y;
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

		switch (pInst->pObject->type){
		case TYPE_BULLET:
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
						// newVel = oldVel - 2(oldVel.normal)normal
						/*AEVec2 normal, newBulletVel;
						normal = { -pOtherInst->boundingBox.max.y, pOtherInst->boundingBox.max.x };
						AEVec2Normalize(&normal, &normal);
						newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
						newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
						pInst->velCurr = newBulletVel;*/

						// Find angle instead
						//std::cout << "Old dir: " << AERadToDeg(pInst->dirCurr) << " | ";
						float newDir = pOtherInst->dirCurr - (pInst->dirCurr - pOtherInst->dirCurr);
						newDir = AEWrap(newDir, -PI, PI);
						pInst->dirCurr = newDir;
						//std::cout << "New dir: " << AERadToDeg(pInst->dirCurr) << std::endl;
						AEVec2 added;
						AEVec2Set(&added, cosf(pInst->dirCurr), sinf(pInst->dirCurr));
						//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
						// Find the velocity according to the acceleration
						added.x *= BULLET_SPEED;
						added.y *= BULLET_SPEED;
						pInst->velCurr = added;
						AEVec2 velDt;
						AEVec2Scale(&velDt, &pInst->velCurr, static_cast<f32>(g_dt));
						AEVec2Add(&pInst->posCurr, &pInst->posCurr, &velDt);
						//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << std::endl;
					
						// Find new angle first
						//float newDirection = pOtherInst->dirCurr - (pInst->dirCurr - pOtherInst->dirCurr);
						//newDirection = AEWrap(newDirection, -PI, PI);
						////std::cout << newDirection << std::endl;
						//pInst->dirCurr = newDirection;

						//// Get the new velocity
						//AEVec2Set(&pInst->velCurr, cosf(pInst->dirCurr), sinf(pInst->dirCurr));
						//pInst->velCurr.x *= BULLET_SPEED;
						//pInst->velCurr.y *= BULLET_SPEED;
					}
					break;
				case TYPE_PLAYER:
					continue;
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
			break;
		case TYPE_PLAYER:
			for (unsigned long j = 0; j < GAME_OBJ_INST_NUM_MAX; j++)
			{
				GameObjInst* pOtherInst = sGameObjInstList + j;
				// skip non-active object
				if ((pOtherInst->flag & FLAG_ACTIVE) == 0)
					continue;

				switch (pOtherInst->pObject->type) {
				case TYPE_PLATFORM:
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						pInst->velCurr.y = 0;
					}
				case TYPE_PLAYER:
					continue;
					break;
				case TYPE_BULLET:
					continue;
					break;
				}
			}
			break;
		case TYPE_PLAYERGUN:
			continue;
			break;
		case TYPE_PLATFORM:
			continue;
			break;
		}
	}




	// ===================================
	// update active game object instances
	// ===================================
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		switch (pInst->pObject->type) {
		// check if the object is a player
		case TYPE_PLAYER:
			// warp the player from one end of the screen to the other
			pInst->posCurr.x = AEWrap(pInst->posCurr.x, AEGfxGetWinMinX() - pInst->scale.x,
				AEGfxGetWinMaxX() + pInst->scale.x);
			pInst->posCurr.y = AEWrap(pInst->posCurr.y, AEGfxGetWinMinY() - pInst->scale.y,
				AEGfxGetWinMaxY() + pInst->scale.y);
			break;
			
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
		AEMtx33Scale(&scale, pInst->scale.x, pInst->scale.y);
		// Compute the rotation matrix 
		//if (pInst->pObject->type == TYPE_BULLET || pInst->pObject->type == TYPE_PLATFORM) 
		if (pInst->pObject->type == TYPE_BULLET || pInst->pObject->type == TYPE_PLATFORM)
			AEMtx33Rot(&rot, 0);
		else
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
void GameStateLevel1Draw(void)
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
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateLevel1Free(void)
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
void GameStateLevel1Unload(void)
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
	AEVec2* pScale,
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
			pInst->scale = pScale ? *pScale : zero;
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