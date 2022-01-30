#pragma once

#include "AEEngine.h"
#include "globals.h"
#include <iostream>

struct Player {
	AEGfxTexture* pPlayerTexture;
	AEGfxVertexList* pPlayerMesh;
	AEVec2 playerPos;
	
	float width;
	float height;

	bool toRender;
	bool aliveStatus;
};

// Add or edit functions as required

Player* player_load(const s8* texture_filename, float width, float height);

void player_initialize(AEVec2 &playerPos, float startPosX, float startPosY, bool &playerRender, bool &playerAlive);

void player_update(Player* &player);

void player_draw(AEGfxTexture* &texture, AEGfxVertexList* &mesh, AEVec2 &position, bool &toRender);

void player_free(bool &toRender, bool &aliveStatus);

void player_unload(Player* &player);