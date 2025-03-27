/*
Tic Tac Toe Game
*/
#pragma once

#include "game.h"

#include <vector>

using Matrix = std::vector<std::vector<int>>;
constexpr int SIZE = 3;
const std::vector<char> SYMBOLS{' ', 'X', 'O'};

class XO3 : public Game
{
	Matrix board;

public:
	XO3()
		: board(3, std::vector<int>(3, 0))
	{}
	void play() override;
	int getWinner() const override;

private:
	int checkWinner() const;
	void printBoard(unsigned int printSpeed = 100) const;
};