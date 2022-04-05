/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       BinaryMap.cpp
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

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "AEEngine.h"
#include <fmod.hpp>

// ---------------------------------------------------------------------------
// Globals
extern float	 g_dt;
extern double	 g_appTime;
extern double	 levelTime;
extern s8		 g_font12;
extern s8		 g_font20;
extern s8		 g_font30;
extern int		 g_mouseX;
extern int		 g_mouseY;
extern int		 g_chosenLevel;

extern int winWidth;
extern int winHeight;

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
const unsigned int	GAME_OBJ_NUM_MAX = 32;			//The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_INST_NUM_MAX = 4096;			//The total number of different game object instances


const float			PLAYER_DEATH_ANIME_TIME = 2.0f;
const float			PLAYER_INITIAL_HEALTH = 100;			// initial number of player lives
extern AEVec2		PLAYER_MESHSIZE;// = { 0.8f, 1.0f };
extern AEVec2		PLAYER_SCALE;// = { 2.0f, 1.0f };		// player scaling
extern AEVec2		GUN_MESHSIZE;// = { 0.5f, 0.5f };
extern AEVec2		GUN_SCALE;// = { 2.6f, 0.7f };		// gun size

extern AEVec2		BULLET_MESHSIZE;// = { 1.0f, 1.0f };
extern AEVec2		BULLET_SCALE;// = { 0.25f, 0.25f };
const float			BULLET_SPEED = 10.0f;		// bullet speed (m/s)

extern AEVec2		PLATFORM_MESHSIZE;// = { 1.0f, 1.0f };
extern AEVec2		PLATFORM_SCALE;// = { 5.0f, 5.0f };
extern AEVec2		EMPTY_MESHSIZE;// = { 1.0f, 1.0f };
extern AEVec2		EMPTY_SCALE;// = { 1.0f, 1.0f };

extern AEVec2		HEALTHBAR_MESHSIZE;// = { 0.1f, 0.75f };
extern AEVec2		HEALTHBAR_SCALE;// = { 15.0f, 0.5f, };


//Gameplay related variables and values
const float			GRAVITY = -9.8f;
const float			JUMP_VELOCITY = 800.0f;
const float			HOVER_VELOCITY = 7.0f;

const float			MOVE_VELOCITY = 17.0f;
const float			MOVE_VELOCITY_ENEMY = 2.5f;
const double		ENEMY_IDLE_TIME = 2.0;
const float			ENEMY_DETECTION_RANGE = 150.0f;
const float			ENEMY_SHOOT_DELAY = 2.0f;



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
const unsigned int	COLLISION_Destructable = 0x00000016;	//10000

/******************************************************************************/
/*!
	Enums Definitions
*/
/******************************************************************************/
enum TYPE
{
	// list of game object types
	TYPE_EMPTY = 0,		// 0
	TYPE_PLATFORM,		// 1
	TYPE_PLAYER,		// 2
	TYPE_PLAYERGUN,		// 3
	TYPE_BULLET,		// 4
	TYPE_ENEMY1,		// 5
	TYPE_ENEMY2,		// 6
	TYPE_PARTICLE1,		// 7
	TYPE_DIRT,			// 8
	TYPE_DOTTED,		// 9
	TYPE_HEALTHBAR,		// 10
	TYPE_BUTTON,		// 11
	TYPE_SPLASH,
	TYPE_BG,
	TYPE_MOUSE,
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

enum INNER_GAME_STATE {
	GAME_PAUSE,
	GAME_PLAY,
	GAME_WIN,
	GAME_LOSE
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


	//Used to hold the current 
	int					gridCollisionFlag;

	// pointer to custom data specific for each object type
	void* pUserData;

	// Subtypes if required
	int				sub_type;

	//State of the object instance
	enum			STATE state;
	enum			INNER_STATE innerState;

	int				bulletbounce;

	//General purpose counter (This variable will be used for the enemy state machine)
	double			counter;
	float			shoot_timer;
	float			shoot_timer2;
	//void				(*pfUpdate)(void);
	//void				(*pfDraw)(void);
};

/******************************************************************************/
/*!
	(Static) Variables
*/
/******************************************************************************/
extern bool toFullScreen;

extern unsigned long currInnerState;

// Sounds
extern FMOD::System* fmodSys;
extern FMOD::Sound* mainMenuBG;

// list of original object
extern GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
extern unsigned long		sGameObjNum;								// The number of defined game objects

// list of object instances
extern GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
extern unsigned long		sGameObjInstNum;							// The number of used game object instances

// pointer to the PlayerBody and gun object
extern GameObjInst* PlayerBody;
extern GameObjInst* PlayerGun;
extern GameObj* PlayerHealthBar;
extern GameObjInst* Enemydetection;

extern GameObjInst* EmptyInstance;
extern GameObjInst* PlatformInstance;
extern GameObjInst* DirtInstance;

// Game specific variables
extern float				playerHealth;				// The amount of health left
extern double				jumpFuel;
extern int					totalEnemyCount;
extern int					ammoCount;

extern unsigned long emptyObjIndex;
extern unsigned long platformObjIndex;
extern unsigned long playerObjIndex;
extern unsigned long playerGunObjIndex;
extern unsigned long bulletObjIndex;
extern unsigned long enemy1ObjIndex;
extern unsigned long enemy2ObjIndex;
extern unsigned long particleObjIndex;
extern unsigned long dottedObjIndex;
extern unsigned long dirtObjIndex;
extern unsigned long hpObjIndex;
extern unsigned long splashObjIndex;
extern unsigned long bgObjIndex;
extern unsigned long buttonObjIndex;

// Current mouse position
extern float worldMouseX;
extern float worldMouseY;

//Binary map data
extern int** MapData;
extern int** BinaryCollisionArray;
extern int				BINARY_MAP_WIDTH;
extern int				BINARY_MAP_HEIGHT;

// Transform matrix containing shift of grid to world coordinates
// concatenate this with object instance's own transform matrix
extern AEMtx33			MapTransform;

/******************************************************************************/
 /*!
	 Textures
 */
 /******************************************************************************/
extern AEGfxTexture* tex_stone;

// ---------------------------------------------------------------------------
// functions to create/destroy a game object instance
//GameObjInst* gameObjInstCreate(unsigned int type, AEVec2* scale,
//	AEVec2* pPos, AEVec2* pVel,
//	float dir, enum STATE startState);
GameObjInst* gameObjInstCreate(GameObj* objType, AEVec2* scale,
								AEVec2* pPos, AEVec2* pVel,
								float dir, enum STATE startState);
void					gameObjInstDestroy(GameObjInst* pInst);
void EnemyStateMachine(GameObjInst* pInst);

void RectMeshAdd(float x, float  y, float length, float height);

#endif 