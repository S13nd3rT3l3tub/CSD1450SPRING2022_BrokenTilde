#pragma once

#include "AEEngine.h"
#include "globals.h"
#include <iostream>

struct Player {
	AEGfxTexture* pPlayerTexture;
	AEGfxVertexList* pPlayerMesh;
	AEVec2 playerPos;
	AEVec2 playerVelocity;
	
	float width;
	float height;

	bool renderFlag;
	bool aliveFlag;
};

// Add or edit functions as required

Player* Player_Load(const s8* texture_filename, float width, float height);

void Player_Initialize(AEVec2 &playerPos, float startPosX, float startPosY, bool &playerRenderFlag, bool &playerAliveFlag);

void Player_Update(Player* &player);

void Player_Draw(AEGfxTexture* &texture, AEGfxVertexList* &mesh, AEVec2 &position, bool &playerRenderFlag);

void Player_Free(bool &playerRenderFlag, bool &playerAliveFlag);

void Player_Unload(Player* &player);