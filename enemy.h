#pragma once

#include "AEEngine.h"
#include "globals.h"

struct Enemy {
	AEVec2 enemyPos;

};

extern AEGfxVertexList* pEnemyMesh;
extern AEGfxTexture* pEnemyTexture;

// Add or edit functions as required

void enemy_load(const char* texture_filename, float startPosX, float startPosY, float width, float height);

void enemy_update();

void enemy_draw(AEVec2& position, AEGfxTexture*& texture, AEGfxVertexList*& mesh);

void enemy_unload(AEGfxTexture*& texture);