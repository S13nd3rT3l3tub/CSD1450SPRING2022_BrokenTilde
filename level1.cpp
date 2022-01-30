#include "Level1.h"
#include "Player.h"
#include "GameStateList.h"

Player* player;

void Level1_Load() {
	player = player_load("tank.png", 100, 50);
}

void Level1_Initialize() {
	player_initialize(player->playerPos, 0, 0, player->toRender, player->aliveStatus);
}

void Level1_Update() {
	player_update(player);

}

void Level1_Draw() {
	player_draw(player->pPlayerTexture, player->pPlayerMesh, player->playerPos, player->toRender);
}

void Level1_Free(){
	player_free(player->toRender, player->aliveStatus);
}

void Level1_Unload() {
	player_unload(player);
}
