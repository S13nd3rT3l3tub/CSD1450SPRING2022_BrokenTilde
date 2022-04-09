/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       Globals.h
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		
-------------------------------------------------------------------------------
@author		Mohamed Zafir (m.zafir@digipen.edu)
@role		
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu)
@role		
*//*_________________________________________________________________________*/

// ----- Include Guard -----
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

// ----- Include Files -----
#include "AEEngine.h"
#include <fmod.hpp>

/******************************************************************************/
/*!
	Defines
*/
/******************************************************************************/
// ----- Object Pool Variables ------
const unsigned int	GAME_OBJ_NUM_MAX{ 32 };				// The total number of different objects (Shapes)
const unsigned int	GAME_OBJ_INST_NUM_MAX{ 4096 };		// The total number of different game object instances

// ----- Mesh Sizes & Scales -----
extern AEVec2		PLAYER_MESHSIZE;			// Player mesh size	
extern AEVec2		PLAYER_SCALE;				// Player scale size
extern AEVec2		GUN_MESHSIZE;				// Gun barrel mesh size
extern AEVec2		GUN_SCALE;					// Gun barrel scale size

extern AEVec2		BULLET_MESHSIZE;			// Bullet mesh size
extern AEVec2		BULLET_SCALE;				// Bullet scale size
const float			BULLET_SPEED{ 10.0f };		// Bullet speed

extern AEVec2		PLATFORM_MESHSIZE;			// Platform mesh size
extern AEVec2		PLATFORM_SCALE;				// Platform scale size
extern AEVec2		EMPTY_MESHSIZE;				// Void mesh size
extern AEVec2		EMPTY_SCALE;				// Void scale size

extern AEVec2		HEALTHBAR_MESHSIZE;			// Health bar mesh size
extern AEVec2		HEALTHBAR_SCALE;			// Health bar scale size

// ----- Gameplay Related Variables -----
// Environment
const float			GRAVITY{ -9.8f };					// World gravity

// Player
const float			PLAYER_DEATH_ANIME_TIME	{ 2.0f };	// Player death animation max time
const float			PLAYER_INITIAL_HEALTH	{ 100.0f };	// Player initial health
const float			JUMP_VELOCITY			{ 800.0f };	// Player jump speed
const float			HOVER_VELOCITY			{ 7.0f };	// Player hover speed
const float			FUEL_MAX_VALUE			{ 1.5f };	// Max amount of jump fuel
const float			MOVE_VELOCITY			{ 17.0f };	// Player move speed


// Enemy
const float			MOVE_VELOCITY_ENEMY{ 2.5f };		// Enemy move speed
const double		ENEMY_IDLE_TIME{ 2.0 };				// Enemy max idle time
const float			ENEMY_DETECTION_RANGE{ 300.0f };	// Enemy vision range
const float			ENEMY_SHOOT_DELAY{ 2.0f };			// Enemy projectile burst delay

// Object flag definition
const unsigned int	FLAG_ACTIVE = 0x00000001;			// Bit flag for active object
const unsigned int	FLAG_VISIBLE = 0x00000002;			// Bit flag for visible object
const unsigned int	FLAG_NON_COLLIDABLE = 0x00000004;	// Bit flag for non-collidable object

// Collision flags
const unsigned int	COLLISION_LEFT = 0x00000001;			// Bit flag for left collision		   (0001)
const unsigned int	COLLISION_RIGHT = 0x00000002;			// Bit flag for right collision		   (0010)
const unsigned int	COLLISION_TOP = 0x00000004;				// Bit flag for top collision		   (0100)
const unsigned int	COLLISION_BOTTOM = 0x00000008;			// Bit flag for bottom collision	   (1000)
const unsigned int	COLLISION_Destructable = 0x00000016;	// Bit flag for destructable collision (10000)

/******************************************************************************/
/*!
	Enums Definitions
*/
/******************************************************************************/
// Object types
enum TYPE
{
	// List of game object types
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
	TYPE_SPLASH,		// 12
	TYPE_BG,			// 13
	TYPE_MOUSE,			// 14
	TYPE_NUM
};

//State machine states
enum STATE
{
	STATE_NONE = 0,
	STATE_GOING_LEFT,
	STATE_GOING_RIGHT,
	STATE_ALERT
};

//State machine inner states
enum INNER_STATE
{
	INNER_STATE_ON_ENTER = 0,
	INNER_STATE_ON_UPDATE,
	INNER_STATE_ON_EXIT
};

// Game state inner states
enum INNER_GAME_STATE {
	GAME_PAUSE = 0,
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
	unsigned long		type;		// Object type
	AEGfxVertexList*	pMesh;		// Object mesh
	AEVec2				meshSize;	// Object mesh size
};

//Game object instance structure
struct GameObjInst
{
	GameObj*			pObject;				// Pointer to 'original' shape
	unsigned long		flag;					// Bit flag or-ed together
	AEVec2				scale;					// Scaling value of the object instance
	AEVec2				posCurr;				// Object current position
	AEVec2				velCurr;				// Object current velocity
	float				dirCurr;				// Object current direction
	AABB				boundingBox;			// Object bouding box that encapsulates the object
	AEMtx33				transform;				// Object transformation matrix
	//void*				pUserData;				// Pointer to custom data specific for each object type
	//void				(*pfUpdate)(void);		// Pointer to custom update function of the object
	//void				(*pfDraw)(void);		// Pointer to custom draw function of the object
	int					gridCollisionFlag;		// Object collision flag
	int					sub_type;				// Object Subtypes if required
	int					bounceCount;
	// State of the object instance for enemy
	enum				STATE state;			// Object main state
	enum				INNER_STATE innerState;	// Object sub state
	double				counter;				// General purpose counter 
	float				shoot_timer;			// Object shoot timer
	float				shoot_timer2;			// Object shoot timer 2
};

/******************************************************************************/
/*!
	(Static) Variables
*/
/******************************************************************************/
// List of original object
extern GameObj				sGameObjList[GAME_OBJ_NUM_MAX];				// Each element in this array represents a unique game object (shape)
extern unsigned long		sGameObjNum;								// The number of defined game objects

// List of object instances
extern GameObjInst			sGameObjInstList[GAME_OBJ_INST_NUM_MAX];	// Each element in this array represents a unique game object instance (sprite)
extern unsigned long		sGameObjInstNum;							// The number of used game object instances

// Pointer to specific game object instances
extern GameObjInst* PlayerBody;			// Player object instance
extern GameObjInst* PlayerGun;			// Player barrel object instance
extern GameObj*		PlayerHealthBar;	// Player health bar object instance

extern GameObjInst* EmptyInstance;		// Void object instance
extern GameObjInst* PlatformInstance;	// Platform object instance
extern GameObjInst* DirtInstance;		// Dirt object instance

extern GameObjInst* Enemydetection;		// Enemy LoS check object instance

// Game specific variables
extern float		playerHealth;		// The amount of health left
extern double		jumpFuel;			// Hover fuel
extern int			totalEnemyCount;	// Total enemy count in current level
extern int			ammoCount;			// Total ammo count in current level

// Object index in object list
extern unsigned long emptyObjIndex;			// Empty object index
extern unsigned long platformObjIndex;		// Platform object index
extern unsigned long playerObjIndex;		// Player object index
extern unsigned long playerGunObjIndex;		// Player gun object index
extern unsigned long bulletObjIndex;		// Bullet object index
extern unsigned long enemy1ObjIndex;		// Enemy 1st variant object index
extern unsigned long enemy2ObjIndex;		// Enemy 2nd variant object index
extern unsigned long particleObjIndex;		// Particle object index
extern unsigned long dottedObjIndex;		// Dotted object index
extern unsigned long dirtObjIndex;			// Dirt object index
extern unsigned long hpObjIndex;			// Health bar object index
extern unsigned long splashObjIndex;		// Splash screen object index
extern unsigned long bgObjIndex;			// Background object index
extern unsigned long buttonObjIndex;		// Button object index

// Level Map Data
extern int**	MapData;				// Level map data	
extern int**	BinaryCollisionArray;	// Level collision data
extern int		BINARY_MAP_WIDTH;		// Data width size
extern int		BINARY_MAP_HEIGHT;		// Data height size
extern AEMtx33	MapTransform;			// Transform matrix containing shift of grid to world coordinates
extern int		g_chosenLevel;			// Chosen level number

/******************************************************************************/
/*!
	Textures
*/
/******************************************************************************/
extern AEGfxTexture* stoneTexture;	// Stone texture
extern AEGfxTexture* dirtTexture;	// Dirt texture

/******************************************************************************/
/*!
	Helper Functions
*/
/******************************************************************************/
// Function to create a game object instance
GameObjInst*	gameObjInstCreate(GameObj* objType, AEVec2* scale,
								  AEVec2* pPos,		AEVec2* pVel,
								  float dir,		enum STATE startState);
// Function to destroy a game object instance
void			gameObjInstDestroy(GameObjInst* pInst);
// Enemy state machine
void			EnemyStateMachine(GameObjInst* pInst);

#endif 