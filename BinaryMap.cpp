/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       BinaryMap.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role
-------------------------------------------------------------------------------
@author		Mohamed Zafir (m.zafir@digipen.edu)
@role
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu)
@role
*//*_________________________________________________________________________*/

// ----- Include File -----
#include "Main.h"

/******************************************************************************/
/*!
	Import Map Data From File to Given MapData & BinaryCollisionArray
*/
/******************************************************************************/
int ImportMapDataFromFile(std::string FileName, int*** _MapData, int*** _BinaryCollisionArray, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT)
{
	// Attempt to open file
	std::fstream fs(FileName, std::ios_base::in);
	// Check if file was unsuccessfully opened
	if (!fs){
		// Print out debug and return 0
		//std::cout << "File " << FileName << " not found.\n";
		return 0;
	}

	// Variable declaration
	std::string filler{};
	int hold{};

	fs >> filler >> _BINARY_MAP_WIDTH; // Read width
	fs >> filler >> _BINARY_MAP_HEIGHT; // Read height

	// Allocate memory for _MapData
	(*_MapData) = new int* [_BINARY_MAP_HEIGHT];
	for (int r = 0; r < _BINARY_MAP_HEIGHT; ++r)
		(*_MapData)[r] = new int[_BINARY_MAP_WIDTH];

	// Allocate memory for _BinaryCollisionArray
	(*_BinaryCollisionArray) = new int* [_BINARY_MAP_HEIGHT];
	for (int r = 0; r < _BINARY_MAP_HEIGHT; ++r)
		(*_BinaryCollisionArray)[r] = new int[_BINARY_MAP_WIDTH];

	// Loop for the size of the grid
	for (int r = 0; r < _BINARY_MAP_HEIGHT; ++r) {
		for (int c = 0; c < _BINARY_MAP_WIDTH; ++c) {
			// Read ints from file
			fs >> hold; 

			// Assign value to MapData
			(*_MapData)[r][c] = hold;

			// ----- Check value and assign to binary map data -----
			// Check if value is 0
			if (hold == 0)
				(*_BinaryCollisionArray)[r][c] = 0; 
			else if(hold == 1)	// Check if value is 1
				(*_BinaryCollisionArray)[r][c] = 1;
			else if (hold == 8)	// Check if value is 8
				(*_BinaryCollisionArray)[r][c] = 8;
			else    // value is more than 1
				(*_BinaryCollisionArray)[r][c] = 0; 
		}
	}

	// Close the file
	fs.close();

	// Return 1
	return 1;
}

/******************************************************************************/
/*!
	Obtain Grid Value at Index
*/
/******************************************************************************/
int	GetCellValue(int X, int Y, int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT)
{
	//	Check if value is outside of binary map grid
	if (X < 0 || X >= _BINARY_MAP_WIDTH ||
		Y < 0 || Y >= _BINARY_MAP_HEIGHT)
		return 0;

	// Return value at index in MapData
	return (*_MapData)[Y][X];
}

/******************************************************************************/
/*!
	Snap Coordinate To Grid
*/
/******************************************************************************/
void	SnapToCell(float* Coordinate)
{
	// Snap by casting it to integer and adding 0.5f
	// May need to be changed depending on the cell
	*Coordinate = static_cast<int>(*Coordinate) + 0.5f;
}

/******************************************************************************/
/*!
	Print Retrieved Information of Given MapData & BinaryCollisionData
*/
/******************************************************************************/
void	PrintRetrievedInformation(int*** _MapData, int*** _BinaryCollisionArray, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT)
{
	// Print out the width & height of the data
	std::cout << "WIDTH: " << _BINARY_MAP_WIDTH << "\nHEIGHT: " << _BINARY_MAP_HEIGHT << std::endl;

	// ----- Map Data -----
	std::cout << "Map Data\n";	// Console formatting
	// Loop through each row
	for (int j = 0; j < _BINARY_MAP_HEIGHT; ++j) {
		// Loop through each column
		for (int i = 0; i < _BINARY_MAP_WIDTH; ++i)
			std::cout << (*_MapData)[j][i] << " ";	// Print out value in column
		// End of row reached; print a new line for next row
		std::cout << std::endl;
	}

	// ----- Binary Collision Data -----
	std::cout << "\nBinary Collision Data\n";	// Console formatting
	// Loop through each row
	for (int j = 0; j < _BINARY_MAP_HEIGHT; ++j) {
		// Loop through each column
		for (int i = 0; i < _BINARY_MAP_WIDTH; ++i)
			std::cout << (*_BinaryCollisionArray)[j][i] << " ";	// Print out value in column
		// End of row reached; print a new line for next row
		std::cout << std::endl;
	}
}


int		CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY, 
										int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT)
{
	//At the end of this function, "Flag" will be used to determine which sides
	//of the object instance are colliding. 2 hot spots will be placed on each side.

	//UNREFERENCED_PARAMETER(PosX);
	//UNREFERENCED_PARAMETER(PosY);
	//UNREFERENCED_PARAMETER(scaleX);
	//UNREFERENCED_PARAMETER(scaleY);
	//return 0;

	// Variable declaration and initialization to 0
	int Flag{ 0 };
	float x1{ 0 }, y1{ 0 }, x2{ 0 }, y2{ 0 }, x3{}, x4{};

	// Right side hotspots
	// - hotspot 1 (above center line)
	x1 = PosX + scaleX / 2;
	y1 = PosY + scaleY / 4;
	// - hotspot 2 (below center line)
	y2 = PosY - scaleY / 4;

	// Check if any of the hotspot is colliding
	//if (_BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//_BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM || GetCellValue(static_cast<int>(x1), static_cast<int>(y2), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM ||
		GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT || GetCellValue(static_cast<int>(x1), static_cast<int>(y2), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT)
		Flag = (Flag | COLLISION_RIGHT);	// Case is true: OR the Flag variable with the COLLISION_RIGHT const

	// Top side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX - scaleX / 4;
	y1 = PosY + scaleY / 2;
	// - hotspot 2 (right of center line)
	x2 = PosX + scaleX / 4;
	x3 = PosX + scaleX / 32 * 15;
	x4 = PosX - scaleX / 32 * 15;


	// Check if any of the hotspot is colliding
	//if (_BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//_BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM || GetCellValue(static_cast<int>(x2), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM
		|| GetCellValue(static_cast<int>(x3), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM || GetCellValue(static_cast<int>(x4), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM ||
		GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT || GetCellValue(static_cast<int>(x2), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT
		|| GetCellValue(static_cast<int>(x3), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT || GetCellValue(static_cast<int>(x4), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT)
		Flag = (Flag | COLLISION_TOP);	// Case is true: OR the Flag variable with the COLLISION_TOP const


	// Left side hotspots
	// - hotspot 1 (above center line)
	x1 = PosX - scaleX / 2;
	y1 = PosY + scaleY / 4;
	// - hotspot 2 (below center line)
	y2 = PosY - scaleY / 4;

	// Check if any of the hotspot is colliding
	//if (_BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//_BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM || GetCellValue(static_cast<int>(x1), static_cast<int>(y2), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM ||
		GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT || GetCellValue(static_cast<int>(x1), static_cast<int>(y2), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT)
		Flag = (Flag | COLLISION_LEFT);	// Case is true: OR the Flag variable with the COLLISION_LEFT const


	// Bottom side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX - scaleX / 4;
	y1 = PosY - scaleY / 2;
	// - hotspot 2 (right of center line)
	x2 = PosX + scaleX / 4;
	x3 = PosX + scaleX / 32 * 15;
	x4 = PosX - scaleX / 32 * 15;

	// Check if any of the hotspot is colliding
	//if (_BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//_BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM || GetCellValue(static_cast<int>(x2), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM
		|| GetCellValue(static_cast<int>(x3), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM || GetCellValue(static_cast<int>(x4), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM ||
		GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT || GetCellValue(static_cast<int>(x2), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT
		|| GetCellValue(static_cast<int>(x3), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT || GetCellValue(static_cast<int>(x4), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT)
		Flag = (Flag | COLLISION_BOTTOM);	// Case is true: OR the Flag variable with the COLLISION_BOTTOM const

	
	// Return value of Flag
	return Flag;
}

int		CheckInstanceBinaryMapCollision_Bullet(float PosX, float PosY, float scaleX, float scaleY, 
											   int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT, int*** _BinaryCollisionArray)
{
	//At the end of this function, "Flag" will be used to determine which sides
	//of the object instance are colliding. 2 hot spots will be placed on each side.

	//UNREFERENCED_PARAMETER(PosX);
	//UNREFERENCED_PARAMETER(PosY);
	//UNREFERENCED_PARAMETER(scaleX);
	//UNREFERENCED_PARAMETER(scaleY);
	//return 0;

	// Variable declaration and initialization to 0
	int Flag{ 0 };
	float x1{ 0 }, y1{ 0 };// , x2{ 0 }, y2{ 0 }, x3{}, x4{};

	// Right side hotspots
	// - hotspot 1 (above center line)
	x1 = PosX + scaleX / 2;
	y1 = PosY;
	// - hotspot 2 (below center line)

	// Check if any of the hotspot is colliding
	//if (_BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//_BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM)
		Flag = (Flag | COLLISION_RIGHT);	// Case is true: OR the Flag variable with the COLLISION_RIGHT const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT)
	{
		Flag = (Flag | COLLISION_Destructable);
		(*_BinaryCollisionArray)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
		(*_MapData)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
	}


	// Top side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX;
	y1 = PosY + scaleY / 2 + 0.1f;
	// - hotspot 2 (right of center line)


	// Check if any of the hotspot is colliding
	//if (_BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//_BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM)
		Flag = (Flag | COLLISION_TOP);	// Case is true: OR the Flag variable with the COLLISION_TOP const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT)
	{
		Flag = (Flag | COLLISION_Destructable);
		(*_BinaryCollisionArray)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
		(*_MapData)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
	}


	// Left side hotspots
	// - hotspot 1 (above center line)
	x1 = PosX - scaleX / 2;
	y1 = PosY;
	// - hotspot 2 (below center line)

	// Check if any of the hotspot is colliding
	//if (_BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//_BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM)
		Flag = (Flag | COLLISION_LEFT);	// Case is true: OR the Flag variable with the COLLISION_LEFT const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT)
	{
		Flag = (Flag | COLLISION_Destructable);
		(*_BinaryCollisionArray)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
		(*_MapData)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
	}

	// Bottom side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX;
	y1 = PosY - scaleY / 2 - 0.1f;
	// - hotspot 2 (right of center line)

	// Check if any of the hotspot is colliding
	//if (_BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//_BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM)
		Flag = (Flag | COLLISION_BOTTOM);	// Case is true: OR the Flag variable with the COLLISION_BOTTOM const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT)
	{
		Flag = (Flag | COLLISION_Destructable);
		(*_BinaryCollisionArray)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
		(*_MapData)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
	}

	// Return value of Flag
	return Flag;
}

int		CheckInstanceBinaryMapCollision_Dotted(float PosX, float PosY, float scaleX, float scaleY, int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT)
{
	//At the end of this function, "Flag" will be used to determine which sides
	//of the object instance are colliding. 2 hot spots will be placed on each side.

	//UNREFERENCED_PARAMETER(PosX);
	//UNREFERENCED_PARAMETER(PosY);
	//UNREFERENCED_PARAMETER(scaleX);
	//UNREFERENCED_PARAMETER(scaleY);
	//return 0;

	// Variable declaration and initialization to 0
	int Flag{ 0 };
	float x1{ 0 }, y1{ 0 };// , x2{ 0 }, y2{ 0 }, x3{}, x4{};

	// Right side hotspots
	// - hotspot 1 (above center line)
	x1 = PosX + scaleX / 2;
	y1 = PosY;
	// - hotspot 2 (below center line)

	// Check if any of the hotspot is colliding
	//if (_BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//_BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM)
		Flag = (Flag | COLLISION_RIGHT);	// Case is true: OR the Flag variable with the COLLISION_RIGHT const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT)
	{
		Flag = (Flag | COLLISION_Destructable);
	}


	// Top side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX;
	y1 = PosY + scaleY / 2 + 0.1f;
	// - hotspot 2 (right of center line)


	// Check if any of the hotspot is colliding
	//if (_BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//_BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM)
		Flag = (Flag | COLLISION_TOP);	// Case is true: OR the Flag variable with the COLLISION_TOP const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT)
	{
		Flag = (Flag | COLLISION_Destructable);
	}


	// Left side hotspots
	// - hotspot 1 (above center line)
	x1 = PosX - scaleX / 2;
	y1 = PosY;
	// - hotspot 2 (below center line)

	// Check if any of the hotspot is colliding
	//if (_BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//_BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM)
		Flag = (Flag | COLLISION_LEFT);	// Case is true: OR the Flag variable with the COLLISION_LEFT const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT)
	{
		Flag = (Flag | COLLISION_Destructable);
	}

	// Bottom side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX;
	y1 = PosY - scaleY / 2 - 0.1f;
	// - hotspot 2 (right of center line)

	// Check if any of the hotspot is colliding
	//if (_BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//_BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_PLATFORM)
		Flag = (Flag | COLLISION_BOTTOM);	// Case is true: OR the Flag variable with the COLLISION_BOTTOM const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), _MapData, _BINARY_MAP_WIDTH, _BINARY_MAP_HEIGHT) == TYPE_DIRT)
	{
		Flag = (Flag | COLLISION_Destructable);
	}

	// Return value of Flag
	return Flag;
}

/******************************************************************************/
/*!
	Free Allocated Map Data
*/
/******************************************************************************/
void	FreeMapData(int*** _MapData, int*** _BinaryCollisionArray, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT)
{
	// Inform Intellisense of unused parameter
	UNREFERENCED_PARAMETER(_BINARY_MAP_WIDTH);

	// Loop through the rows in MapData & BinaryCollisionArray
	for (int i = 0; i < _BINARY_MAP_HEIGHT; i++) {
		delete[](*_MapData)[i];					// Free allocated row to MapData
		delete[](*_BinaryCollisionArray)[i];	// Free allocated row to BinaryCollisionArray
	}

	// Free memory allocated to MapData & BinaryCollisionArray
	delete[] *_MapData;
	delete[] *_BinaryCollisionArray;
}