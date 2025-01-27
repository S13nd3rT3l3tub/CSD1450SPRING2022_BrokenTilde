/*!
@copyright	All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
@project	Ricochet
@file       Globals.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu) [~40% of code]
@role		Initial creation & defining variables used across multiple files.
			Assisted with Enemy AI algorithm
-------------------------------------------------------------------------------
@author		Mohamed Zafir (m.zafir@digipen.edu) [~30% of code]
@role		Added variables and parts of the Enemy AI algorithm
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu) [~30% of code]
@role		Added variables and functions required for pause menu & in-game pause
			menu
*//*_________________________________________________________________________*/

// ----- Include Files -----
#include "Main.h"

/******************************************************************************/
/*!
	Main Globals
*/
/******************************************************************************/
// ----- Time -----
float	 g_dt{ 0.0f };			// Delta time
double	 g_appTime{ 0.0 };		// Application time
double	 levelTime{ 0.0 };		// Level time

// ----- Font -----
s8		 g_font12{};			// Font size 12
s8		 g_font20{};			// Font size 20
s8		 g_font30{};			// Font size 30

// ----- Mouse -----
int		 g_mouseX{ 0 };			// Mouse window pos X
int		 g_mouseY{ 0 };			// Mouse window pos Y
float	 worldMouseX{ 0.0f };	// World window pos X
float	 worldMouseY{ 0.0f };	// World window pos Y

// ----- Window -----
bool winFocused{ true };		// Window focused flag variable
bool toFullScreen{ true };		// Window fullscreen flag variable

// ----- Sound -----
bool			soundVolumeLevel{ true };	// Sound volume flag variable
FMOD::System* fModSys{ nullptr };			// FMOD system
FMOD::Sound* mainMenuBG{ nullptr };		// Background sound pointer
FMOD::Sound* playerShoot{ nullptr };		// Projectile shooting sount pointer
FMOD::Channel* soundChannel{ nullptr };	// FMOD sound channel pointer

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
// ----- Mesh Sizes & Scales -----
AEVec2		PLAYER_MESHSIZE	{ 0.8f, 1.0f };			// Player mesh size	
AEVec2		PLAYER_SCALE	{ 2.0f, 1.0f };			// Player scale size
AEVec2		GUN_MESHSIZE	{ 0.5f, 0.5f };			// Gun barrel mesh size
AEVec2		GUN_SCALE		{ 2.6f, 0.7f };			// Gun barrel scale size

AEVec2		BULLET_MESHSIZE	{ 1.0f, 1.0f };			// Bullet mesh size
AEVec2		BULLET_SCALE	{ 0.25f, 0.25f };		// Bullet scale size

AEVec2		PLATFORM_MESHSIZE	{ 1.0f, 1.0f };		// Platform mesh size
AEVec2		PLATFORM_SCALE		{ 5.0f, 5.0f };		// Platform scale size
AEVec2		EMPTY_MESHSIZE		{ 1.0f, 1.0f };		// Void mesh size
AEVec2		EMPTY_SCALE			{ 1.0f, 1.0f };		// Void scale size

AEVec2		HEALTHBAR_MESHSIZE	{ 0.1f, 0.75f };	// Health bar mesh size
AEVec2		HEALTHBAR_SCALE		{ 15.0f, 0.5f, };	// Health bar scale size

// ----- Mesh Sizes & Scales -----
AEVec2		BUTTON_MESHSIZE{ 500.0f, 100.0f };		// Button mesh size
AEVec2		BUTTON_SCALE{ 1.0f, 1.0f };				// Button scale size

/******************************************************************************/
/*!
	Static Variables
*/
/******************************************************************************/
// list of original object
GameObj				sGameObjList[GAME_OBJ_NUM_MAX]{};				// Each element in this array represents a unique game object (shape)
unsigned long		sGameObjNum{};									// The number of defined game objects

// list of object instances
GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX]{};	// Each element in this array represents a unique game object instance (sprite)
unsigned long		sGameObjInstNum{};							// The number of used game object instances

// ----- Pause Menu Textures ------
GameObjInst			sPauseMenuInstList[GAME_OBJ_INST_NUM_MAX]{};	// Each element in this array represents a unique game object instance for Pause Menu (sprite)
unsigned long		sPauseMenuInstNum{};							// The number of used game object instances for Pause Menu

// Pointer to specific game object instances
GameObjInst*	PlayerBody{};		// Player object instance
GameObjInst*	PlayerGun{};		// Player barrel object instance
GameObj*		PlayerHealthBar{};	// Player health bar object instance

GameObjInst*	EmptyInstance;		// Void object instance
GameObjInst*	PlatformInstance;	// Platform object instance
GameObjInst*	DirtInstance;		// Dirt object instance

GameObjInst*	Enemydetection;		// Enemy LoS check object instance

// Game specific variables
float			playerHealth{};			// The amount of health left
float			playerDeathTimer{};		// Timer for player defeat
double			jumpFuel{};				// Hover fuel
int				totalEnemyCount{};		// Total enemy count in current level
int				ammoCount{};			// Total ammo count in current level

// Object index in object list
unsigned long	emptyObjIndex{};		// Empty object index
unsigned long	platformObjIndex{};		// Platform object index
unsigned long	playerObjIndex{};		// Player object index
unsigned long	playerGunObjIndex{};	// Player gun object index
unsigned long	bulletObjIndex{};		// Bullet object index
unsigned long	enemy1ObjIndex{};		// Enemy 1st variant object instance
unsigned long	enemy2ObjIndex{};		// Enemy 2nd variant object instance
unsigned long	particleObjIndex{};		// Particle object index
unsigned long	dottedObjIndex{};		// Dotted object index
unsigned long	dirtObjIndex{};			// Dirt object index
unsigned long	hpObjIndex{};			// Health bar object index
unsigned long	splashObjIndex{};		// Splash screen object index
unsigned long	bgObjIndex{};			// Background object index
unsigned long	buttonObjIndex{};		// Button object index

 //Binary map data
int**	MapData;				// Level map data
int**	BinaryCollisionArray;	// Level collision data
int		BINARY_MAP_WIDTH;		// Data width size
int		BINARY_MAP_HEIGHT;		// Data height size
AEMtx33	MapTransform;			// Transform matrix containing shift of grid to world coordinates
int		g_chosenLevel{ 0 };	// Chosen level number

/******************************************************************************/
/*!
	Textures
*/
/******************************************************************************/
AEGfxTexture* stoneTexture	{ nullptr };	// Stone texture
AEGfxTexture* dirtTexture	{ nullptr };	// Dirt texture

// ----- Textures for Menu ------
AEGfxTexture* backgroundTexture{};				// Background texture	
AEGfxTexture* buttonTexture_QUIT{};				// Quit game button texture
AEGfxTexture* buttonTexture_OPTIONS{};			// Options button texture
AEGfxTexture* buttonTexture_RETURN{};			// Return button texture
AEGfxTexture* buttonTexture_TOGGLE_FS{};		// Toggle fullscreen button texture
AEGfxTexture* buttonTexture_TOGGLE_SOUND{};		// Toggle sound button texture
AEGfxTexture* buttonTexture_YES{};				// Yes button texture
AEGfxTexture* buttonTexture_NO{};				// No button texture
AEGfxTexture* buttonTexture_RESUME_GAME;		// Resume game button texture
AEGfxTexture* buttonTexture_RESTART_GAME;		// Restart game button texture
AEGfxTexture* buttonTexture_RETURN_MAIN_MENU;	// Return to main menu game button texture

// Pointer to specific game object instances
GameObjInst* ButtonInstance_QUIT{};				// Quit button object instance
GameObjInst* ButtonInstance_OPTIONS{};			// Options button object instance
GameObjInst* ButtonInstance_TOGGLE_FS{};		// Toggle fullscreen button object instance
GameObjInst* ButtonInstance_TOGGLE_SOUND{};		// Toggle sound button object instance
GameObjInst* ButtonInstance_RETURN{};			// Return button object instance
GameObjInst* ButtonInstance_YES{};				// Yes button object instance
GameObjInst* ButtonInstance_NO{};				// No button object instance
GameObjInst* ButtonInstance_RESUME_GAME;		// Resume game button object instance
GameObjInst* ButtonInstance_RESTART_GAME;		// Restart game button object instance
GameObjInst* ButtonInstance_RETURN_MAIN_MENU;	// Return to main menu game button object instance

/******************************************************************************/
/*!
	Helper Functions
*/
/******************************************************************************/
// Function to create a game object instance
 GameObjInst* gameObjInstCreate(GameObj* objType,	AEVec2* scale,
								AEVec2* pPos,		AEVec2* pVel,
								float dir,			enum STATE startState)
{
	// Create a zero vector
	AEVec2 zero;
	AEVec2Zero(&zero);

	// Loop through the object instance list to find a non-used object instance
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// Check if current instance is not used
		if (pInst->flag == 0)
		{
			// It is not used => use it to create the new instance (initialize its starting values)
			pInst->pObject = objType;
			pInst->flag = FLAG_ACTIVE | FLAG_VISIBLE;
			pInst->scale = *scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;

			pInst->gridCollisionFlag = 0;

			pInst->state = startState;
			pInst->innerState = INNER_STATE::INNER_STATE_ON_ENTER;

			pInst->bounceCount = 0;

			pInst->counter = 0.0;
			pInst->shoot_timer = -1.0f;
			pInst->shoot_timer2 = -1.0f;

			// Return the newly created instance
			return pInst;
		}
	}

	return 0;
}

 // Function to destroy a game object instance
 void gameObjInstDestroy(GameObjInst* pInst)
{
	// If instance is destroyed before, just return
	if (pInst->flag == 0)
		return;

	// Zero out the flag
	pInst->flag = 0;
}

// Enemy state machine
void EnemyStateMachine(GameObjInst* pInst)
{
	int collisionOffsetCheck{};
	// Switch logic based on the state of the enemy
	switch (pInst->state) {
		// Patrolling towards left side
		case STATE_GOING_LEFT: {
			// Switch logic based on the innerState of the enemy
			switch (pInst->innerState) {
				// On state entry
				case INNER_STATE_ON_ENTER: {
					// Set velocity to the negative of its defined velocity
					pInst->velCurr.x = -MOVE_VELOCITY_ENEMY;
					// Update innerState
					pInst->innerState = INNER_STATE_ON_UPDATE;
					break;
				}
				// State update loop
				case INNER_STATE_ON_UPDATE: {
					// Find out collision flag in offset location 
					collisionOffsetCheck = CheckInstanceBinaryMapCollision(pInst->posCurr.x - 2.0f, pInst->posCurr.y - 1.0f, 2.0f, 1.f, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
					// Check if enemy is colliding with a wall or his offset indicates theres no platform below him
					if ( (pInst->gridCollisionFlag & COLLISION_LEFT) == COLLISION_LEFT || (collisionOffsetCheck & COLLISION_RIGHT) != COLLISION_RIGHT ){
						// Case is true: idle at current location and change innerState
						pInst->velCurr.x = 0;
						pInst->counter = ENEMY_IDLE_TIME;
						pInst->innerState = INNER_STATE_ON_EXIT;
					}

					// Check if player was detected within circle range
					if (AEVec2SquareDistance(&(PlayerBody->posCurr), &(pInst->posCurr)) <= ENEMY_DETECTION_RANGE) {
						// Variable declaration and initialization
						AEVec2 offset{}, dist{ PlayerBody->posCurr.x - pInst->posCurr.x, PlayerBody->posCurr.y - pInst->posCurr.y };
						// Normalize dist vector
						AEVec2Normalize(&dist, &dist);
						// Loop and check if LoS will collide with environment
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
				// Exiting state
				case INNER_STATE_ON_EXIT: {
					// Decrement counter
					pInst->counter -= g_dt;
					// Once counter hits below 0
					if (pInst->counter < 0.0) {
						// Change main state and innerState to patrol to the right
						pInst->state = STATE_GOING_RIGHT;
						pInst->innerState = INNER_STATE_ON_ENTER;
					}
					// Check if player was detected within circle range
					if (AEVec2SquareDistance(&(PlayerBody->posCurr), &(pInst->posCurr)) <= ENEMY_DETECTION_RANGE) {
						AEVec2 offset{}, dist{ PlayerBody->posCurr.x - pInst->posCurr.x, PlayerBody->posCurr.y - pInst->posCurr.y };
						// Normalize dist vector
						AEVec2Normalize(&dist, &dist);
						// Loop and check if LoS will collide with environment
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
			}
			break;
		}
		// Patrolling towards right side
		case STATE_GOING_RIGHT: {
			// Switch logic based on the innerState of the enemy
			switch (pInst->innerState) {
				// On state entry
				case INNER_STATE_ON_ENTER: {
					// Set velocity its defined velocity
					pInst->velCurr.x = MOVE_VELOCITY_ENEMY;
					// Update innerState
					pInst->innerState = INNER_STATE_ON_UPDATE;
					break;
				}
				// State update loop
				case INNER_STATE_ON_UPDATE: {
					// Find out collision flag in offset location 
					collisionOffsetCheck = CheckInstanceBinaryMapCollision(pInst->posCurr.x + 2.0f, pInst->posCurr.y - 1.0f, 2.0f, 1.0f, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
					// Check if enemy is colliding with a wall or his offset indicates theres no platform below him
					if ( (pInst->gridCollisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT || (collisionOffsetCheck & COLLISION_LEFT) != COLLISION_LEFT ){
						// Case is true: idle at current location and change innerState
						pInst->velCurr.x = 0;
						pInst->counter = ENEMY_IDLE_TIME;
						pInst->innerState = INNER_STATE_ON_EXIT;
					}

					// Check if player was detected within circle range
					if (AEVec2SquareDistance(&(PlayerBody->posCurr), &(pInst->posCurr)) <= ENEMY_DETECTION_RANGE) {
						// Variable declaration and initialization
						AEVec2 offset{}, dist{ PlayerBody->posCurr.x - pInst->posCurr.x, PlayerBody->posCurr.y - pInst->posCurr.y };
						// Normalize dist vector
						AEVec2Normalize(&dist, &dist);
						// Loop and check if LoS will collide with environment
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
				// Exiting state
				case INNER_STATE_ON_EXIT: {
					// Decrement counter
					pInst->counter -= g_dt;
					// Once counter hits below 0
					if (pInst->counter < 0.0){
						// Change main state and innerState to patrol to the left
						pInst->state = STATE_GOING_LEFT;
						pInst->innerState = INNER_STATE_ON_ENTER;
					}
					// Check if player was detected within circle range
					if (AEVec2SquareDistance(&(PlayerBody->posCurr), &(pInst->posCurr)) <= ENEMY_DETECTION_RANGE) {
						// Normalize dist vector
						AEVec2 offset{}, dist{ PlayerBody->posCurr.x - pInst->posCurr.x, PlayerBody->posCurr.y - pInst->posCurr.y };
						// Normalize dist vector
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
			}
			break;
		}
		// Player was detected in range
		case STATE_ALERT: {
			// Switch logic based on the innerState of the enemy
			switch (pInst->innerState) {
				// On state entry
				case INNER_STATE_ON_ENTER: {
					// Stop moving
					pInst->velCurr.x = 0;
					// Update innerState
					pInst->innerState = INNER_STATE_ON_UPDATE;
					break;
				}
				// State update loop
				case INNER_STATE_ON_UPDATE: {
					// Check if player is still within range
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
					// Check if player is outside of range
					if (AEVec2SquareDistance(&(PlayerBody->posCurr), &(pInst->posCurr)) > ENEMY_DETECTION_RANGE) {
						pInst->innerState = INNER_STATE_ON_EXIT;
						break;
					}

					// Decrement second shoot timer
					pInst->shoot_timer2 -= g_dt;
					// Check if the timer is lower than 0.39f
					if (pInst->shoot_timer2 > 0.39f) {
						// Decrement first shoot timer
						pInst->shoot_timer -= g_dt;
						// Check if first shoot timer is less than 0
						if (pInst->shoot_timer < 0.0f) {
							// Create the projectile
							AEVec2 dist{ PlayerBody->posCurr.x - pInst->posCurr.x, PlayerBody->posCurr.y - pInst->posCurr.y };
							AEVec2Normalize(&dist, &dist);
							AEVec2 shootpos{ pInst->posCurr.x + dist.x * 1.5f, pInst->posCurr.y + dist.y * 1.5f };
							AEVec2 bulletvelocity{ dist.x * 7 , dist.y * 7 };
							gameObjInstCreate(&sGameObjList[bulletObjIndex], &BULLET_SCALE, &shootpos, &bulletvelocity, pInst->dirCurr, STATE::STATE_GOING_LEFT);
							pInst->shoot_timer = 0.2f;
						}
					}

					// Check if second shoot timer is less than 0
					if (pInst->shoot_timer2 < 0.0f)
						pInst->shoot_timer2 = 1.0f;	// Set it back to 1.0f
					break;

				}
				// Exiting state
				case INNER_STATE_ON_EXIT: {
					// Update main state & innerState
					pInst->state = STATE_GOING_RIGHT;
					pInst->innerState = INNER_STATE_ON_ENTER;
					break;
				}
			}
			break;
		}
	}
}

/******************************************************************************/
/*!
	Create Game object instance for Pause Menu
*/
/******************************************************************************/
// Function to create a game object instance
GameObjInst* PauseMenuInstCreate(GameObj* objType, AEVec2* scale,
	AEVec2* pPos, AEVec2* pVel,
	float dir, enum STATE startState)
{
	// Create a zero vector
	AEVec2 zero;
	AEVec2Zero(&zero);

	// Loop through the object instance list to find a non-used object instance
	for (unsigned int i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sPauseMenuInstList + i;

		// Check if current instance is not used
		if (pInst->flag == 0)
		{
			// It is not used => use it to create the new instance (initialize its starting values)
			pInst->pObject = objType;
			pInst->flag = FLAG_ACTIVE | FLAG_VISIBLE;
			pInst->scale = *scale;
			pInst->posCurr = pPos ? *pPos : zero;
			pInst->velCurr = pVel ? *pVel : zero;
			pInst->dirCurr = dir;

			pInst->gridCollisionFlag = 0;

			pInst->state = startState;
			pInst->innerState = INNER_STATE::INNER_STATE_ON_ENTER;

			// Return the newly created instance
			return pInst;
		}
	}

	return 0;
}

/******************************************************************************/
/*!
	Create Pause Menu
*/
/******************************************************************************/
void PauseMenuInIt()
{
	// Create an instance of the background 
	AEVec2 scaling{ 1.0f, 1.0f }, pos{ 0.0f, 0.0f };
	AEGfxGetCamPosition(&pos.x, &pos.y);
	GameObjInst* bg{ PauseMenuInstCreate(&sGameObjList[bgObjIndex], &scaling, &pos,0, 0.0f, STATE_NONE) };
	bg->sub_type = BUTTON_TYPE::BG;

	// Resume Game button
	scaling = { 1.0f, 1.0f };
	AEGfxGetCamPosition(&pos.x, &pos.y);
	pos = { pos.x + 0.0f, pos.y - 0.0f };
	ButtonInstance_RESUME_GAME = PauseMenuInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_RESUME_GAME->sub_type = BUTTON_TYPE::RESUME_GAME;

	// Restart Game button
	scaling = { 1.0f, 1.0f };
	AEGfxGetCamPosition(&pos.x, &pos.y);
	pos = { pos.x + 0.0f, pos.y - 100.0f };
	ButtonInstance_RESTART_GAME = PauseMenuInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_RESTART_GAME->sub_type = BUTTON_TYPE::RESTART_GAME;

	// Options button
	scaling = { 1.0f, 1.0f };
	AEGfxGetCamPosition(&pos.x, &pos.y);
	pos = { pos.x + 0.0f, pos.y - 200.0f };
	ButtonInstance_OPTIONS = PauseMenuInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_OPTIONS->sub_type = BUTTON_TYPE::OPTIONS;

	// Return to Main Menu game button
	scaling = { 1.0f, 1.0f };
	AEGfxGetCamPosition(&pos.x, &pos.y);
	pos = { pos.x + 0.0f, pos.y - 300.0f };
	ButtonInstance_RETURN_MAIN_MENU = PauseMenuInstCreate(&sGameObjList[buttonObjIndex], &BUTTON_SCALE, &pos, 0, 0.0f, STATE_NONE);
	ButtonInstance_RETURN_MAIN_MENU->sub_type = BUTTON_TYPE::EXIT_GAME;
}

void PauseMenuDestroy() {
	// kill all object instances in the array using "gameObjInstDestroy"
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sPauseMenuInstList + i;

		gameObjInstDestroy(pInst);
	}
}