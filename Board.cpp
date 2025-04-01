#include <unordered_map>
#include "raylib.h"

class Board
{
public:
	int height = 21;
	int width = 10;

	const int cubeWidth = 42;

	const std::unordered_map<char, Color> colors = {
	{'I', SKYBLUE},
	{'J', DARKBLUE},
	{'L', ORANGE},
	{'O', YELLOW},
	{'S', GREEN},
	{'T', VIOLET},
	{'Z', RED},
	{' ', Color{27, 31, 42, 150}},
	};

	char** matrix;

	int xOffset, yOffset;

	Board(int x, int y): xOffset(x), yOffset(y)
	{
		matrix = new char* [height];
		for (size_t i = 0; i < height; i++) {
			matrix[i] = new char[width];
			for (size_t j = 0; j < width; j++)
				matrix[i][j] = ' ';
		}
	}

	~Board()
	{
		for (size_t i = 0; i < height; i++)
			delete[] matrix[i];
		delete[] matrix;
		matrix = nullptr;
	}
};