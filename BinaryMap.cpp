#include "main.h"

/*The number of horizontal elements*/
int BINARY_MAP_WIDTH;
int BINARY_CELL_WIDTH;

/*The number of vertical elements*/
int BINARY_MAP_HEIGHT;
int BINARY_CELL_HEIGHT;

/*This will contain all the data of the map, which will be retreived from a file
when the "ImportMapDataFromFile" function is called*/
int** MapData;

/*This will contain the collision data of the binary map. It will be filled in the
"ImportMapDataFromFile" after filling "MapData". Basically, if an array element
in MapData is 1, it represents a collision cell, any other value is a non-collision
cell*/
int** BinaryCollisionArray;

int ImportMapDataFromFile(std::string FileName)
{
	//std::vector<int>	vint;
	//std::string			line;

	////	This function opens the file name "FileName" and retrieves all the map data.
	//std::fstream input(FileName, std::ios_base::in);

	////	function returns 0 if the file named "FileName" does not exists
	//if (!input)
	//{
	//	std::cout << "File " << FileName << " not found.\n";
	//	return 0;
	//}

	////	The first line in this file is the width of the map.
	////	The second line in this file is the height of the map.
	//for (int i = 0; i < 2; i++)
	//{
	//	std::getline(input, line);
	//	//	Convert char into int
	//	int temp = (line.back() - '0');
	//	vint.push_back(temp);
	//}
	//BINARY_MAP_WIDTH = vint[0];
	//BINARY_MAP_HEIGHT = vint[1];

	////	It allocates memory for the 2 arrays: MapData & BinaryCollisionArray
	//MapData = new int* [BINARY_MAP_HEIGHT];
	//BinaryCollisionArray = new int* [BINARY_MAP_HEIGHT];

	//for (int i = 0; i < BINARY_MAP_HEIGHT; i++)
	//{
	//	MapData[i] = new int[BINARY_MAP_WIDTH];
	//	BinaryCollisionArray[i] = new int[BINARY_MAP_WIDTH];
	//}

	///*
	//	The remaining part of the file is a series of numbers
	//	Each number represents the ID(or value) of a different element in the
	//	double dimensionaly array.
	//*/
	//for (int row = 0; row < BINARY_MAP_HEIGHT; row++)
	//{
	//	//	Get each line from the input
	//	std::getline(input, line);
	//	//	Remove any spacing from the input
	//	line.erase(remove(line.begin(), line.end(), ' '), line.end());

	//	for (int col = 0; col < BINARY_MAP_WIDTH; col++)
	//	{
	//		int idata = (line[col] - '0');
	//		MapData[row][col] = idata;
	//		//	Convert all objects into empty for binary collision array
	//		if (idata > 1)
	//			idata = 0;
	//		BinaryCollisionArray[row][col] = idata;
	//	}
	//}
	////	function returns 1 if the file named "FileName" exists
	//return 1;

	// Const string containing numbers
	static const std::string numbers{ "0123456789" };

	// Variable declaration and initialization
	std::ifstream ifs{ FileName };		// Open the given file
	std::string widthStr{ 0 }, heightStr{ 0 }, currentStr{ 0 };	// Declare and set to 0

	// Check if given file was not opened successfully
	if (!ifs)
		return 0;	// Case is true: Error encountered; Return 0;

	// ----- Read Width & Height from file ----- 
	// Read the 1st line containing the width
	getline(ifs, widthStr);
	// Read the 2nd line containing the height
	getline(ifs, heightStr);
	// Extract the number from the width string
	widthStr = widthStr.substr(widthStr.find_first_of(numbers), widthStr.find_last_of(numbers) - widthStr.find_first_of(numbers) + 1);
	// Extract the number from the height string
	heightStr = heightStr.substr(heightStr.find_first_of(numbers), heightStr.find_last_of(numbers) - heightStr.find_first_of(numbers) + 1);

	// Assign the width and height of the extracted width and height to the respective variables
	BINARY_MAP_WIDTH = std::stoi(widthStr);
	BINARY_MAP_HEIGHT = std::stoi(heightStr);

	// ----- Creation of MapData & BinaryCollisionArray ------
	// Allocate memory for the rows of int pointers of both arrays
	MapData = new int* [BINARY_MAP_HEIGHT];
	BinaryCollisionArray = new int* [BINARY_MAP_HEIGHT];

	// Loop through each row
	for (int i = 0; i < BINARY_MAP_HEIGHT; ++i) {
		// Allocate memory for the columns of int of both arrays
		MapData[i] = new int[BINARY_MAP_WIDTH];
		BinaryCollisionArray[i] = new int[BINARY_MAP_WIDTH];
	}

	// Variable declaration and initialization to 0
	int rowCount{ 0 }, colCount{ 0 };

	// Read the rest of the file
	while (ifs >> currentStr) {
		// Assign the read value to the Mapdata
		MapData[rowCount][colCount] = std::stoi(currentStr);
		// Assign either 1 or 0 to the binary collision data depending on the read value
		// Read value:
		// - More than 1 > store 0
		// - 1 > Store 1
		// - 0 > Store 0
		BinaryCollisionArray[rowCount][colCount] = std::stoi(currentStr) > 1 ? 0 : std::stoi(currentStr);

		// Increment column Count and check if it would be the same or larger than the width
		if (++colCount >= BINARY_MAP_WIDTH) {
			// Increment row count and check if it would be same or larger than the height
			if (++rowCount >= BINARY_MAP_HEIGHT)
				break;	// Break out of while loop; potential error if there is more data after this
			// Set column back to 0 (start of the next row)
			colCount = 0;
		}
	}

	// Close the input file
	ifs.close();

	// Return successfully read
	return 1;
}

void PrintRetrievedInformation(void)
{

	std::cout << "WIDTH: " << BINARY_MAP_WIDTH << "\nHEIGHT: " << BINARY_MAP_HEIGHT << std::endl;

	// ----- Map Data -----
	std::cout << "Map Data\n";	// Console formatting
	// Loop through each row
	for (int j = 0; j < BINARY_MAP_HEIGHT; ++j) {
		// Loop through each column
		for (int i = 0; i < BINARY_MAP_WIDTH; ++i)
			std::cout << MapData[j][i] << " ";	// Print out value in column
		// End of row reached; print a new line for next row
		std::cout << std::endl;
	}

	// ----- Binary Collision Data -----
	std::cout << "\nBinary Collision Data\n";	// Console formatting
	// Loop through each row
	for (int j = 0; j < BINARY_MAP_HEIGHT; ++j) {
		// Loop through each column
		for (int i = 0; i < BINARY_MAP_WIDTH; ++i)
			std::cout << BinaryCollisionArray[j][i] << " ";	// Print out value in column
		// End of row reached; print a new line for next row
		std::cout << std::endl;
	}
}

void FreeMapData(void)
{
	for (int i = 0; i < BINARY_MAP_HEIGHT; i++)
	{
		delete[] MapData[i];
		delete[] BinaryCollisionArray[i];
	}

	delete[] MapData;
	delete[] BinaryCollisionArray;
}

int GetCellValue(int X, int Y)
{
	//	Check if value is outside of binary map grid
	if (X < 0 || X >= BINARY_MAP_WIDTH ||
		Y < 0 || Y >= BINARY_MAP_HEIGHT)
		return 0;

	return MapData[Y][X];
}

void SnapToCell(float* Coordinate)
{
	// Snap by casting it to integer and adding 0.5f
	// May need to be changed depending on the cell
	*Coordinate = static_cast<int>(*Coordinate) + 0.5f;
}

int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY)
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
	x2 = PosX + scaleX / 2;
	y2 = PosY - scaleY / 4;

	// Check if any of the hotspot is colliding
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1)) == 1 || GetCellValue(static_cast<int>(x2), static_cast<int>(y2)) == 1)
		Flag = (Flag | COLLISION_RIGHT);	// Case is true: OR the Flag variable with the COLLISION_RIGHT const

	// Top side hotspots
	// - hotspot 1 (left of center line)
	x1 = PosX - scaleX / 4;
	y1 = PosY + scaleY / 2;
	// - hotspot 2 (right of center line)
	x2 = PosX + scaleX / 4;
	y2 = PosY + scaleY / 2;

	// Check if any of the hotspot is colliding
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1)) == 1 || GetCellValue(static_cast<int>(x2), static_cast<int>(y2)) == 1)
		Flag = (Flag | COLLISION_TOP);	// Case is true: OR the Flag variable with the COLLISION_TOP const


	// Left side hotspots
	// - hotspot 1 (above center line)
	x1 = PosX - scaleX / 2;
	y1 = PosY + scaleY / 4;
	// - hotspot 2 (below center line)
	x2 = PosX - scaleX / 2;
	y2 = PosY - scaleY / 4;

	// Check if any of the hotspot is colliding
	//std::cout << "Left Check\n";
	//std::cout << "hs1 : (" << x1 << ", " << y1 << ") = " << BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] << " | ";
	//std::cout << "hs2 : (" << x2 << ", " << y2 << ") = " << BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)];
	//if (BinaryCollisionArray[static_cast<int>(y1)][static_cast<int>(x1)] ||
		//BinaryCollisionArray[static_cast<int>(y2)][static_cast<int>(x2)])
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1)) == 1 || GetCellValue(static_cast<int>(x2), static_cast<int>(y2)) == 1)
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
	if (GetCellValue(static_cast<int>(x1), static_cast<int>(y1)) == 1 || GetCellValue(static_cast<int>(x2), static_cast<int>(y1)) == 1
		|| GetCellValue(static_cast<int>(x3), static_cast<int>(y1)) == 1 || GetCellValue(static_cast<int>(x4), static_cast<int>(y1)) == 1)
		Flag = (Flag | COLLISION_BOTTOM);	// Case is true: OR the Flag variable with the COLLISION_BOTTOM const

	//std::cout << std::endl;

	// Return value of Flag
	return Flag;
}