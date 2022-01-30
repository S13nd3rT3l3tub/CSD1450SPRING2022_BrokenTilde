#include "GameStateManager.h"
#include "GameStateList.h"
#include "Level1.h"

int currentState = 0, previousState = 0, nextState = 0;	// States
FP fpLoad = nullptr, fpInitialize = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr; // Function Pointers for the current state

void GSM_Initialize(int startingState) {
	currentState = previousState = nextState = startingState;
}

void GSM_Update() {
	switch (currentState){
	case GS_SPLASHSCREEN:
		break;
	case GS_MAINMENU:
		break;
	case GS_LEVEL1:
		fpLoad = Level1_Load;
		fpInitialize = Level1_Initialize;
		fpUpdate = Level1_Update;
		fpDraw = Level1_Draw;
		fpFree = Level1_Free;
		fpUnload = Level1_Unload;
		break;
	case GS_RESTART:
		break;
	case GS_QUIT:
		break;

	default:
		break;
	}

}