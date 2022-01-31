#include "Level1.h"
#include "Player.h"
#include "GameStateList.h"

Player* player;

void Level1_Load() {
	player = Player_Load("tank.png", 100, 50);
}

void Level1_Initialize() {
	Player_Initialize(player->playerPos, 0, 0, player->renderFlag, player->aliveFlag);
}

void Level1_Update() {
	Player_Update(player);

}

void Level1_Draw() {
	Player_Draw(player->pPlayerTexture, player->pPlayerMesh, player->playerPos, player->renderFlag);
}

void Level1_Free(){
	Player_Free(player->renderFlag, player->aliveFlag);
}

void Level1_Unload() {
	Player_Unload(player);
}
