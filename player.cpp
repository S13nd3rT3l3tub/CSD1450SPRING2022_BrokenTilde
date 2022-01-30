#include "Player.h"

// Add or edit functions as required

Player* player_load(const s8* texture_filename, float width, float height) {
	
	Player* player = new Player;

	player->pPlayerTexture = AEGfxTextureLoad(texture_filename);
	AE_ASSERT_MESG(player->pPlayerTexture, "Failed to create pPlayerTexture");

	AEGfxMeshStart();
	RectMeshAdd(0, 0, width, height);
	player->pPlayerMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(player->pPlayerMesh, "Failed to create pPlayerMesh");

	player->width = width;
	player->height = height;

	player->toRender = false;
	player->aliveStatus = false;

	return player;
}

void player_initialize(AEVec2 &playerPos, float startPosX, float startPosY, bool &playerRender, bool &playerAlive){
	playerPos.x = startPosX;
	playerPos.y = startPosY;

	playerRender = true;
	playerAlive = true;
}

void player_update(Player* &player)  {

}

void player_draw(AEGfxTexture* &texture, AEGfxVertexList* &mesh, AEVec2 &position, bool &toRender) {
	if (toRender) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set position for object 1
		AEGfxSetPosition(position.x, position.y);
		// No texture for object 1
		AEGfxTextureSet(texture, 0, 0);
		// No tint
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
	}
}

void player_free(bool &toRender, bool &aliveStatus){
	toRender = false;
	aliveStatus = false;
}

void player_unload(Player* &player) {
	AEGfxTextureUnload(player->pPlayerTexture);
	delete player;
}


