/******************************************************************************/
/*!
\file		GameState_Levels.h
\author 	DigiPen
\par    	email: digipen\@digipen.edu
\date   	January 01, 20xx
\brief		ToDo: give a brief explanation here

Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#ifndef GAME_STATE_PLAY_H_
#define GAME_STATE_PLAY_H_

// ---------------------------------------------------------------------------

void GameStateLevel1Load(void);
void GameStateLevel1Init(void);
void GameStateLevel1Update(void);
void GameStateLevel1Draw(void);
void GameStateLevel1Free(void);
void GameStateLevel1Unload(void);

void GameStateLevelsLoad(void);
void GameStateLevelsInit(void);
void GameStateLevelsUpdate(void);
void GameStateLevelsDraw(void);
void GameStateLevelsFree(void);
void GameStateLevelsUnload(void);

extern AEVec2		PLAYER_SCALE;		// player scaling
extern AEVec2		GUN_MESHSIZE;
extern AEVec2		GUN_SCALE;		// gun size

extern AEVec2		BULLET_MESHSIZE;
extern AEVec2		BULLET_SCALE;

extern AEVec2		PLATFORM_MESHSIZE;
extern AEVec2		PLATFORM_SCALE;
extern AEVec2		EMPTY_MESHSIZE;
extern AEVec2		EMPTY_SCALE;

extern AEGfxTexture* tex_stone;

// ---------------------------------------------------------------------------

#endif 


