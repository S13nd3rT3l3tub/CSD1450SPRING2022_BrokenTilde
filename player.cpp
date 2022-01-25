#include "player.h"

// Add or edit functions as required

AEGfxVertexList* pPlayerMesh;
AEGfxTexture* pPlayerTexture;
AEVec2 playerPos;

void player_load(char* texture_filename, float startPosX, float startPosY, float width, float height) {
	const s8* filename = texture_filename;
	
	pPlayerTexture = AEGfxTextureLoad(filename);
	
	playerPos.x = startPosX;
	playerPos.y = startPosY;

	//Call function to create rectangle for the player

	AE_ASSERT_MESG(pPlayerMesh, "Failed to create pTankMesh");
}

void player_update();

void player_draw(AEVec2& position, AEGfxTexture* &texture, AEGfxVertexList* &mesh) {
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