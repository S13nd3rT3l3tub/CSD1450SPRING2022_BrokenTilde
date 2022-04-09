/*!
@Copyright	Copyright � 2022 DigiPen, All rights reserved.
@file       GameState_Levels.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Authored Functions
-------------------------------------------------------------------------------
@author		Mohamed Zafir (m.zafir@digipen.edu)
@role		Authored Functions
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu)
@role		Authored Functions
-------------------------------------------------------------------------------
@author		Desmond Too Wei Kang (d.too@digipen.edu)
@role		Authored Functions
*//*_________________________________________________________________________*/

#include "Main.h"

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
//const float MAX_SCRIPT_DISPLAY_TIME{ 3.0f };


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
static float playerdeathtimer{};

// Level Timers for script
//float scriptTimer1;

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

	// Set objects to null first
	PlayerBody = nullptr;
	PlayerGun = nullptr;
	EmptyInstance = nullptr;
	PlatformInstance = nullptr;
	DirtInstance = nullptr;

	/******************************************************************************/
	/*!
		Load/create the mesh data (game objects / Shapes)
	*/
	/******************************************************************************/
	{
		GameObj* pObj;

		// =========================
		// create the non collision shape
		// =========================
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

		// =========================
		// create the platform shape
		// =========================
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

		// =====================
		// create the player shape
		// =====================
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

		// =====================
		// create the player gun shape
		// =====================
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

		// =======================
		// create the bullet shape
		// =======================
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

		// =====================
		// create the Enemy1 shape
		// =====================
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

		// =====================
		// create the Enemy2 shape
		// =====================
		{
			enemy2ObjIndex = sGameObjNum;
			pObj = sGameObjList + sGameObjNum++;
			pObj->type = TYPE_ENEMY1;

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

		// =====================
		// create the Particle1 shape
		// =====================
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

		// =====================
		// create the dotted line shape
		// =====================
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

		// =========================
		// create the dirt block shape
		// =========================
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

		// =====================
		// create the health bar shape
		// =====================
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
	std::string fileName{ "" };
	switch (g_chosenLevel) {
	case 2:
		fileName = ".\\Resources\\Level Data\\Level2.txt";
		break;
	default:
		break;
	}
	if (ImportMapDataFromFile(fileName, &MapData, &BinaryCollisionArray, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 0)
		gGameStateNext = GS_QUIT;


	//Computing the matrix which take a point out of the normalized coordinates system of the binary map
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
	// Initialize variables to zero
	levelTime = 0;
	totalEnemyCount = 0;
	// Set player's initial health
	playerHealth = PLAYER_INITIAL_HEALTH;

	// Create platform instance
	PlatformInstance = gameObjInstCreate(&sGameObjList[platformObjIndex], &PLATFORM_SCALE, 0, 0, 0.0f, STATE_NONE);
	PlatformInstance->flag ^= FLAG_VISIBLE;
	PlatformInstance->flag |= FLAG_NON_COLLIDABLE;

	// Create destructible platform instance
	DirtInstance = gameObjInstCreate(&sGameObjList[dirtObjIndex], &PLATFORM_SCALE, 0, 0, 0.0f, STATE_NONE);
	DirtInstance->flag ^= FLAG_VISIBLE;
	DirtInstance->flag |= FLAG_NON_COLLIDABLE;

	AEVec2 Pos{}, Scale{};

	srand(static_cast<unsigned int>(time(NULL))); // Seed random generator

	
	// Initialise Binary map
	for (int row = 0; row < BINARY_MAP_HEIGHT; row++) {
		for (int col = 0; col < BINARY_MAP_WIDTH; col++) {
			Pos.x = col + 0.5f;
			Pos.y = row + 0.5f;

			switch (GetCellValue(col, row, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT)) {
			case TYPE_PLAYER:
				PlayerBody = gameObjInstCreate(&sGameObjList[playerObjIndex], &PLAYER_SCALE, &Pos, nullptr, 0.0f, STATE_NONE);
				PlayerGun = gameObjInstCreate(&sGameObjList[playerGunObjIndex], &GUN_SCALE, &Pos, nullptr, 0.0f, STATE_NONE);
				break;
			case TYPE_ENEMY1:
				gameObjInstCreate(&sGameObjList[enemy1ObjIndex], &PLAYER_SCALE, &Pos, nullptr, 0.0f, STATE_GOING_LEFT);
				++totalEnemyCount;
				break;
			case TYPE_ENEMY2:
				gameObjInstCreate(&sGameObjList[enemy2ObjIndex], &PLAYER_SCALE, &Pos, nullptr, 0.0f, STATE_GOING_LEFT);
				++totalEnemyCount;
				break;
			default:
				break;
			}
		}
	}

	// Set ammoCount
	ammoCount = static_cast<int>(totalEnemyCount * 4);
}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateLevelsUpdate(void)
{
	switch (gGameStateInnerState) {
	case GAME_PAUSE:
		soundChannel->setPaused(true);

		if (AEInputCheckReleased(AEVK_ESCAPE) && winFocused)
			gGameStateInnerState = GAME_PLAY;
		else if (AEInputCheckReleased(AEVK_R) && winFocused) {
			gGameStateNext = GS_RESTART;
			gGameStateInnerState = GAME_PLAY;
		}
		else if (AEInputCheckReleased(AEVK_M) && winFocused) {
			gGameStateNext = GS_MAINMENU;
			gGameStateInnerState = GAME_PLAY;
		}
		break;
	case GAME_WIN:
		gGameStateNext = GS_WINSCREEN;
		gGameStateInnerState = GAME_PLAY;
		++g_chosenLevel;
		break;
	case GAME_LOSE: {
		if (playerdeathtimer == PLAYER_DEATH_ANIME_TIME) {
			gameObjInstDestroy(PlayerGun);
			gameObjInstDestroy(PlayerBody);
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
		}

		// post player death control
		if (playerdeathtimer > 0) 
		{
			playerdeathtimer -= g_dt;
		}
		else if (playerdeathtimer < 0)
		{
			gGameStateInnerState = GAME_PLAY;
			gGameStateNext = GS_RESTART;
			playerdeathtimer = 0;
			
			std::string fileName{ "" };
			switch (g_chosenLevel) {
			case 2:
				fileName = ".\\Resources\\Level Data\\Level2.txt";
				break;
			default:
				break;
			}
			// reload map data
			if (ImportMapDataFromFile(fileName, &MapData, &BinaryCollisionArray, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 0)
				gGameStateNext = GS_QUIT;
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
	case GAME_PLAY:
		if (winFocused == false)
			gGameStateInnerState = GAME_PAUSE;

		bool result;
		soundChannel->getPaused(&result);
		if (result)
			soundChannel->setPaused(false);

		// Update level time
		levelTime += g_dt;

		// Check win state
		if (totalEnemyCount <= 0 && ammoCount > 1) {
			gGameStateInnerState = GAME_WIN;
		}
		// Check lose state
		if (playerHealth <= 0.0f || ammoCount <= 0) {
			playerHealth = 0;
			playerdeathtimer = PLAYER_DEATH_ANIME_TIME;
			gGameStateInnerState = GAME_LOSE;
		}

		// =========================
		// update according to input
		// =========================
		
		//	if escape key is pressed
		if (AEInputCheckReleased(AEVK_ESCAPE))
			gGameStateInnerState = GAME_PAUSE;

		// ----------------------------------------------------------------------------------------------------------------------------------------------
		// Change the following input movement based on our player movement
		// ----------------------------------------------------------------------------------------------------------------------------------------------
		
		// cheatcode to go next level		
		if (AEInputCheckReleased(AEVK_DOWN))
			totalEnemyCount = 0;

		if (AEInputCheckCurr(AEVK_W) && jumpFuel > 0) // Hold to thrust upward
		{
			AEVec2 added;
			AEVec2Set(&added, 0.f, 1.f);

			// Find the velocity according to the acceleration
			added.x *= 1;
			added.y *= 20 * g_dt; 

			AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);

			// Limit your velocity
			AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.99f);

			// Deplete jumpfuel
			jumpFuel -= g_dt;

			// Create particles when thrusting upward
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

		if (AEInputCheckCurr(AEVK_A)) // Move left
		{
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
			AEVec2 added;
			AEVec2Set(&added, cosf(PlayerBody->dirCurr), sinf(PlayerBody->dirCurr));

			// Find the velocity according to the acceleration
			added.x *= MOVE_VELOCITY * g_dt;
			added.y *= MOVE_VELOCITY * g_dt;
			AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);

			// Limit velocity
			AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f);
		}

		AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f); \


		// ------------------------------------------------------------------------------------
		// Change to bullet spawning on mouse click in direction
		// ------------------------------------------------------------------------------------
			AEVec2 BarrelEnd;
		// Shoot a bullet if left mouse button is triggered (Create a new object instance)
		if (AEInputCheckReleased(VK_LBUTTON) && ammoCount > 0)
		{
			AEVec2 dirBullet;

			// Get the bullet's direction according to the player's direction
			AEVec2Set(&dirBullet, cosf(PlayerGun->dirCurr), sinf(PlayerGun->dirCurr));

			// Set the velocity
			dirBullet.x *= BULLET_SPEED;
			dirBullet.y *= BULLET_SPEED;

			// Calculate end of gun barrel position
			BarrelEnd.x = PlayerGun->posCurr.x + dirBullet.x * 0.11f;
			BarrelEnd.y = PlayerGun->posCurr.y + dirBullet.y * 0.11f;

			// Create bullet instance
			gameObjInstCreate(&sGameObjList[bulletObjIndex], &BULLET_SCALE, &BarrelEnd, &dirBullet, PlayerGun->dirCurr, STATE_NONE);
			--ammoCount; // Deplete ammoCount

			// Play shooting sound
			if (soundVolumeLevel) 
				fModSys->playSound(playerShoot, nullptr, false, &soundChannel);
		}

		if (AEInputCheckCurr(VK_RBUTTON)) // Aim with trajectory dotted line
		{
			AEVec2 bulletDir{};
			AEVec2Set(&bulletDir, cosf(PlayerGun->dirCurr), sinf(PlayerGun->dirCurr));
			BarrelEnd.x = PlayerGun->posCurr.x + bulletDir.x * BULLET_SPEED * 0.11f;
			BarrelEnd.y = PlayerGun->posCurr.y + bulletDir.y * BULLET_SPEED * 0.11f;

			AEVec2 currPos{ BarrelEnd };

			int bounceCount{ 0 };
			for (int i{ 1 }; i < 1000; ++i) {
				currPos.x += bulletDir.x * g_dt;
				currPos.y += bulletDir.y * g_dt;

				int collisionFlag = CheckInstanceBinaryMapCollision_Dotted(currPos.x, currPos.y, BULLET_MESHSIZE.x * BULLET_SCALE.x, BULLET_MESHSIZE.y * BULLET_SCALE.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);

				if ((collisionFlag & COLLISION_Destructable) == COLLISION_Destructable)
					break;

				bool reflectedFlag{ false };
				AEVec2 normal{};
				if ((collisionFlag & COLLISION_TOP) == COLLISION_TOP && reflectedFlag == false) {
					normal = { 0, -1 };
					if (++bounceCount < 3)
						reflectedFlag = true;
					else
						break;
				}
				if ((collisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM && reflectedFlag == false) {
					normal = { 0, 1 };
					if (++bounceCount < 3)
						reflectedFlag = true;
					else
						break;
				}
				if ((collisionFlag & COLLISION_LEFT) == COLLISION_LEFT && reflectedFlag == false) {
					normal = { 1, 0 };
					if (++bounceCount < 3)
						reflectedFlag = true;
					else
						break;
				}
				if ((collisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT && reflectedFlag == false) {
					normal = { -1, 0 };
					if (++bounceCount < 3)
						reflectedFlag = true;
					else
						break;
				}

				if (reflectedFlag) {
					AEVec2 newVel{ bulletDir.x - 2 * (AEVec2DotProduct(&bulletDir, &normal)) * normal.x,  bulletDir.y - 2 * (AEVec2DotProduct(&bulletDir, &normal)) * normal.y };
					AEVec2Normalize(&newVel, &newVel);
					bulletDir = newVel;
					currPos.x += bulletDir.x * g_dt;
					currPos.y += bulletDir.y * g_dt;
				}

				if (i % 30 == 0)
					gameObjInstCreate(&sGameObjList[dottedObjIndex], &BULLET_SCALE, &currPos, 0, 0, STATE_NONE);
			}

		}

		int i{};
		GameObjInst* pInst;

		//Update object instances physics and behavior
		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
		{
			pInst = sGameObjInstList + i;

			// Skip non-active object
			if (0 == (pInst->flag & FLAG_ACTIVE))
				continue;

			// Destory player shot bullet after 3 bounces
			if (pInst->pObject->type == TYPE_BULLET && pInst->bounceCount >= 3)
				gameObjInstDestroy(pInst);

			// Destory enemy shot bullet after it collides with game
			if (pInst->pObject->type == TYPE_BULLET && pInst->state == STATE::STATE_GOING_LEFT && pInst->bounceCount >= 1)
				gameObjInstDestroy(pInst);

			// Enable gravity for player and enemies
			if (pInst->pObject->type == TYPE_ENEMY1 || pInst->pObject->type == TYPE_PLAYER)
				pInst->velCurr.y += GRAVITY * g_dt;

			// Enemy AI
			if (pInst->pObject->type == TYPE_ENEMY1 || pInst->pObject->type == TYPE_ENEMY2) {
				EnemyStateMachine(pInst);
			}
		}

		//Update object instances positions
		for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
		{
			pInst = sGameObjInstList + i;

			// skip non-active object
			if (0 == (pInst->flag & FLAG_ACTIVE))
				continue;

			/********************************
			update the position of instances
			********************************/

			// ----- Update Position -----
			pInst->posCurr.x += pInst->velCurr.x * g_dt;
			pInst->posCurr.y += pInst->velCurr.y * g_dt;

			// ----- Update Bounding Box -----
			pInst->boundingBox.min.x = -(pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
			pInst->boundingBox.min.y = -(pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;

			pInst->boundingBox.max.x = (pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
			pInst->boundingBox.max.y = (pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;
		}

		AEVec2 playerWorldPos{ PlayerBody->posCurr.x, PlayerBody->posCurr.y };
		AEMtx33MultVec(&playerWorldPos, &MapTransform, &playerWorldPos);
		
		float dotProduct = atan2(worldMouseY - playerWorldPos.y, worldMouseX - playerWorldPos.x);
		PlayerGun->dirCurr = dotProduct;

		break;
	}

	int i{};
	GameObjInst* pInst;
	//Check for grid collision
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		pInst = sGameObjInstList + i;

		// skip non-active object instances
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;

		if (pInst->pObject->type == TYPE_PLAYERGUN || pInst->pObject->type == TYPE_PLATFORM
			|| pInst->pObject->type == TYPE_EMPTY || pInst->pObject->type == TYPE_DIRT || pInst->pObject->type == TYPE_DOTTED)
			continue;


		/*************************
		Update grid collision flag
		**************************/
		int prevbounce = pInst->bounceCount;
		if (pInst->pObject->type == TYPE_BULLET) // Check map collision for bullets
		{
			pInst->gridCollisionFlag = CheckInstanceBinaryMapCollision_Bullet(pInst->posCurr.x, pInst->posCurr.y, pInst->pObject->meshSize.x * pInst->scale.x, pInst->pObject->meshSize.y * pInst->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT, &BinaryCollisionArray);
		}
		else // Check map collision for all other dynamic objects
		{
			pInst->gridCollisionFlag = CheckInstanceBinaryMapCollision(pInst->posCurr.x, pInst->posCurr.y, pInst->pObject->meshSize.x * pInst->scale.x, pInst->pObject->meshSize.y * pInst->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
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
				gameObjInstCreate(&sGameObjList[particleObjIndex], &particlescale, &particlepos, &particlevel, 1.5f, STATE_ALERT);
			}
			gameObjInstDestroy(pInst); //destroy bullet
		}

		bool reflectedFlag = false;

		// If dynamic game objects collides with left side of level object
		if ((pInst->gridCollisionFlag & COLLISION_LEFT) == COLLISION_LEFT) {
			if (pInst->pObject->type == TYPE_BULLET) {
				if (reflectedFlag == false) {
					AEVec2 normal{ 1, 0 }, newBulletVel{};
					newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					pInst->velCurr = newBulletVel;

					//Limit number of bullet bounces:
					reflectedFlag = true;
					if (prevbounce == pInst->bounceCount)
						++(pInst->bounceCount);
				}
			}
			else {
				pInst->velCurr.x = 0;
				SnapToCell(&pInst->posCurr.x);
				pInst->posCurr.x += 0.3f;
			}
		}

		if ((pInst->gridCollisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT) {
			if (pInst->pObject->type == TYPE_BULLET) {
				if (reflectedFlag == false) {
					AEVec2 normal{ -1, 0 }, newBulletVel{};
					newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					pInst->velCurr = newBulletVel;
					reflectedFlag = true;
					if (prevbounce == pInst->bounceCount)
						++(pInst->bounceCount);
				}
			}
			else {
				pInst->velCurr.x = 0;
				SnapToCell(&pInst->posCurr.x);
				pInst->posCurr.x -= 0.3f;
			}
		}

		if ((pInst->gridCollisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM) {
			if (pInst->pObject->type == TYPE_BULLET) {
				if (reflectedFlag == false) {
					AEVec2 normal{ 0, 1 }, newBulletVel{};
					newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					pInst->velCurr = newBulletVel;
					//Limit number of bullet bounces:
					reflectedFlag = true;
					if (prevbounce == pInst->bounceCount)
						++(pInst->bounceCount);
				}
			}
			else {
				pInst->velCurr.y = 0;
				SnapToCell(&pInst->posCurr.y);
				if (pInst->pObject->type == TYPE_PARTICLE1)
				{
					pInst->posCurr.y -= 0.25f; // offset for particles
				}
				if (pInst->pObject->type == TYPE_PLAYER)
				{
					jumpFuel = 1.5f; // refuel jumpfuel
				}
			}
		}

		if ((pInst->gridCollisionFlag & COLLISION_TOP) == COLLISION_TOP) {
			if (pInst->pObject->type == TYPE_BULLET) {
				if (reflectedFlag == false) {
					AEVec2 normal{ 0, -1 }, newBulletVel{};
					newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					pInst->velCurr = newBulletVel;

					//Limit number of bullet bounces:
					if (prevbounce == pInst->bounceCount)
						++(pInst->bounceCount);
				}
			}
			else {
				pInst->velCurr.y = 0;
				SnapToCell(&pInst->posCurr.y);
			}
		}


	}

	// Attach gun to player after grid collision checks
	PlayerGun->posCurr = PlayerBody->posCurr;

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
		case TYPE_BULLET:
			for (unsigned long j = 0; j < GAME_OBJ_INST_NUM_MAX; j++)
			{
				GameObjInst* pOtherInst = sGameObjInstList + j;
				// skip non-active object
				if ((pOtherInst->flag & FLAG_ACTIVE) == 0)
					continue;

				switch (pOtherInst->pObject->type) {
				case TYPE_PLAYER: // If player gets hit by bullet
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						if (pInst->state == STATE::STATE_GOING_LEFT || pInst->state == STATE::STATE_NONE)
							playerHealth -= 10.0f; // Deplete playerhealth
						gameObjInstDestroy(pInst); // Destroy bullet
					}
					break;
				case TYPE_ENEMY1: // If enemy gets hit by bullet 
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						gameObjInstDestroy(pInst); // Destroy bullet
						gameObjInstDestroy(pOtherInst); // Destory enemey
						--totalEnemyCount; // Deplete enemy count

						// Simulate particles upon enemy death
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
				case TYPE_BULLET: // If a bullet collides with another bullet
					if (pInst->posCurr.x == pOtherInst->posCurr.x && pInst->posCurr.y == pOtherInst->posCurr.y) // Don't check the same bullet to itself
					{
						break;
					}
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						gameObjInstDestroy(pInst); // destory bullet
						gameObjInstDestroy(pOtherInst); // destory bullet
					}
				}
			}
			break;
		case TYPE_ENEMY1: // If player collides with enemy
			if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, PlayerBody->boundingBox, PlayerBody->velCurr)) {
				playerHealth = 0; // Player dies if collide with enemy
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

		// skip non-active object
		if ((pInst->flag & FLAG_ACTIVE) == 0)
			continue;

		// Compute the scaling matrix
		AEMtx33Scale(&scale, pInst->scale.x, pInst->scale.y);

		if (pInst->pObject->type == TYPE_BULLET)
			AEMtx33Rot(&rot, 0);
		else
			AEMtx33Rot(&rot, pInst->dirCurr);
		// Compute the translation matrix
		AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
		// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
		AEMtx33Concat(&pInst->transform, &trans, &rot);
		AEMtx33Concat(&pInst->transform, &pInst->transform, &scale);
	}

	// Update Camera position, for Levels
	float cameraX, cameraY;
	AEGfxGetCamPosition(&cameraX, &cameraY);

	AEVec2 NewCamPos{ PlayerBody->posCurr.x, PlayerBody->posCurr.y };
	AEMtx33MultVec(&NewCamPos, &MapTransform, &NewCamPos);

	// Clamp camera within the level frame
	NewCamPos.x = AEClamp(NewCamPos.x, -(static_cast<float>(AEGetWindowWidth() / static_cast<float>(BINARY_MAP_WIDTH) * 141.0f)), (static_cast<float>(AEGetWindowWidth() / static_cast<float>(BINARY_MAP_WIDTH) * 141.0f)));
	NewCamPos.y = AEClamp(NewCamPos.y, -(static_cast<float>(AEGetWindowHeight()) / static_cast<float>(BINARY_MAP_HEIGHT) * 46.0f), (static_cast<float>(AEGetWindowHeight()) / static_cast<float>(BINARY_MAP_HEIGHT) * 46.0f));
	if (playerdeathtimer == 0)
		AEGfxSetCamPosition(NewCamPos.x, NewCamPos.y); // reset camera upon level reset

	// Mouse in world coordinates
	worldMouseX = cameraX + (static_cast<float>(g_mouseX) - static_cast<float>(AEGetWindowWidth()) / 2);
	worldMouseY = cameraY + (-1) * (static_cast<float>(g_mouseY) - static_cast<float>(AEGetWindowHeight()) / 2);
}

/******************************************************************************/
/*!
	Render game objects
*/
/******************************************************************************/
void GameStateLevelsDraw(void)
{

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
	AEGfxSetTransparency(1.0f);

	//Drawing the tile map (the grid)
	AEMtx33 cellTranslation, cellFinalTransformation;

	for (int i = 0; i < BINARY_MAP_WIDTH; ++i)
		for (int j = 0; j < BINARY_MAP_HEIGHT; ++j)
		{
			AEMtx33Trans(&cellTranslation, static_cast<f32>(AEGetWindowWidth() / BINARY_MAP_WIDTH * i), static_cast<f32>(AEGetWindowHeight() / BINARY_MAP_HEIGHT * j));

			AEMtx33Trans(&cellTranslation, i + 0.5f, j + 0.5f);

			AEMtx33Concat(&cellFinalTransformation, &MapTransform, &cellTranslation);
			AEGfxSetTransform(cellFinalTransformation.m);

			AEGfxSetBlendMode(AE_GFX_BM_NONE);
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

			if (GetCellValue(i, j, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == TYPE_PLATFORM)
			{
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxTextureSet(stoneTexture, 0.f, 0.f);
				AEGfxMeshDraw(PlatformInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			}
			else if (GetCellValue(i, j, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == TYPE_DIRT)
			{
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxTextureSet(dirtTexture, 0.f, 0.f);
				AEGfxMeshDraw(DirtInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);

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

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxTextureSet(NULL, 0.0f, 0.0f);
		AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
		AEGfxSetTransparency(0.8f);
		AEGfxMeshDraw(PlayerHealthBar->pMesh, AE_GFX_MDM_TRIANGLES);
	}
	// draw all object instances in the list
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE) || 0 == (pInst->flag & FLAG_VISIBLE))
			continue;

		AEMtx33Concat(&cellFinalTransformation, &MapTransform, &pInst->transform);
		AEGfxSetTransform(cellFinalTransformation.m);

		// Particle transparancy handler
		if (pInst->pObject->type == TYPE_PARTICLE1) 
		{
			AEGfxSetTransparency(pInst->dirCurr); // Set transparancy of particle
			if (pInst->dirCurr <= 0)
			{
				gameObjInstDestroy(pInst); // Destory particle if it is fully transparent
			}

			if (pInst->state == STATE_ALERT)
			{
				AEGfxSetBlendColor(1.0f, 0.35f, 0.0f, 1.f); // Orange color for particle
			}
			pInst->dirCurr -= g_dt; // Deplete transparancy of particles
		}
		else if (pInst->pObject->type == TYPE_DOTTED && pInst->state == STATE_GOING_RIGHT)           
		{
			AEGfxSetTransparency(0.f); // Set enemy line of sight to be invisible
		}
		else if (pInst->pObject->type == TYPE_DOTTED && pInst->state == STATE_NONE)
		{
			AEGfxSetTransparency(0.4f); // Set trajectory dotted line to be translucent
		}
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxTextureSet(NULL, 0.0f, 0.0f);
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
		AEGfxSetTransparency(1.f);
		if (pInst->pObject->type == TYPE_DOTTED)
		{
			gameObjInstDestroy(pInst); // Destory all dotted line instances every frame
		}
	}

	//// ----- Drawing UI elements -----

	//	Drawing for Font for all states
	f32 TextWidth = 1.0f;
	f32 TextHeight = 1.0f;
	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));

	sprintf_s(strBuffer, "Current Time : %.2f", levelTime);
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, 0.8f - TextWidth / 2, 0.9f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Current Ammo : %d", ammoCount);
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, 0.8f - TextWidth / 2, 0.8f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	if (ammoCount < 8) // Warning for low Ammo
	{
		sprintf_s(strBuffer, "Warning: Low Ammo");
		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font20, strBuffer, 0.0f - TextWidth / 2, -0.9f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
	}
	sprintf_s(strBuffer, "Enemies Left : %d", totalEnemyCount);
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, 0.8f - TextWidth / 2, 0.7f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	if (gGameStateInnerState == GAME_PAUSE) {
		sprintf_s(strBuffer, "GAME PAUSED");
		AEGfxGetPrintSize(g_font30, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font30, strBuffer, 0.0f - TextWidth / 2, 0.4f - TextHeight / 2, 1.0f, 1.f, 1.f, 0.f);
	
		sprintf_s(strBuffer, "Press 'Esc' to Continue Game");
		AEGfxGetPrintSize(g_font30, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font30, strBuffer, 0.0f - TextWidth / 2, 0.2f - TextHeight / 2, 1.0f, 1.f, 1.f, 0.f);
		
		sprintf_s(strBuffer, "Press 'R' to Restart Level");
		AEGfxGetPrintSize(g_font30, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font30, strBuffer, 0.0f - TextWidth / 2, 0.0f - TextHeight / 2, 1.0f, 1.f, 1.f, 0.f);

		sprintf_s(strBuffer, "Press 'M' to Return to Main Menu");
		AEGfxGetPrintSize(g_font30, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font30, strBuffer, 0.0f - TextWidth / 2, -0.2f - TextHeight / 2, 1.0f, 1.f, 1.f, 0.f);
	}
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateLevelsFree(void)
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
void GameStateLevelsUnload(void)
{
	// free all mesh data (shapes) of each object using "AEGfxTriFree"
	for (unsigned long i = 0; i < sGameObjNum; i++) {
		GameObj* pObj = sGameObjList + i;
		if (pObj->pMesh != nullptr)
			AEGfxMeshFree(pObj->pMesh);
	}

	FreeMapData(&MapData, &BinaryCollisionArray, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
	AEGfxTextureUnload(stoneTexture);
	stoneTexture = nullptr;
	AEGfxTextureUnload(dirtTexture);
	dirtTexture = nullptr;
}
