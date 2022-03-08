//#pragma once
//
//#ifndef BINARY_MAP_H_
//#define BINARY_MAP_H_
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string>
//#include <vector>
//#include <fstream>
//#include <iostream>
//
////extern int BINARY_MAP_WIDTH;
////extern int BINARY_MAP_HEIGHT;
//
////extern int** MapData;
////extern int** BinaryCollisionArray;
//
//extern const unsigned int	COLLISION_LEFT;// = 0x00000001;	//0001
//extern const unsigned int	COLLISION_RIGHT;// = 0x00000002;	//0010
//extern const unsigned int	COLLISION_TOP;// = 0x00000004;	//0100
//extern const unsigned int	COLLISION_BOTTOM;// = 0x00000008;	//1000
//
//int ImportMapDataFromFile(std::string FileName, int** MapData, int** BinaryCollisionArray, int& BINARY_MAP_WIDTH, int& BINARY_MAP_HEIGHT);
//int GetCellValue(int X, int Y, int** MapData, int BINARY_MAP_WIDTH, int BINARY_MAP_HEIGHT);
//void	SnapToCell(float* Coordinate);
//void PrintRetrievedInformation(int** MapData, int** BinaryCollisionArray, int BINARY_MAP_WIDTH, int BINARY_MAP_HEIGHT);
//int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY, int** MapData, int BINARY_MAP_WIDTH, int BINARY_MAP_HEIGHT);
//void FreeMapData(int** MapData, int** BinaryCollisionArray, int BINARY_MAP_WIDTH, int BINARY_MAP_HEIGHT);
//void	SnapToCell(float* Coordinate);
//
//#endif // BINARY_MAP_H_