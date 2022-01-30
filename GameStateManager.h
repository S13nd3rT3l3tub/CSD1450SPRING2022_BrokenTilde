#pragma once

typedef void(*FP)(void);
extern int currentState, previousState, nextState;
extern FP fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);
void GSM_Update();

