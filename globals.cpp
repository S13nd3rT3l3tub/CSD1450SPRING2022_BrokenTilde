#include "globals.h"

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