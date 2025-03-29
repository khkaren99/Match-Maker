/*
Tic Tac Toe Game
*/
#pragma once

#include "game.h"

#include <vector>

class XO3 : public Game
{
public:
	using Matrix = std::vector<std::vector<int>>;

private:
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