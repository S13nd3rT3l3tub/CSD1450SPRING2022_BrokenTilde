#pragma once

#ifndef BINARY_MAP_H_
#define BINARY_MAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

extern int BINARY_MAP_WIDTH;
extern int BINARY_MAP_HEIGHT;

extern int** MapData;
extern int** BinaryCollisionArray;


int		ImportMapDataFromFile(const char* FileName);
int		GetCellValue(int X, int Y);
void	FreeMapData(void);
void	SnapToCell(float* Coordinate);

#endif // BINARY_MAP_H_