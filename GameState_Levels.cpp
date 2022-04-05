/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
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
			tex_stone = AEGfxTextureLoad(".\\Resources\\Assets\\stone.png"); // Load stone texture
			AE_ASSERT_MESG(tex_stone, "Failed to create texture1!!");
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

	//PrintRetrievedInformation(&MapData, &BinaryCollisionArray, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);

	//Computing the matrix which take a point out of the normalized coordinates system
	//of the binary map
	/***********
	Compute a transformation matrix and save it in "MapTransform".
	This transformation transforms any point from the normalized coordinates system of the binary map.
	Later on, when rendering each object instance, we should concatenate "MapTransform" with the
	object instance's own transformation matrix

	Compute a translation matrix (-Grid width/2, -Grid height/2) and save it in "trans"
	Compute a scaling matrix and save it in "scale". The scale must account for the window width and height.
		Alpha engine has 2 helper functions to get the window width and height: AEGetWindowWidth() and AEGetWindowHeight()
	Concatenate scale and translate and save the result in "MapTransform"
	***********/
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
	levelTime = 0;
	totalEnemyCount = 0;
	// Set player's initial health
	playerHealth = PLAYER_INITIAL_HEALTH;
	EmptyInstance = gameObjInstCreate(&sGameObjList[emptyObjIndex], &EMPTY_SCALE, 0, 0, 0.0f, STATE_NONE);
	EmptyInstance->flag ^= FLAG_VISIBLE;
	EmptyInstance->flag |= FLAG_NON_COLLIDABLE;

	PlatformInstance = gameObjInstCreate(&sGameObjList[platformObjIndex], &PLATFORM_SCALE, 0, 0, 0.0f, STATE_NONE);
	PlatformInstance->flag ^= FLAG_VISIBLE;
	PlatformInstance->flag |= FLAG_NON_COLLIDABLE;

	DirtInstance = gameObjInstCreate(&sGameObjList[dirtObjIndex], &PLATFORM_SCALE, 0, 0, 0.0f, STATE_NONE);
	DirtInstance->flag ^= FLAG_VISIBLE;
	DirtInstance->flag |= FLAG_NON_COLLIDABLE;

	AEVec2 Pos{}, Scale{};

	srand(static_cast<unsigned int>(time(NULL))); // Seed random generator

	//PrintRetrievedInformation();

	// Initialise Binary map
	for (int row = 0; row < BINARY_MAP_HEIGHT; row++) {
		for (int col = 0; col < BINARY_MAP_WIDTH; col++) {
			Pos.x = col + 0.5f;
			Pos.y = row + 0.5f;

			switch (GetCellValue(col, row, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT)) {
				//	Boundary walls - Top and Bottom
				//case 1:
				//	platScale = { 150, 50 };
				//	platPos = { AEGfxGetWinMinX() + (col * 120), AEGfxGetWinMaxY() - (row * 120) };
				//	gameObjInstCreate(TYPE_PLATFORM, &platScale, &platPos, nullptr, 0);
				//	break;
				////	Platforms
				//case 2:
				//	platScale = { 150, 25 };
				//	platPos = { AEGfxGetWinMinX() + (col * 120), AEGfxGetWinMaxY() - (row * 120) };
				//	gameObjInstCreate(TYPE_PLATFORM, &platScale, &platPos, nullptr, 0);
				//	break;

			//case TYPE_PLATFORM:
			//	gameObjInstCreate(TYPE_PLATFORM, &PLATFORM_SCALE, &Pos, nullptr, 0.0f, STATE::STATE_NONE);
			//	break;
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
	switch (currInnerState) {
	case GAME_PAUSE:
		if (AEInputCheckReleased(AEVK_M)) {
			gGameStateNext = GS_MAINMENU;
			currInnerState = GAME_PLAY;
		}
		if (AEInputCheckReleased(AEVK_ESCAPE))
			currInnerState = GAME_PLAY;
		break;
	case GAME_WIN:
		gGameStateNext = GS_MAINMENU;
		currInnerState = GAME_PLAY;
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

		if (playerdeathtimer > 0) // post player death control
		{
			playerdeathtimer -= g_dt;
		}
		else if (playerdeathtimer < 0)
		{
			//GameStateLevel1Load();
			//GameStateLevel1Init();
			currInnerState = GAME_PLAY;
			gGameStateNext = GS_RESTART;
			playerdeathtimer = 0;
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


		break;
	}
	case GAME_PLAY:
		// Update level time
		levelTime += g_dt;

		/*if (scriptTimer1 > 0.0f) {
			scriptTimer1 -= g_dt;
		}*/

		// Check win state
		if (totalEnemyCount <= 0) {
			currInnerState = GAME_WIN;
		}
		// Check lose state
		if (playerHealth <= 0.0f || ammoCount <= 0) {
			playerHealth = 0;
			//gameObjInstDestroy(PlayerGun);
			//gameObjInstDestroy(PlayerBody);
			playerdeathtimer = PLAYER_DEATH_ANIME_TIME;
			currInnerState = GAME_LOSE;

			//AEVec2 particleVel;
			//for (double x = PlayerBody->posCurr.x - 1.5; x < PlayerBody->posCurr.x + 1.5; x += ((1.f + rand() % 50) / 100.f))
			//{
			//	AEVec2 particlespawn = { static_cast<float>(x), PlayerBody->posCurr.y };
			//	if (rand() % 2) // randomize polarity of particleVel.x
			//	{
			//		particleVel = { rand() % 20 / -10.0f, rand() % 20 / 10.f };
			//		gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_NONE);
			//	}
			//	else
			//	{
			//		particleVel = { rand() % 20 / 10.f, rand() % 20 / 10.f };
			//		gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_ALERT);
			//	}
			//}
		}

		// =========================
		// update according to input
		// =========================
		// 
		//	if escape key is pressed
		if (AEInputCheckReleased(AEVK_ESCAPE))
			currInnerState = GAME_PAUSE;

		// ----------------------------------------------------------------------------------------------------------------------------------------------
		// Change the following input movement based on our player movement
		// ----------------------------------------------------------------------------------------------------------------------------------------------
		if (AEInputCheckReleased(AEVK_UP)) // DEV TOOL, Delete all bullet on screen.
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

		if (AEInputCheckReleased(AEVK_DOWN))
			totalEnemyCount = 0;

		if (AEInputCheckCurr(AEVK_W) && jumpFuel > 0) // Hold to hover (experimental) 
		{
			AEVec2 added;
			AEVec2Set(&added, 0.f, 1.f);
			//if (AEInputCheckCurr(AEVK_W) && jumpFuel > 0 && playerdeathtimer == 0) //Jump
			//{									
			//	AEVec2 added;
			//	AEVec2Set(&added, 0.f, 1.f);

				// Find the velocity according to the acceleration
			added.x *= 1;//PLAYER_ACCEL_FORWARD * g_dt;
			added.y *= 20 * g_dt; //500

			AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);
			// Limit your speed over here
			AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.99f);
			jumpFuel -= g_dt;
			AEVec2 particleVel = { 0, -1.5f };
			for (double i = PlayerBody->posCurr.x - 0.6; i < PlayerBody->posCurr.x + 0.6; i += ((1.f + rand() % 50) / 100.f))
			{
				AEVec2 particlespawn = { static_cast<float>(i), PlayerBody->posCurr.y - 0.5f };
				if (rand() % 2) {

					gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 0.6f, STATE_NONE); // red color
				}
				else {
					gameObjInstCreate(&sGameObjList[particleObjIndex], &EMPTY_SCALE, &particlespawn, &particleVel, 0.6f, STATE_ALERT); // orangy red color
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
			// Limit your speed over here
			AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f);
		}
		else if (AEInputCheckCurr(AEVK_D))
		{
			AEVec2 added;
			AEVec2Set(&added, cosf(PlayerBody->dirCurr), sinf(PlayerBody->dirCurr));

			// Find the velocity according to the acceleration
			added.x *= MOVE_VELOCITY * g_dt;
			added.y *= MOVE_VELOCITY * g_dt;
			AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);
			// Limit your speed over here
			AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f);
		}

		AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f); \


			// ----------------------------------------------------------------------------------------------------------------------------------------------
			// Change to bullet spawning on mouse click in direction
			// ----------------------------------------------------------------------------------------------------------------------------------------------
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
			// 
			// 
			// Create an instance

			//std::cout << "Gun Pos: (" << PlayerGun->posCurr.x << ", " << PlayerGun->posCurr.y << ") | Direction: " << PlayerGun->dirCurr << std::endl;
			BarrelEnd.x = PlayerGun->posCurr.x + dirBullet.x * 0.11f;
			BarrelEnd.y = PlayerGun->posCurr.y + dirBullet.y * 0.11f;
			gameObjInstCreate(&sGameObjList[bulletObjIndex], &BULLET_SCALE, &BarrelEnd, &dirBullet, PlayerGun->dirCurr, STATE_NONE);
			--ammoCount;
		}

		if (AEInputCheckCurr(VK_RBUTTON)) // TRAJECTORY PREDICTION DOTTED LINE
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

				int collisionFlag = CheckInstanceBinaryMapCollision_dotted(currPos.x, currPos.y, BULLET_MESHSIZE.x * BULLET_SCALE.x, BULLET_MESHSIZE.y * BULLET_SCALE.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);

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

			// skip non-active object
			if (0 == (pInst->flag & FLAG_ACTIVE))
				continue;

			/****************
			Apply gravity
				Velocity Y = Gravity * Frame Time + Velocity Y

			If object instance is an enemy
				Apply enemy state machine
			****************/
			if (pInst->pObject->type == TYPE_BULLET && pInst->bulletbounce >= 3)
				gameObjInstDestroy(pInst);

			if (pInst->pObject->type == TYPE_BULLET && pInst->state == STATE::STATE_GOING_LEFT && pInst->bulletbounce >= 1) // enemy bullet
				gameObjInstDestroy(pInst);

			if (pInst->pObject->type == TYPE_ENEMY1 || pInst->pObject->type == TYPE_PLAYER)
				pInst->velCurr.y += GRAVITY * g_dt;


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

		AEVec2 playerWorldPos{ PlayerBody->posCurr.x, PlayerBody->posCurr.y };
		AEMtx33MultVec(&playerWorldPos, &MapTransform, &playerWorldPos);
		//std::cout << "Player World Pos : (" << playerWorldPos.x << ", " << playerWorldPos.y << ")\n";

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


		/*************
		Update grid collision flag

		if collision from bottom
			Snap to cell on Y axis
			Velocity Y = 0

		if collision from top
			Snap to cell on Y axis
			Velocity Y = 0

		if collision from left
			Snap to cell on X axis
			Velocity X = 0

		if collision from right
			Snap to cell on X axis
			Velocity X = 0
		*************/
		int prevbounce = pInst->bulletbounce;
		if (pInst->pObject->type == TYPE_BULLET)
		{
			pInst->gridCollisionFlag = CheckInstanceBinaryMapCollision_bullet(pInst->posCurr.x, pInst->posCurr.y, pInst->pObject->meshSize.x * pInst->scale.x, pInst->pObject->meshSize.y * pInst->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT, &BinaryCollisionArray);
		}
		else //if(pInst->pObject->type != TYPE_PARTICLE1 && pInst->state != STATE_ALERT)
		{
			pInst->gridCollisionFlag = CheckInstanceBinaryMapCollision(pInst->posCurr.x, pInst->posCurr.y, pInst->pObject->meshSize.x * pInst->scale.x, pInst->pObject->meshSize.y * pInst->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
		}

		if (pInst->pObject->type == TYPE_BULLET && (pInst->gridCollisionFlag & COLLISION_Destructable) == COLLISION_Destructable) // dirt destroy particles
		{
			AEVec2 particlevel{ 0,0 };
			AEVec2 hold;
			AEVec2 particlescale{ 0.85f, 0.85f };
			AEVec2Normalize(&hold, &pInst->velCurr);
			hold.x = hold.x / 6;
			hold.y = hold.y / 6;
			AEVec2 particlepos = { static_cast<float>(static_cast<int>(pInst->posCurr.x + hold.x)),  static_cast<float>(static_cast<int>(pInst->posCurr.y + hold.y)) + 0.65f };
			//std::cout << particlepos.x << " " << particlepos.y << std::endl;
			for (int innerI{}; innerI < 7; ++innerI)
			{
				particlevel.y = static_cast<float>(rand() % 7 + 2) / -3.5f;
				if (rand() % 2) { particlevel.x = static_cast<float>(rand() % 10) / 9.0f; }
				else { particlevel.x = static_cast<float>(rand() % 10) / 9.0f; }

				particlepos.x += 0.13f;
				gameObjInstCreate(&sGameObjList[particleObjIndex], &particlescale, &particlepos, &particlevel, 1.5f, STATE_ALERT);
			}
			gameObjInstDestroy(pInst);
		}

		bool reflectedFlag = false;
		if ((pInst->gridCollisionFlag & COLLISION_LEFT) == COLLISION_LEFT) {
			if (pInst->pObject->type == TYPE_BULLET) {
				if (reflectedFlag == false) {
					AEVec2 normal{ 1, 0 }, newBulletVel{};
					//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
					newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					pInst->velCurr = newBulletVel;
					//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
					//Limit number of bullet bounces:
					//std::cout << pInst->bulletbounce;
					reflectedFlag = true;
					if (prevbounce == pInst->bulletbounce)
						++(pInst->bulletbounce);
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
					//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
					newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					pInst->velCurr = newBulletVel;
					//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
					//std::cout << pInst->bulletbounce;6
					reflectedFlag = true;
					if (prevbounce == pInst->bulletbounce)
						++(pInst->bulletbounce);
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
					//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
					newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					pInst->velCurr = newBulletVel;
					//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
					//Limit number of bullet bounces:
					//std::cout << pInst->bulletbounce;
					reflectedFlag = true;
					if (prevbounce == pInst->bulletbounce)
						++(pInst->bulletbounce);
				}
			}
			else {
				pInst->velCurr.y = 0;
				SnapToCell(&pInst->posCurr.y);
				if (pInst->pObject->type == TYPE_PARTICLE1)
				{
					pInst->posCurr.y -= 0.25f;
				}
				if (pInst->pObject->type == TYPE_PLAYER)
				{
					jumpFuel = 1.5f;
				}
			}
		}

		if ((pInst->gridCollisionFlag & COLLISION_TOP) == COLLISION_TOP) {
			if (pInst->pObject->type == TYPE_BULLET) {
				if (reflectedFlag == false) {
					AEVec2 normal{ 0, -1 }, newBulletVel{};
					//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
					newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
					pInst->velCurr = newBulletVel;

					//Limit number of bullet bounces:
					//std::cout << pInst->bulletbounce;
					if (prevbounce == pInst->bulletbounce)
						++(pInst->bulletbounce);
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
					//case TYPE_PLATFORM:
					//	if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
					//		std::cout << "Collided";
					//		AEVec2 normal{}, newBulletVel{};
					//		if ((pInst->gridCollisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM ||
					//			(pInst->gridCollisionFlag & COLLISION_TOP) == COLLISION_TOP)
					//			normal = { 0, 1 };
					//		else if ((pInst->gridCollisionFlag & COLLISION_LEFT) == COLLISION_LEFT || 
					//			(pInst->gridCollisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT)
					//			normal = { 1, 0 };
					//		//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
					//		newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
					//		newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
					//		pInst->velCurr = newBulletVel;
					//		//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";

					//		// Increment bullet bounces
					//		++(pInst->bulletbounce);
					//	}
					//	break;
				case TYPE_PLAYER:
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) { // player is hit
						if (pInst->state == STATE::STATE_GOING_LEFT || pInst->state == STATE::STATE_NONE)
							playerHealth -= 10.0f;
						gameObjInstDestroy(pInst);
					}
					break;
				case TYPE_ENEMY1:
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						gameObjInstDestroy(pInst);
						gameObjInstDestroy(pOtherInst);
						--totalEnemyCount;
						AEVec2 particleVel;
						for (double x = pOtherInst->posCurr.x - 1.5; x < pOtherInst->posCurr.x + 1.5; x += ((1.f + rand() % 50) / 100.f))
						{
							AEVec2 particlespawn = { static_cast<float>(x), pOtherInst->posCurr.y };
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
						if (totalEnemyCount <= 0 && gGameStateNext != GS_RESTART) // WIN CONDITION, KILL ALL ENEMIES TO WIN LEVEL
						{
							gGameStateNext = GS_MAINMENU; // temporary - go to main menu after level completion.
						}
					}
					break;
				case TYPE_BULLET:
					if (pInst->posCurr.x == pOtherInst->posCurr.x && pInst->posCurr.y == pOtherInst->posCurr.y) // Don't check the same bullet to itself
					{
						break;
					}
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						gameObjInstDestroy(pInst);
						gameObjInstDestroy(pOtherInst);
					}
				}
			}
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
		//if (pInst->pObject->type == TYPE_BULLET || pInst->pObject->type == TYPE_PLATFORM) 
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

	// Update Camera position, for Level2
		// To follow the player's position
		// To clamp the position at the level's borders, between (0,0) and and maximum camera position
			// You may use an alpha engine helper function to clamp the camera position: AEClamp()
	//f32 clampedx = PlayerBody->posCurr.x * BINARY_MAP_WIDTH - AEGetWindowWidth() - 500;
	////clampedx = AEClamp(clampedx, -400, 400); // clamp x axis
	//f32 clampedy = PlayerBody->posCurr.y * static_cast<f32>(BINARY_MAP_HEIGHT) / 2.5f - AEGetWindowHeight();
	////clampedy = AEClamp(clampedy, -200, 270); // clamp y axis
	//AEGfxSetCamPosition(clampedx, clampedy); // set camera to pHero clamped to map

	float cameraX, cameraY;
	AEGfxGetCamPosition(&cameraX, &cameraY);
	//std::cout << "Camera Pos: (" << cameraX << ", " << cameraY << ")\n";

	AEVec2 NewCamPos{ PlayerBody->posCurr.x, PlayerBody->posCurr.y };
	AEMtx33MultVec(&NewCamPos, &MapTransform, &NewCamPos);

	// 128x54
	NewCamPos.x = AEClamp(NewCamPos.x, -(static_cast<float>(AEGetWindowWidth() / static_cast<float>(BINARY_MAP_WIDTH) * 141.0f)), (static_cast<float>(AEGetWindowWidth() / static_cast<float>(BINARY_MAP_WIDTH) * 141.0f)));
	NewCamPos.y = AEClamp(NewCamPos.y, -(static_cast<float>(AEGetWindowHeight()) / static_cast<float>(BINARY_MAP_HEIGHT) * 46.0f), (static_cast<float>(AEGetWindowHeight()) / static_cast<float>(BINARY_MAP_HEIGHT) * 46.0f));
	if (playerdeathtimer == 0)
		AEGfxSetCamPosition(NewCamPos.x, NewCamPos.y);


	//std::cout << "Mouse Window Pos: (" << g_mouseX << ", " << g_mouseY << ")\n";
	//worldMouseX = static_cast<float>(g_mouseX) / (static_cast<float>(AEGetWindowWidth()) / static_cast<float>(40));
	//worldMouseY = (static_cast<float>(AEGetWindowHeight()) - static_cast<float>(g_mouseY)) / (static_cast<float>(AEGetWindowHeight()) / static_cast<float>(20));
	// Mouse in world coordinates
	worldMouseX = cameraX + (static_cast<float>(g_mouseX) - static_cast<float>(AEGetWindowWidth()) / 2);
	worldMouseY = cameraY + (-1) * (static_cast<float>(g_mouseY) - static_cast<float>(AEGetWindowHeight()) / 2);
	//std::cout << "Mouse World Pos: (" << worldMouseX << ", " << worldMouseY << ")\n";

	//AEVec2 playerWorldPos{ PlayerBody->posCurr.x, PlayerBody->posCurr.y };
	//AEMtx33MultVec(&playerWorldPos, &MapTransform, &playerWorldPos);
	////std::cout << "Player World Pos : (" << playerWorldPos.x << ", " << playerWorldPos.y << ")\n";

	//float dotProduct = atan2(worldMouseY - playerWorldPos.y, worldMouseX - playerWorldPos.x);
	//PlayerGun->dirCurr = dotProduct;
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateLevelsDraw(void)
{
	//char strBuffer[1024];

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
	AEGfxSetTransparency(1.0f);

	//Drawing the tile map (the grid)
	AEMtx33 cellTranslation, cellFinalTransformation;

	//Drawing the tile map

	/******REMINDER*****
	You need to concatenate MapTransform with the transformation matrix
	of any object you want to draw. MapTransform transform the instance
	from the normalized coordinates system of the binary map
	*******************/
	/*********
	for each array element in BinaryCollisionArray (2 loops)
		Compute the cell's translation matrix acoording to its
		X and Y coordinates and save it in "cellTranslation"
		Concatenate MapTransform with the cell's transformation
		and save the result in "cellFinalTransformation"
		Send the resultant matrix to the graphics manager using "AEGfxSetTransform"

		Draw the instance's shape depending on the cell's value using "AEGfxMeshDraw"
			Use the black instance in case the cell's value is TYPE_OBJECT_EMPTY
			Use the white instance in case the cell's value is TYPE_OBJECT_COLLISION
	*********/
	for (int i = 0; i < BINARY_MAP_WIDTH; ++i)
		for (int j = 0; j < BINARY_MAP_HEIGHT; ++j)
		{
			AEMtx33Trans(&cellTranslation, static_cast<f32>(AEGetWindowWidth() / BINARY_MAP_WIDTH * i), static_cast<f32>(AEGetWindowHeight() / BINARY_MAP_HEIGHT * j));

			AEMtx33Trans(&cellTranslation, i + 0.5f, j - 0.5f);

			AEMtx33Concat(&cellFinalTransformation, &MapTransform, &cellTranslation);
			AEGfxSetTransform(cellFinalTransformation.m);

			AEGfxSetBlendMode(AE_GFX_BM_NONE);
			AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

			if (GetCellValue(i, j, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == TYPE_PLATFORM)
			{
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxTextureSet(tex_stone, 0.f, 0.f);
				AEGfxMeshDraw(PlatformInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			}
			else if (GetCellValue(i, j - 1, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == TYPE_DIRT) // remove -1 after adding dirt texture
			{
				AEGfxMeshDraw(DirtInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			}
			else
			{
				AEGfxSetRenderMode(AE_GFX_RM_COLOR);
				AEGfxTextureSet(NULL, 0, 0);
				AEGfxMeshDraw(EmptyInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);

	// draw all object instances in the list
	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	{
		GameObjInst* pInst = sGameObjInstList + i;

		// skip non-active object
		if (0 == (pInst->flag & FLAG_ACTIVE) || 0 == (pInst->flag & FLAG_VISIBLE))
			continue;

		//Don't forget to concatenate the MapTransform matrix with the transformation of each game object instance
		//AEMtx33 concatTransform{};
		//AEMtx33Concat(&concatTransform, &MapTransform, &pInst->transform);

		// Set the current object instance's transform matrix using "AEGfxSetTransform"
		//AEGfxSetTransform(concatTransform.m);
		AEMtx33Concat(&cellFinalTransformation, &MapTransform, &pInst->transform);
		AEGfxSetTransform(cellFinalTransformation.m);
		// Draw the shape used by the current object instance using "AEGfxMeshDraw"
		//if (pInst->pObject->type == TYPE_DIRT && pInst->state == STATE_ALERT) // particles for dirt destroy
		//{
		//	std::cout << "HERE";
		//	AEGfxSetTransparency(pInst->dirCurr);
		//	if (pInst->dirCurr <= 0)
		//	{
		//		gameObjInstDestroy(pInst);
		//	}
		//	pInst->dirCurr -= g_dt;
		//}
		if (pInst->pObject->type == TYPE_PARTICLE1) // Particle transparancy handler
		{
			AEGfxSetTransparency(pInst->dirCurr);
			if (pInst->dirCurr <= 0)
			{
				gameObjInstDestroy(pInst);
			}

			if (pInst->state == STATE_ALERT)
			{
				AEGfxSetBlendColor(1.0f, 0.35f, 0.0f, 1.f); //0.5f, 0.1f, 0.f, 1.f alt color
			}
			pInst->dirCurr -= g_dt;
		}
		else if (pInst->pObject->type == TYPE_DOTTED && pInst->state == STATE_GOING_RIGHT)             // uncomment this if want to hide enemy line of sight
		{
			AEGfxSetTransparency(0.f);
		}
		else if (pInst->pObject->type == TYPE_DOTTED && pInst->state == STATE_NONE)
		{
			AEGfxSetTransparency(0.4f);
		}
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxTextureSet(NULL, 0.0f, 0.0f);
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
		AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
		AEGfxSetTransparency(1.f);
		if (pInst->pObject->type == TYPE_DOTTED)
		{
			gameObjInstDestroy(pInst);
		}
	}

	if (playerHealth >= 0.0f) {
		// ----- Draw health bar on screen -----
		// Find offset transform from player's position	
		AEMtx33 scale, translate, transform;
		AEMtx33Scale(&scale, HEALTHBAR_SCALE.x * (playerHealth / PLAYER_INITIAL_HEALTH), HEALTHBAR_SCALE.y);
		// Compute the translation matrix
		AEMtx33Trans(&translate, PlayerBody->posCurr.x - 0.75f, PlayerBody->posCurr.y - 1.0f);
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

	//// ----- Draw ammo count on screen -----
	//// Find offset transform from player's position	
	//AEMtx33 translate, transform;
	//// Compute the translation matrix
	//AEMtx33Trans(&translate, PlayerBody->posCurr.x - 0.75f, PlayerBody->posCurr.y - 1.0f);
	//// Concat with map transform
	//AEMtx33Concat(&transform, &MapTransform, &translate);
	//// Set transform
	//AEGfxSetTransform(transform.m);

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

	sprintf_s(strBuffer, "Enemies Left : %d", totalEnemyCount);
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, 0.8f - TextWidth / 2, 0.7f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	if (currInnerState == GAME_PAUSE) {
		sprintf_s(strBuffer, "GAME PAUSED");
		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font30, strBuffer, 0.0f - TextWidth / 2, 0.4f - TextHeight / 2, 1.0f, 1.f, 1.f, 0.f);

		sprintf_s(strBuffer, "Press 'M' to go back to main menu");
		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
		AEGfxPrint(g_font30, strBuffer, 0.0f - TextWidth / 2, 0.2f - TextHeight / 2, 1.0f, 1.f, 1.f, 0.f);
	}

	// Tutorial text scripts
	//sprintf_s(strBuffer, "Use A & D keys to move left & right!");
	//AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	//AEGfxPrint(g_font20, strBuffer, -0.9f, -0.5f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);


	/*sprintf_s(strBuffer, "D key - Move Right");
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, 0.85f - TextWidth / 2, 0.50f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "W key - Jump Up");
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, 0.70f - TextWidth / 2, 0.40f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Left mouse button - Fire bullet");
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, 0.70f - TextWidth / 2, 0.30f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Use the walls to ricochet your bullets");
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, -0.40f - TextWidth / 2, 0.75f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
	sprintf_s(strBuffer, "to destroy the enemy tanks");
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, -0.40f - TextWidth / 2, 0.65f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Destroy all enemy tanks");
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, -0.40f - TextWidth / 2, -0.15f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "to clear the level");
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	AEGfxPrint(g_font20, strBuffer, -0.40f - TextWidth / 2, -0.25f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);*/

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
	AEGfxTextureUnload(tex_stone);
	tex_stone = nullptr;
}
