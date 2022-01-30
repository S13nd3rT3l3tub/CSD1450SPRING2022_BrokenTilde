#include "Player.h"

// Add or edit functions as required

AEGfxVertexList* pPlayerMesh;
AEGfxTexture* pPlayerTexture;
AEVec2 playerPos;

void player_load(float startPosX, float startPosY, float width, float height) {
	
	pPlayerTexture = AEGfxTextureLoad("tank.png");
	AE_ASSERT_MESG(pPlayerTexture, "Failed to create pPlayerTexture");
	
	playerPos.x = startPosX;
	playerPos.y = startPosY;

	AEGfxMeshStart();
	makerect(playerPos.x, playerPos.y, 100, 50);
	pPlayerMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pPlayerMesh, "Failed to create pPlayerMesh");

	
}

void player_update() {
	return;
}

void player_draw(AEVec2 &position, AEGfxTexture* &texture, AEGfxVertexList* &mesh) {
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


void player_unload(AEGfxTexture*& texture) {
	AEGfxTextureUnload(texture);
}


