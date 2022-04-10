/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       GameState_Levels.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		
-------------------------------------------------------------------------------
@author		Mohamed Zafir (m.zafir@digipen.edu)
@role		Physics, particle system,
			texture mapping, game logic
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu)
@role		
*//*_________________________________________________________________________*/

// ----- Include Files -----
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


/******************************************************************************/
/*!
	(Static) Variables
*/
/******************************************************************************/
static std::string levelFileName{ "" };


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
void GameStateLevelsLoad(void)
{
	// zero the game object array
	memset(sGameObjList, 0, sizeof(GameObj) * GAME_OBJ_NUM_MAX);
	// No game objects (shapes) at this point
	sGameObjNum = 0;

	// zero the game object instance array
	memset(sGameObjInstList, 0, sizeof(GameObjInst) * GAME_OBJ_INST_NUM_MAX);
	// No game object instances (sprites) at this point
	sGameObjInstNum = 0;

	// ----- Set objects to nullptr first -----
	{
		PlayerBody = nullptr;
		PlayerGun = nullptr;
		EmptyInstance = nullptr;
		PlatformInstance = nullptr;
		DirtInstance = nullptr;
	}

	// =========================
	// Load/create the mesh data 
	// Game objects / Shapes
	// =========================	L
	{
		GameObj* pObj;
		// ----- Create the non collidable shape -----
		{
			emptyObjIndex = sGameObjNum;
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_EMPTY;
			AEGfxMeshStart();
			AEGfxTriAdd(
				-EMPTY_MESHSIZE.x / 2, -EMPTY_MESHSIZE.y / 2, 0xFF000000, 0.0f, 0.0f,
				EMPTY_MESHSIZE.x / 2, -EMPTY_MESHSIZE.y / 2, 0xFF000000, 0.0f, 0.0f,
				-EMPTY_MESHSIZE.x / 2, EMPTY_MESHSIZE.y / 2, 0xFF000000, 0.0f, 0.0f);

			AEGfxTriAdd(
				-EMPTY_MESHSIZE.x / 2, EMPTY_MESHSIZE.y / 2, 0xFF000000, 0.0f, 0.0f,
				EMPTY_MESHSIZE.x / 2, -EMPTY_MESHSIZE.y / 2, 0xFF000000, 0.0f, 0.0f,
				EMPTY_MESHSIZE.x / 2, EMPTY_MESHSIZE.y / 2, 0xFF000000, 0.0f, 0.0f);
			pObj->pMesh = AEGfxMeshEnd();
			pObj->meshSize = AEVec2{ EMPTY_MESHSIZE.x, EMPTY_MESHSIZE.y };
			AE_ASSERT_MESG(pObj->pMesh, "fail to create non-collidable object!!");
		}
		// ----- Create the platform shape -----
		{
			platformObjIndex = sGameObjNum;
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_PLATFORM;
			AEGfxMeshStart();
			AEGfxTriAdd(
				-PLATFORM_MESHSIZE.x / 2, -PLATFORM_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 1.0f,
				PLATFORM_MESHSIZE.x / 2, -PLATFORM_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 1.0f,
				-PLATFORM_MESHSIZE.x / 2, PLATFORM_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f);

			AEGfxTriAdd(
				-PLATFORM_MESHSIZE.x / 2, PLATFORM_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 1.0f,
				PLATFORM_MESHSIZE.x / 2, -PLATFORM_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 0.0f,
				PLATFORM_MESHSIZE.x / 2, PLATFORM_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f);
			pObj->pMesh = AEGfxMeshEnd();
			pObj->meshSize = AEVec2{ PLATFORM_MESHSIZE.x, PLATFORM_MESHSIZE.y };
			AE_ASSERT_MESG(pObj->pMesh, "fail to create platform object!!");
		}
		// ----- Create the player shape -----
		{
			playerObjIndex = sGameObjNum;
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_PLAYER;

			AEGfxMeshStart();
			AEGfxTriAdd(
				-PLAYER_MESHSIZE.x / 2, -PLAYER_MESHSIZE.y / 2, 0xFF32a852, 0.0f, 0.0f,
				PLAYER_MESHSIZE.x / 2, -PLAYER_MESHSIZE.y / 2, 0xFF32a852, 0.0f, 0.0f,
				-PLAYER_MESHSIZE.x / 2, PLAYER_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f);

			AEGfxTriAdd(
				PLAYER_MESHSIZE.x / 2, -PLAYER_MESHSIZE.y / 2, 0xFF32a852, 0.0f, 0.0f,
				PLAYER_MESHSIZE.x / 2, PLAYER_MESHSIZE.y / 2, 0xFF32a852, 0.0f, 0.0f,
				-PLAYER_MESHSIZE.x / 2, PLAYER_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f);
			pObj->pMesh = AEGfxMeshEnd();
			pObj->meshSize = AEVec2{ PLAYER_MESHSIZE.x, PLAYER_MESHSIZE.y };
			AE_ASSERT_MESG(pObj->pMesh, "fail to create player object!!");
		}
		// ----- Create the player gun shape -----
		{
			playerGunObjIndex = sGameObjNum;
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_PLAYERGUN;
			AEGfxMeshStart();
			AEGfxTriAdd(
				0.0f, GUN_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f,
				0.0f, -GUN_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 1.0f,
				GUN_MESHSIZE.x, -GUN_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 1.0f);
			AEGfxTriAdd(
				0.0f, GUN_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f,
				GUN_MESHSIZE.x, GUN_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 0.0f,
				GUN_MESHSIZE.x, -GUN_MESHSIZE.y / 2, 0xFF4D5853, 1.0f, 1.0f);
			pObj->pMesh = AEGfxMeshEnd();
			pObj->meshSize = AEVec2{ GUN_MESHSIZE.x, GUN_MESHSIZE.y };
			AE_ASSERT_MESG(pObj->pMesh, "fail to create player gun object!!");
		}
		// ----- Create the bullet shape -----
		{
			bulletObjIndex = sGameObjNum;
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_BULLET;
			AEGfxMeshStart();
			AEGfxTriAdd(
				-BULLET_MESHSIZE.x / 2, BULLET_MESHSIZE.y / 2, 0xFFFFFF00, 0.0f, 0.0f,
				-BULLET_MESHSIZE.x / 2, -BULLET_MESHSIZE.y / 2, 0xFFFFFF00, 0.0f, 1.0f,
				BULLET_MESHSIZE.x / 2, -BULLET_MESHSIZE.y / 2, 0xFFFFFF00, 1.0f, 1.0f);
			AEGfxTriAdd(
				-BULLET_MESHSIZE.x / 2, BULLET_MESHSIZE.y / 2, 0xFFFFFF00, 0.0f, 0.0f,
				BULLET_MESHSIZE.x / 2, BULLET_MESHSIZE.y / 2, 0xFFFFFF00, 1.0f, 0.0f,
				BULLET_MESHSIZE.x / 2, -BULLET_MESHSIZE.y / 2, 0xFFFFFF00, 1.0f, 1.0f);
			pObj->pMesh = AEGfxMeshEnd();
			pObj->meshSize = AEVec2{ BULLET_MESHSIZE.x, BULLET_MESHSIZE.y };
			AE_ASSERT_MESG(pObj->pMesh, "fail to create bullet object!!");
		}
		// ----- Create the enemy 1 shape -----
		{
			enemy1ObjIndex = sGameObjNum;
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_ENEMY1;
			AEGfxMeshStart();
			AEGfxTriAdd(
				-PLAYER_MESHSIZE.x / 2, -PLAYER_MESHSIZE.y / 2, 0xFFFF0000, 0.0f, 0.0f,
				PLAYER_MESHSIZE.x / 2, -PLAYER_MESHSIZE.y / 2, 0xFFFF0000, 0.0f, 0.0f,
				-PLAYER_MESHSIZE.x / 2, PLAYER_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f);
			AEGfxTriAdd(
				PLAYER_MESHSIZE.x / 2, -PLAYER_MESHSIZE.y / 2, 0xFFFF0000, 0.0f, 0.0f,
				PLAYER_MESHSIZE.x / 2, PLAYER_MESHSIZE.y / 2, 0xFFFF0000, 0.0f, 0.0f,
				-PLAYER_MESHSIZE.x / 2, PLAYER_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f);
			pObj->pMesh = AEGfxMeshEnd();
			pObj->meshSize = AEVec2{ PLAYER_MESHSIZE.x, PLAYER_MESHSIZE.y };
			AE_ASSERT_MESG(pObj->pMesh, "fail to create ENEMY1 object!!");
		}
		// ----- Create the enemy 2 shape -----
		{
			enemy2ObjIndex = sGameObjNum;
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_ENEMY2;
			AEGfxMeshStart();
			AEGfxTriAdd(
				-PLAYER_MESHSIZE.x / 2, -PLAYER_MESHSIZE.y / 2, 0xFF3265a8, 0.0f, 0.0f,
				PLAYER_MESHSIZE.x / 2, -PLAYER_MESHSIZE.y / 2, 0xFF3265a8, 0.0f, 0.0f,
				-PLAYER_MESHSIZE.x / 2, PLAYER_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f);
			AEGfxTriAdd(
				PLAYER_MESHSIZE.x / 2, -PLAYER_MESHSIZE.y / 2, 0xFF3265a8, 0.0f, 0.0f,
				PLAYER_MESHSIZE.x / 2, PLAYER_MESHSIZE.y / 2, 0xFF3265a8, 0.0f, 0.0f,
				-PLAYER_MESHSIZE.x / 2, PLAYER_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f);
			pObj->pMesh = AEGfxMeshEnd();
			pObj->meshSize = AEVec2{ PLAYER_MESHSIZE.x, PLAYER_MESHSIZE.y };
			AE_ASSERT_MESG(pObj->pMesh, "fail to create ENEMY2 object!!");
		}
		// ----- Create the particle1 shape -----
		{
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
		}
		// ----- Create the dotted shape -----
		{
			dottedObjIndex = sGameObjNum;
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_DOTTED;
			AEGfxMeshStart();
			AEGfxTriAdd(
				-BULLET_MESHSIZE.x / 2, BULLET_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f,
				-BULLET_MESHSIZE.x / 2, -BULLET_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 1.0f,
				BULLET_MESHSIZE.x / 2, -BULLET_MESHSIZE.y / 2, 0xFFFFFFFF, 1.0f, 1.0f);
			AEGfxTriAdd(
				-BULLET_MESHSIZE.x / 2, BULLET_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f,
				BULLET_MESHSIZE.x / 2, BULLET_MESHSIZE.y / 2, 0xFFFFFFFF, 1.0f, 0.0f,
				BULLET_MESHSIZE.x / 2, -BULLET_MESHSIZE.y / 2, 0xFFFFFFFF, 1.0f, 1.0f);
			pObj->pMesh = AEGfxMeshEnd();
			pObj->meshSize = AEVec2{ BULLET_MESHSIZE.x, BULLET_MESHSIZE.y };
			AE_ASSERT_MESG(pObj->pMesh, "fail to create TYPE_DOTTED object!!");

			//Load textures
			stoneTexture = AEGfxTextureLoad(".\\Resources\\Assets\\stone.png"); // Load stone texture
			AE_ASSERT_MESG(stoneTexture, "Failed to create texture1!!");
			dirtTexture = AEGfxTextureLoad(".\\Resources\\Assets\\dirt.png"); // Load dirt texture
			AE_ASSERT_MESG(dirtTexture, "Failed to create texture1!!");
		}
		// ----- Create the dirt block shape -----
		{
			dirtObjIndex = sGameObjNum;
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_DIRT;
			AEGfxMeshStart();
			AEGfxTriAdd(
				-PLATFORM_MESHSIZE.x / 2, -PLATFORM_MESHSIZE.y / 2, 0x00FF5853, 0.0f, 1.0f,
				PLATFORM_MESHSIZE.x / 2, -PLATFORM_MESHSIZE.y / 2, 0x00FF5853, 1.0f, 1.0f,
				-PLATFORM_MESHSIZE.x / 2, PLATFORM_MESHSIZE.y / 2, 0x00FF5853, 0.0f, 0.0f);
			AEGfxTriAdd(
				-PLATFORM_MESHSIZE.x / 2, PLATFORM_MESHSIZE.y / 2, 0x00FF5853, 1.0f, 1.0f,
				PLATFORM_MESHSIZE.x / 2, -PLATFORM_MESHSIZE.y / 2, 0x00FF5853, 1.0f, 0.0f,
				PLATFORM_MESHSIZE.x / 2, PLATFORM_MESHSIZE.y / 2, 0x00FF5853, 0.0f, 0.0f);
			pObj->pMesh = AEGfxMeshEnd();
			pObj->meshSize = AEVec2{ PLATFORM_MESHSIZE.x, PLATFORM_MESHSIZE.y };
			AE_ASSERT_MESG(pObj->pMesh, "fail to create DIRT object!!");
		}
		// ----- Create the health bar shape -----
		{
			hpObjIndex = sGameObjNum;
			PlayerHealthBar = pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_HEALTHBAR;
			AEGfxTriAdd(
				0.0f, HEALTHBAR_MESHSIZE.y / 2, 0xFFc24040, 0.0f, 0.0f,
				0.0f, -HEALTHBAR_MESHSIZE.y / 2, 0xFFc24040, 0.0f, 1.0f,
				HEALTHBAR_MESHSIZE.x, -HEALTHBAR_MESHSIZE.y / 2, 0xFFc24040, 1.0f, 1.0f);
			AEGfxTriAdd(
				0.0f, HEALTHBAR_MESHSIZE.y / 2, 0xFFc24040, 0.0f, 0.0f,
				HEALTHBAR_MESHSIZE.x, HEALTHBAR_MESHSIZE.y / 2, 0xFFc24040, 1.0f, 0.0f,
				HEALTHBAR_MESHSIZE.x, -HEALTHBAR_MESHSIZE.y / 2, 0xFFc24040, 1.0f, 1.0f);
			pObj->pMesh = AEGfxMeshEnd();
			pObj->meshSize = AEVec2{ HEALTHBAR_MESHSIZE.x, HEALTHBAR_MESHSIZE.y };
			AE_ASSERT_MESG(pObj->pMesh, "fail to create healthbar object!!");
		}
	}

	// =====================
	// Load Level Binary Map
	// =====================
	//	Import Level data from txt file depending on chosen level
	switch (g_chosenLevel) {
	case 2:
		levelFileName = ".\\Resources\\Level Data\\Level2.txt";
		break;
	default:
		break;
	}
	if (ImportMapDataFromFile(levelFileName, &MapData, &BinaryCollisionArray, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 0)
		gGameStateNext = GS_QUIT;


	// ----- Computing MapTransform Matrix -----
	AEMtx33 scale, trans;
	AEMtx33Trans(&trans, static_cast<f32>(-(BINARY_MAP_WIDTH / 2)), static_cast<f32>(-(BINARY_MAP_HEIGHT / 2)));
	AEMtx33Scale(&scale, static_cast<f32>(AEGetWindowWidth() / 40), static_cast<f32>(AEGetWindowHeight() / 20));
	AEMtx33Concat(&MapTransform, &scale, &trans);
}

/******************************************************************************/
/*!
	"Initialize" function of this state
*/
/******************************************************************************/
void GameStateLevelsInit(void)
{
	// ----- Set gameplay variables to initial value -----
	levelTime = 0.0;						// Time in level
	totalEnemyCount = 0;						// Total enemies in level
	playerDeathTimer = 0.0f;					// Timer for player defeat
	playerHealth = PLAYER_INITIAL_HEALTH;	// Player health

	// ----- Create platform instance -----
	PlatformInstance = gameObjInstCreate(&sGameObjList[platformObjIndex], &PLATFORM_SCALE, 0, 0, 0.0f, STATE_NONE);
	PlatformInstance->flag ^= FLAG_VISIBLE;
	PlatformInstance->flag |= FLAG_NON_COLLIDABLE;

	// ----- Create distructable wall instance -----
	DirtInstance = gameObjInstCreate(&sGameObjList[dirtObjIndex], &PLATFORM_SCALE, 0, 0, 0.0f, STATE_NONE);
	DirtInstance->flag ^= FLAG_VISIBLE;
	DirtInstance->flag |= FLAG_NON_COLLIDABLE;

	// Seed random generator
	srand(static_cast<unsigned int>(time(NULL))); // Seed random generator

	// Variable declaration
	AEVec2 Pos{}, Scale{};
	
	// Loop through MapData and create game objects
	for (int row = 0; row < BINARY_MAP_HEIGHT; row++) {
		for (int col = 0; col < BINARY_MAP_WIDTH; col++) {
			// Set position based on current index
			Pos.x = col + 0.5f;
			Pos.y = row + 0.5f;

			// Switch logic creation based on the value stored in MapData at current index
			switch (GetCellValue(col, row, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT)) {
				// Player 
				case TYPE_PLAYER: {
					// Create player body & gun
					PlayerBody = gameObjInstCreate(&sGameObjList[playerObjIndex], &PLAYER_SCALE, &Pos, nullptr, 0.0f, STATE_NONE);
					PlayerGun = gameObjInstCreate(&sGameObjList[playerGunObjIndex], &GUN_SCALE, &Pos, nullptr, 0.0f, STATE_NONE);
					break;
				}
				// 1st enemy variant
				case TYPE_ENEMY1: {
					// Create enemy
					gameObjInstCreate(&sGameObjList[enemy1ObjIndex], &PLAYER_SCALE, &Pos, nullptr, 0.0f, STATE_GOING_LEFT);
					// Increment level enemy count
					++totalEnemyCount;
					break;
				}
				// 2nd enemy variant
				case TYPE_ENEMY2: {
					// Create enemy
					gameObjInstCreate(&sGameObjList[enemy2ObjIndex], &PLAYER_SCALE, &Pos, nullptr, 0.0f, STATE_GOING_LEFT);
					// Increment level enemy count
					++totalEnemyCount;
					break;
				}
				// None of the above
				default: {
					// Do Nothing
					break;
				}
			}
		}
	}

	// Set ammo capacity based on the number of enemies in the level
	ammoCount = static_cast<int>(totalEnemyCount * 5.0f);
}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateLevelsUpdate(void)
{
	// Switch logic based on the InnerState of the current game stae
	switch (gGameStateInnerState) {
		// Pause State
		case GAME_PAUSE: {
			// Pause sound channel
			soundChannel->setPaused(true);

			// Check if game is to be unpaused
			if (AEInputCheckReleased(AEVK_ESCAPE) && winFocused)
				gGameStateInnerState = GAME_PLAY;	// Update innerState to play state
			else if (AEInputCheckReleased(AEVK_R) && winFocused) {	// Check if game is to be restarted
				gGameStateNext = GS_RESTART;		// Update nextState to restart
				gGameStateInnerState = GAME_PLAY;	// Update innerState to play state
				// Reload level data
				FreeMapData(&MapData, &BinaryCollisionArray, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
				if (ImportMapDataFromFile(levelFileName, &MapData, &BinaryCollisionArray, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 0)
					gGameStateNext = GS_QUIT;
			}
			else if (AEInputCheckReleased(AEVK_M) && winFocused) {	// Check if game is to be returned to main menu
				gGameStateNext = GS_MAINMENU;		// Update next state to main menu
				gGameStateInnerState = GAME_PLAY;	// Update innerState to play state
			}
			break;
		}
		// Win State
		case GAME_WIN: {
			// Update next state to win screen state
			gGameStateNext = GS_WINSCREEN;
			// Update innerState to play state
			gGameStateInnerState = GAME_PLAY;
			// Increment chosen level
			++g_chosenLevel;
			break;
		}
		// Lose State
		case GAME_LOSE: {
			// Check if it is the first time we are entering this innerState
			if (playerDeathTimer == PLAYER_DEATH_ANIME_TIME) {
				// Simulate/Create particles upon player death
				AEVec2 particleVel;
				for (double x = PlayerBody->posCurr.x - 1.5; x < PlayerBody->posCurr.x + 1.5; x += ((1.f + rand() % 50) / 100.f))
				{
					AEVec2 particlespawn = { static_cast<float>(x), PlayerBody->posCurr.y };
					if (rand() % 2) // randomize polarity of particleVel.x
					{
						particleVel = { rand() % 20 / -10.0f, rand() % 20 / 10.f };
						gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_NONE);
					}
					else
					{
						particleVel = { rand() % 20 / 10.f, rand() % 20 / 10.f };
						gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_ALERT);
					}
				}

			// Destroy player instance
			gameObjInstDestroy(PlayerGun);
			gameObjInstDestroy(PlayerBody);
		}

		// ----- Post player death control ------
		// Decrement timer
		playerDeathTimer -= g_dt;
		// Check if timer is less than 0
		if (playerDeathTimer < 0)	// Restart level
		{
			// Update innerState to play state
			gGameStateInnerState = GAME_PLAY;
			// Restart the level
			gGameStateNext = GS_RESTART;
			// Reload level data
			FreeMapData(&MapData, &BinaryCollisionArray, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
			if (ImportMapDataFromFile(levelFileName, &MapData, &BinaryCollisionArray, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 0)
				gGameStateNext = GS_QUIT;
			break;
		}

		// Variable declaration
		int i{};
		GameObjInst* pInst;

		// Update object instances positions
		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
		{
			pInst = sGameObjInstList + i;

			// skip non-active object
			if (0 == (pInst->flag & FLAG_ACTIVE))
				continue;

			// ----- Update Position -----
			pInst->posCurr.x += pInst->velCurr.x * g_dt;
			pInst->posCurr.y += pInst->velCurr.y * g_dt;

			// ----- Update Bounding Box -----
			pInst->boundingBox.min.x = -(pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
			pInst->boundingBox.min.y = -(pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;

			pInst->boundingBox.max.x = (pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
			pInst->boundingBox.max.y = (pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;
		}
		break;
	}
				  // Play State		  
	case GAME_PLAY: {
		// Check if window was unfocused
		if (winFocused == false)
			gGameStateInnerState = GAME_PAUSE;	// Set innerState to pause state

		// Check if sound is paused; If so, unpause it
		bool result;
		soundChannel->getPaused(&result);
		if (result)
			soundChannel->setPaused(false);

		// Update level time
		levelTime += g_dt;

		// Check win condition
		if (totalEnemyCount <= 0 && ammoCount > 1)
			gGameStateInnerState = GAME_WIN;	// Update innerState to win state

		// Check lose condition
		if (playerHealth <= 0.0f || ammoCount <= 0) {
			playerDeathTimer = PLAYER_DEATH_ANIME_TIME;	// Set timer to defined const
			gGameStateInnerState = GAME_LOSE;			// Update innerState to lose state
		}

		// =========================
		// Ipdate according to input
		// =========================
		{
			// Check if escape key is pressed
			if (AEInputCheckReleased(AEVK_ESCAPE))
				gGameStateInnerState = GAME_PAUSE;	// Update innerState to pause state

			// Check if down arrow key was pressed (Cheatcode to go next level)
			if (AEInputCheckReleased(AEVK_DOWN))
				totalEnemyCount = 0;	// Set enemy count in level to 0

			// ----------------------------------------------------------------------------------------------------------------------------------------------
			// Change the following input movement based on our player movement
			// ----------------------------------------------------------------------------------------------------------------------------------------------
			{
				// Check if W key was pressed and there is jumpfuel
				if (AEInputCheckCurr(AEVK_W) && jumpFuel > 0) // Hold to thrust upward
				{
					// Variable declaration
					AEVec2 added;

					// Add velocity to player
					AEVec2Set(&added, 0.f, 1.f);
					added.x *= 1;
					added.y *= 20 * g_dt;
					AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);

					// Limit velocity
					AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.99f);

					// Deplete jumpfuel
					jumpFuel -= g_dt;

					// Simulate/Create particles when thrust is active
					AEVec2 particleVel = { 0, -1.5f };
					for (double i = PlayerBody->posCurr.x - 0.6; i < PlayerBody->posCurr.x + 0.6; i += ((1.f + rand() % 50) / 100.f))
					{
						AEVec2 particlespawn = { static_cast<float>(i), PlayerBody->posCurr.y - 0.5f };
						if (rand() % 2) {

							gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 0.6f, STATE_NONE); // red color
						}
						else {
							gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 0.6f, STATE_ALERT); // orange color
						}
					}
				}

				// Check if A or D key was pressed
				if (AEInputCheckCurr(AEVK_A)) // Move left
				{
					// Variable declaration
					AEVec2 added;
					AEVec2Set(&added, cosf(PlayerBody->dirCurr), sinf(PlayerBody->dirCurr));

					// Find the velocity according to the acceleration
					added.x *= -MOVE_VELOCITY * g_dt;
					added.y *= -MOVE_VELOCITY * g_dt;
					AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);

					// Limit velocity
					AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f);
				}
				else if (AEInputCheckCurr(AEVK_D)) // Move right
				{
					// Variable declaration
					AEVec2 added;
					AEVec2Set(&added, cosf(PlayerBody->dirCurr), sinf(PlayerBody->dirCurr));

					// Find the velocity according to the acceleration
					added.x *= MOVE_VELOCITY * g_dt;
					added.y *= MOVE_VELOCITY * g_dt;
					AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);

					// Limit velocity
					AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f);
				}

				// Limit player movement velocity
				AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f);
			}

			// ------------------------------------------------------------------------------------
			// Change to bullet spawning on mouse click in direction
			// ------------------------------------------------------------------------------------
			// Variable declaration
			AEVec2 BarrelEnd{}, dirBullet{};
			// Shoot a bullet if left mouse button is triggered (Create a new object instance)
			if (AEInputCheckReleased(VK_LBUTTON) && ammoCount > 0) {
				// Variable declaration
				AEVec2 dirBullet;

				// Get the bullet's direction according to the player's direction
				AEVec2Set(&dirBullet, cosf(PlayerGun->dirCurr), sinf(PlayerGun->dirCurr));

				// Set the velocity
				dirBullet.x *= BULLET_SPEED;
				dirBullet.y *= BULLET_SPEED;

				// Calculate end of gun barrel position
				BarrelEnd.x = PlayerGun->posCurr.x + dirBullet.x * 0.11f;
				BarrelEnd.y = PlayerGun->posCurr.y + dirBullet.y * 0.11f;

				// Create projectile instance
				gameObjInstCreate(&sGameObjList[bulletObjIndex], &BULLET_SCALE, &BarrelEnd, &dirBullet, PlayerGun->dirCurr, STATE_NONE);
				// Deplete ammoCount
				--ammoCount;

				// Check soundVolume flag variable
				if (soundVolumeLevel)
					fModSys->playSound(playerShoot, nullptr, false, &soundChannel);	// Play sound on projectile spawned
			}

			// Projectile trajectory prediction (Aim with trajectory line)
			if (AEInputCheckCurr(VK_RBUTTON)) {
				// Get the bullet's direction according to the player's direction
				AEVec2Set(&dirBullet, cosf(PlayerGun->dirCurr), sinf(PlayerGun->dirCurr));

				// Calculate end of gun barrel position
				BarrelEnd.x = PlayerGun->posCurr.x + dirBullet.x * BULLET_SPEED * 0.11f;
				BarrelEnd.y = PlayerGun->posCurr.y + dirBullet.y * BULLET_SPEED * 0.11f;

				// Variable declaration
				AEVec2 currPos{ BarrelEnd };
				int bounceCount{ 0 };

				// Loop through calculated trajectory of the projectile
				for (int i{ 1 }; i < 1000; ++i) {
					// Move current position
					currPos.x += dirBullet.x * g_dt;
					currPos.y += dirBullet.y * g_dt;

					// Call to find for collision with environment
					int collisionFlag = CheckInstanceBinaryMapCollision_Dotted(currPos.x, currPos.y, BULLET_MESHSIZE.x * BULLET_SCALE.x, BULLET_MESHSIZE.y * BULLET_SCALE.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);

					// Check if there is collision with a destructable wall
					if ((collisionFlag & COLLISION_Destructable) == COLLISION_Destructable)
						break;

					// Check which side is the collision coming from
					bool reflectedFlag{ false };
					AEVec2 normal{};
					if ((collisionFlag & COLLISION_TOP) == COLLISION_TOP && reflectedFlag == false) {	// Collision with top of platform
						// Set normal of surface
						normal = { 0, -1 };
						// Increment bounceCount and check if less than or equal to 3
						if (++bounceCount < 3)
							reflectedFlag = true;
						else
							break;
					}
					if ((collisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM && reflectedFlag == false) {	// Collision with bottom of platform
						// Set normal of surface
						normal = { 0, 1 };
						// Increment bounceCount and check if less than or equal to 3
						if (++bounceCount < 3)
							reflectedFlag = true;
						else
							break;
					}
					if ((collisionFlag & COLLISION_LEFT) == COLLISION_LEFT && reflectedFlag == false) {	// Collision with left side of platform
						// Set normal of surface
						normal = { 1, 0 };
						// Increment bounceCount and check if less than or equal to 3
						if (++bounceCount < 3)
							reflectedFlag = true;
						else
							break;
					}
					if ((collisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT && reflectedFlag == false) {	// Collision with right side of platform
						// Set normal of surface
						normal = { -1, 0 };
						// Increment bounceCount and check if less than or equal to 3
						if (++bounceCount < 3)
							reflectedFlag = true;
						else
							break;
					}

					// To reflect trajectory
					if (reflectedFlag) {
						// Calculate new direction after ricochet (newVel = currVel - 2(currVel . normal) * normal)
						AEVec2 newVel{ dirBullet.x - 2 * (AEVec2DotProduct(&dirBullet, &normal)) * normal.x,  dirBullet.y - 2 * (AEVec2DotProduct(&dirBullet, &normal)) * normal.y };
						AEVec2Normalize(&newVel, &newVel);
						// Assign it to the bullet
						dirBullet = newVel;

						// Move by 1 frame
						currPos.x += dirBullet.x * g_dt;
						currPos.y += dirBullet.y * g_dt;
					}

					// Check if current iteration of i is a multiple of 30
					if (i % 30 == 0)
						gameObjInstCreate(&sGameObjList[dottedObjIndex], &BULLET_SCALE, &currPos, 0, 0, STATE_NONE);	// Create a dotted instance of it
				}
			}
		}

		// Variable declaration
		int i{};
		GameObjInst* pInst;

		//Update object instances physics and behavior
		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
		{
			pInst = sGameObjInstList + i;

			// Skip non-active object
			if (0 == (pInst->flag & FLAG_ACTIVE))
				continue;

			// Destroy player shot bullet after 3 bounces
			if (pInst->pObject->type == TYPE_BULLET && pInst->bounceCount >= 3)
				gameObjInstDestroy(pInst);

			// Destroy enemy shot bullet after it collides with game
			if (pInst->pObject->type == TYPE_BULLET && pInst->state == STATE::STATE_GOING_LEFT && pInst->bounceCount >= 1)
				gameObjInstDestroy(pInst);

			// Apply gravity to enemies and player
			if (pInst->pObject->type == TYPE_ENEMY1 || pInst->pObject->type == TYPE_PLAYER)
				pInst->velCurr.y += GRAVITY * g_dt;

			// Apply enemy state machine to enemy AI.
			if (pInst->pObject->type == TYPE_ENEMY1 || pInst->pObject->type == TYPE_ENEMY2) {
				EnemyStateMachine(pInst);
			}
		}

		// Update object instances positions
		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
		{
			pInst = sGameObjInstList + i;

			// Skip non-active object
			if (0 == (pInst->flag & FLAG_ACTIVE))
				continue;

			// ----- Update Position -----
			pInst->posCurr.x += pInst->velCurr.x * g_dt;
			pInst->posCurr.y += pInst->velCurr.y * g_dt;

			// ----- Update Bounding Box -----
			pInst->boundingBox.min.x = -(pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
			pInst->boundingBox.min.y = -(pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;

			pInst->boundingBox.max.x = (pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
			pInst->boundingBox.max.y = (pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;
		}

		// Calculate the direction of the player's barrel based on the mouse position
		{
			AEVec2 playerWorldPos{ PlayerBody->posCurr.x, PlayerBody->posCurr.y };
			// Calculate mouse position in world coordinates
			AEMtx33MultVec(&playerWorldPos, &MapTransform, &playerWorldPos);
			// Find the direction
			float dotProduct = atan2(worldMouseY - playerWorldPos.y, worldMouseX - playerWorldPos.x);
			// Set it to player gun
			PlayerGun->dirCurr = dotProduct;
		}
		break;
	}
	}

	// Variable declaration 
	int i{};
	GameObjInst* pInst;

	// ====================
	// Check for grid collision
	// ====================
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		pInst = sGameObjInstList + i;

		// Skip non-active object instances
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;

		// Skip if object is the player gun, the platforms, the dirt, dotted or empty
		if (pInst->pObject->type == TYPE_PLAYERGUN || pInst->pObject->type == TYPE_PLATFORM
			|| pInst->pObject->type == TYPE_EMPTY || pInst->pObject->type == TYPE_DIRT || pInst->pObject->type == TYPE_DOTTED)
			continue;


		// **************************
		// Update grid collision flag
		// **************************

		// Variable declaration
		int prevbounce{ pInst->bounceCount };
		bool reflectedFlag{ false };

		// Check if current object is a bullet
		if (pInst->pObject->type == TYPE_BULLET) // Check map collision for bullets
		{
			// Call bullet type binary map collision check
			pInst->gridCollisionFlag = CheckInstanceBinaryMapCollision_Bullet(pInst->posCurr.x, pInst->posCurr.y, pInst->pObject->meshSize.x * pInst->scale.x, pInst->pObject->meshSize.y * pInst->scale.y,
				&MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT, &BinaryCollisionArray);
		}
		else // Check map collision for all other dynamic objects
		{
			pInst->gridCollisionFlag = CheckInstanceBinaryMapCollision(pInst->posCurr.x, pInst->posCurr.y, pInst->pObject->meshSize.x * pInst->scale.x, pInst->pObject->meshSize.y * pInst->scale.y,
				&MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
		}

		// Simulate particles when destructible block gets destroyed
		if (pInst->pObject->type == TYPE_BULLET && (pInst->gridCollisionFlag & COLLISION_Destructable) == COLLISION_Destructable)
		{
			AEVec2 particlevel{ 0,0 };
			AEVec2 hold;
			AEVec2 particlescale{ 0.85f, 0.85f };
			AEVec2Normalize(&hold, &pInst->velCurr);
			hold.x = hold.x / 6;
			hold.y = hold.y / 6;
			AEVec2 particlepos = { static_cast<float>(static_cast<int>(pInst->posCurr.x + hold.x)),  static_cast<float>(static_cast<int>(pInst->posCurr.y + hold.y)) + 0.65f };
			for (int innerI{}; innerI < 7; ++innerI)
			{
				particlevel.y = static_cast<float>(rand() % 7 + 2) / -3.5f;
				if (rand() % 2) { particlevel.x = static_cast<float>(rand() % 10) / 9.0f; }
				else { particlevel.x = static_cast<float>(rand() % 10) / 9.0f; }

				particlepos.x += 0.13f;
				gameObjInstCreate(&sGameObjList[particleObjIndex], &particlescale, &particlepos, &particlevel, 1.5f, STATE_GOING_LEFT);
			}
			// Destroy the projectile
			gameObjInstDestroy(pInst);
		}

		// Check if dynamic game objects collides with left side of level object
		if ((pInst->gridCollisionFlag & COLLISION_LEFT) == COLLISION_LEFT) {
			// Current object is a bullet
			if (pInst->pObject->type == TYPE_BULLET) {
				// Not reflected yet
				if (reflectedFlag == false) {
					// Variable declaration
					AEVec2 normal{ 1, 0 }, newBulletVel{};
					// Calculate new direction (newVel = currVel - 2 * (velCurr . normal) * normal)
					newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					// Set the vel to the calculated direction
					pInst->velCurr = newBulletVel;
					// Set flag variable to true
					reflectedFlag = true;
					//Limit number of bullet bounces
					if (prevbounce == pInst->bounceCount)
						++(pInst->bounceCount);
				}
			}
			else {
				// Set x-axis velocity to 0
				pInst->velCurr.x = 0;
				// Snap to grid
				SnapToCell(&pInst->posCurr.x);
				// Increment by 0.3f
				pInst->posCurr.x += 0.3f;
			}
		}

		// Check if dynamic game objects collides with right side of level object
		if ((pInst->gridCollisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT) {
			// Current object is a bullet
			if (pInst->pObject->type == TYPE_BULLET) {
				// Not reflected yet
				if (reflectedFlag == false) {
					// Variable declaration
					AEVec2 normal{ -1, 0 }, newBulletVel{};
					// Calculate new direction (newVel = currVel - 2 * (velCurr . normal) * normal)
					newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					// Set the vel to the calculated direction
					pInst->velCurr = newBulletVel;
					// Set flag variable to true
					reflectedFlag = true;
					//Limit number of bullet bounces
					if (prevbounce == pInst->bounceCount)
						++(pInst->bounceCount);
				}
			}
			else {
				// Set x-axis velocity to 0
				pInst->velCurr.x = 0;
				// Snap to grid
				SnapToCell(&pInst->posCurr.x);
				// Increment by 0.3f
				pInst->posCurr.x -= 0.3f;
			}
		}

		// Check if dynamic game objects collides with bottom side of level object
		if ((pInst->gridCollisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM) {
			// Current object is a bullet
			if (pInst->pObject->type == TYPE_BULLET) {
				// Not reflected yet
				if (reflectedFlag == false) {
					// Variable declaration
					AEVec2 normal{ 0, 1 }, newBulletVel{};
					// Calculate new direction (newVel = currVel - 2 * (velCurr . normal) * normal)
					newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					// Set the vel to the calculated direction
					pInst->velCurr = newBulletVel;
					// Set flag variable to true
					reflectedFlag = true;
					//Limit number of bullet bounces
					if (prevbounce == pInst->bounceCount)
						++(pInst->bounceCount);
				}
			}
			else {
				// Set y-axis velocity to 0
				pInst->velCurr.y = 0;
				// Snap to grid
				SnapToCell(&pInst->posCurr.y);
				// Check if object is a particle
				if (pInst->pObject->type == TYPE_PARTICLE1)
					pInst->posCurr.y -= 0.25f;	// Decrement by 0.25f
				// Check if object is the player
				if (pInst->pObject->type == TYPE_PLAYER)
					jumpFuel = FUEL_MAX_VALUE;	// Set its jump fuel
			}
		}

		// Check if dynamic game objects collides with top side of level object
		if ((pInst->gridCollisionFlag & COLLISION_TOP) == COLLISION_TOP) {
			// Current object is a bullet
			if (pInst->pObject->type == TYPE_BULLET) {
				// Not reflected yet
				if (reflectedFlag == false) {
					// Variable declaration
					AEVec2 normal{ 0, -1 }, newBulletVel{};
					// Calculate new direction (newVel = currVel - 2 * (velCurr . normal) * normal)
					newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					// Set the vel to the calculated direction
					pInst->velCurr = newBulletVel;
					//Limit number of bullet bounces
					if (prevbounce == pInst->bounceCount)
						++(pInst->bounceCount);
				}
			}
			else {
				// Set y-axis velocity to 0
				pInst->velCurr.y = 0;
				// Snap to grid
				SnapToCell(&pInst->posCurr.y);
			}
		}
	}

	// Attach gun to player after grid collision checks
	PlayerGun->posCurr = PlayerBody->posCurr;

	// ====================
	// Check for object collision
	// ====================
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		pInst = sGameObjInstList + i;

		// Skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// Switch logic based on current object's type
		switch (pInst->pObject->type) {
			// Projectile
		case TYPE_BULLET: {
			// Loop through all other objects
			for (unsigned long j = 0; j < GAME_OBJ_INST_NUM_MAX; j++)
			{
				GameObjInst* pOtherInst = sGameObjInstList + j;

				// Skip non-active object
				if ((pOtherInst->flag & FLAG_ACTIVE) == 0)
					continue;

				// Switch logic based on other object's type
				switch (pOtherInst->pObject->type) {
					// Player (Gets hit by bullet)
				case TYPE_PLAYER: {
					// Check for collision
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						// Deplete player's health
						playerHealth -= 10.0f;
						// Destroy the projectile
						gameObjInstDestroy(pInst);
					}
					break;
				}
								// 1st Enemy Variant (Gets hit by bullet)
				case TYPE_ENEMY1: {
					// Check for collision
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						// Destroy enemy and projectile instance
						gameObjInstDestroy(pInst);
						gameObjInstDestroy(pOtherInst);
						// Decrement number of enemies in level
						--totalEnemyCount;

						// Simulate/Create  particles upon enemy death
						AEVec2 particleVel;
						for (double x = pOtherInst->posCurr.x - 1.5; x < pOtherInst->posCurr.x + 1.5; x += ((1.f + rand() % 50) / 100.f))
						{
							AEVec2 particlespawn = { static_cast<float>(x), pOtherInst->posCurr.y };
							if (rand() % 2) // Randomize polarity of particleVel.x
							{
								particleVel = { rand() % 20 / -10.0f, rand() % 20 / 10.f };
								gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_NONE);
							}
							else
							{
								particleVel = { rand() % 20 / 10.f, rand() % 20 / 10.f };
								gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_ALERT);
							}
						}
					}
					break;
				}
								// Projectile (Collides with another projectile)
				case TYPE_BULLET: {
					// Don't check the same bullet to itself
					if (pInst->posCurr.x == pOtherInst->posCurr.x && pInst->posCurr.y == pOtherInst->posCurr.y)
						break;

					// Destroy two bullets if they collide with one another
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						gameObjInstDestroy(pInst);
						gameObjInstDestroy(pOtherInst);
					}
					break;
				}
				}
			}
			break;
		}
						// 1st Enemy Variant (Player collides with enemy)
		case TYPE_ENEMY1: {
			// Check for collision
			if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, PlayerBody->boundingBox, PlayerBody->velCurr))
				playerHealth = 0.0f; // Player dies if collide with enemy

		}
		}
	}


	// =====================================
	// calculate the matrix for all objects
	// =====================================
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		pInst = sGameObjInstList + i;
		AEMtx33		 trans, rot, scale;

		// Skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// Compute the scaling matrix
		AEMtx33Scale(&scale, pInst->scale.x, pInst->scale.y);
		// Compute the rotation matrix
		if (pInst->pObject->type == TYPE_BULLET)
			AEMtx33Rot(&rot, 0);	// No rotation for projectiles
		else
			AEMtx33Rot(&rot, pInst->dirCurr);
		// Compute the translation matrix
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
		AEMtx33Concat(&pInst->transform, &trans, &rot);
		AEMtx33Concat(&pInst->transform, &pInst->transform, &scale);
	}

	// =====================================
	// Camera 
	// =====================================
	{
		// Update Camera position, for Levels
		float cameraX, cameraY;
		AEGfxGetCamPosition(&cameraX, &cameraY);

		// Calculate the camera position based on the player's position
		AEVec2 NewCamPos{ PlayerBody->posCurr.x, PlayerBody->posCurr.y };
		AEMtx33MultVec(&NewCamPos, &MapTransform, &NewCamPos);

		// Clamp camera within the level frame
		NewCamPos.x = AEClamp(NewCamPos.x, -(static_cast<float>(AEGetWindowWidth() / static_cast<float>(BINARY_MAP_WIDTH) * 141.0f)), (static_cast<float>(AEGetWindowWidth() / static_cast<float>(BINARY_MAP_WIDTH) * 141.0f)));
		NewCamPos.y = AEClamp(NewCamPos.y, -(static_cast<float>(AEGetWindowHeight()) / static_cast<float>(BINARY_MAP_HEIGHT) * 46.0f), (static_cast<float>(AEGetWindowHeight()) / static_cast<float>(BINARY_MAP_HEIGHT) * 46.0f));
		
		// Reset camera upon level reset
		if (playerDeathTimer == 0)
			AEGfxSetCamPosition(NewCamPos.x, NewCamPos.y);

		// Calculate mouse position in world coordinates based on camera
		worldMouseX = cameraX + (static_cast<float>(g_mouseX) - static_cast<float>(AEGetWindowWidth()) / 2);
		worldMouseY = cameraY + (-1) * (static_cast<float>(g_mouseY) - static_cast<float>(AEGetWindowHeight()) / 2);
	}
}

/******************************************************************************/
/*!
	"Draw" function of this state / Render all game elements
*/
/******************************************************************************/
void GameStateLevelsDraw(void)
{
	// Set render settings
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
	AEGfxSetTransparency(1.0f);

	// Variable declaration
	AEMtx33 cellTranslation, cellFinalTransformation;

	// ----- Draw the tile map (the grid) -----
	for (int i = 0; i < BINARY_MAP_WIDTH; ++i) {
		for (int j = 0; j < BINARY_MAP_HEIGHT; ++j) {
			// Calculate transform matrix
			AEMtx33Trans(&cellTranslation, static_cast<f32>(AEGetWindowWidth() / BINARY_MAP_WIDTH * i), static_cast<f32>(AEGetWindowHeight() / BINARY_MAP_HEIGHT * j));
			AEMtx33Trans(&cellTranslation, i + 0.5f, j + 0.5f);
			AEMtx33Concat(&cellFinalTransformation, &MapTransform, &cellTranslation);
			AEGfxSetTransform(cellFinalTransformation.m);

			// Set render settings
			AEGfxSetBlendMode(AE_GFX_BM_NONE);
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

			// Switch logic based on the current object at index
			switch (GetCellValue(i, j, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT)) {
				// Platform
				case TYPE_PLATFORM: {
					// Set render settings
					AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
					AEGfxTextureSet(stoneTexture, 0.f, 0.f);
					// Draw
					AEGfxMeshDraw(PlatformInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
					break;
				}
				// Dirt
				case TYPE_DIRT: {
					// Set render settings
					AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
					AEGfxTextureSet(dirtTexture, 0.f, 0.f);
					// Draw
					AEGfxMeshDraw(DirtInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
					break;
				}
			}
		}
	}

	// Set render settings
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);

	// Render health bar is player health more than 0
	if (playerHealth >= 0.0f) {
		// ----- Draw health bar on screen -----
		// Find offset transform from player's position	
		AEMtx33 scale, translate, transform;
		AEMtx33Scale(&scale, HEALTHBAR_SCALE.x * (playerHealth / PLAYER_INITIAL_HEALTH), HEALTHBAR_SCALE.y);
		// Compute the translation matrix
		AEMtx33Trans(&translate, PlayerBody->posCurr.x - 0.75f, PlayerBody->posCurr.y + 1.0f);
		// Concat Matrix together
		AEMtx33Concat(&transform, &translate, &scale);
		// Concat with map transform
		AEMtx33Concat(&transform, &MapTransform, &transform);
		// Set transform
		AEGfxSetTransform(transform.m);

		// Set render settings
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxTextureSet(NULL, 0.0f, 0.0f);
		AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
		AEGfxSetTransparency(0.8f);
		
		//Draw player health bar
		AEGfxMeshDraw(PlayerHealthBar->pMesh, AE_GFX_MDM_TRIANGLES);
	}

	// Set render settings
	AEGfxSetTransparency(1.0f);

	// ----- Draw all object instances in the list -----
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE) || 0 == (pInst->flag & FLAG_VISIBLE))
			continue;

		// Calculate transform matrix
		AEMtx33Concat(&cellFinalTransformation, &MapTransform, &pInst->transform);
		AEGfxSetTransform(cellFinalTransformation.m);

		// Particle transparancy handler
		if (pInst->pObject->type == TYPE_PARTICLE1) 
		{
			AEGfxSetTransparency(pInst->dirCurr); // Set transparancy of particle
			
			// Destory particle if it is fully transparent
			if (pInst->dirCurr <= 0)
			{
				gameObjInstDestroy(pInst); 
			}

			// Orange particles
			if (pInst->state == STATE_ALERT) 
			{
				AEGfxSetBlendColor(1.0f, 0.35f, 0.0f, 1.f); 
			}

			// Brown particles
			if (pInst->state == STATE_GOING_LEFT) 
			{
				AEGfxSetBlendColor(0.627f, 0.321f, 0.176f, 1.f);
			}

			// Deplete transparancy of particles
			pInst->dirCurr -= g_dt; 
		}
		else if (pInst->pObject->type == TYPE_DOTTED && pInst->state == STATE_GOING_RIGHT)	// Enemy LoS object instance       
			AEGfxSetTransparency(0.f); // Make enemy line of sight detection invisible
		else if (pInst->pObject->type == TYPE_DOTTED && pInst->state == STATE_NONE) // Projectile trajectory prediction object instance
			AEGfxSetTransparency(0.4f); // Make projectile trajectory prediction translucent
		
		// Set render settings
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxTextureSet(NULL, 0.0f, 0.0f);
		// Draw object instance
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		
		// Set render settings
		AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
		AEGfxSetTransparency(1.f);

		//Destroy all dotted line objects every frame after rendering
		if (pInst->pObject->type == TYPE_DOTTED)
			gameObjInstDestroy(pInst); 
	}

	//// ----- Drawing UI elements -----

	//	Drawing for Font for all states
	f32 TextWidth = 1.0f;
	f32 TextHeight = 1.0f;
	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));

	// Display time spent in level
	sprintf_s(strBuffer, "Current Time : %.2f", levelTime);
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, 0.8f - TextWidth / 2, 0.9f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	// Display ammo capacity
	sprintf_s(strBuffer, "Current Ammo : %d", ammoCount);
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, 0.8f - TextWidth / 2, 0.8f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	// Warning for low ammo
	if (ammoCount < 8) // Warning for low Ammo
	{
		sprintf_s(strBuffer, "Warning: Low Ammo");
		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font20, strBuffer, 0.0f - TextWidth / 2, -0.9f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
	}
	
	// Display number of enemies left in the level
	sprintf_s(strBuffer, "Enemies Left : %d", totalEnemyCount);
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, 0.8f - TextWidth / 2, 0.7f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	// ----- Render Pause Menu ------
	if (gGameStateInnerState == GAME_PAUSE) {
		// Inform player of pause state
		sprintf_s(strBuffer, "GAME PAUSED");
		AEGfxGetPrintSize(g_font30, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font30, strBuffer, 0.0f - TextWidth / 2, 0.4f - TextHeight / 2, 1.0f, 1.f, 1.f, 0.f);
	
		// Inform player of key press to continue game
		sprintf_s(strBuffer, "Press 'Esc' to Continue Game");
		AEGfxGetPrintSize(g_font30, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font30, strBuffer, 0.0f - TextWidth / 2, 0.2f - TextHeight / 2, 1.0f, 1.f, 1.f, 0.f);
		
		// Inform player of key press to restart game
		sprintf_s(strBuffer, "Press 'R' to Restart Level");
		AEGfxGetPrintSize(g_font30, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font30, strBuffer, 0.0f - TextWidth / 2, 0.0f - TextHeight / 2, 1.0f, 1.f, 1.f, 0.f);

		// Inform player of key press to return to main menu
		sprintf_s(strBuffer, "Press 'M' to Return to Main Menu");
		AEGfxGetPrintSize(g_font30, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font30, strBuffer, 0.0f - TextWidth / 2, -0.2f - TextHeight / 2, 1.0f, 1.f, 1.f, 0.f);
	}
}

/******************************************************************************/
/*!
	"Free" function of this state
*/
/******************************************************************************/
void GameStateLevelsFree(void)
{
	// kill all object instances in the array using "gameObjInstDestroy"
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
		GameObjInst* pInst = sGameObjInstList + i;

		gameObjInstDestroy(pInst);
	}

	// Stop all sounds in the sound channel
	soundChannel->stop();
}

/******************************************************************************/
/*!
	"Unload" function of this state
*/
/******************************************************************************/
void GameStateLevelsUnload(void)
{
	// Free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned long i = 0; i < sGameObjNum; i++) {
		GameObj* pObj = sGameObjList + i;
		// Check if object mesh is not a nullptr
		if (pObj->pMesh != nullptr)
			AEGfxMeshFree(pObj->pMesh);
	}

	// Free the allocated map data
	FreeMapData(&MapData, &BinaryCollisionArray, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
	
	// ----- Texture Unload -----
	AEGfxTextureUnload(stoneTexture);
	stoneTexture = nullptr;
	AEGfxTextureUnload(dirtTexture);
	dirtTexture = nullptr;
}
