#include <iostream>
#include "Block.cpp"

int main()
{
	Block temp(3);

	temp.matrix[0][0] = true;
	temp.matrix[0][1] = true;
	temp.matrix[0][2] = true;

	temp.RotateLeft();
	temp.RotateRight();

	std::cout << std::boolalpha;

	for (size_t i = 0; i < temp.size; i++) {
		for (size_t j = 0; j < temp.size; j++) {
			std::cout << temp.matrix[i][j] << '|';
		}std::cout << '\n';
	}

}
