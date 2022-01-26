#pragma once

#include "AEEngine.h"
#include <iostream>

extern AEGfxVertexList* pPlayerMesh;
extern AEGfxTexture* pPlayerTexture;
extern AEVec2 playerPos;

// Add or edit functions as required

void player_load(const char* texture_filename, float startPosX, float startPosY, float width, float height);

void player_update();

void player_draw(AEVec2 &position, AEGfxTexture* &texture, AEGfxVertexList* &mesh);

void player_unload(AEGfxTexture* &texture);