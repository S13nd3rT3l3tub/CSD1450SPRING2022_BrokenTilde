#include "BinaryMap.h"

/*The number of horizontal elements*/
int BINARY_MAP_WIDTH;

/*The number of vertical elements*/
int BINARY_MAP_HEIGHT;

/*This will contain all the data of the map, which will be retreived from a file
when the "ImportMapDataFromFile" function is called*/
int** MapData;

/*This will contain the collision data of the binary map. It will be filled in the
"ImportMapDataFromFile" after filling "MapData". Basically, if an array element
in MapData is 1, it represents a collision cell, any other value is a non-collision
cell*/
int** BinaryCollisionArray;

int ImportMapDataFromFile(const char* FileName)
{
	std::vector<int>	vint;
	std::string			line;

	//	This function opens the file name "FileName" and retrieves all the map data.
	std::fstream input(FileName, std::ios_base::in);

	//	function returns 0 if the file named "FileName" does not exists
	if (!input)
	{
		std::cout << "File " << FileName << " not found.\n";
		return 0;
	}

	//	The first line in this file is the width of the map.
	//	The second line in this file is the height of the map.
	for (int i = 0; i < 2; i++)
	{
		std::getline(input, line);
		//	Convert char into int
		int temp = (line.back() - '0');
		vint.push_back(temp);
	}
	BINARY_MAP_WIDTH = vint[0];
	BINARY_MAP_HEIGHT = vint[1];

	//	It allocates memory for the 2 arrays: MapData & BinaryCollisionArray
	MapData = new int* [BINARY_MAP_HEIGHT];
	BinaryCollisionArray = new int* [BINARY_MAP_HEIGHT];

	for (int i = 0; i < BINARY_MAP_HEIGHT; i++)
	{
		MapData[i] = new int[BINARY_MAP_WIDTH];
		BinaryCollisionArray[i] = new int[BINARY_MAP_WIDTH];
	}

	/*
		The remaining part of the file is a series of numbers
		Each number represents the ID(or value) of a different element in the
		double dimensionaly array.
	*/
	for (int row = 0; row < BINARY_MAP_HEIGHT; row++)
	{
		//	Get each line from the input
		std::getline(input, line);
		//	Remove any spacing from the input
		line.erase(remove(line.begin(), line.end(), ' '), line.end());

		for (int col = 0; col < BINARY_MAP_WIDTH; col++)
		{
			int idata = (line[col] - '0');
			MapData[row][col] = idata;
			//	Convert all objects into empty for binary collision array
			if (idata > 1)
				idata = 0;
			BinaryCollisionArray[row][col] = idata;
		}
	}
	//	function returns 1 if the file named "FileName" exists
	return 1;
}

int GetCellValue(int X, int Y)
{
	//	Check if value is outside of binary map grid
	if (X < 0 || X >= BINARY_MAP_WIDTH ||
		Y < 0 || Y >= BINARY_MAP_HEIGHT)
		return 0;

	return BinaryCollisionArray[Y][X];
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