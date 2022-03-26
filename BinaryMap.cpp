#include "main.h"
#include "BinaryMap.h"

////Binary map data
//static int** MapData;
//static int** BinaryCollisionArray;
//static int	BINARY_MAP_WIDTH;
//static int	BINARY_MAP_HEIGHT;

int ImportMapDataFromFile(std::string FileName, int*** MapData, int*** BinaryCollisionArray, int& BINARY_MAP_WIDTH, int& BINARY_MAP_HEIGHT)
{
	std::fstream fs(FileName, std::ios_base::in);
	if (!fs)
	{
		std::cout << "File " << FileName << " not found.\n";
		return 0;
	}
	std::string filler;
	fs >> filler >> BINARY_MAP_WIDTH; // Read width
	fs >> filler >> BINARY_MAP_HEIGHT; // Read height

	//Allocate memory for MapData
	(*MapData) = new int* [BINARY_MAP_HEIGHT];
	for (int i = 0; i < BINARY_MAP_HEIGHT; ++i)
	{
		(*MapData)[i] = new int[BINARY_MAP_WIDTH];
	}

	//Allocate memory for BinaryCollisionArray
	(*BinaryCollisionArray) = new int* [BINARY_MAP_HEIGHT];
	for (int i = 0; i < BINARY_MAP_HEIGHT; ++i)
	{
		(*BinaryCollisionArray)[i] = new int[BINARY_MAP_WIDTH];
	}

	int hold{};
	for (int i = 0; i < BINARY_MAP_HEIGHT; ++i)
	{
		for (int u = 0; u < BINARY_MAP_WIDTH; ++u)
		{
			fs >> hold; // Read ints from file
			(*MapData)[i][u] = hold;
			if (hold == 0)
			{
				(*BinaryCollisionArray)[i][u] = 0; // if int is more than 1
			}
			else if(hold == 1)
			{
				(*BinaryCollisionArray)[i][u] = 1;
			}
			else if (hold == 8)
			{
				(*BinaryCollisionArray)[i][u] = 8;
			}
			else
			{
				(*BinaryCollisionArray)[i][u] = 0; // if int is more than 1
			}
		}
	}
	return 1;
}

int	GetCellValue(int X, int Y, int*** MapData, int& BINARY_MAP_WIDTH, int& BINARY_MAP_HEIGHT)
{
	//	Check if value is outside of binary map grid
	if (X < 0 || X >= BINARY_MAP_WIDTH ||
		Y < 0 || Y >= BINARY_MAP_HEIGHT)
		return 0;

	return (*MapData)[Y][X];
}

void	SnapToCell(float* Coordinate)
{
	// Snap by casting it to integer and adding 0.5f
	// May need to be changed depending on the cell
	*Coordinate = static_cast<int>(*Coordinate) + 0.5f;
}

void	PrintRetrievedInformation(int*** MapData, int*** BinaryCollisionArray, int& BINARY_MAP_WIDTH, int& BINARY_MAP_HEIGHT)
{
	std::cout << "WIDTH: " << BINARY_MAP_WIDTH << "\nHEIGHT: " << BINARY_MAP_HEIGHT << std::endl;

	// ----- Map Data -----
	std::cout << "Map Data\n";	// Console formatting
	// Loop through each row
	for (int j = 0; j < BINARY_MAP_HEIGHT; ++j) {
		// Loop through each column
		for (int i = 0; i < BINARY_MAP_WIDTH; ++i)
			std::cout << (*MapData)[j][i] << " ";	// Print out value in column
		// End of row reached; print a new line for next row
		std::cout << std::endl;
	}

	// ----- Binary Collision Data -----
	std::cout << "\nBinary Collision Data\n";	// Console formatting
	// Loop through each row
	for (int j = 0; j < BINARY_MAP_HEIGHT; ++j) {
		// Loop through each column
		for (int i = 0; i < BINARY_MAP_WIDTH; ++i)
			std::cout << (*BinaryCollisionArray)[j][i] << " ";	// Print out value in column
		// End of row reached; print a new line for next row
		std::cout << std::endl;
	}
}

int		CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY, int*** MapData, int& BINARY_MAP_WIDTH, int& BINARY_MAP_HEIGHT)
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
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1 || GetCellValue(static_cast<int>(x1), static_cast<int>(y2), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1 ||
		GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8 || GetCellValue(static_cast<int>(x1), static_cast<int>(y2), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8)
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
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1 || GetCellValue(static_cast<int>(x2), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1
		|| GetCellValue(static_cast<int>(x3), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1 || GetCellValue(static_cast<int>(x4), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1 ||
		GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8 || GetCellValue(static_cast<int>(x2), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8
		|| GetCellValue(static_cast<int>(x3), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8 || GetCellValue(static_cast<int>(x4), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8)
		Flag = (Flag | COLLISION_TOP);	// Case is true: OR the Flag variable with the COLLISION_TOP const


	// Left side hotspots
	// - hotspot 1 (above center line)
	x1 = PosX - scaleX / 2;
	y1 = PosY + scaleY / 4;
	// - hotspot 2 (below center line)
	y2 = PosY - scaleY / 4;

	// Check if any of the hotspot is colliding
	//std::cout << "Left Check\n";
	//std::cout << "hs1 : (" << x1 << ", " << y1 << ") = " << BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] << " | ";
	//std::cout << "hs2 : (" << x2 << ", " << y2 << ") = " << BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)];
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1 || GetCellValue(static_cast<int>(x1), static_cast<int>(y2), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1 ||
		GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8 || GetCellValue(static_cast<int>(x1), static_cast<int>(y2), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8)
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
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1 || GetCellValue(static_cast<int>(x2), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1
		|| GetCellValue(static_cast<int>(x3), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1 || GetCellValue(static_cast<int>(x4), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1 ||
		GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8 || GetCellValue(static_cast<int>(x2), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8
		|| GetCellValue(static_cast<int>(x3), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8 || GetCellValue(static_cast<int>(x4), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8)
		Flag = (Flag | COLLISION_BOTTOM);	// Case is true: OR the Flag variable with the COLLISION_BOTTOM const

	//std::cout << std::endl;

	// Return value of Flag
	return Flag;
}

int		CheckInstanceBinaryMapCollision_bullet(float PosX, float PosY, float scaleX, float scaleY, int*** MapData, int& BINARY_MAP_WIDTH, int& BINARY_MAP_HEIGHT, int*** BinaryCollisionArray)
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
	y1 = PosY;
	// - hotspot 2 (below center line)

	// Check if any of the hotspot is colliding
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1)
		Flag = (Flag | COLLISION_RIGHT);	// Case is true: OR the Flag variable with the COLLISION_RIGHT const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8)
	{
		Flag = (Flag | COLLISION_Destructable);
		(*BinaryCollisionArray)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
		(*MapData)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
	}


	// Top side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX;
	y1 = PosY + scaleY / 2 + 0.1f;
	// - hotspot 2 (right of center line)


	// Check if any of the hotspot is colliding
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1)
		Flag = (Flag | COLLISION_TOP);	// Case is true: OR the Flag variable with the COLLISION_TOP const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8)
	{
		Flag = (Flag | COLLISION_Destructable);
		(*BinaryCollisionArray)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
		(*MapData)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
	}


	// Left side hotspots
	// - hotspot 1 (above center line)
	x1 = PosX - scaleX / 2;
	y1 = PosY;
	// - hotspot 2 (below center line)

	// Check if any of the hotspot is colliding
	//std::cout << "Left Check\n";
	//std::cout << "hs1 : (" << x1 << ", " << y1 << ") = " << BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] << " | ";
	//std::cout << "hs2 : (" << x2 << ", " << y2 << ") = " << BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)];
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1)
		Flag = (Flag | COLLISION_LEFT);	// Case is true: OR the Flag variable with the COLLISION_LEFT const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8)
	{
		Flag = (Flag | COLLISION_Destructable);
		(*BinaryCollisionArray)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
		(*MapData)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
	}

	// Bottom side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX;
	y1 = PosY - scaleY / 2 - 0.1f;
	// - hotspot 2 (right of center line)

	// Check if any of the hotspot is colliding
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1)
		Flag = (Flag | COLLISION_BOTTOM);	// Case is true: OR the Flag variable with the COLLISION_BOTTOM const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8)
	{
		Flag = (Flag | COLLISION_Destructable);
		(*BinaryCollisionArray)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
		(*MapData)[static_cast<int>(y1)][static_cast<int>(x1)] = 0;
	}

	// Return value of Flag
	return Flag;
}

int		CheckInstanceBinaryMapCollision_dotted(float PosX, float PosY, float scaleX, float scaleY, int*** MapData, int& BINARY_MAP_WIDTH, int& BINARY_MAP_HEIGHT)
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
	y1 = PosY;
	// - hotspot 2 (below center line)

	// Check if any of the hotspot is colliding
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1)
		Flag = (Flag | COLLISION_RIGHT);	// Case is true: OR the Flag variable with the COLLISION_RIGHT const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8)
	{
		Flag = (Flag | COLLISION_Destructable);
	}


	// Top side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX;
	y1 = PosY + scaleY / 2 + 0.1f;
	// - hotspot 2 (right of center line)


	// Check if any of the hotspot is colliding
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1)
		Flag = (Flag | COLLISION_TOP);	// Case is true: OR the Flag variable with the COLLISION_TOP const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8)
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
	//std::cout << "hs1 : (" << x1 << ", " << y1 << ") = " << BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] << " | ";
	//std::cout << "hs2 : (" << x2 << ", " << y2 << ") = " << BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)];
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1)
		Flag = (Flag | COLLISION_LEFT);	// Case is true: OR the Flag variable with the COLLISION_LEFT const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8)
	{
		Flag = (Flag | COLLISION_Destructable);
	}

	// Bottom side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX;
	y1 = PosY - scaleY / 2 - 0.1f;
	// - hotspot 2 (right of center line)

	// Check if any of the hotspot is colliding
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 1)
		Flag = (Flag | COLLISION_BOTTOM);	// Case is true: OR the Flag variable with the COLLISION_BOTTOM const

	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1), MapData, BINARY_MAP_WIDTH, BINARY_MAP_HEIGHT) == 8)
	{
		Flag = (Flag | COLLISION_Destructable);
	}

	// Return value of Flag
	return Flag;
}

void	FreeMapData(int*** MapData, int*** BinaryCollisionArray, int& BINARY_MAP_WIDTH, int& BINARY_MAP_HEIGHT)
{
	for (int i = 0; i < BINARY_MAP_HEIGHT; i++)
	{
		delete[](*MapData)[i];
		delete[](*BinaryCollisionArray)[i];
	}

	delete[] * MapData;
	delete[] * BinaryCollisionArray;
}