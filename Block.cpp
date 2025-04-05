#include <iostream>

class Block
{
public:
	char** matrix;
	size_t size;

	float xBlockOffset;
	float yBlockOffset;

	Block() = delete;

	Block(size_t blocks_size): xBlockOffset(84.0f), yBlockOffset(((int)size - 1) * -42.0f), size(blocks_size)
	{
		matrix = new char*[size];

		for (size_t i = 0; i < size; i++) {
			matrix[i] = new char[size];
			for (size_t j = 0; j < size; j++) {
				matrix[i][j] = ' ';
			}
		}
	}
	virtual ~Block()
	{
		for (size_t i = 0; i < size; i++) {
			delete[] matrix[i];
		}
		delete[] matrix;
		matrix = nullptr;
	}

	void virtual RotateLeft() {
		Block temp(this->size);

		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				temp.matrix[size - 1 - j][i] = this->matrix[i][j];
			}
		}

		Copy(temp.matrix, size);
	}

	void virtual RotateRight() {
		Block temp(this->size);

		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				temp.matrix[j][size - 1 - i] = this->matrix[i][j];
			}
		}

		Copy(temp.matrix, size);
	}

	void Copy(char** matrix, size_t size) {
		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				this->matrix[i][j] = matrix[i][j];
			}
		}
	}

	int TrueSizeUpY()
	{
		int index{};

		for (int i = size - 1; i >= 0; i--) {
			bool flag = false;
			for (size_t j = 0; j < size; j++) {
				if (matrix[i][j] != ' ')
					flag = true;
			}

			if (flag) {
				index = i;
			}
		}
		return index;
	}

	int TrueSizeDownY()
	{
		int index{};

		for (int i = 0; i < size; i++) {
			bool flag = false;
			for (size_t j = 0; j < size; j++) {
				if (matrix[i][j] != ' ')
					flag = true;
			}

			if (flag) {
				index = i;
			}
		}
		return index;
	}

	int TrueSizeLeftX() {
		int index{};

		for (int i = size - 1; i >= 0; i--) {
			bool flag = false;
			for (size_t j = 0; j < size; j++) {
				if (matrix[j][i] != ' ')
					flag = true;
			}

			if (flag) {
				index = i;
			}
		}
		return index;
	}

	int TrueSizeRightX() {
		int index{};

		for (int i = 0; i < size; i++) {
			bool flag = false;
			for (size_t j = 0; j < size; j++) {
				if (matrix[j][i] != ' ') {
					flag = true;
					break;
				}
			}

			if (flag) {
				index = i;
			}
		}
		return index;
	}
};