#ifndef  GAMEUTILS_H
#define GAMEUTILS_H

#include "Board.cpp"
#include "Tetrominos.h"

class GameUtils
{
public:
	const int width = 800;
	const int height = 900;

	const int FPS = 60;
	const int speed = 4;

	float multiplier = 1;
	float xDirection{};

	int frameCount{};

	const Board* board;

	std::unique_ptr<Block> currentTetromino;
	std::unique_ptr<Block> nextTetromino;

	GameUtils();
	~GameUtils();
	void RunGame();
	void DrawMatrix();
	void DisplayNextTetromino();
	void Replace();
	bool MoveDown();
	void DisplayTetromino();
	bool HitEndOrOtherBLock();
	void InsertIntoMatrix();
	void MoveTetromino();
	bool CanGoLeftOrRight();
};

#endif
