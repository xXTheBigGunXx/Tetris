#include "GameUtils.h"
#include "raylib.h"
#include "TaskUtils.h"
#include <iostream>
#include <thread>
#include <chrono>

GameUtils::GameUtils()
{
	SetTargetFPS(FPS);
	InitWindow(width, height, "Tetris game!");

	board = new Board(10,10);

	currentTetromino = ReturnObj();
	nextTetromino = ReturnObj();

	std::cout << currentTetromino->yBlockOffset << std::endl;

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

		//std::cout << currentTetromino->yBlockOffset << '\n';

		DrawMatrix();

		if (frameCount == FPS / speed / 3)
			xDirection = 0.0f;

		if (frameCount == FPS / speed) {
			TaskUtils::Direction();

			//std::cout << TaskUtils::xDir << ' ' << TaskUtils::rotate << ' ' << TaskUtils::oneDown << ' ' << TaskUtils::goDown << '\n';
		//	std::cout << currentTetromino->TrueSizeLeftX() << ' ' << currentTetromino->TrueSizeRightX() << '|' << currentTetromino->TrueSizeUpY() << ' ' << currentTetromino->TrueSizeDownY() << '\n';
			MoveTetromino();

			frameCount = 0;
			TaskUtils::Reset();
		}
		//DisplayTetromino();
		MoveDown();

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

bool GameUtils::MoveDown()
{
	currentTetromino->yBlockOffset += static_cast<float>(board->cubeWidth * GetFrameTime() * speed * multiplier * 2);
	currentTetromino->xBlockOffset += xDirection * static_cast<float>(board->cubeWidth * GetFrameTime() * speed * multiplier * 3);

	if (HitEndOrOtherBLock()) {
		InsertIntoMatrix();
		Replace();
		return false;
	}
	return true;
}

void GameUtils::DisplayTetromino()
{
	for (size_t i = 0; i < currentTetromino->size; i++) {
		for (size_t j = 0; j < currentTetromino->size; j++) {
			if (currentTetromino->matrix[i][j] != ' ' && (i * board->cubeWidth) + currentTetromino->yBlockOffset + board->yOffset > 0) {
				Rectangle rect = {
					(j * board->cubeWidth) + board->xOffset + currentTetromino->xBlockOffset,
					(i * board->cubeWidth) + board->yOffset + currentTetromino->yBlockOffset,
					board->cubeWidth,
					board->cubeWidth
				};
				DrawRectangle(static_cast<float>((j * board->cubeWidth) + board->xOffset) + currentTetromino->xBlockOffset, static_cast<float>((i * board->cubeWidth) + board->yOffset) + currentTetromino->yBlockOffset, board->cubeWidth, board->cubeWidth, board->colors.at(currentTetromino->matrix[i][j]));
				DrawRectangleLinesEx(rect, 1.5f, Color{ 26, 32, 43 , 100 });
			}
		}
	}
}

bool GameUtils::HitEndOrOtherBLock()
{
	if (static_cast<float>(((currentTetromino->TrueSizeDownY() + 1) * board->cubeWidth) + board->yOffset) + currentTetromino->yBlockOffset > static_cast<float>(height - board->yOffset))
	{
		//std::cout << "Hit other tetromino or the end" << currentTetromino->TrueSizeUpY() << ", " << currentTetromino->TrueSizeDownY() << std::endl;
		return true;
	}

	int i = static_cast<int>(static_cast<float>(board->yOffset) + currentTetromino->yBlockOffset) / board->cubeWidth;
	int j = static_cast<int>(static_cast<float>(board->xOffset) + currentTetromino->xBlockOffset) / board->cubeWidth;
	i++;

	for (size_t k = 0; k < currentTetromino->size && (i + k) < board->height; k++) {
		for (size_t t = 0; t < currentTetromino->size && (j + t) < board->width; t++) {
			if (board->matrix[i + k][j + t] != ' ' && currentTetromino->matrix[k][t] != ' ')
				return true;
		}
	}
	return false;
}

void GameUtils::InsertIntoMatrix()
{
	int i = static_cast<int>(static_cast<float>(board->yOffset) + currentTetromino->yBlockOffset) / board->cubeWidth;
	int j = static_cast<int>(static_cast<float>(board->xOffset) + currentTetromino->xBlockOffset) / board->cubeWidth;

	for (size_t k = 0; k < currentTetromino->size && (i + k) < board->height; k++) {
		for (size_t t = 0; t < currentTetromino->size && (j + t) < board->width; t++) {
			if(currentTetromino->matrix[k][t] != ' ')
				board->matrix[i + k][j + t] = currentTetromino->matrix[k][t];
		}
	}
}

void GameUtils::MoveTetromino()
{
	multiplier = TaskUtils::oneDown ? 1.5f : 1.0f;
	xDirection = TaskUtils::xDir;

	if (CanGoLeftOrRight()) {
		/*std::this_thread::sleep_for(std::chrono::milliseconds(150));
		TaskUtils::Direction();
		float currDir = xDirection;

		while (TaskUtils::xDir == currDir && CanGoLeftOrRight()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(75));
			xDirection += TaskUtils::xDir;
			TaskUtils::Direction();
		}*/
	}
	else {
		xDirection = 0.0f;
	}
	if (TaskUtils::goDown) {
		while (MoveDown() != false) {}
	}
	if (TaskUtils::rotate) {

		currentTetromino->RotateRight();

		if (HitEndOrOtherBLock()) {
			currentTetromino->RotateLeft();
		}
	}
}

bool GameUtils::CanGoLeftOrRight()
{
	int i = static_cast<int>(static_cast<float>(board->xOffset) + currentTetromino->xBlockOffset) / board->cubeWidth;

	int leftIndex = i + currentTetromino->TrueSizeLeftX();
	int rightIndex = i + currentTetromino->TrueSizeRightX();

	std::cout << "Indexs: " << leftIndex << ", " << rightIndex << '|' << xDirection << '\n';

	if (xDirection == -1)
		if (leftIndex <= 0) return false;
	if (xDirection == 1)
		if (rightIndex >= board->width - 1) return false;

	i = static_cast<int>(static_cast<float>(board->yOffset) + currentTetromino->yBlockOffset) / board->cubeWidth;
	int j = static_cast<int>(static_cast<float>(board->xOffset) + currentTetromino->xBlockOffset) / board->cubeWidth;
	j += xDirection;

	for (size_t k = 0; k < currentTetromino->size && (i + k) < board->height; k++) {
		for (size_t t = 0; t < currentTetromino->size && (j + t) < board->width; t++) {
			if (board->matrix[i + k][j + t] != ' ' && currentTetromino->matrix[k][t] != ' ')
				return false;
		}
	}

	return true;
}