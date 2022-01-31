#include "enemy.h"

Enemy* Enemy_Load(const s8* texture_filename, int enemyType, float width, float height){
	/*Player* player = new Player;

	player->pPlayerTexture = AEGfxTextureLoad(texture_filename);
	AE_ASSERT_MESG(player->pPlayerTexture, "Failed to create pPlayerTexture");

	AEGfxMeshStart();
	RectMeshAdd(0, 0, width, height);
	player->pPlayerMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(player->pPlayerMesh, "Failed to create pPlayerMesh");

	player->width = width;
	player->height = height;

	player->renderFlag = false;
	player->aliveFlag = false;

	return player;*/

	Enemy* enemy = new Enemy;

	pEnemyTexture = AEGfxTextureLoad("soldier.png");
	AE_ASSERT_MESG(pEnemyTexture, "Failed to create pEnemyTexture");

	AEGfxMeshStart();
	RectMeshAdd(0, 0, width, height);
	pEnemyMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pEnemyTexture, "Failed to create pEnemyMesh");
}

void Enemy_Initialize(AEVec2& enemyPos, float startPosX, float startPosY, bool& enemyRenderFlag, bool& enemyAliveFlag) {

}

void Enemy_Update(Enemy*& enemy) {

}

void Enemy_Draw(AEGfxTexture*& texture, AEGfxVertexList*& mesh, AEVec2& position, bool& enemyRenderFlag) {

}

void Enemy_Free(bool& enemyRenderFlag, bool& enemyAliveFlag) {

}

void Enemy_Unload(Enemy*& enemy) {

}