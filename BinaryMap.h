/*!
@copyright	All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
@project	Ricochet
@file       BinaryMap.h
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu)	[80% of code]
@role		Initial creation
-------------------------------------------------------------------------------
@author		Mohamed Zafir (m.zafir@digipen.edu) [20% of code]
@role		Authored CheckInstanceBinaryMapCollision_Bullet & 
			CheckInstanceBinaryMapCollision_Dotted
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu) [1% of code]
@role		Assisted Wai Kit with conversion of functions to take in MapData &
			BinaryCollisionArray 
*//*_________________________________________________________________________*/

// ----- Include Guard -----
#ifndef BINARY_MAP_H_
#define BINARY_MAP_H_

/******************************************************************************/
/*!
	Import Map Data From File to Given MapData & BinaryCollisionArray
*/
/******************************************************************************/
int		ImportMapDataFromFile(std::string FileName, int*** __MapData, int*** __BinaryCollisionArray, 
							  int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT);

/******************************************************************************/
/*!
	Obtain Grid Value at Index
*/
/******************************************************************************/
int		GetCellValue(int X, int Y, int*** _MapData, 
					 int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT);

/******************************************************************************/
/*!
	Snap Coordinate To Grid
*/
/******************************************************************************/
void	SnapToCell(float* Coordinate);

/******************************************************************************/
/*!
	Print Retrieved Information of Given MapData & BinaryCollisionData
*/
/******************************************************************************/
void	PrintRetrievedInformation(int*** _MapData, int*** _BinaryCollisionArray, 
								  int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT);

/******************************************************************************/
/*!
	Check Instance Collision w/ Binary Map
*/
/******************************************************************************/
int		CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY, 
										int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT);

/******************************************************************************/
/*!
	Check Bullet Instance Collision w/ Binary Map
*/
/******************************************************************************/
int		CheckInstanceBinaryMapCollision_Bullet(float PosX, float PosY, float scaleX, float scaleY, 
											   int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT, int*** _BinaryCollisionArray);

/******************************************************************************/
/*!
	Check Dotted Instance Collision w/ Binary Map
*/
/******************************************************************************/
int		CheckInstanceBinaryMapCollision_Dotted(float PosX, float PosY, float scaleX, float scaleY, 
											   int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT);

/******************************************************************************/
/*!
	Free Allocated Map Data
*/
/******************************************************************************/
void	FreeMapData(int*** _MapData, int*** _BinaryCollisionArray, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT);


#endif // BINARY_MAP_H_