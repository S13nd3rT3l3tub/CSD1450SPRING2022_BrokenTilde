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

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
const unsigned int	GAME_OBJ_NUM_MAX		= 32;			//The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_INST_NUM_MAX	= 2048;			//The total number of different game object instances


const unsigned int	PLAYER_INITIAL_NUM		= 100;			// initial number of player lives
AEVec2		PLAYER_MESHSIZE			= { 1.0f, 1.0f };
AEVec2		PLAYER_SCALE			= { 2.0f, 1.0f};		// player scaling
AEVec2		GUN_MESHSIZE			= { 0.5f, 0.5f };
AEVec2		GUN_SCALE				= { 3.0f, 0.7f };		// gun size

AEVec2		BULLET_MESHSIZE			= { 1.0f, 1.0f };
AEVec2		BULLET_SCALE			= { 0.25f, 0.25f };
const float	BULLET_SPEED			= 10.0f;		// bullet speed (m/s)

AEVec2		PLATFORM_MESHSIZE = { 1.0f, 1.0f };
AEVec2		PLATFORM_SCALE			= { 1.0f, 1.0f };
AEVec2		EMPTY_MESHSIZE =		{ 1.0f, 1.0f };
AEVec2		EMPTY_SCALE				= { 1.0f, 1.0f };

//Gameplay related variables and values
const float			GRAVITY = -9.8f;
const float			JUMP_VELOCITY = 800.0f;
const float			HOVER_VELOCITY = 7.0f;
const float			MOVE_VELOCITY = 7.0f;
const float			MOVE_VELOCITY_ENEMY = 2.5f;
const double		ENEMY_IDLE_TIME = 2.0;
const int			HERO_LIVES = 3;


// -----------------------------------------------------------------------------
// object flag definition
const unsigned int	FLAG_ACTIVE = 0x00000001;
const unsigned int	FLAG_VISIBLE = 0x00000002;
const unsigned int	FLAG_NON_COLLIDABLE = 0x00000004;

// Collision flags
const unsigned int	COLLISION_LEFT = 0x00000001;	//0001
const unsigned int	COLLISION_RIGHT = 0x00000002;	//0010
const unsigned int	COLLISION_TOP = 0x00000004;	//0100
const unsigned int	COLLISION_BOTTOM = 0x00000008;	//1000

/******************************************************************************/
/*!
	Enums/Struct/Class Definitions
*/
/******************************************************************************/

enum TYPE
{
	// list of game object types
	TYPE_EMPTY = 0,
	TYPE_PLATFORM,
	TYPE_PLAYER,
	TYPE_PLAYERGUN,
	TYPE_BULLET,
	TYPE_ENEMY1,
	TYPE_NUM
};

//State machine states
enum STATE
{
	STATE_NONE,
	STATE_GOING_LEFT,
	STATE_GOING_RIGHT,
	STATE_ALERT
};

//State machine inner states
enum INNER_STATE
{
	INNER_STATE_ON_ENTER,
	INNER_STATE_ON_UPDATE,
	INNER_STATE_ON_EXIT
};

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
	int					bulletbounce;

	//Used to hold the current 
	int					gridCollisionFlag;

	// pointer to custom data specific for each object type
	void* pUserData;

	//State of the object instance
	enum			STATE state;
	enum			INNER_STATE innerState;

	//General purpose counter (This variable will be used for the enemy state machine)
	double			counter;
	double			shoot_timer{};
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

static GameObjInst* EmptyInstance;
static GameObjInst* PlatformInstance;

// number of player lives available (lives 0 = game over)
static long					playerLives;									// The number of lives left

// Current mouse position
//static signed int mouseX{ 0 }, mouseY{ 0 };
static float localMouseX{ 0 }, localMouseY{ 0 };

//Binary map data
static int** MapData;
static int** BinaryCollisionArray;
static int				BINARY_MAP_WIDTH;
static int				BINARY_MAP_HEIGHT;

// Transform matrix containing shift of grid to world coordinates
// concatenate this with object instance's own transform matrix
static AEMtx33			MapTransform;


// ---------------------------------------------------------------------------
// functions to create/destroy a game object instance
static GameObjInst* gameObjInstCreate(unsigned int type, AEVec2* scale,
	AEVec2* pPos, AEVec2* pVel,
	float dir, enum STATE startState);
void					gameObjInstDestroy(GameObjInst* pInst);
void EnemyStateMachine(GameObjInst* pInst);
// functions to process map data and binary collision data
int						GetCellValue(int X, int Y);
int						CheckInstanceBinaryMapCollision(float PosX, float PosY,
	float scaleX, float scaleY);
void					SnapToCell(float* Coordinate);
int						ImportMapDataFromFile(std::string FileName);
void					FreeMapData(void);
void					PrintRetrievedInformation(void);

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
	PlayerGun = nullptr;
	EmptyInstance = nullptr;
	PlatformInstance = nullptr;

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
		-PLATFORM_MESHSIZE.x / 2, -PLATFORM_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f,
		PLATFORM_MESHSIZE.x / 2, -PLATFORM_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f,
		-PLATFORM_MESHSIZE.x / 2, PLATFORM_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f);

	AEGfxTriAdd(
		-PLATFORM_MESHSIZE.x / 2, PLATFORM_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f,
		PLATFORM_MESHSIZE.x / 2, -PLATFORM_MESHSIZE.y / 2, 0xFF4D5853, 0.0f, 0.0f,
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
	// Load Level 1 Binary Map
	// =====================
	//	Import Level data from txt file depending on chosen level
	std::string fileName{ "" };
	switch (g_chosenLevel) {
	case 1:
		fileName = "Level1.txt";
		break;
	default:break;
	}
	if (ImportMapDataFromFile(fileName) == 0)
		gGameStateNext = GS_QUIT;

	PrintRetrievedInformation();
	
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
	AEMtx33Scale(&scale, static_cast<f32>(AEGetWindowWidth() / BINARY_MAP_WIDTH), static_cast<f32>(AEGetWindowHeight() / BINARY_MAP_HEIGHT));
	AEMtx33Concat(&MapTransform, &scale, &trans);

}

/******************************************************************************/
/*!
	"Initialize" function of this state
*/
/******************************************************************************/
void GameStateLevel1Init(void)
{
	EmptyInstance = gameObjInstCreate(TYPE_EMPTY, &EMPTY_SCALE, 0, 0, 0.0f, STATE_NONE);
	EmptyInstance->flag ^= FLAG_VISIBLE;
	EmptyInstance->flag |= FLAG_NON_COLLIDABLE;

	PlatformInstance = gameObjInstCreate(TYPE_PLATFORM, &PLATFORM_SCALE, 0, 0, 0.0f, STATE_NONE);
	PlatformInstance->flag ^= FLAG_VISIBLE;
	PlatformInstance->flag |= FLAG_NON_COLLIDABLE;

	AEVec2 Pos{}, Scale{};

	//PrintRetrievedInformation();

	// Initialise Binary map
	for (int row = 0; row < BINARY_MAP_HEIGHT; row++) {
		for (int col = 0; col < BINARY_MAP_WIDTH; col++) {
			Pos.x = col + 0.5f;
			Pos.y = row + 0.5f;

			switch(GetCellValue(col, row)) {
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

			case TYPE_PLATFORM:
				gameObjInstCreate(TYPE_PLATFORM, &PLATFORM_SCALE, &Pos, nullptr, 0.0f, STATE::STATE_NONE);
				break;
			case TYPE_PLAYER:
				PlayerBody = gameObjInstCreate(TYPE_PLAYER, &PLAYER_SCALE, &Pos, nullptr, 0.0f, STATE_NONE);
				PlayerGun = gameObjInstCreate(TYPE_PLAYERGUN, &GUN_SCALE, &Pos, nullptr, 0.0f, STATE_NONE);
				break;
			case TYPE_ENEMY1:
				gameObjInstCreate(TYPE_ENEMY1, &PLAYER_SCALE, &Pos, nullptr, 0.0f, STATE_GOING_LEFT);
				break;
			default:
				break;
			}
		}
	}

	// reset the score and the number of lives
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
	//AEInputGetCursorPosition(&mouseX, &mouseY);

	localMouseX = static_cast<float>(g_mouseX) / (static_cast<float>(AEGetWindowWidth()) / static_cast<float>(BINARY_MAP_WIDTH));
	localMouseY = (static_cast<float>(AEGetWindowHeight()) - static_cast<float>(g_mouseY)) / (static_cast<float>(AEGetWindowHeight()) / static_cast<float>(BINARY_MAP_HEIGHT));
	//std::cout << "Mouse Pos: (" << windowMouse.x << ", " << windowMouse.y << ")\n";
	float dotProduct = atan2(localMouseY - PlayerBody->posCurr.y, localMouseX - PlayerBody->posCurr.x);
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
	//std::cout << PlayerBody->gridCollisionFlag;
	if (AEInputCheckTriggered(AEVK_W) && ((PlayerBody->gridCollisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM)) // JUMP - 
	{									
		AEVec2 added;
		AEVec2Set(&added, 0.f, 1.f);

		// Find the velocity according to the acceleration
		added.x *= 1;//PLAYER_ACCEL_FORWARD * g_dt;
		added.y *= JUMP_VELOCITY * g_dt; // 29000
		AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);
		// Limit your speed over here
		AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.99f);
	}

	if (AEInputCheckCurr(AEVK_W)) // Hold to hover (experimental) 
	{									
		AEVec2 added;
		AEVec2Set(&added, 0.f, 1.f);

		// Find the velocity according to the acceleration
		added.x *= 1;//PLAYER_ACCEL_FORWARD * g_dt;
		added.y *= HOVER_VELOCITY * g_dt; //500
		AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);
		// Limit your speed over here
		AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.99f);
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
		//Player->dirCurr -= PLAYER_ROT_SPEED * (float)(g_dt);
		//Player->dirCurr = AEWrap(Player->dirCurr, -PI, PI);

		AEVec2 added;
		AEVec2Set(&added, cosf(PlayerBody->dirCurr), sinf(PlayerBody->dirCurr));

		// Find the velocity according to the acceleration
		added.x *= MOVE_VELOCITY * g_dt;
		added.y *= MOVE_VELOCITY * g_dt;
		AEVec2Add(&PlayerBody->velCurr, &PlayerBody->velCurr, &added);
		// Limit your speed over here
		AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f);
	}
	
	AEVec2Scale(&PlayerBody->velCurr, &PlayerBody->velCurr, 0.98f);
		

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
		AEVec2 BarrelEnd;
		//std::cout << "Gun Pos: (" << PlayerGun->posCurr.x << ", " << PlayerGun->posCurr.y << ") | Direction: " << PlayerGun->dirCurr << std::endl;
		BarrelEnd.x = PlayerGun->posCurr.x + dirBullet.x*0.15f;
		BarrelEnd.y = PlayerGun->posCurr.y + dirBullet.y*0.15f;
		gameObjInstCreate(TYPE_BULLET, &BULLET_SCALE, &BarrelEnd, &dirBullet, PlayerGun->dirCurr, STATE_NONE);
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
		if (pInst == PlayerGun) // attach turret to body
			pInst->posCurr = PlayerBody->posCurr;

		if (pInst->pObject->type == TYPE_BULLET && pInst->bulletbounce >= 3)
			gameObjInstDestroy(pInst);
	
		if (pInst->pObject->type == TYPE_BULLET && pInst->state == STATE_ALERT && pInst->bulletbounce >= 1) // enemy bullet
			gameObjInstDestroy(pInst);
		
		if (pInst->pObject->type == TYPE_ENEMY1 || pInst->pObject->type == TYPE_PLAYER)
			pInst->velCurr.y += GRAVITY * g_dt;

		if (pInst->pObject->type == TYPE_ENEMY1){
			EnemyStateMachine(pInst);
			if (pInst->posCurr.y <= PlayerBody->posCurr.y)
			{
				pInst->shoot_timer -= AEFrameRateControllerGetFrameTime();
				AEVec2 EnemytoPlayer{ pInst->posCurr.x - PlayerBody->posCurr.x, pInst->posCurr.y - PlayerBody->posCurr.y };
				AEVec2Normalize(&EnemytoPlayer, &EnemytoPlayer);
				EnemytoPlayer.x *= -1.5;
				EnemytoPlayer.y *= -1.5;
				AEVec2 shootpos{ pInst->posCurr.x + EnemytoPlayer.x, pInst->posCurr.y + EnemytoPlayer.y};
				EnemytoPlayer.x *= 4;
				EnemytoPlayer.y *= 4;
				if (pInst->shoot_timer < 0)
				{
					gameObjInstCreate(TYPE_BULLET, &BULLET_SCALE, &shootpos, &EnemytoPlayer, pInst->dirCurr, STATE_ALERT); // ALERT STATE FOR ENEMY
					pInst->shoot_timer = 2;
				}
			}
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

	//Check for grid collision
	for (i = 0; i < GAME_OBJ_INST_NUM_MAX; ++i)
	{
		pInst = sGameObjInstList + i;

		// skip non-active object instances
		if (0 == (pInst->flag & FLAG_ACTIVE))
			continue;

		if (pInst == PlayerGun || pInst == PlatformInstance || pInst == EmptyInstance)
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
		pInst->gridCollisionFlag = CheckInstanceBinaryMapCollision(pInst->posCurr.x, pInst->posCurr.y, pInst->pObject->meshSize.x * pInst->scale.x, pInst->pObject->meshSize.y * pInst->scale.y);
		if ((pInst->gridCollisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM) {
			if (pInst->pObject->type == TYPE_BULLET) {
				AEVec2 normal{ 0, 1 }, newBulletVel{};
				//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
				newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
				newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
				pInst->velCurr = newBulletVel;
				//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
				//Limit number of bullet bounces:
				//std::cout << pInst->bulletbounce;
				if(prevbounce == pInst->bulletbounce)
				++(pInst->bulletbounce);
			}
			else {
				pInst->velCurr.y = 0;
				SnapToCell(&pInst->posCurr.y);
			}
		}

		if ((pInst->gridCollisionFlag & COLLISION_TOP) == COLLISION_TOP) {
			if (pInst->pObject->type == TYPE_BULLET) {
				AEVec2 normal{ 0, 1 }, newBulletVel{};
				//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
				newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
				newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
				pInst->velCurr = newBulletVel;
				//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
				//Limit number of bullet bounces:
				//std::cout << pInst->bulletbounce;
				if (prevbounce == pInst->bulletbounce)
				++(pInst->bulletbounce);
			}
			else {
				pInst->velCurr.y = 0;
				SnapToCell(&pInst->posCurr.y);
			}
		}

		if ((pInst->gridCollisionFlag & COLLISION_LEFT) == COLLISION_LEFT) {
			if (pInst->pObject->type == TYPE_BULLET) {
				AEVec2 normal{ 1, 0 }, newBulletVel{};
				//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
				newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
				newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
				pInst->velCurr = newBulletVel;
				//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
				//Limit number of bullet bounces:
				//std::cout << pInst->bulletbounce;
				if (prevbounce == pInst->bulletbounce)
				++(pInst->bulletbounce);
			}
			else {
				pInst->velCurr.x = 0;
				SnapToCell(&pInst->posCurr.x);
				//pInst->posCurr.x += 0.5f;
			}
		}

		if ((pInst->gridCollisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT) {
			if (pInst->pObject->type == TYPE_BULLET) {
				AEVec2 normal{ 1, 0 }, newBulletVel{};
				//std::cout << "Old vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << " | ";
				newBulletVel.x = pInst->velCurr.x - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.x;
				newBulletVel.y = pInst->velCurr.y - 2 * (AEVec2DotProduct(&pInst->velCurr, &normal)) * normal.y;
				pInst->velCurr = newBulletVel;
				//std::cout << "New vector: " << pInst->velCurr.x << ", " << pInst->velCurr.y << "\n";
				//std::cout << pInst->bulletbounce;
				if (prevbounce == pInst->bulletbounce)
				++(pInst->bulletbounce);
			}
			else {
				pInst->velCurr.x = 0;
				SnapToCell(&pInst->posCurr.x);
				//pInst->posCurr.x -= 0.5f;
			}
		}		
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

		switch (pInst->pObject->type){
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
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						gameObjInstDestroy(pInst);
						gameObjInstDestroy(PlayerBody);
						gameObjInstDestroy(PlayerGun);
						gGameStateNext = GS_RESTART;
					}
					break;
				case TYPE_ENEMY1:
					if (CollisionIntersection_RectRect(pInst->boundingBox, pInst->velCurr, pOtherInst->boundingBox, pOtherInst->velCurr)) {
						gameObjInstDestroy(pInst);
						gameObjInstDestroy(pOtherInst);
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
	// Do we need?
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
void GameStateLevel1Draw(void)
{
	//char strBuffer[1024];

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);

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
	//for (int i = 0; i < BINARY_MAP_WIDTH; ++i)
	//	for (int j = 0; j < BINARY_MAP_HEIGHT; ++j)
	//	{
	//		//AEMtx33Trans(&cellTranslation, static_cast<f32>(AEGetWindowWidth() / BINARY_MAP_WIDTH * i), static_cast<f32>(AEGetWindowHeight() / BINARY_MAP_HEIGHT * j));
	//		AEMtx33Trans(&cellTranslation, i + 0.5f, j + 0.5f);
	//		AEMtx33Concat(&cellFinalTransformation, &MapTransform, &cellTranslation);
	//		AEGfxSetTransform(cellFinalTransformation.m);

	//		if (GetCellValue(i, j) == TYPE_PLATFORM)
	//			AEGfxMeshDraw(PlatformInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	//		else
	//			AEGfxMeshDraw(EmptyInstance->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	//	}

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
		AEGfxMeshDraw(pInst->pObject->pMesh, AE_GFX_MDM_TRIANGLES);
	}

	//	Drawing for Font Level 1
	f32 TextWidth = 1.0f;
	f32 TextHeight = 1.0f;
	char strBuffer[100];
	memset(strBuffer, 0, 100 * sizeof(char));
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxGetPrintSize(g_font20, strBuffer, 1.0f, TextWidth, TextHeight);

	sprintf_s(strBuffer, "A key - Move Left");
	AEGfxPrint(g_font20, strBuffer, -0.85f, 0.15f, 1.0f, 1.f, 1.f, 1.f);
	sprintf_s(strBuffer, "D key - Move Right");
	AEGfxPrint(g_font20, strBuffer, -0.85f, 0.05f, 1.0f, 1.f, 1.f, 1.f);
	sprintf_s(strBuffer, "Spacebar key - Jump Up");
	AEGfxPrint(g_font20, strBuffer, -0.85f, -0.05f, 1.0f, 1.f, 1.f, 1.f);
	sprintf_s(strBuffer, "Left mouse button - Fire bullet");
	AEGfxPrint(g_font20, strBuffer, -0.85f, -0.15f, 1.0f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Use the walls to ricochet your bullets");
	AEGfxPrint(g_font20, strBuffer, -0.26f, 0.7f, 1.0f, 1.f, 1.f, 1.f);
	sprintf_s(strBuffer, "   to destroy the enemy tanks above   ");
	AEGfxPrint(g_font20, strBuffer, -0.26f, 0.6f, 1.0f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Destroy all enemy tanks");
	AEGfxPrint(g_font20, strBuffer, 0.5f, -0.5f, 1.0f, 1.f, 1.f, 1.f);
	sprintf_s(strBuffer, "   to clear the level  ");
	AEGfxPrint(g_font20, strBuffer, 0.5f, -0.6f, 1.0f, 1.f, 1.f, 1.f);
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

	FreeMapData();
}

/******************************************************************************/
/*!

*/
/******************************************************************************/
GameObjInst* gameObjInstCreate(unsigned int type, AEVec2* scale,
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
			pInst->pUserData = 0;
			pInst->gridCollisionFlag = 0;
			pInst->state = startState;
			pInst->innerState = INNER_STATE_ON_ENTER;
			pInst->counter = 0;
			pInst->bulletbounce = 0;

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
	/***********
	This state machine has 2 states: STATE_GOING_LEFT and STATE_GOING_RIGHT
	Each state has 3 inner states: INNER_STATE_ON_ENTER, INNER_STATE_ON_UPDATE, INNER_STATE_ON_EXIT
	Use "switch" statements to determine which state and inner state the enemy is currently in.


	STATE_GOING_LEFT
		INNER_STATE_ON_ENTER
			Set velocity X to -MOVE_VELOCITY_ENEMY
			Set inner state to "on update"

		INNER_STATE_ON_UPDATE
			If collision on left side OR bottom left cell is non collidable
				Initialize the counter to ENEMY_IDLE_TIME
				Set inner state to on exit
				Set velocity X to 0


		INNER_STATE_ON_EXIT
			Decrement counter by frame time
			if counter is less than 0 (sprite's idle time is over)
				Set state to "going right"
				Set inner state to "on enter"

	STATE_GOING_RIGHT is basically the same, with few modifications.

	***********/

	//std::cout << pInst->innerState << std::endl;
	int offsetcheck{};
	//std::cout << pInst->velCurr.x << std::endl;
	//std::cout << (pInst->gridCollisionFlag & COLLISION_RIGHT) << std::endl;
	switch (pInst->state) {

	case STATE_GOING_LEFT:

		switch (pInst->innerState) {
		case INNER_STATE_ON_ENTER:
			//std::cout << "GOING LEFT :INNER_STATE_ON_ENTER\n";
			pInst->velCurr.x = -MOVE_VELOCITY_ENEMY;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			break;

		case INNER_STATE_ON_UPDATE:
			//offsetcheck = CheckInstanceBinaryMapCollision(pInst->posCurr.x - 2, pInst->posCurr.y - 1, 2.f, 1.f);
			//std::cout << "GOING LEFT : INNER_STATE_ON_UPDATE\n";
			pInst->velCurr.x = -MOVE_VELOCITY_ENEMY;
			//if ((pInst->gridCollisionFlag & COLLISION_LEFT) == COLLISION_LEFT || (offsetcheck & COLLISION_RIGHT) != COLLISION_RIGHT)
			if ( (CheckInstanceBinaryMapCollision(pInst->posCurr.x - pInst->pObject->meshSize.x * pInst->scale.x, 
												pInst->posCurr.y, 
												pInst->pObject->meshSize.x * pInst->scale.x, 
												pInst->pObject->meshSize.y * pInst->scale.y) & COLLISION_LEFT) == COLLISION_LEFT || 
				(CheckInstanceBinaryMapCollision(pInst->posCurr.x - pInst->pObject->meshSize.x * pInst->scale.x,
					pInst->posCurr.y - pInst->pObject->meshSize.y * pInst->scale.y,
					pInst->pObject->meshSize.x * pInst->scale.x,
					pInst->pObject->meshSize.y * pInst->scale.y) & COLLISION_RIGHT) != COLLISION_RIGHT )
			{
				pInst->counter = ENEMY_IDLE_TIME;
				pInst->innerState = INNER_STATE_ON_EXIT;
				pInst->velCurr.x = 0;
			}

			break;


		case INNER_STATE_ON_EXIT:
			//std::cout << "GOING LEFT : INNER_STATE_ON_EXIT\n";
			pInst->counter -= AEFrameRateControllerGetFrameTime();
			if (pInst->counter < 0)
			{
				pInst->state = STATE_GOING_RIGHT;
				pInst->innerState = INNER_STATE_ON_ENTER;
			}
			break;
		}
		break;
	case STATE_GOING_RIGHT:

		switch (pInst->innerState) {

		case INNER_STATE_ON_ENTER:
			pInst->velCurr.x = MOVE_VELOCITY_ENEMY;
			pInst->innerState = INNER_STATE_ON_UPDATE;
			//std::cout << "GOING RIGHT : INNER_STATE_ON_UPDATE\n";
			break;

		case INNER_STATE_ON_UPDATE:
			//offsetcheck = CheckInstanceBinaryMapCollision(pInst->posCurr.x + 2, pInst->posCurr.y - 1, 2.f, 1.f);
			//if ((pInst->gridCollisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT || (offsetcheck & COLLISION_LEFT) != COLLISION_LEFT)
			if ((CheckInstanceBinaryMapCollision(pInst->posCurr.x + pInst->pObject->meshSize.x * pInst->scale.x,
				pInst->posCurr.y,
				pInst->pObject->meshSize.x * pInst->scale.x,
				pInst->pObject->meshSize.y * pInst->scale.y) & COLLISION_RIGHT) == COLLISION_RIGHT ||
				(CheckInstanceBinaryMapCollision(pInst->posCurr.x + pInst->pObject->meshSize.x * pInst->scale.x,
					pInst->posCurr.y - pInst->pObject->meshSize.y * pInst->scale.y,
					pInst->pObject->meshSize.x * pInst->scale.x,
					pInst->pObject->meshSize.y * pInst->scale.y) & COLLISION_LEFT) != COLLISION_LEFT)
			{
				pInst->counter = ENEMY_IDLE_TIME;
				pInst->innerState = INNER_STATE_ON_EXIT;
				pInst->velCurr.x = 0;
				//std::cout << "GOING RIGHT : INNER_STATE_ON_UPDATE\n";
			}

			break;

		case INNER_STATE_ON_EXIT:
			pInst->counter -= AEFrameRateControllerGetFrameTime();
			//std::cout << "GOING RIGHT : INNER_STATE_ON_EXIT\n";
			if (pInst->counter < 0)
			{
				pInst->state = STATE_GOING_LEFT;
				pInst->innerState = INNER_STATE_ON_ENTER;
			}
			break;
		}
		break;
	case STATE_ALERT:
		switch (pInst->innerState) {

		case INNER_STATE_ON_ENTER:


			break;
		case INNER_STATE_ON_UPDATE:

			break;


		case INNER_STATE_ON_EXIT:

			break;
		}
	}
}

int ImportMapDataFromFile(std::string FileName)
{
	//std::vector<int>	vint;
	//std::string			line;

	////	This function opens the file name "FileName" and retrieves all the map data.
	//std::fstream input(FileName, std::ios_base::in);

	////	function returns 0 if the file named "FileName" does not exists
	//if (!input)
	//{
	//	std::cout << "File " << FileName << " not found.\n";
	//	return 0;
	//}

	////	The first line in this file is the width of the map.
	////	The second line in this file is the height of the map.
	//for (int i = 0; i < 2; i++)
	//{
	//	std::getline(input, line);
	//	//	Convert char into int
	//	int temp = (line.back() - '0');
	//	vint.push_back(temp);
	//}
	//BINARY_MAP_WIDTH = vint[0];
	//BINARY_MAP_HEIGHT = vint[1];

	////	It allocates memory for the 2 arrays: MapData & BinaryCollisionArray
	//MapData = new int* [BINARY_MAP_HEIGHT];
	//BinaryCollisionArray = new int* [BINARY_MAP_HEIGHT];

	//for (int i = 0; i < BINARY_MAP_HEIGHT; i++)
	//{
	//	MapData[i] = new int[BINARY_MAP_WIDTH];
	//	BinaryCollisionArray[i] = new int[BINARY_MAP_WIDTH];
	//}

	///*
	//	The remaining part of the file is a series of numbers
	//	Each number represents the ID(or value) of a different element in the
	//	double dimensionaly array.
	//*/
	//for (int row = 0; row < BINARY_MAP_HEIGHT; row++)
	//{
	//	//	Get each line from the input
	//	std::getline(input, line);
	//	//	Remove any spacing from the input
	//	line.erase(remove(line.begin(), line.end(), ' '), line.end());

	//	for (int col = 0; col < BINARY_MAP_WIDTH; col++)
	//	{
	//		int idata = (line[col] - '0');
	//		MapData[row][col] = idata;
	//		//	Convert all objects into empty for binary collision array
	//		if (idata > 1)
	//			idata = 0;
	//		BinaryCollisionArray[row][col] = idata;
	//	}
	//}
	////	function returns 1 if the file named "FileName" exists
	//return 1;

	// Const string containing numbers
	static const std::string numbers{ "0123456789" };

	// Variable declaration and initialization
	std::ifstream ifs{ FileName };		// Open the given file
	std::string widthStr{ 0 }, heightStr{ 0 }, currentStr{ 0 };	// Declare and set to 0

	// Check if given file was not opened successfully
	if (!ifs)
		return 0;	// Case is true: Error encountered; Return 0;

	// ----- Read Width & Height from file ----- 
	// Read the 1st line containing the width
	getline(ifs, widthStr);
	// Read the 2nd line containing the height
	getline(ifs, heightStr);
	// Extract the number from the width string
	widthStr = widthStr.substr(widthStr.find_first_of(numbers), widthStr.find_last_of(numbers) - widthStr.find_first_of(numbers) + 1);
	// Extract the number from the height string
	heightStr = heightStr.substr(heightStr.find_first_of(numbers), heightStr.find_last_of(numbers) - heightStr.find_first_of(numbers) + 1);

	// Assign the width and height of the extracted width and height to the respective variables
	BINARY_MAP_WIDTH = std::stoi(widthStr);
	BINARY_MAP_HEIGHT = std::stoi(heightStr);

	// ----- Creation of MapData & BinaryCollisionArray ------
	// Allocate memory for the rows of int pointers of both arrays
	MapData = new int* [BINARY_MAP_HEIGHT];
	BinaryCollisionArray = new int* [BINARY_MAP_HEIGHT];

	// Loop through each row
	for (int i = 0; i < BINARY_MAP_HEIGHT; ++i) {
		// Allocate memory for the columns of int of both arrays
		MapData[i] = new int[BINARY_MAP_WIDTH];
		BinaryCollisionArray[i] = new int[BINARY_MAP_WIDTH];
	}

	// Variable declaration and initialization to 0
	int rowCount{ 0 }, colCount{ 0 };

	// Read the rest of the file
	while (ifs >> currentStr) {
		// Assign the read value to the Mapdata
		MapData[rowCount][colCount] = std::stoi(currentStr);
		// Assign either 1 or 0 to the binary collision data depending on the read value
		// Read value:
		// - More than 1 > store 0
		// - 1 > Store 1
		// - 0 > Store 0
		BinaryCollisionArray[rowCount][colCount] = std::stoi(currentStr) > 1 ? 0 : std::stoi(currentStr);

		// Increment column Count and check if it would be the same or larger than the width
		if (++colCount >= BINARY_MAP_WIDTH) {
			// Increment row count and check if it would be same or larger than the height
			if (++rowCount >= BINARY_MAP_HEIGHT)
				break;	// Break out of while loop; potential error if there is more data after this
			// Set column back to 0 (start of the next row)
			colCount = 0;
		}
	}

	// Close the input file
	ifs.close();

	// Return successfully read
	return 1;
}

void PrintRetrievedInformation(void)
{

	std::cout << "WIDTH: " << BINARY_MAP_WIDTH << "\nHEIGHT: " << BINARY_MAP_HEIGHT << std::endl;

	// ----- Map Data -----
	std::cout << "Map Data\n";	// Console formatting
	// Loop through each row
	for (int j = 0; j < BINARY_MAP_HEIGHT; ++j) {
		// Loop through each column
		for (int i = 0; i < BINARY_MAP_WIDTH; ++i)
			std::cout << MapData[j][i] << " ";	// Print out value in column
		// End of row reached; print a new line for next row
		std::cout << std::endl;
	}

	// ----- Binary Collision Data -----
	std::cout << "\nBinary Collision Data\n";	// Console formatting
	// Loop through each row
	for (int j = 0; j < BINARY_MAP_HEIGHT; ++j) {
		// Loop through each column
		for (int i = 0; i < BINARY_MAP_WIDTH; ++i)
			std::cout << BinaryCollisionArray[j][i] << " ";	// Print out value in column
		// End of row reached; print a new line for next row
		std::cout << std::endl;
	}
}

void FreeMapData(void)
{
	for (int i = 0; i < BINARY_MAP_HEIGHT; i++)
	{
		delete[] MapData[i];
		delete[] BinaryCollisionArray[i];
	}

	delete[] MapData;
	delete[] BinaryCollisionArray;
}

int GetCellValue(int X, int Y)
{
	//	Check if value is outside of binary map grid
	if (X < 0 || X >= BINARY_MAP_WIDTH ||
		Y < 0 || Y >= BINARY_MAP_HEIGHT)
		return 0;

	return MapData[Y][X];
}

void SnapToCell(float* Coordinate)
{
	// Snap by casting it to integer and adding 0.5f
	// May need to be changed depending on the cell
	*Coordinate = static_cast<int>(*Coordinate) + 0.5f;
}

int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY)
{
	//At the end of this function, "Flag" will be used to determine which sides
	//of the object instance are colliding. 2 hot spots will be placed on each side.

	//UNREFERENCED_PARAMETER(PosX);
	//UNREFERENCED_PARAMETER(PosY);
	//UNREFERENCED_PARAMETER(scaleX);
	//UNREFERENCED_PARAMETER(scaleY);
	//return 0;

	// Variable declaration and initialization to 0
	int Flag{ 0 };
	float x1{ 0 }, y1{ 0 }, x2{ 0 }, y2{ 0 }, x3{}, x4{};

	// Right side hotspots
	// - hotspot 1 (above center line)
	x1 = PosX + scaleX / 2;
	y1 = PosY + scaleY / 4;
	// - hotspot 2 (below center line)
	x2 = PosX + scaleX / 2;
	y2 = PosY - scaleY / 4;

	// Check if any of the hotspot is colliding
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1)) == 1 || GetCellValue(static_cast<int>(x2), static_cast<int>(y2)) == 1)
		Flag = (Flag | COLLISION_RIGHT);	// Case is true: OR the Flag variable with the COLLISION_RIGHT const

	// Top side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX - scaleX / 4;
	y1 = PosY + scaleY / 2;
	// - hotspot 2 (right of center line)
	x2 = PosX + scaleX / 4;
	y2 = PosY + scaleY / 2;

	// Check if any of the hotspot is colliding
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1)) == 1 || GetCellValue(static_cast<int>(x2), static_cast<int>(y2)) == 1)
		Flag = (Flag | COLLISION_TOP);	// Case is true: OR the Flag variable with the COLLISION_TOP const


	// Left side hotspots
	// - hotspot 1 (above center line)
	x1 = PosX - scaleX / 2;
	y1 = PosY + scaleY / 4;
	// - hotspot 2 (below center line)
	x2 = PosX - scaleX / 2;
	y2 = PosY - scaleY / 4;

	// Check if any of the hotspot is colliding
	//std::cout << "Left Check\n";
	//std::cout << "hs1 : (" << x1 << ", " << y1 << ") = " << BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] << " | ";
	//std::cout << "hs2 : (" << x2 << ", " << y2 << ") = " << BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)];
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1)) == 1 || GetCellValue(static_cast<int>(x2), static_cast<int>(y2)) == 1)
		Flag = (Flag | COLLISION_LEFT);	// Case is true: OR the Flag variable with the COLLISION_LEFT const


	// Bottom side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX - scaleX / 4;
	y1 = PosY - scaleY / 2;
	// - hotspot 2 (right of center line)
	x2 = PosX + scaleX / 4;
	x3 = PosX + scaleX / 32 * 15;
	x4 = PosX - scaleX / 32 * 15;

	// Check if any of the hotspot is colliding
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1)) == 1 || GetCellValue(static_cast<int>(x2), static_cast<int>(y1)) == 1
		|| GetCellValue(static_cast<int>(x3), static_cast<int>(y1)) == 1 || GetCellValue(static_cast<int>(x4), static_cast<int>(y1)) == 1)
		Flag = (Flag | COLLISION_BOTTOM);	// Case is true: OR the Flag variable with the COLLISION_BOTTOM const

	//std::cout << std::endl;

	// Return value of Flag
	return Flag;
}