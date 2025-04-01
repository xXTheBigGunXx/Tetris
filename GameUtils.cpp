#include "GameUtils.h"
#include "raylib.h"
#include "TaskUtils.h"
#include <iostream>

GameUtils::GameUtils()
{
	SetTargetFPS(FPS);
	InitWindow(width, height, "Tetris game!");

	board = new Board(10,10);

	currentTetromino = ReturnObj();
	nextTetromino = ReturnObj();

	currentTetromino->xBlockOffset;

	//std::cout << std::boolalpha;
}

GameUtils::~GameUtils()
{
	delete board;
}

void GameUtils::RunGame()
{
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(Color{ 59, 85, 162 , 255 });

		TaskUtils::Direction();

		DrawMatrix();
		
		if (frameCount == FPS / speed){

			//std::cout << TaskUtils::xDir << ' ' << TaskUtils::rotate << ' ' << TaskUtils::oneDown << ' ' << TaskUtils::goDown << '\n';
			//MoveTetromino();

			MoveDown();
			frameCount = 0;
			TaskUtils::Reset();
		}

		MoveDown();

		currentTetromino->yBlockOffset;
		
		DisplayNextTetromino();
		DisplayTetromino();
		EndDrawing();
		frameCount++;
	}
}

void GameUtils::DrawMatrix()
{
	for (size_t i = 0; i < board->height; i++) {
		for (size_t j = 0; j < board->width; j++) {
			Rectangle rect = {
					j * board->cubeWidth + board->xOffset,
					i * board->cubeWidth + board->yOffset,
					board->cubeWidth,
					board->cubeWidth,
			};

			DrawRectangle(j * board->cubeWidth + board->xOffset, i * board->cubeWidth + board->yOffset, board->cubeWidth, board->cubeWidth, board->colors.at(board->matrix[i][j]));
			DrawRectangleLinesEx(rect, 1.5f, Color{ 26, 32, 43 , 100});
		}
	}
}

void GameUtils::DisplayNextTetromino()
{
	for (size_t i = 0; i < nextTetromino->size; i++) {
		for (size_t j = 0; j < nextTetromino->size; j++) {
			if (nextTetromino->matrix[i][j] != ' ') {
				Rectangle rect = {
					j * board->cubeWidth + 500,
					i * board->cubeWidth + 445,
					board->cubeWidth,
					board->cubeWidth
				};
				DrawRectangle(j * board->cubeWidth + 500, i * board->cubeWidth + 445, board->cubeWidth, board->cubeWidth, board->colors.at(nextTetromino->matrix[i][j]));
				DrawRectangleLinesEx(rect, 1.5f, Color{ 26, 32, 43 , 100 });
			}
		}
	}
}

void GameUtils::Replace()
{
	currentTetromino = std::move(nextTetromino);
	nextTetromino = ReturnObj();
}

void GameUtils::MoveDown()
{
	currentTetromino->yBlockOffset -= static_cast<float>(board->cubeWidth * GetFrameTime() * speed * 2);

	if (Stop()) {
		InsertIntoMatrix();
		Replace();
	}
}

void GameUtils::DisplayTetromino()
{
	for (size_t i = 0; i < currentTetromino->size; i++) {
		for (size_t j = 0; j < currentTetromino->size; j++) {
			if (currentTetromino->matrix[i][j] != ' ' && (i * board->cubeWidth) - currentTetromino->yBlockOffset + board->yOffset > 0) {
				Rectangle rect = {
					(j * board->cubeWidth) + board->xOffset + currentTetromino->xBlockOffset,
					(i * board->cubeWidth) + board->yOffset - currentTetromino->yBlockOffset,
					board->cubeWidth,
					board->cubeWidth
				};
				DrawRectangle(static_cast<float>((j * board->cubeWidth) + board->xOffset) + currentTetromino->xBlockOffset, static_cast<float>((i * board->cubeWidth) + board->yOffset) + -currentTetromino->yBlockOffset, board->cubeWidth, board->cubeWidth, board->colors.at(currentTetromino->matrix[i][j]));
				DrawRectangleLinesEx(rect, 1.5f, Color{ 26, 32, 43 , 100 });
			}
		}
	}
}

bool GameUtils::Stop()
{
	std::cout << static_cast<float>(((currentTetromino->TrueSizeDownY() + 1) * board->cubeWidth) + board->yOffset) + (-currentTetromino->yBlockOffset) << " and " << static_cast<float>(height - board->yOffset) << '\n';
	if (static_cast<float>(((currentTetromino->TrueSizeDownY() + 1) * board->cubeWidth) + board->yOffset) + (-currentTetromino->yBlockOffset) > static_cast<float>(height - board->yOffset))
	{
		std::cout << currentTetromino->TrueSizeUpY() << ", " << currentTetromino->TrueSizeDownY() << std::endl;
		return true;
	}
	return false;

	/*int y = (-currentTetromino->yBlockOffset - (currentTetromino->TrueSizeDownY() * board->cubeWidth)) / board->cubeWidth;
	int x = (currentTetromino->xBlockOffset + (currentTetromino->TrueSizeLeftX() * board->cubeWidth)) / board->cubeWidth;
	y++;

	for (size_t i = 0; i < currentTetromino->size; i++) {
		if (i + x < 0 || i + x >= board->height)
			continue;
		for (size_t j = 0; j < currentTetromino->size; j++) {
			if (j + y < 0 || j + y >= board->width)
				continue;
			else if (board->matrix[i + x][j + y] != ' ' && currentTetromino->matrix[i][j] != ' ')
				return true;
		}
	}
	return false;*/
}

void GameUtils::InsertIntoMatrix()
{
	int i = (-currentTetromino->yBlockOffset - (currentTetromino->TrueSizeDownY() * board->cubeWidth)) / board->cubeWidth;
	int j = (currentTetromino->xBlockOffset + (currentTetromino->TrueSizeLeftX() * board->cubeWidth)) / board->cubeWidth;

	//std::cout << i << ' ' << j << '\n';

	for (size_t k = 0; k < currentTetromino->size && (i + k) < board->height; k++) {
		for (size_t t = 0; t < currentTetromino->size && (j + t) < board->width; t++) {
			if(currentTetromino->matrix[k][t] != ' ')
				board->matrix[i + k][j + t] = currentTetromino->matrix[k][t];
		}
	}
}

void GameUtils::MoveTetromino()
{
	if (TaskUtils::xDir != 0) {
		bool flag = false;
		int j{};
		int offset{};

		if (TaskUtils::xDir == -1) {
			j = (currentTetromino->xBlockOffset + (currentTetromino->TrueSizeLeftX() * board->cubeWidth)) / board->cubeWidth;
			offset = -1;
			if (j - 1 >= 0) {
				flag = true;
			}
		}
		else if(TaskUtils::xDir == 1){
			j = (currentTetromino->xBlockOffset + (currentTetromino->TrueSizeRightX() * board->cubeWidth)) / board->cubeWidth;
			offset = 1;
			if (j + 1 < board->width) {
				flag = true;
			}
		}

		if (flag)
			currentTetromino->xBlockOffset += (offset * board->cubeWidth);
	}
	else if (TaskUtils::oneDown) {
		int i = (-currentTetromino->yBlockOffset - (currentTetromino->TrueSizeDownY() * board->cubeWidth)) / board->cubeWidth;
		int j = (currentTetromino->xBlockOffset + (currentTetromino->TrueSizeLeftX() * board->cubeWidth)) / board->cubeWidth;

		/*if (!TouchOtherBlocks(j, i)) {
			currentTetromino->yBlockOffset -= board->cubeWidth;
		}*/
		MoveDown();
	}
	else if (TaskUtils::rotate) {
		currentTetromino->RotateRight();

		if (OutOfBounds()) {
			currentTetromino->RotateLeft();
		}
	}
}

bool GameUtils::TouchOtherBlocks(int x, int y)
{
	for (size_t i = 0; i < currentTetromino->size; i++) {
		if (i + x < 0 || i + x >= board->height)
			continue;
		for (size_t j = 0; j < currentTetromino->size; j++) {
			if (j + y < 0 || j + y >= board->width)
				continue;
			else if (board->matrix[i + x][j + y] != ' ' && currentTetromino->matrix[i][j] != ' ')
				return true;
			}
	}
	return false;
}

bool GameUtils::OutOfBounds()
{
	int y1 = (-currentTetromino->yBlockOffset - (currentTetromino->TrueSizeUpY() * board->cubeWidth)) / board->cubeWidth;
	int y2 = (-currentTetromino->yBlockOffset - (currentTetromino->TrueSizeDownY() * board->cubeWidth)) / board->cubeWidth;
	int x1 = (currentTetromino->xBlockOffset + (currentTetromino->TrueSizeLeftX() * board->cubeWidth)) / board->cubeWidth;
	int x2 = (currentTetromino->xBlockOffset + (currentTetromino->TrueSizeRightX() * board->cubeWidth)) / board->cubeWidth;

	if (x1 < 0 || x2 >= board->width) {
		return true;
	}
	else if (y1 < 0 || y2 >= board->height) {
		return true;
	}
	return false;
}
