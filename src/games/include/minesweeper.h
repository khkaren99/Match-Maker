/*
Minesweeper Game
*/
#pragma once

#include "game.h"

#include <vector>
#include <ctime>
#include <stdlib.h>


class Minesweeper : public Game
{
public:
	using Matrix = std::vector<std::vector<int>>;

private:
	Matrix board;
	int winner;
	size_t mineCount;

public:
	Minesweeper()
	: board(5, std::vector<int>(5, 0))
	, winner(0)
	, mineCount(board.size() * 2)
	{
		srand(time(NULL));
		size_t size = board.size();
		// init the board with 0.
		// the mine taged as -1.
		// user1 steps 1.
		// user2 steps 2.
		// add size*2 mine
		for (size_t i = 0; i < mineCount; ++i)
		{
			while (true)
			{
				int x = rand() % size;
				int y = rand() % size;
				if (board[x][y] == -1)
					continue;
				board[x][y] = -1;
				break;
			}
		}
	}
	void play() override;
	int getWinner() const override;

private:
	void printBoard(unsigned int printSpeed = 100) const;
	bool isBoardFull() const;
};