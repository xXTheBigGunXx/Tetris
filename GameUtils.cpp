#include "GameUtils.h"
#include "raylib.h"
#include "TaskUtils.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <vector>

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

		if (frameCount < FPS / speed / 2) {
			TaskUtils::rotate = false;
			TaskUtils::goDown = false;
		}

		TaskUtils::Direction();

		if (frameCount == FPS / speed) {
			frameCount = 0;
			TaskUtils::Direction();

			MoveTetromino();
			TaskUtils::Reset();
		}
		MoveDown();

		RemoveLines();

		DrawMatrix();

		DisplayNextTetromino();
		DisplayTetromino();
		EndDrawing();

		if (frameCount == 0) {
			TaskUtils::rotate = false;
		}

		frameCount++;
	}
}

void GameUtils::RemoveLines() {
	auto& mat = board->matrix;
	auto isFullRow = [](const std::vector<char>& row) {
		return std::all_of(row.begin(), row.end(), [](char c) { return c != ' '; });
		};

	mat.erase(std::remove_if(mat.begin(), mat.end(), isFullRow), mat.end());

	while (mat.size() < board->height) {
		mat.insert(mat.begin(), std::vector<char>(board->width, ' '));
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

			DrawRectangle(rect.x, rect.y , board->cubeWidth, board->cubeWidth, board->colors.at(board->matrix[i][j]));
			DrawRectangleLinesEx(rect, 1.5f, Color{ 26, 32, 43 , 100});
		}
	}
}

void GameUtils::DisplayNextTetromino()
{
	const Rectangle myRect = { 440, 400, 350, 200 };
	DrawRectangleRounded(myRect, 0.25f, 3, Color{ 9, 85, 162, 150 });

	for (size_t i = 0; i < nextTetromino->size; i++) {
		for (size_t j = 0; j < nextTetromino->size; j++) {
			if (nextTetromino->matrix[i][j] != ' ') {
				Rectangle rect = {
					j * board->cubeWidth + myRect.x + ((myRect.width - (((nextTetromino->TrueSizeRightX() - nextTetromino->TrueSizeLeftX()) + 1) * board->cubeWidth)) / 2),
					i * board->cubeWidth + myRect.y + ((myRect.height - (((nextTetromino->TrueSizeDownY() - nextTetromino->TrueSizeUpY()) + 1) * board->cubeWidth)) / 2),
					board->cubeWidth,
					board->cubeWidth
				};
				DrawRectangle(rect.x, rect.y, board->cubeWidth, board->cubeWidth, board->colors.at(nextTetromino->matrix[i][j]));
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
	currentTetromino->yBlockOffset += 4.2f * multiplier;
	currentTetromino->xBlockOffset +=  xDirection * 4.2f;

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
			if (currentTetromino->matrix[i][j] != ' ') {
				Rectangle rect = {
					(j * board->cubeWidth) + board->xOffset + currentTetromino->xBlockOffset,
					(i * board->cubeWidth) + board->yOffset + currentTetromino->yBlockOffset,
					board->cubeWidth,
					board->cubeWidth
				};
				DrawRectangle(rect.x, rect.y, board->cubeWidth, board->cubeWidth, board->colors.at(currentTetromino->matrix[i][j]));
				DrawRectangleLinesEx(rect, 1.5f, Color{ 26, 32, 43 , 100 });
			}
		}
	}
}

bool GameUtils::HitEndOrOtherBLock()
{
	if (static_cast<float>(((currentTetromino->TrueSizeDownY() + 1) * board->cubeWidth)) + currentTetromino->yBlockOffset >= static_cast<float>(height - board->yOffset))
	{
		return true;
	}

	int i = static_cast<int>(currentTetromino->yBlockOffset) / board->cubeWidth;
	int j = static_cast<int>(currentTetromino->xBlockOffset) / board->cubeWidth;

	i++;

	/*for (size_t k = 0; k < currentTetromino->size && (i + k) < board->height; k++) {
		for (size_t t = 0; t < currentTetromino->size && (j + t) < board->width; t++) {
			if (board->matrix[i + k][j + t] != ' ' && currentTetromino->matrix[k][t] != ' ')
				return true;
		}
	}*/
	return false;
}

void GameUtils::InsertIntoMatrix()
{
	int i = static_cast<int>(currentTetromino->yBlockOffset) / board->cubeWidth;
	int j = static_cast<int>(currentTetromino->xBlockOffset) / board->cubeWidth;

	for (size_t k = 0; k < currentTetromino->size && (i + k) < board->height; k++) {
		for (size_t t = 0; t < currentTetromino->size && (j + t) < board->width; t++) {
			if(currentTetromino->matrix[k][t] != ' ')
				board->matrix[i + k][j + t] = currentTetromino->matrix[k][t];
		}
	}
}

void GameUtils::MoveTetromino()
{
	multiplier = TaskUtils::oneDown ? 1.75f : 1.0f;
	xDirection = TaskUtils::xDir;

	if (!CanGoLeftOrRight()) {
		xDirection = 0.0f;
	}
	if (TaskUtils::goDown) {
		while (MoveDown() != false) {}
	}
	if (TaskUtils::rotate) {
		if (CantRotate()) {
			currentTetromino->RotateLeft();
		}
	}
}

bool GameUtils::CanGoLeftOrRight()
{
	int i = static_cast<int>(currentTetromino->xBlockOffset) / board->cubeWidth;

	int leftIndex = i + currentTetromino->TrueSizeLeftX();
	int rightIndex = i + currentTetromino->TrueSizeRightX();

	if (xDirection == -1)
		if (leftIndex <= 0) return false;
	if (xDirection == 1)
		if (rightIndex >= board->width - 1) return false;
	if (xDirection == 0)
		return false;

	i = static_cast<int>(currentTetromino->yBlockOffset) / board->cubeWidth;
	int j = static_cast<int>(currentTetromino->xBlockOffset) / board->cubeWidth;
	j += xDirection;

	for (size_t k = 0; k < currentTetromino->size && (i + k) < board->height; k++) {
		for (size_t t = 0; t < currentTetromino->size && (j + t) < board->width; t++) {
			if (board->matrix[i + k][j + t] != ' ' && currentTetromino->matrix[k][t] != ' ')
				return false;
		}
	}

	return true;
}

bool GameUtils::CantRotate()
{
	currentTetromino->RotateRight();

	int i = static_cast<int>(currentTetromino->xBlockOffset) / board->cubeWidth;

	int leftIndex = i + currentTetromino->TrueSizeLeftX();
	int rightIndex = i + currentTetromino->TrueSizeRightX();

	std::cout << i << '|' << currentTetromino->TrueSizeRightX() << '\n';

	if (leftIndex < 0) return true;
	if (rightIndex > board->width - 1) return true;

	i = static_cast<int>(currentTetromino->yBlockOffset) / board->cubeWidth;
	int j = static_cast<int>(currentTetromino->xBlockOffset) / board->cubeWidth;

	for (size_t k = 0; k < currentTetromino->size && (i + k) < board->height; k++) {
		for (size_t t = 0; t < currentTetromino->size && (j + t) < board->width; t++) {
			if (board->matrix[i + k][j + t] != ' ' && currentTetromino->matrix[k][t] != ' ')
				return true;
		}
	}

	return false;
}