/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       Globals.cpp
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

// ---------------------------------------------------------------------------
// Globals
float	 g_dt;
double	 g_appTime;
double	 levelTime;
s8		 g_font12;
s8		 g_font20;
s8		 g_font30;
int		 g_mouseX;
int		 g_mouseY;
int		 g_chosenLevel{ 0 };

int winWidth{ 1280 };
int winHeight{ 720 };


 AEVec2		PLAYER_MESHSIZE = { 0.8f, 1.0f };
 AEVec2		PLAYER_SCALE = { 2.0f, 1.0f };		// player scaling
 AEVec2		GUN_MESHSIZE = { 0.5f, 0.5f };
 AEVec2		GUN_SCALE = { 2.6f, 0.7f };		// gun size

 AEVec2		BULLET_MESHSIZE = { 1.0f, 1.0f };
 AEVec2		BULLET_SCALE = { 0.25f, 0.25f };


 AEVec2		PLATFORM_MESHSIZE = { 1.0f, 1.0f };
 AEVec2		PLATFORM_SCALE = { 5.0f, 5.0f };
 AEVec2		EMPTY_MESHSIZE = { 1.0f, 1.0f };
 AEVec2		EMPTY_SCALE = { 1.0f, 1.0f };

 AEVec2		HEALTHBAR_MESHSIZE = { 0.1f, 0.75f };
 AEVec2		HEALTHBAR_SCALE = { 15.0f, 0.5f, };

 AEVec2		SCRIPTIMAGE_MESHSIZE = { 1.0f, 1.0f };
 AEVec2		SCRIPTIMAGE_SCALE = { 1.0f, 1.0f };

 /******************************************************************************/
/*!
	Static Variables
*/
/******************************************************************************/
 bool toFullScreen{ true };
 bool soundVolumeLevel{ true };
 unsigned long currInnerState{GAME_PLAY};

 FMOD::System *fmodSys{nullptr};
 FMOD::Sound *mainMenuBG{nullptr};
 FMOD::Sound* playerShoot{ nullptr };
 FMOD::Channel* soundChannel{nullptr};

// list of original object
GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
unsigned long		sGameObjNum;								// The number of defined game objects

// list of object instances
GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
unsigned long		sGameObjInstNum;							// The number of used game object instances

// pointer to the PlayerBody and gun object
GameObjInst* PlayerBody;
GameObjInst* PlayerGun;
GameObj* PlayerHealthBar;
GameObjInst* Enemydetection;

GameObjInst* EmptyInstance;
GameObjInst* PlatformInstance;
GameObjInst* DirtInstance;

// Game variables 
float			playerHealth;									// The amount of health left
double			jumpFuel;
int				totalEnemyCount;
int				ammoCount;

unsigned long emptyObjIndex;
unsigned long platformObjIndex;
unsigned long playerObjIndex;
unsigned long playerGunObjIndex;
unsigned long bulletObjIndex;
unsigned long enemy1ObjIndex;
unsigned long enemy2ObjIndex;
unsigned long particleObjIndex;
unsigned long dottedObjIndex;
unsigned long dirtObjIndex;
unsigned long hpObjIndex;
unsigned long splashObjIndex;
unsigned long bgObjIndex;
unsigned long buttonObjIndex;

 // Current mouse position
  float worldMouseX{ 0 }, worldMouseY{ 0 };

 //Binary map data
  int** MapData;
  int** BinaryCollisionArray;
  int				BINARY_MAP_WIDTH;
  int				BINARY_MAP_HEIGHT;

 // Transform matrix containing shift of grid to world coordinates
 // concatenate this with object instance's own transform matrix
  AEMtx33			MapTransform;

/******************************************************************************/
 /*!
	 Textures
 */
 /******************************************************************************/
  AEGfxTexture* stoneTexture = nullptr;
  AEGfxTexture* dirtTexture = nullptr;

/******************************************************************************/
/*!

*/
/******************************************************************************/
/*GameObjInst* gameObjInstCreate(unsigned int type, AEVec2* scale,
	AEVec2* pPos, AEVec2* pVel,
	float dir, enum STATE startState)
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

			pInst->gridCollisionFlag = 0;
			pInst->pUserData = 0;

			pInst->state = startState;
			pInst->innerState = INNER_STATE::INNER_STATE_ON_ENTER;

			pInst->bulletbounce = 0;

			pInst->counter = 0;
			pInst->shoot_timer = -1.0f;
			pInst->shoot_timer2 = -1.0f;

			// return the newly created instance
			return pInst;
		}
	}

	return 0;
}*/

 GameObjInst* gameObjInstCreate(GameObj* objType, AEVec2* scale,
	  AEVec2* pPos, AEVec2* pVel,
	  float dir, enum STATE startState)
{
	AEVec2 zero;
	AEVec2Zero(&zero);

	// loop through the object instance list to find a non-used object instance
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// check if current instance is not used
		if (pInst->flag == 0)
		{
			// it is not used => use it to create the new instance
			pInst->pObject = objType;
			pInst->flag = FLAG_ACTIVE | FLAG_VISIBLE;
			pInst->scale = *scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;

			pInst->gridCollisionFlag = 0;
			pInst->pUserData = 0;

			pInst->state = startState;
			pInst->innerState = INNER_STATE::INNER_STATE_ON_ENTER;

			pInst->bulletbounce = 0;

			pInst->counter = 0;
			pInst->shoot_timer = -1.0f;
			pInst->shoot_timer2 = -1.0f;

			// return the newly created instance
			return pInst;
		}
	}

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

void EnemyStateMachine(GameObjInst* pInst)
{
	//std::cout << pInst->innerState << std::endl;
	int offsetcheck{};
	//std::cout << pInst->velCurr.x << std::endl;
	//std::cout << (pInst->gridCollisionFlag & COLLISION_RIGHT) << std::endl;
	switch (pInst->state) {

	case STATE_GOING_LEFT:

		switch (pInst->innerState) {
		case INNER_STATE_ON_ENTER:
			//std::cout << "GOING LEFT: INNER_STATE_ON_ENTER\n";
			pInst->velCurr.x = -MOVE_VELOCITY_ENEMY;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			break;

		case INNER_STATE_ON_UPDATE:
			//std::cout << "GOING LEFT: INNER_STATE_ON_UPDATE\n";
			offsetcheck = CheckInstanceBinaryMapCollision(pInst->posCurr.x - 2.0f, pInst->posCurr.y - 1.0f, 2.0f, 1.f, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
			pInst->velCurr.x = -MOVE_VELOCITY_ENEMY;
			/*if ( (CheckInstanceBinaryMapCollision(pInst->posCurr.x - pInst->pObject->meshSize.x * pInst->scale.x,
												pInst->posCurr.y,
												pInst->pObject->meshSize.x * pInst->scale.x,
												pInst->pObject->meshSize.y * pInst->scale.y) & COLLISION_LEFT) == COLLISION_LEFT ||
				(CheckInstanceBinaryMapCollision(pInst->posCurr.x - pInst->pObject->meshSize.x * pInst->scale.x,
					pInst->posCurr.y - pInst->pObject->meshSize.y * pInst->scale.y,
					pInst->pObject->meshSize.x * pInst->scale.x,
					pInst->pObject->meshSize.y * pInst->scale.y) & COLLISION_RIGHT) != COLLISION_RIGHT )*/

			if ((pInst->gridCollisionFlag & COLLISION_LEFT) == COLLISION_LEFT || (offsetcheck & COLLISION_RIGHT) != COLLISION_RIGHT)
			{
				pInst->counter = ENEMY_IDLE_TIME;
				pInst->innerState = INNER_STATE_ON_EXIT;
				pInst->velCurr.x = 0;
			}

			if (AEVec2SquareDistance(&(PlayerBody->posCurr), &(pInst->posCurr)) <= ENEMY_DETECTION_RANGE) {
				AEVec2 offset{};
				AEVec2 dist{ PlayerBody->posCurr.x - pInst->posCurr.x, PlayerBody->posCurr.y - pInst->posCurr.y };
				AEVec2Normalize(&dist, &dist);
				for (int multiply{ 1 }; multiply < 60; ++multiply) {	// set range of sight here (multiply)
					offset.x = pInst->posCurr.x + dist.x * multiply * 0.3f;
					offset.y = pInst->posCurr.y + dist.y * multiply * 0.3f;
					Enemydetection = gameObjInstCreate(&sGameObjList[dottedObjIndex], &BULLET_SCALE, &offset, 0, 0.f, STATE_GOING_RIGHT);
					Enemydetection->gridCollisionFlag = CheckInstanceBinaryMapCollision(Enemydetection->posCurr.x, Enemydetection->posCurr.y, Enemydetection->pObject->meshSize.x * Enemydetection->scale.x, Enemydetection->pObject->meshSize.y * Enemydetection->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
					if (Enemydetection->gridCollisionFlag > 0)	// Environment collision
						break;
					else if (CollisionIntersection_RectRect(Enemydetection->boundingBox, Enemydetection->velCurr, PlayerBody->boundingBox, PlayerBody->velCurr)) {
						pInst->state = STATE_ALERT;
						pInst->innerState = INNER_STATE_ON_ENTER;
					}
				}
			}
			break;
		case INNER_STATE_ON_EXIT:
			//std::cout << "GOING LEFT: INNER_STATE_ON_EXIT\n";
			pInst->counter -= g_dt;
			if (pInst->counter < 0)
			{
				pInst->state = STATE_GOING_RIGHT;
				pInst->innerState = INNER_STATE_ON_ENTER;
			}

			if (AEVec2SquareDistance(&(PlayerBody->posCurr), &(pInst->posCurr)) <= ENEMY_DETECTION_RANGE) {
				AEVec2 offset{};
				AEVec2 dist{ PlayerBody->posCurr.x - pInst->posCurr.x, PlayerBody->posCurr.y - pInst->posCurr.y };
				AEVec2Normalize(&dist, &dist);
				for (int multiply{ 1 }; multiply < 60; ++multiply) {	// set range of sight here (multiply)
					offset.x = pInst->posCurr.x + dist.x * multiply * 0.3f;
					offset.y = pInst->posCurr.y + dist.y * multiply * 0.3f;
					Enemydetection = gameObjInstCreate(&sGameObjList[dottedObjIndex], &BULLET_SCALE, &offset, 0, 0.f, STATE_GOING_RIGHT);
					Enemydetection->gridCollisionFlag = CheckInstanceBinaryMapCollision(Enemydetection->posCurr.x, Enemydetection->posCurr.y, Enemydetection->pObject->meshSize.x * Enemydetection->scale.x, Enemydetection->pObject->meshSize.y * Enemydetection->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
					if (Enemydetection->gridCollisionFlag > 0)	// Environment collision
						break;
					else if (CollisionIntersection_RectRect(Enemydetection->boundingBox, Enemydetection->velCurr, PlayerBody->boundingBox, PlayerBody->velCurr)) {
						pInst->state = STATE_ALERT;
						pInst->innerState = INNER_STATE_ON_ENTER;
					}
				}
			}

			break;
		}
		break;
	case STATE_GOING_RIGHT:

		switch (pInst->innerState) {

		case INNER_STATE_ON_ENTER:
			//std::cout << "GOING RIGHT: INNER_STATE_ON_ENTER\n";
			pInst->velCurr.x = MOVE_VELOCITY_ENEMY;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			break;

		case INNER_STATE_ON_UPDATE:
			//std::cout << "GOING RIGHT: INNER_STATE_ON_UPDATE\n";
			offsetcheck = CheckInstanceBinaryMapCollision(pInst->posCurr.x + 2.0f, pInst->posCurr.y - 1.0f, 2.0f, 1.0f, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
			/*if ((CheckInstanceBinaryMapCollision(pInst->posCurr.x + pInst->pObject->meshSize.x * pInst->scale.x,
				pInst->posCurr.y,
				pInst->pObject->meshSize.x * pInst->scale.x,
				pInst->pObject->meshSize.y * pInst->scale.y) & COLLISION_RIGHT) == COLLISION_RIGHT ||
				(CheckInstanceBinaryMapCollision(pInst->posCurr.x + pInst->pObject->meshSize.x * pInst->scale.x,
					pInst->posCurr.y - pInst->pObject->meshSize.y * pInst->scale.y,
					pInst->pObject->meshSize.x * pInst->scale.x,
					pInst->pObject->meshSize.y * pInst->scale.y) & COLLISION_LEFT) != COLLISION_LEFT)*/
			if ((pInst->gridCollisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT || (offsetcheck & COLLISION_LEFT) != COLLISION_LEFT)
			{
				pInst->counter = ENEMY_IDLE_TIME;
				pInst->innerState = INNER_STATE_ON_EXIT;
				pInst->velCurr.x = 0;
			}

			if (AEVec2SquareDistance(&(PlayerBody->posCurr), &(pInst->posCurr)) <= ENEMY_DETECTION_RANGE) {
				AEVec2 offset{};
				AEVec2 dist{ PlayerBody->posCurr.x - pInst->posCurr.x, PlayerBody->posCurr.y - pInst->posCurr.y };
				AEVec2Normalize(&dist, &dist);
				for (int multiply{ 1 }; multiply < 30; ++multiply) {	// set range of sight here (multiply)
					offset.x = pInst->posCurr.x + dist.x * multiply * 0.3f;
					offset.y = pInst->posCurr.y + dist.y * multiply * 0.3f;
					Enemydetection = gameObjInstCreate(&sGameObjList[dottedObjIndex], &BULLET_SCALE, &offset, 0, 0.f, STATE_GOING_RIGHT);
					Enemydetection->gridCollisionFlag = CheckInstanceBinaryMapCollision(Enemydetection->posCurr.x, Enemydetection->posCurr.y, Enemydetection->pObject->meshSize.x * Enemydetection->scale.x, Enemydetection->pObject->meshSize.y * Enemydetection->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
					if (Enemydetection->gridCollisionFlag > 0)	// Environment collision
						break;
					else if (CollisionIntersection_RectRect(Enemydetection->boundingBox, Enemydetection->velCurr, PlayerBody->boundingBox, PlayerBody->velCurr)) {
						pInst->state = STATE_ALERT;
						pInst->innerState = INNER_STATE_ON_ENTER;
					}
				}
			}

			break;

		case INNER_STATE_ON_EXIT:
			pInst->counter -= g_dt;
			//std::cout << "GOING RIGHT: INNER_STATE_ON_EXIT\n";
			if (pInst->counter < 0)
			{
				pInst->state = STATE_GOING_LEFT;
				pInst->innerState = INNER_STATE_ON_ENTER;
			}

			if (AEVec2SquareDistance(&(PlayerBody->posCurr), &(pInst->posCurr)) <= ENEMY_DETECTION_RANGE) {
				AEVec2 offset{};
				AEVec2 dist{ PlayerBody->posCurr.x - pInst->posCurr.x, PlayerBody->posCurr.y - pInst->posCurr.y };
				AEVec2Normalize(&dist, &dist);
				for (int multiply{ 1 }; multiply < 30; ++multiply) {	// set range of sight here (multiply)
					offset.x = pInst->posCurr.x + dist.x * multiply * 0.3f;
					offset.y = pInst->posCurr.y + dist.y * multiply * 0.3f;
					Enemydetection = gameObjInstCreate(&sGameObjList[dottedObjIndex], &BULLET_SCALE, &offset, 0, 0.f, STATE_GOING_RIGHT);
					Enemydetection->gridCollisionFlag = CheckInstanceBinaryMapCollision(Enemydetection->posCurr.x, Enemydetection->posCurr.y, Enemydetection->pObject->meshSize.x * Enemydetection->scale.x, Enemydetection->pObject->meshSize.y * Enemydetection->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
					if (Enemydetection->gridCollisionFlag > 0)	// Environment collision
						break;
					else if (CollisionIntersection_RectRect(Enemydetection->boundingBox, Enemydetection->velCurr, PlayerBody->boundingBox, PlayerBody->velCurr)) {
						pInst->state = STATE_ALERT;
						pInst->innerState = INNER_STATE_ON_ENTER;
					}
				}
			}

			break;
		}
		break;
	case STATE_ALERT:
		switch (pInst->innerState) {
		case INNER_STATE_ON_ENTER:
			//std::cout << "ALERT: INNER_STATE_ON_ENTER\n";
			pInst->velCurr.x = 0;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			break;
		case INNER_STATE_ON_UPDATE:
			//std::cout << "ALERT: INNER_STATE_ON_UPDATE\n";

			if (AEVec2SquareDistance(&(PlayerBody->posCurr), &(pInst->posCurr)) <= ENEMY_DETECTION_RANGE) {
				AEVec2 offset{};
				AEVec2 dist{ PlayerBody->posCurr.x - pInst->posCurr.x, PlayerBody->posCurr.y - pInst->posCurr.y };
				AEVec2Normalize(&dist, &dist);
				for (int multiply{ 1 }; multiply < 30; ++multiply) {	// set range of sight here (multiply)
					offset.x = pInst->posCurr.x + dist.x * multiply * 0.3f;
					offset.y = pInst->posCurr.y + dist.y * multiply * 0.3f;
					Enemydetection = gameObjInstCreate(&sGameObjList[dottedObjIndex], &BULLET_SCALE, &offset, 0, 0.f, STATE_GOING_RIGHT);
					Enemydetection->gridCollisionFlag = CheckInstanceBinaryMapCollision(Enemydetection->posCurr.x, Enemydetection->posCurr.y, Enemydetection->pObject->meshSize.x * Enemydetection->scale.x, Enemydetection->pObject->meshSize.y * Enemydetection->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
					if (Enemydetection->gridCollisionFlag > 0) {// Environment collision
						pInst->innerState = INNER_STATE_ON_EXIT;
						break;
					}
				}
			}
			if (AEVec2SquareDistance(&(PlayerBody->posCurr), &(pInst->posCurr)) > ENEMY_DETECTION_RANGE) {
				pInst->innerState = INNER_STATE_ON_EXIT;
				break;
			}

			pInst->shoot_timer2 -= g_dt;
			if (pInst->shoot_timer2 > 0.39f) {
				pInst->shoot_timer -= g_dt;
				if (pInst->shoot_timer < 0)
				{
					AEVec2 dist{ PlayerBody->posCurr.x - pInst->posCurr.x, PlayerBody->posCurr.y - pInst->posCurr.y };
					AEVec2Normalize(&dist, &dist);
					AEVec2 shootpos{ pInst->posCurr.x + dist.x * 1.5f, pInst->posCurr.y + dist.y * 1.5f };
					AEVec2 bulletvelocity{ dist.x * 7 , dist.y * 7 };
					gameObjInstCreate(&sGameObjList[bulletObjIndex], &BULLET_SCALE, &shootpos, &bulletvelocity, pInst->dirCurr, STATE::STATE_GOING_LEFT);
					pInst->shoot_timer = 0.2f;
				}
			}

			if (pInst->shoot_timer2 < 0)
			{
				pInst->shoot_timer2 = 1.f;
			}

			break;


		case INNER_STATE_ON_EXIT:
			//std::cout << "ALERT: INNER_STATE_ON_EXIT\n";
			pInst->state = STATE_GOING_RIGHT;
			pInst->innerState = INNER_STATE_ON_ENTER;
			break;
		}
		break;
	}
}


void RectMeshAdd(float x, float  y, float length, float height){
	AEGfxTriAdd(
		x, y - height, 0xFFFF0000, 0.0f, 1.0f,
		x + length, y - height, 0xFFFF0000, 1.0f, 1.0f,
		x, y, 0xFFFF0000, 0.0f, 0.0f);

	AEGfxTriAdd(
		x + length, y, 0xFFFF0000, 1.0f, 0.0f,
		x + length, y - height, 0xFFFF0000, 1.0f, 1.0f,
		x, y, 0xFFFF0000, 0.0f, 0.0f);
}