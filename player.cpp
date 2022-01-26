#include "player.h"

// Add or edit functions as required

AEGfxVertexList* pPlayerMesh;
AEGfxTexture* pPlayerTexture;
AEVec2 playerPos;

void makerect(float x, float  y, float length, float height)
{
	AEGfxTriAdd(
		x, y - height, 0xFFFF0000, 0.0f, 1.0f,
		x + length, y - height, 0xFFFF0000, 1.0f, 1.0f,
		x, y, 0xFFFF0000, 0.0f, 0.0f);

	AEGfxTriAdd(
		x + length, y, 0xFFFF0000, 1.0f, 0.0f,
		x + length, y - height, 0xFFFF0000, 1.0f, 1.0f,
		x, y, 0xFFFF0000, 0.0f, 0.0f);
}

void player_load(const char* texture_filename, float startPosX, float startPosY, float width, float height) {
	const s8* filename = texture_filename;
	
	pPlayerTexture = AEGfxTextureLoad(texture_filename);
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


