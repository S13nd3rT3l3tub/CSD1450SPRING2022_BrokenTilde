/*!
@Copyright	Copyright © 2022 DigiPen, All rights reserved.
@file       BinaryMap.cpp
-------------------------------------------------------------------------------
@author     Lee Hsien Wei, Joachim (l.hsienweijoachim@digipen.edu)
@role		Authored Functions
-------------------------------------------------------------------------------
@author		Mohamed Zafir (m.zafir@digipen.edu)
@role		Authored Functions
-------------------------------------------------------------------------------
@author		Leong Wai Kit (l.waikit@digipen.edu)
@role		Authored Functions
*//*_________________________________________________________________________*/
#include "Main.h"

int ImportMapDataFromFile(std::string FileName, int*** _MapData, int*** _BinaryCollisionArray, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT)
{
	std::fstream fs(FileName, std::ios_base::in);
	if (!fs)
	{
		std::cout << "File " << FileName << " not found.\n";
		return 0;
	}
	std::string filler;
	fs >> filler >> _BINARY_MAP_WIDTH; // Read width
	fs >> filler >> _BINARY_MAP_HEIGHT; // Read height

	//Allocate memory for _MapData
	(*_MapData) = new int* [_BINARY_MAP_HEIGHT];
	for (int i = 0; i < _BINARY_MAP_HEIGHT; ++i)
	{
		(*_MapData)[i] = new int[_BINARY_MAP_WIDTH];
	}

	//Allocate memory for _BinaryCollisionArray
	(*_BinaryCollisionArray) = new int* [_BINARY_MAP_HEIGHT];
	for (int i = 0; i < _BINARY_MAP_HEIGHT; ++i)
	{
		(*_BinaryCollisionArray)[i] = new int[_BINARY_MAP_WIDTH];
	}

	int hold{};
	for (int i = 0; i < _BINARY_MAP_HEIGHT; ++i)
	{
		for (int u = 0; u < _BINARY_MAP_WIDTH; ++u)
		{
			fs >> hold; // Read ints from file
			(*_MapData)[i][u] = hold;
			if (hold == 0)
			{
				(*_BinaryCollisionArray)[i][u] = 0; // if int is more than 1
			}
			else if(hold == 1)
			{
				(*_BinaryCollisionArray)[i][u] = 1;
			}
			else if (hold == 8)
			{
				(*_BinaryCollisionArray)[i][u] = 8;
			}
			else
			{
				(*_BinaryCollisionArray)[i][u] = 0; // if int is more than 1
			}
		}
	}

	fs.close();

	return 1;
}

int	GetCellValue(int X, int Y, int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT)
{
	//	Check if value is outside of binary map grid
	if (X < 0 || X >= _BINARY_MAP_WIDTH ||
		Y < 0 || Y >= _BINARY_MAP_HEIGHT)
		return 0;

	return (*_MapData)[Y][X];
}

void	SnapToCell(float* Coordinate)
{
	// Snap by casting it to integer and adding 0.5f
	// May need to be changed depending on the cell
	*Coordinate = static_cast<int>(*Coordinate) + 0.5f;
}

void	PrintRetrievedInformation(int*** _MapData, int*** _BinaryCollisionArray, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT)
{
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

int		CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY, int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT)
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
	//std::cout << "Left Check\n";
	//std::cout << "hs1 : (" << x1 << ", " << y1 << ") = " << _BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] << " | ";
	//std::cout << "hs2 : (" << x2 << ", " << y2 << ") = " << _BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)];
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

	//std::cout << std::endl;

	// Return value of Flag
	return Flag;
}

int		CheckInstanceBinaryMapCollision_bullet(float PosX, float PosY, float scaleX, float scaleY, int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT, int*** _BinaryCollisionArray)
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
	//std::cout << "Left Check\n";
	//std::cout << "hs1 : (" << x1 << ", " << y1 << ") = " << _BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] << " | ";
	//std::cout << "hs2 : (" << x2 << ", " << y2 << ") = " << _BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)];
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

int		CheckInstanceBinaryMapCollision_dotted(float PosX, float PosY, float scaleX, float scaleY, int*** _MapData, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT)
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
	//std::cout << "Left Check\n";
	//std::cout << "hs1 : (" << x1 << ", " << y1 << ") = " << _BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] << " | ";
	//std::cout << "hs2 : (" << x2 << ", " << y2 << ") = " << _BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)];
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

void	FreeMapData(int*** _MapData, int*** _BinaryCollisionArray, int& _BINARY_MAP_WIDTH, int& _BINARY_MAP_HEIGHT)
{
	for (int i = 0; i < _BINARY_MAP_HEIGHT; i++)
	{
		delete[](*_MapData)[i];
		delete[](*_BinaryCollisionArray)[i];
	}

	delete[] * _MapData;
	delete[] * _BinaryCollisionArray;
}