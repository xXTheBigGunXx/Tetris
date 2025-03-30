class Block
{
public:
	bool** matrix;
	size_t size;

	Block(size_t blocks_size)
	{
		size = blocks_size;
		matrix = new bool*[size];

		for (size_t i = 0; i < size; i++) {
			matrix[i] = new bool[size];
			for (size_t j = 0; j < size; j++) {
				matrix[i][j] = false;
			}
		}
	}
	~Block()
	{
		for (size_t i = 0; i < size; i++) {
			delete[] matrix[i];
		}
		delete[] matrix;
		matrix = nullptr;
	}

	void RotateLeft() {
		Block temp(this->size);

		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				temp.matrix[size - 1 - j][i] = this->matrix[i][j];
			}
		}

		Copy(temp.matrix, size);
	}

	void RotateRight() {
		Block temp(this->size);

		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				temp.matrix[j][i] = this->matrix[i][j];
			}
		}

		Copy(temp.matrix, size);
	}

	void Copy(bool** matrix, size_t size) {
		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				this->matrix[i][j] = matrix[i][j];
			}
		}
	}
};