#pragma once

#include "AEEngine.h"
#include "globals.h"

enum E_TYPES {

};

enum E_STATES {
	IDLE,
	ATTACK
};

struct Enemy {
	AEGfxVertexList* pEnemyMesh;
	AEVec2 enemyPos;
	AEVec2 enemyVelocity;

	float width;
	float height;

	int enemyType;
	int currentState;
	bool toRenderFlag;
	bool aliveFlag;
};

extern AEGfxTexture* pEnemyTexture;
// Add additional textures for different enemy types

// Add or edit functions as required

Enemy* Enemy_Load(const s8* texture_filename, float width, float height);

void Enemy_Initialize(AEVec2& enemyPos, float startPosX, float startPosY, bool& enemyRenderFlag, bool& enemyAliveFlag);

void Enemy_Update(Enemy*& enemy);

void Enemy_Draw(AEGfxTexture*& texture, AEGfxVertexList*& mesh, AEVec2& position, bool& enemyRenderFlag);

void Enemy_Free(bool& enemyRenderFlag, bool& enemyAliveFlag);

void Enemy_Unload(Enemy*& enemy);