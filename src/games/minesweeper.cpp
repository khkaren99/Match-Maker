/* Minesweeper
  The game get players username.
  The game outputs the username of the winner. 
  	If it’s a draw, it outputs usernames of both players.
*/

#include "minesweeper.h"
#include "helper.h"

#include <iostream>
#include <time.h>
#include <thread>
#include <chrono>

void Minesweeper::play()
{
	// Indicate which players turn, and switching.
	int player = 1;
	auto nextPlayer = [&]()
	{
		player = player % 2 + 1;
	};

	size_t size = board.size();
//	printBoard(0);
	// Start
	// The maximum step count should be:
	// size - mineCount
	size_t stepCount = 0;
	// Do while no winner and there is a step
	while (winner == 0 && stepCount < size - mineCount)
	{
//		std::cout.flush();
		// Report player and step.
		std::cout << (player==1?"First":"Second") << " player: ";
		size_t x,y;
		while (true)
		{
			x = rand() % size;
			y = rand() % size;
			// Make sure the coordinate is not selected.
			if (board[x][y] < 1)
				break;
		}
		// print coordinates +1 as player count from 1 )))
		std::cout << x + 1 << ":" << y + 1 << std::endl;
		// Check the coordinate
		if (board[x][y] != -1)
		{
			board[x][y] = player;
			nextPlayer();
		}
		else
		{
			nextPlayer();
			winner = player;
		}

		// print the board, if no winner prepare for next step
		printBoard(50);
		if (winner == 0)
			clean(size*2 + 2);
	}
}

int Minesweeper::getWinner() const
{
	return winner;
}

void Minesweeper::printBoard(unsigned int printSpeed) const
{
	// The first player in board indicates like F
	// The second player in board indicates like S
	// The means in board indicates like O
	const std::vector<char> SYMBOLS{' ', 'F', 'S', 'O'};
	std::string horizontal = "+";
	for (size_t i = 0; i < board.size(); ++i) {
        horizontal += "-+";
    }

	for (size_t i = 0; i < board.size(); ++i)
	{
		std::cout << horizontal << std::endl;
		std::cout << "|";
		for (size_t j = 0; j < board[i].size(); ++j)
		{
			// if the game didn't finish hide mines
			if (winner == 0)
				std::cout << ( board[i][j] == -1 ? SYMBOLS[0] : SYMBOLS[board[i][j]] );
			else
				std::cout << ( board[i][j] == -1 ? SYMBOLS[3] : SYMBOLS[board[i][j]] );

			std::cout << "|";
			std::cout.flush();
			std::this_thread::sleep_for(std::chrono::milliseconds(printSpeed));
		}
		std::cout << std::endl;
	}
	std::cout << horizontal << std::endl;
}

bool Minesweeper::isBoardFull() const
{
	for (size_t i = 0; i < board.size(); ++i)
	{
		for (size_t j = 0; j < board[i].size(); ++j)
		{
			if (board[i][j] == 0)
				return false;
		}
	}
	return true;
}