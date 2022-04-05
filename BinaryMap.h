/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       BinaryMap.h
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Authored Functions
-------------------------------------------------------------------------------
@author		Mohamed Zafir (m.zafir@digipen.edu)
@role		Authored Functions
-------------------------------------------------------------------------------
@author		Desmond Too Wei Kang (d.too@digipen.edu)
@role		Authored Functions
*//*_________________________________________________________________________*/
#pragma once

#ifndef BINARY_MAP_H_
#define BINARY_MAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

extern const unsigned int	COLLISION_LEFT;// = 0x00000001;	//0001
extern const unsigned int	COLLISION_RIGHT;// = 0x00000002;	//0010
extern const unsigned int	COLLISION_TOP;// = 0x00000004;	//0100
extern const unsigned int	COLLISION_BOTTOM;// = 0x00000008;	//1000
extern const unsigned int	COLLISION_Destructable;// = 0x00000016;	//10000

int		ImportMapDataFromFile(std::string FileName, int*** __MapData, int*** __BinaryCollisionArray, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT);
int		GetCellValue(int X, int Y, int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT);
void	SnapToCell(float* Coordinate);
void	PrintRetrievedInformation(int*** _MapData, int*** _BinaryCollisionArray, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT);
int		CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY, int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT);
void	FreeMapData(int*** _MapData, int*** _BinaryCollisionArray, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT);
int		CheckInstanceBinaryMapCollision_bullet(float PosX, float PosY, float scaleX, float scaleY, int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT, int*** _BinaryCollisionArray);
int		CheckInstanceBinaryMapCollision_dotted(float PosX, float PosY, float scaleX, float scaleY, int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT);

#endif // BINARY_MAP_H_