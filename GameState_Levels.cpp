/******************************************************************************/
/*!
\file		GameState_Levels.cpp
\author 	DigiPen
\par    	email: digipen\@digipen.edu
\date   	January 01, 20xx
\brief		ToDo: give a brief explanation here

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Main.h"

// texutres
extern AEGfxTexture* tex_stone;

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

	PlayerBody = nullptr;
	PlayerGun = nullptr;
	EmptyInstance = nullptr;
	PlatformInstance = nullptr;
	DirtInstance = nullptr;

	// load/create the mesh data (game objects / Shapes)
	GameObj* pObj;

	// =========================
	// create the non collision shape
	// =========================
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

	// =========================
	// create the platform shape
	// =========================
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

	// =====================
	// create the player shape
	// =====================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_PLAYER;

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
	AE_ASSERT_MESG(pObj->pMesh, "fail to create player object!!");



	// =====================
	// create the player gun shape
	// =====================
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
	/*AEGfxTriAdd(
		-GUN_MESHSIZE.x / 2, -GUN_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f,
		GUN_MESHSIZE.x / 2, -GUN_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f,
		-GUN_MESHSIZE.x / 2, GUN_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f);
	AEGfxTriAdd(
		-GUN_MESHSIZE.x / 2, GUN_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f,
		GUN_MESHSIZE.x / 2, -GUN_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f,
		GUN_MESHSIZE.x / 2, GUN_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f);*/
	pObj->pMesh = AEGfxMeshEnd();
	pObj->meshSize = AEVec2{ GUN_MESHSIZE.x, GUN_MESHSIZE.y };
	AE_ASSERT_MESG(pObj->pMesh, "fail to create player gun object!!");

	// =======================
	// create the bullet shape
	// =======================
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

	// =====================
	// create the Enemy1 shape
	// =====================

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

	// =====================
	// create the Particle1 shape
	// =====================

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

	// =====================
	// create the dotted line shape
	// =====================

	pObj = sGameObjList + sGameObjNum++;
	pObj->type = TYPE_DOTTED;

	AEGfxMeshStart();
	AEGfxTriAdd(
		-PLAYER_MESHSIZE.x / 2, -PLAYER_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f,
		PLAYER_MESHSIZE.x / 2, -PLAYER_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f,
		-PLAYER_MESHSIZE.x / 2, PLAYER_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		PLAYER_MESHSIZE.x / 2, -PLAYER_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f,
		PLAYER_MESHSIZE.x / 2, PLAYER_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f,
		-PLAYER_MESHSIZE.x / 2, PLAYER_MESHSIZE.y / 2, 0xFFFFFFFF, 0.0f, 0.0f);
	pObj->pMesh = AEGfxMeshEnd();
	pObj->meshSize = AEVec2{ PLAYER_MESHSIZE.x, PLAYER_MESHSIZE.y };
	AE_ASSERT_MESG(pObj->pMesh, "fail to create TYPE_DOTTED object!!");

	//Load textures
	tex_stone = AEGfxTextureLoad(".\\Resources\\Assets\\stone.png"); // Load stone texture
	AE_ASSERT_MESG(tex_stone, "Failed to create texture1!!");

	// =========================
	// create the dirt block shape
	// =========================
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

	// =====================
	// Load Level 1 Binary Map
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
	//levelTime = 0;
	//totalEnemyCount = 0;
	//EmptyInstance = gameObjInstCreate(TYPE_EMPTY, &EMPTY_SCALE, 0, 0, 0.0f, STATE_NONE);
	//EmptyInstance->flag ^= FLAG_VISIBLE;
	//EmptyInstance->flag |= FLAG_NON_COLLIDABLE;

	//PlatformInstance = gameObjInstCreate(TYPE_PLATFORM, &PLATFORM_SCALE, 0, 0, 0.0f, STATE_NONE);
	//PlatformInstance->flag ^= FLAG_VISIBLE;
	//PlatformInstance->flag |= FLAG_NON_COLLIDABLE;

	//DirtInstance = gameObjInstCreate(TYPE_DIRT, &PLATFORM_SCALE, 0, 0, 0.0f, STATE_NONE);
	//DirtInstance->flag ^= FLAG_VISIBLE;
	//DirtInstance->flag |= FLAG_NON_COLLIDABLE;

	//AEVec2 Pos{}, Scale{};

	//srand(static_cast<unsigned int>(time(NULL))); // Seed random generator

	////PrintRetrievedInformation();

	//// Initialise Binary map
	//for (int row = 0; row < BINARY_MAP_HEIGHT; row++) {
	//	for (int col = 0; col < BINARY_MAP_WIDTH; col++) {
	//		Pos.x = col + 0.5f;
	//		Pos.y = row + 0.5f;

	//		switch (GetCellValue(col, row, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT)) {
	//			//	Boundary walls - Top and Bottom
	//			//case 1:
	//			//	platScale = { 150, 50 };
	//			//	platPos = { AEGfxGetWinMinX() + (col * 120), AEGfxGetWinMaxY() - (row * 120) };
	//			//	gameObjInstCreate(TYPE_PLATFORM, &platScale, &platPos, nullptr, 0);
	//			//	break;
	//			////	Platforms
	//			//case 2:
	//			//	platScale = { 150, 25 };
	//			//	platPos = { AEGfxGetWinMinX() + (col * 120), AEGfxGetWinMaxY() - (row * 120) };
	//			//	gameObjInstCreate(TYPE_PLATFORM, &platScale, &platPos, nullptr, 0);
	//			//	break;

	//		//case TYPE_PLATFORM:
	//		//	gameObjInstCreate(TYPE_PLATFORM, &PLATFORM_SCALE, &Pos, nullptr, 0.0f, STATE::STATE_NONE);
	//		//	break;
	//		case TYPE_PLAYER:
	//			PlayerBody = gameObjInstCreate(TYPE_PLAYER, &PLAYER_SCALE, &Pos, nullptr, 0.0f, STATE_NONE);
	//			PlayerGun = gameObjInstCreate(TYPE_PLAYERGUN, &GUN_SCALE, &Pos, nullptr, 0.0f, STATE_NONE);
	//			break;
	//		case TYPE_ENEMY1:
	//			gameObjInstCreate(TYPE_ENEMY1, &PLAYER_SCALE, &Pos, nullptr, 0.0f, STATE_GOING_LEFT);
	//			++totalEnemyCount;
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//}


	//// reset the score and the number of lives
	//playerHealth = PLAYER_INITIAL_HEALTH;
}

/******************************************************************************/
/*!
	"Update" function of this state
*/
/******************************************************************************/
void GameStateLevelsUpdate(void)
{
	//// =========================
	//// update according to input
	//// =========================

	//// ----------------------------------------------------------------------------------------------------------------------------------------------
	//// Change the following input movement based on our player movement
	//// ----------------------------------------------------------------------------------------------------------------------------------------------
	//levelTime += g_dt;

	//if (AEInputCheckCurr(AEVK_UP)) // DEV TOOL, Delete all bullet on screen.
	//{
	//	for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++) {
	//		GameObjInst* pInst = sGameObjInstList + i;

	//		// skip non-active object
	//		if ((pInst->flag & FLAG_ACTIVE) == 0)
	//			continue;

	//		if (pInst->pObject->type == TYPE_BULLET)
	//			gameObjInstDestroy(pInst);

	//	}
	//}
	////std::cout << PlayerBody->gridCollisionFlag;
	////if (AEInputCheckTriggered(AEVK_W) && ((PlayerBody->gridCollisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM)) // JUMP - 
	////{									
	////	AEVec2 added;
	////	AEVec2Set(&added, 0.f, 1.f);

	////	// Find the velocity according to the acceleration
	////	added.x *= 1;//PLAYER_ACCEL_FORWARD * g_dt;
	////	added.y *= JUMP_VELOCITY * g_dt; // 29000
	////	AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);
	////	// Limit your speed over here
	////	AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.99f);
	////}

	//if (AEInputCheckCurr(AEVK_W) && jumpFuel > 0) // Hold to hover (experimental) 
	//{
	//	AEVec2 added;
	//	AEVec2Set(&added, 0.f, 1.f);

	//	// Find the velocity according to the acceleration
	//	added.x *= 1;//PLAYER_ACCEL_FORWARD * g_dt;
	//	added.y *= 20 * g_dt; //500

	//	AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);
	//	// Limit your speed over here
	//	AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.99f);
	//	jumpFuel -= g_dt;
	//	AEVec2 particleVel = { 0, -1.5f };
	//	for (double i = PlayerBody->posCurr.x - 0.6; i < PlayerBody->posCurr.x + 0.6; i += ((1.f + rand() % 50) / 100.f))
	//	{
	//		AEVec2 particlespawn = { static_cast<float>(i), PlayerBody->posCurr.y - 0.5f };
	//		if (rand() % 2) {

	//			gameObjInstCreate(TYPE_PARTICLE1, &EMPTY_SCALE, &particlespawn, &particleVel, 0.6f, STATE_NONE); // red color
	//		}
	//		else {
	//			gameObjInstCreate(TYPE_PARTICLE1, &EMPTY_SCALE, &particlespawn, &particleVel, 0.6f, STATE_ALERT); // orangy red color
	//		}
	//	}
	//}

	//if (AEInputCheckCurr(AEVK_A)) // Move left
	//{
	//	AEVec2 added;
	//	AEVec2Set(&added, cosf(PlayerBody->dirCurr), sinf(PlayerBody->dirCurr));

	//	// Find the velocity according to the acceleration
	//	added.x *= -MOVE_VELOCITY * g_dt;
	//	added.y *= -MOVE_VELOCITY * g_dt;
	//	AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);
	//	// Limit your speed over here
	//	AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f);
	//}
	//else if (AEInputCheckCurr(AEVK_D))
	//{
	//	//Player->dirCurr -= PLAYER_ROT_SPEED * (float)(g_dt);
	//	//Player->dirCurr = AEWrap(Player->dirCurr, -PI, PI);

	//	AEVec2 added;
	//	AEVec2Set(&added, cosf(PlayerBody->dirCurr), sinf(PlayerBody->dirCurr));

	//	// Find the velocity according to the acceleration
	//	added.x *= MOVE_VELOCITY * g_dt;
	//	added.y *= MOVE_VELOCITY * g_dt;
	//	AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);
	//	// Limit your speed over here
	//	AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f);
	//}

	//AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f);

	////	if M key is pressed
	//if (AEInputCheckCurr(AEVK_M))
	//	gGameStateNext = GS_MAINMENU;


	//// ----------------------------------------------------------------------------------------------------------------------------------------------
	//// Change to bullet spawning on mouse click in direction
	//// ----------------------------------------------------------------------------------------------------------------------------------------------
	//AEVec2 BarrelEnd;
	//// Shoot a bullet if left mouse button is triggered (Create a new object instance)
	//if (AEInputCheckTriggered(VK_LBUTTON))
	//{
	//	AEVec2 dirBullet;
	//	// Get the bullet's direction according to the player's direction
	//	AEVec2Set(&dirBullet, cosf(PlayerGun->dirCurr), sinf(PlayerGun->dirCurr));
	//	// Set the velocity
	//	dirBullet.x *= BULLET_SPEED;
	//	dirBullet.y *= BULLET_SPEED;
	//	// Calculate end of gun barrel position
	//	// 
	//	// 
	//	// Create an instance

	//	//std::cout << "Gun Pos: (" << PlayerGun->posCurr.x << ", " << PlayerGun->posCurr.y << ") | Direction: " << PlayerGun->dirCurr << std::endl;
	//	BarrelEnd.x = PlayerGun->posCurr.x + dirBullet.x * 0.11f;
	//	BarrelEnd.y = PlayerGun->posCurr.y + dirBullet.y * 0.11f;
	//	gameObjInstCreate(TYPE_BULLET, &BULLET_SCALE, &BarrelEnd, &dirBullet, PlayerGun->dirCurr, STATE_NONE);
	//}

	//if (AEInputCheckCurr(VK_RBUTTON)) // TRAJECTORY PREDICTION DOTTED LINE
	//{
	//	/*AEVec2 dirBullet;
	//	AEVec2Set(&dirBullet, cosf(PlayerGun->dirCurr), sinf(PlayerGun->dirCurr));
	//	AEVec2 offset;
	//	BarrelEnd.x = PlayerGun->posCurr.x + dirBullet.x * 0.15f;
	//	BarrelEnd.y = PlayerGun->posCurr.y + dirBullet.y * 0.15f;
	//	for (int i{ 1 }; i < 25; ++i)
	//	{
	//		offset.x = BarrelEnd.x + dirBullet.x * i;
	//		offset.y = BarrelEnd.y + dirBullet.y * i;
	//		gameObjInstCreate(TYPE_DOTTED, &BULLET_SCALE, &offset, 0, PlayerGun->dirCurr, STATE_GOING_LEFT);
	//	}*/
	//	AEVec2 bulletDir{};
	//	AEVec2Set(&bulletDir, cosf(PlayerGun->dirCurr), sinf(PlayerGun->dirCurr));
	//	BarrelEnd.x = PlayerGun->posCurr.x + bulletDir.x * 0.15f;
	//	BarrelEnd.y = PlayerGun->posCurr.y + bulletDir.y * 0.15f;

	//	AEVec2 currPos{ BarrelEnd };

	//	int bounceCount{ 0 };

	//	for (int i{ 1 }; i < 1000; ++i) {
	//		currPos.x += bulletDir.x * g_dt;
	//		currPos.y += bulletDir.y * g_dt;

	//		int collisionFlag = CheckInstanceBinaryMapCollision_dotted(currPos.x, currPos.y, BULLET_MESHSIZE.x * BULLET_SCALE.x, BULLET_MESHSIZE.y * BULLET_SCALE.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
	//		bool reflectedFlag{ false };
	//		AEVec2 normal{};
	//		if ((collisionFlag & COLLISION_TOP) == COLLISION_TOP && reflectedFlag == false) {
	//			normal = { 0, -1 };
	//			if (++bounceCount < 3)
	//				reflectedFlag = true;
	//			else
	//				break;
	//		}
	//		if ((collisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM && reflectedFlag == false) {
	//			normal = { 0, 1 };
	//			if (++bounceCount < 3)
	//				reflectedFlag = true;
	//			else
	//				break;
	//		}
	//		if ((collisionFlag & COLLISION_LEFT) == COLLISION_LEFT && reflectedFlag == false) {
	//			normal = { 1, 0 };
	//			if (++bounceCount < 3)
	//				reflectedFlag = true;
	//			else
	//				break;
	//		}
	//		if ((collisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT && reflectedFlag == false) {
	//			normal = { -1, 0 };
	//			if (++bounceCount < 3)
	//				reflectedFlag = true;
	//			else
	//				break;
	//		}


	//		if (reflectedFlag) {
	//			AEVec2 newVel{ bulletDir.x - 2 * (AEVec2DotProduct(&bulletDir, &normal)) * normal.x,  bulletDir.y - 2 * (AEVec2DotProduct(&bulletDir, &normal)) * normal.y };
	//			AEVec2Normalize(&newVel, &newVel);
	//			bulletDir = newVel;
	//			currPos.x += bulletDir.x * g_dt;
	//			currPos.y += bulletDir.y * g_dt;
	//		}

	//		if (i % 30 == 0)
	//			gameObjInstCreate(TYPE_DOTTED, &BULLET_SCALE, &currPos, 0, 0, STATE_GOING_LEFT);
	//	}
	//}


	//int i{};
	//GameObjInst* pInst;

	////Update object instances physics and behavior
	//for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	//{
	//	pInst = sGameObjInstList + i;

	//	// skip non-active object
	//	if (0 == (pInst->flag & FLAG_ACTIVE))
	//		continue;

	//	/****************
	//	Apply gravity
	//		Velocity Y = Gravity * Frame Time + Velocity Y

	//	If object instance is an enemy
	//		Apply enemy state machine
	//	****************/
	//	if (pInst == PlayerGun) // attach turret to body
	//		pInst->posCurr = PlayerBody->posCurr;

	//	if (pInst->pObject->type == TYPE_BULLET && pInst->bulletbounce >= 3)
	//		gameObjInstDestroy(pInst);

	//	if (pInst->pObject->type == TYPE_BULLET && pInst->state == STATE::STATE_ALERT && pInst->bulletbounce >= 1) // enemy bullet
	//		gameObjInstDestroy(pInst);

	//	if (pInst->pObject->type == TYPE_ENEMY1 || pInst->pObject->type == TYPE_PLAYER)
	//		pInst->velCurr.y += GRAVITY * g_dt;

	//	if (pInst->pObject->type == TYPE_ENEMY1) {
	//		EnemyStateMachine(pInst);

	//		AEVec2 dist = { PlayerBody->posCurr.x - pInst->posCurr.x, PlayerBody->posCurr.y - pInst->posCurr.y };
	//		AEVec2 offset{};
	//		AEVec2Normalize(&dist, &dist);

	//		AEVec2 shootpos{ pInst->posCurr.x + dist.x * 1.5f, pInst->posCurr.y + dist.y * 1.5f };
	//		AEVec2 bulletvelocity = { dist.x * 4 , dist.y * 4 };
	//		pInst->shoot_timer -= g_dt;
	//		for (int multiply{ 1 }; multiply < 30; ++multiply) // set range of sight here (multiply)
	//		{
	//			offset.x = pInst->posCurr.x + dist.x * multiply * 0.7f;
	//			offset.y = pInst->posCurr.y + dist.y * multiply * 0.7f;
	//			Enemydetection = gameObjInstCreate(TYPE_DOTTED, &BULLET_SCALE, &offset, 0, 0.f, STATE_GOING_RIGHT);
	//			Enemydetection->gridCollisionFlag = CheckInstanceBinaryMapCollision(Enemydetection->posCurr.x, Enemydetection->posCurr.y, Enemydetection->pObject->meshSize.x * Enemydetection->scale.x, Enemydetection->pObject->meshSize.y * Enemydetection->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
	//			if (Enemydetection->gridCollisionFlag > 0)
	//			{
	//				break;
	//			}
	//			else if (CollisionIntersection_RectRect(Enemydetection->boundingBox, Enemydetection->velCurr, PlayerBody->boundingBox, PlayerBody->velCurr))
	//			{
	//				if (pInst->shoot_timer < 0)
	//				{
	//					gameObjInstCreate(TYPE_BULLET, &BULLET_SCALE, &shootpos, &bulletvelocity, pInst->dirCurr, STATE_ALERT); // ALERT STATE FOR ENEMY
	//					pInst->shoot_timer = 2;
	//				}
	//				break;
	//			}
	//		}
	//	}
	//}


	////Update object instances positions
	//for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	//{
	//	pInst = sGameObjInstList + i;

	//	// skip non-active object
	//	if (0 == (pInst->flag & FLAG_ACTIVE))
	//		continue;

	//	/**********
	//	update the position using: P1 = V1*dt + P0
	//	Get the bouding rectangle of every active instance:
	//		boundingRect_min = -BOUNDING_RECT_SIZE * instance->scale + instance->pos
	//		boundingRect_max = BOUNDING_RECT_SIZE * instance->scale + instance->pos
	//	**********/

	//	// ----- Update Position -----
	//	pInst->posCurr.x += pInst->velCurr.x * g_dt;
	//	pInst->posCurr.y += pInst->velCurr.y * g_dt;

	//	// ----- Update Bounding Box -----
	//	pInst->boundingBox.min.x = -(pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
	//	pInst->boundingBox.min.y = -(pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;

	//	pInst->boundingBox.max.x = (pInst->pObject->meshSize.x / 2) * pInst->scale.x + pInst->posCurr.x;
	//	pInst->boundingBox.max.y = (pInst->pObject->meshSize.y / 2) * pInst->scale.y + pInst->posCurr.y;
	//}

	////Check for grid collision
	//for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	//{
	//	pInst = sGameObjInstList + i;

	//	// skip non-active object instances
	//	if (0 == (pInst->flag & FLAG_ACTIVE))
	//		continue;

	//	if (pInst->pObject->type == TYPE_PLAYERGUN || pInst->pObject->type == TYPE_PLATFORM
	//		|| pInst->pObject->type == TYPE_EMPTY) // || pInst->pObject->type == TYPE_ENEMY1GUN)
	//		continue;

	//	if (pInst->pObject->type == TYPE_DOTTED)
	//		continue;

	//	/*************
	//	Update grid collision flag

	//	if collision from bottom
	//		Snap to cell on Y axis
	//		Velocity Y = 0

	//	if collision from top
	//		Snap to cell on Y axis
	//		Velocity Y = 0

	//	if collision from left
	//		Snap to cell on X axis
	//		Velocity X = 0

	//	if collision from right
	//		Snap to cell on X axis
	//		Velocity X = 0
	//	*************/
	////	int prevbounce = pInst->bulletbounce;
	////	pInst->gridCollisionFlag = CheckInstanceBinaryMapCollision(pInst->posCurr.x, pInst->posCurr.y, pInst->pObject->meshSize.x * pInst->scale.x, pInst->pObject->meshSize.y * pInst->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
	////	bool reflectedFlag = false;
	////	if ((pInst->gridCollisionFlag & COLLISION_LEFT) == COLLISION_LEFT) {
	////		if (pInst->pObject->type == TYPE_BULLET) {
	////			if (reflectedFlag == false) {
	////				AEVec2 normal{ 1, 0 }, newBulletVel{};
	////				//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
	////				newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
	////				newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
	////				pInst->velCurr = newBulletVel;
	////				//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
	////				//Limit number of bullet bounces:
	////				//std::cout << pInst->bulletbounce;
	////				reflectedFlag = true;
	////				if (prevbounce == pInst->bulletbounce)
	////					++(pInst->bulletbounce);
	////			}
	////		}
	////		else {
	////			pInst->velCurr.x = 0;
	////			SnapToCell(&pInst->posCurr.x);
	////			pInst->posCurr.x += 0.3f;
	////		}
	////	}

	////	if ((pInst->gridCollisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT) {
	////		if (pInst->pObject->type == TYPE_BULLET) {
	////			if (reflectedFlag == false) {
	////				AEVec2 normal{ -1, 0 }, newBulletVel{};
	////				//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
	////				newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
	////				newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
	////				pInst->velCurr = newBulletVel;
	////				//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
	////				//std::cout << pInst->bulletbounce;6
	////				reflectedFlag = true;
	////				if (prevbounce == pInst->bulletbounce)
	////					++(pInst->bulletbounce);
	////			}
	////		}
	////		else {
	////			pInst->velCurr.x = 0;
	////			SnapToCell(&pInst->posCurr.x);
	////			pInst->posCurr.x -= 0.3f;
	////		}
	////	}

	////	if ((pInst->gridCollisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM) {
	////		if (pInst->pObject->type == TYPE_BULLET) {
	////			if (reflectedFlag == false) {
	////				AEVec2 normal{ 0, 1 }, newBulletVel{};
	////				//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
	////				newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
	////				newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
	////				pInst->velCurr = newBulletVel;
	////				//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
	////				//Limit number of bullet bounces:
	////				//std::cout << pInst->bulletbounce;
	////				reflectedFlag = true;
	////				if (prevbounce == pInst->bulletbounce)
	////					++(pInst->bulletbounce);
	////			}
	////		}
	////		else {
	////			pInst->velCurr.y = 0;
	////			SnapToCell(&pInst->posCurr.y);
	////			if (pInst->pObject->type == TYPE_PLAYER)
	////			{
	////				jumpfuel = 1.5f;
	////			}
	////		}
	////	}

	////	if ((pInst->gridCollisionFlag & COLLISION_TOP) == COLLISION_TOP) {
	////		if (pInst->pObject->type == TYPE_BULLET) {
	////			if (reflectedFlag == false) {
	////				AEVec2 normal{ 0, -1 }, newBulletVel{};
	////				//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
	////				newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
	////				newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
	////				//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
	////				pInst->velCurr = newBulletVel;

	////				//Limit number of bullet bounces:
	////				//std::cout << pInst->bulletbounce;
	////				if (prevbounce == pInst->bulletbounce)
	////					++(pInst->bulletbounce);
	////			}
	////		}
	////		else {
	////			pInst->velCurr.y = 0;
	////			SnapToCell(&pInst->posCurr.y);
	////		}
	////	}


	////}
	//	int prevbounce = pInst->bulletbounce;
	//	if (pInst->pObject->type == TYPE_BULLET)
	//	{
	//		pInst->gridCollisionFlag = CheckInstanceBinaryMapCollision_bullet(pInst->posCurr.x, pInst->posCurr.y, pInst->pObject->meshSize.x * pInst->scale.x, pInst->pObject->meshSize.y * pInst->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT, &BinaryCollisionArray);
	//	}
	//	else //if(pInst->pObject->type != TYPE_PARTICLE1 && pInst->state != STATE_ALERT)
	//	{
	//		pInst->gridCollisionFlag = CheckInstanceBinaryMapCollision(pInst->posCurr.x, pInst->posCurr.y, pInst->pObject->meshSize.x * pInst->scale.x, pInst->pObject->meshSize.y * pInst->scale.y, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
	//	}

	//	if (pInst->pObject->type == TYPE_BULLET && (pInst->gridCollisionFlag & COLLISION_Destructable) == COLLISION_Destructable) // dirt destroy particles
	//	{
	//		AEVec2 particlevel{ 0,0 };
	//		AEVec2 hold;
	//		AEVec2 particlescale{ 0.85f, 0.85f };
	//		AEVec2Normalize(&hold, &pInst->velCurr);
	//		hold.x = hold.x / 6;
	//		hold.y = hold.y / 6;
	//		AEVec2 particlepos = { static_cast<float>(pInst->posCurr.x + hold.x),  static_cast<float>(pInst->posCurr.y + hold.y) + 0.65f };
	//		//std::cout << particlepos.x << " " << particlepos.y << std::endl;
	//		for (int innerI{}; innerI < 7; ++innerI)
	//		{
	//			particlevel.y = (rand() % 7 + 2) / -3.5f;
	//			if (rand() % 2) { particlevel.x = (rand() % 10) / 9; }
	//			else { particlevel.x = (rand() % 10) / 9; }

	//			particlepos.x += 0.13f;
	//			gameObjInstCreate(TYPE_PARTICLE1, &particlescale, &particlepos, &particlevel, 1.5f, STATE_ALERT);
	//		}
	//		gameObjInstDestroy(pInst);
	//	}

	//	bool reflectedFlag = false;
	//	if ((pInst->gridCollisionFlag & COLLISION_LEFT) == COLLISION_LEFT) {
	//		if (pInst->pObject->type == TYPE_BULLET) {
	//			if (reflectedFlag == false) {
	//				AEVec2 normal{ 1, 0 }, newBulletVel{};
	//				//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
	//				newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
	//				newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
	//				pInst->velCurr = newBulletVel;
	//				//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
	//				//Limit number of bullet bounces:
	//				//std::cout << pInst->bulletbounce;
	//				reflectedFlag = true;
	//				if (prevbounce == pInst->bulletbounce)
	//					++(pInst->bulletbounce);
	//			}
	//		}
	//		else {
	//			pInst->velCurr.x = 0;
	//			SnapToCell(&pInst->posCurr.x);
	//			pInst->posCurr.x += 0.3f;
	//		}
	//	}

	//	if ((pInst->gridCollisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT) {
	//		if (pInst->pObject->type == TYPE_BULLET) {
	//			if (reflectedFlag == false) {
	//				AEVec2 normal{ -1, 0 }, newBulletVel{};
	//				//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
	//				newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
	//				newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
	//				pInst->velCurr = newBulletVel;
	//				//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
	//				//std::cout << pInst->bulletbounce;6
	//				reflectedFlag = true;
	//				if (prevbounce == pInst->bulletbounce)
	//					++(pInst->bulletbounce);
	//			}
	//		}
	//		else {
	//			pInst->velCurr.x = 0;
	//			SnapToCell(&pInst->posCurr.x);
	//			pInst->posCurr.x -= 0.3f;
	//		}
	//	}

	//	if ((pInst->gridCollisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM) {
	//		if (pInst->pObject->type == TYPE_BULLET) {
	//			if (reflectedFlag == false) {
	//				AEVec2 normal{ 0, 1 }, newBulletVel{};
	//				//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
	//				newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
	//				newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
	//				pInst->velCurr = newBulletVel;
	//				//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
	//				//Limit number of bullet bounces:
	//				//std::cout << pInst->bulletbounce;
	//				reflectedFlag = true;
	//				if (prevbounce == pInst->bulletbounce)
	//					++(pInst->bulletbounce);
	//			}
	//		}
	//		else {
	//			pInst->velCurr.y = 0;
	//			SnapToCell(&pInst->posCurr.y);
	//			if (pInst->pObject->type == TYPE_PARTICLE1)
	//			{
	//				pInst->posCurr.y -= 0.25f;
	//			}
	//			if (pInst->pObject->type == TYPE_PLAYER)
	//			{
	//				jumpFuel = 1.5f;
	//			}
	//		}
	//	}

	//	if ((pInst->gridCollisionFlag & COLLISION_TOP) == COLLISION_TOP) {
	//		if (pInst->pObject->type == TYPE_BULLET) {
	//			if (reflectedFlag == false) {
	//				AEVec2 normal{ 0, -1 }, newBulletVel{};
	//				//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
	//				newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
	//				newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
	//				//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
	//				pInst->velCurr = newBulletVel;

	//				//Limit number of bullet bounces:
	//				//std::cout << pInst->bulletbounce;
	//				if (prevbounce == pInst->bulletbounce)
	//					++(pInst->bulletbounce);
	//			}
	//		}
	//		else {
	//			pInst->velCurr.y = 0;
	//			SnapToCell(&pInst->posCurr.y);
	//		}
	//	}


	//}

	//// Attach gun to player after grid collision checks
	//PlayerGun->posCurr = PlayerBody->posCurr;

	//// ====================
	//// check for collision
	//// ====================

	//for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	//{
	//	pInst = sGameObjInstList + i;

	//	// skip non-active object
	//	if ((pInst->flag & FLAG_ACTIVE) == 0)
	//		continue;

	//	switch (pInst->pObject->type) {
	//	case TYPE_BULLET:
	//		for (unsigned long j = 0; j < GAME_OBJ_INST_NUM_MAX; j++)
	//		{
	//			GameObjInst* pOtherInst = sGameObjInstList + j;
	//			// skip non-active object
	//			if ((pOtherInst->flag & FLAG_ACTIVE) == 0)
	//				continue;

	//			switch (pOtherInst->pObject->type) {
	//				//case TYPE_PLATFORM:
	//				//	if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
	//				//		std::cout << "Collided";
	//				//		AEVec2 normal{}, newBulletVel{};
	//				//		if ((pInst->gridCollisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM ||
	//				//			(pInst->gridCollisionFlag & COLLISION_TOP) == COLLISION_TOP)
	//				//			normal = { 0, 1 };
	//				//		else if ((pInst->gridCollisionFlag & COLLISION_LEFT) == COLLISION_LEFT || 
	//				//			(pInst->gridCollisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT)
	//				//			normal = { 1, 0 };
	//				//		//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
	//				//		newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
	//				//		newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
	//				//		pInst->velCurr = newBulletVel;
	//				//		//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";

	//				//		// Increment bullet bounces
	//				//		++(pInst->bulletbounce);
	//				//	}
	//				//	break;
	//			case TYPE_PLAYER:
	//				if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
	//					gameObjInstDestroy(pInst);
	//					gameObjInstDestroy(PlayerBody);
	//					gameObjInstDestroy(PlayerGun);
	//					GameStateLevelsLoad();
	//					GameStateLevelsInit();
	//					gGameStateNext = GS_RESTART;
	//				}
	//				break;
	//			case TYPE_ENEMY1:
	//				if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
	//					gameObjInstDestroy(pInst);
	//					gameObjInstDestroy(pOtherInst);
	//					--totalEnemyCount;
	//					AEVec2 particleVel;
	//					for (double x = pOtherInst->posCurr.x - 1.5; x < pOtherInst->posCurr.x + 1.5; x += ((1.f + rand() % 50) / 100.f))
	//					{
	//						AEVec2 particlespawn = { static_cast<float>(x), pOtherInst->posCurr.y };
	//						if (rand() % 2) // randomize polarity of particleVel.x
	//						{
	//							particleVel = { rand() % 20 / -10.f, rand() % 20 / 10.f };
	//							gameObjInstCreate(TYPE_PARTICLE1, &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_NONE);
	//						}
	//						else
	//						{
	//							particleVel = { rand() % 20 / 10.f, rand() % 20 / 10.f };
	//							gameObjInstCreate(TYPE_PARTICLE1, &EMPTY_SCALE, &particlespawn, &particleVel, 1.8f, STATE_ALERT);
	//						}
	//					}
	//					if (totalEnemyCount <= 0 && gGameStateNext != GS_RESTART) // WIN CONDITION, KILL ALL ENEMIES TO WIN LEVEL
	//					{
	//						gGameStateNext = GS_MAINMENU; // temporary - go to main menu after level completion.
	//					}

	//				}
	//				break;
	//			case TYPE_BULLET:
	//				if (pInst->posCurr.x == pOtherInst->posCurr.x && pInst->posCurr.y == pOtherInst->posCurr.y) // Don't check the same bullet to itself
	//				{
	//					break;
	//				}
	//				if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
	//					gameObjInstDestroy(pInst);
	//					gameObjInstDestroy(pOtherInst);
	//				}
	//			}
	//		}
	//		break;
	//	}
	//}


	//// =====================================
	//// calculate the matrix for all objects
	//// =====================================

	//for (i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	//{
	//	pInst = sGameObjInstList + i;
	//	AEMtx33		 trans, rot, scale;

	//	//UNREFERENCED_PARAMETER(trans);
	//	//UNREFERENCED_PARAMETER(rot);
	//	//UNREFERENCED_PARAMETER(scale);

	//	// skip non-active object
	//	if ((pInst->flag & FLAG_ACTIVE) == 0)
	//		continue;

	//	// Compute the scaling matrix
	//	AEMtx33Scale(&scale, pInst->scale.x, pInst->scale.y);
	//	// Compute the rotation matrix 
	//	//if (pInst->pObject->type == TYPE_BULLET || pInst->pObject->type == TYPE_PLATFORM) 
	//	if (pInst->pObject->type == TYPE_BULLET)
	//		AEMtx33Rot(&rot, 0);
	//	else
	//		AEMtx33Rot(&rot, pInst->dirCurr);
	//	// Compute the translation matrix
	//	AEMtx33Trans(&trans, pInst->posCurr.x, pInst->posCurr.y);
	//	// Concatenate the 3 matrix in the correct order in the object instance's "transform" matrix
	//	AEMtx33Concat(&pInst->transform, &trans, &rot);
	//	AEMtx33Concat(&pInst->transform, &pInst->transform, &scale);
	//}

	//// Update Camera position, for Level2
	//	// To follow the player's position
	//	// To clamp the position at the level's borders, between (0,0) and and maximum camera position
	//		// You may use an alpha engine helper function to clamp the camera position: AEClamp()
	////f32 clampedx = PlayerBody->posCurr.x * BINARY_MAP_WIDTH - AEGetWindowWidth() - 500;
	//////clampedx = AEClamp(clampedx, -400, 400); // clamp x axis
	////f32 clampedy = PlayerBody->posCurr.y * static_cast<f32>(BINARY_MAP_HEIGHT) / 2.5f - AEGetWindowHeight();
	//////clampedy = AEClamp(clampedy, -200, 270); // clamp y axis
	////AEGfxSetCamPosition(clampedx, clampedy); // set camera to pHero clamped to map

	//float cameraX, cameraY;
	//AEGfxGetCamPosition(&cameraX, &cameraY);
	//std::cout << "Camera Pos: (" << cameraX << ", " << cameraY << ")\n";

	//AEVec2 NewCamPos{ PlayerBody->posCurr.x, PlayerBody->posCurr.y };
	//AEMtx33MultVec(&NewCamPos, &MapTransform, &NewCamPos);

	//// 128x54
	//NewCamPos.x = AEClamp(NewCamPos.x, -(static_cast<float>(AEGetWindowWidth() / static_cast<float>(BINARY_MAP_WIDTH) * 141.0f)), (static_cast<float>(AEGetWindowWidth() / static_cast<float>(BINARY_MAP_WIDTH) * 141.0f)));
	//NewCamPos.y = AEClamp(NewCamPos.y, -(static_cast<float>(AEGetWindowHeight()) / static_cast<float>(BINARY_MAP_HEIGHT) * 46.0f), (static_cast<float>(AEGetWindowHeight()) / static_cast<float>(BINARY_MAP_HEIGHT) * 46.0f));
	//AEGfxSetCamPosition(NewCamPos.x, NewCamPos.y);


	////std::cout << "Mouse Window Pos: (" << g_mouseX << ", " << g_mouseY << ")\n";
	////worldMouseX = static_cast<float>(g_mouseX) / (static_cast<float>(AEGetWindowWidth()) / static_cast<float>(40));
	////worldMouseY = (static_cast<float>(AEGetWindowHeight()) - static_cast<float>(g_mouseY)) / (static_cast<float>(AEGetWindowHeight()) / static_cast<float>(20));
	//// Mouse in world coordinates
	//worldMouseX = cameraX + (static_cast<float>(g_mouseX) - static_cast<float>(AEGetWindowWidth()) / 2);
	//worldMouseY = cameraY + (-1) * (static_cast<float>(g_mouseY) - static_cast<float>(AEGetWindowHeight()) / 2);
	////std::cout << "Mouse World Pos: (" << worldMouseX << ", " << worldMouseY << ")\n";

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
	////char strBuffer[1024];

	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//AEGfxTextureSet(NULL, 0, 0);

	////Drawing the tile map (the grid)
	//AEMtx33 cellTranslation, cellFinalTransformation;

	////Drawing the tile map

	///******REMINDER*****
	//You need to concatenate MapTransform with the transformation matrix
	//of any object you want to draw. MapTransform transform the instance
	//from the normalized coordinates system of the binary map
	//*******************/
	///*********
	//for each array element in BinaryCollisionArray (2 loops)
	//	Compute the cell's translation matrix acoording to its
	//	X and Y coordinates and save it in "cellTranslation"
	//	Concatenate MapTransform with the cell's transformation
	//	and save the result in "cellFinalTransformation"
	//	Send the resultant matrix to the graphics manager using "AEGfxSetTransform"

	//	Draw the instance's shape depending on the cell's value using "AEGfxMeshDraw"
	//		Use the black instance in case the cell's value is TYPE_OBJECT_EMPTY
	//		Use the white instance in case the cell's value is TYPE_OBJECT_COLLISION
	//*********/
	//for (int i = 0; i < BINARY_MAP_WIDTH; ++i)
	//	for (int j = 0; j < BINARY_MAP_HEIGHT; ++j)
	//	{
	//		AEMtx33Trans(&cellTranslation, static_cast<f32>(AEGetWindowWidth() / BINARY_MAP_WIDTH * i), static_cast<f32>(AEGetWindowHeight() / BINARY_MAP_HEIGHT * j));
	//		AEMtx33Trans(&cellTranslation, i + 0.5f, j + 0.5f);
	//		AEMtx33Concat(&cellFinalTransformation, &MapTransform, &cellTranslation);
	//		AEGfxSetTransform(cellFinalTransformation.m);

	//		AEGfxSetBlendMode(AE_GFX_BM_NONE);
	//		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);

	//		if (GetCellValue(i, j, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == TYPE_PLATFORM)
	//		{
	//			//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//			//AEGfxTextureSet(tex_stone, 0.0f, 0.0f);
	//			AEGfxMeshDraw(PlatformInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	//		}
	//		else if (GetCellValue(i, j, &MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == TYPE_DIRT)
	//		{
	//			AEGfxMeshDraw(DirtInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	//		}
	//		else
	//		{
	//			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//			AEGfxTextureSet(NULL, 0, 0);
	//			AEGfxMeshDraw(EmptyInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	//		}
	//	}
	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//AEGfxTextureSet(NULL, 0, 0);

	//// draw all object instances in the list
	//for (unsigned long i = 0; i < GAME_OBJ_INST_NUM_MAX; i++)
	//{
	//	GameObjInst* pInst = sGameObjInstList + i;

	//	// skip non-active object
	//	if (0 == (pInst->flag & FLAG_ACTIVE) || 0 == (pInst->flag & FLAG_VISIBLE))
	//		continue;

	//	//Don't forget to concatenate the MapTransform matrix with the transformation of each game object instance
	//	//AEMtx33 concatTransform{};
	//	//AEMtx33Concat(&concatTransform, &MapTransform, &pInst->transform);

	//	// Set the current object instance's transform matrix using "AEGfxSetTransform"
	//	//AEGfxSetTransform(concatTransform.m);
	//	AEMtx33Concat(&cellFinalTransformation, &MapTransform, &pInst->transform);
	//	AEGfxSetTransform(cellFinalTransformation.m);
	//	// Draw the shape used by the current object instance using "AEGfxMeshDraw"
	//	if (pInst->pObject->type == TYPE_PARTICLE1) // Particle transparancy handler
	//	{
	//		AEGfxSetTransparency(pInst->dirCurr);
	//		if (pInst->dirCurr <= 0)
	//		{
	//			gameObjInstDestroy(pInst);
	//		}

	//		if (pInst->state == STATE_ALERT)
	//		{
	//			AEGfxSetBlendColor(1.0f, 0.35f, 0.0f, 1.f); //0.5f, 0.1f, 0.f, 1.f alt color
	//		}
	//		pInst->dirCurr -= g_dt;
	//	}
	//	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//	AEGfxTextureSet(NULL, 0.0f, 0.0f);
	//	AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	//	AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
	//	AEGfxSetTransparency(1.f);
	//	if (pInst->pObject->type == TYPE_DOTTED)
	//	{
	//		gameObjInstDestroy(pInst);
	//	}
	//}

	////	Drawing for Font for all states
	//f32 TextWidth = 1.0f;
	//f32 TextHeight = 1.0f;
	//char strBuffer[100];
	//memset(strBuffer, 0, 100 * sizeof(char));

	///*case 2:
	//		sprintf_s(strBuffer, "Current Time : %.2f", leveltime);
	//		AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	//		AEGfxPrint(g_font20, strBuffer, 0.8f - TextWidth / 2, 0.9f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
	//		break;*/

	//		//	Remove font after x time
	//if (levelTime < 20.0f)
	//{
	//	sprintf_s(strBuffer, "A key - Move Left");
	//	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	//	AEGfxPrint(g_font20, strBuffer, 0.55f - TextWidth / 2, -0.70f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	//	sprintf_s(strBuffer, "D key - Move Right");
	//	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	//	AEGfxPrint(g_font20, strBuffer, 0.85f - TextWidth / 2, -0.70f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	//	sprintf_s(strBuffer, "W key - Jump Up");
	//	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	//	AEGfxPrint(g_font20, strBuffer, 0.70f - TextWidth / 2, -0.80f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	//	sprintf_s(strBuffer, "Left mouse button - Fire bullet");
	//	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	//	AEGfxPrint(g_font20, strBuffer, 0.70f - TextWidth / 2, -0.90f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	//	sprintf_s(strBuffer, "Use the walls to ricochet your bullets");
	//	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	//	AEGfxPrint(g_font20, strBuffer, -0.10f - TextWidth / 2, 0.95f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	//	sprintf_s(strBuffer, "to destroy the enemy tanks");
	//	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	//	AEGfxPrint(g_font20, strBuffer, -0.10f - TextWidth / 2, 0.85f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	//	sprintf_s(strBuffer, "Destroy all enemy tanks");
	//	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	//	AEGfxPrint(g_font20, strBuffer, -0.80f - TextWidth / 2, 0.20f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);

	//	sprintf_s(strBuffer, "to clear the level");
	//	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	//	AEGfxPrint(g_font20, strBuffer, -0.80f - TextWidth / 2, 0.10f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
	//}

	//sprintf_s(strBuffer, "Current Time : %.2f", levelTime);
	//AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);
	//AEGfxPrint(g_font20, strBuffer, 0.8f - TextWidth / 2, 0.9f - TextHeight / 2, 1.0f, 1.f, 1.f, 1.f);
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
		AEGfxMeshFree(pObj->pMesh);
	}

	FreeMapData(&MapData, &BinaryCollisionArray, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT);
	AEGfxTextureUnload(tex_stone);
}
