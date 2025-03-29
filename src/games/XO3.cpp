/* Tic Tac Toe Game
  The game get players username.
  The game outputs the username of the winner. 
  	If it’s a draw, it outputs usernames of both players.
*/

#include "XO3.h"
#include "helper.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <thread>
#include <chrono>

void XO3::play()
{
	srand(time(NULL));
	size_t size = board.size();

	printBoard();
	// Start
	for (size_t i = 0; i < size*size && checkWinner() == 0; ++i)
	{
		while (true)
		{
			size_t row = rand() % size;
			size_t column = rand() % size;
			// looking random chosen empty block
			if (board[row][column] != 0)
				continue;
			board[row][column] = i % 2 + 1;
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		clean(size*2+1);
		printBoard();
	}
}

int XO3::getWinner() const
{
	return checkWinner();
}

int XO3::checkWinner() const
{
	size_t n = board.size();
	// Check horizontal and vertical lines muching
	for (size_t i = 0; i < n; ++i)
	{
		int row = 0;
		int col = 0;
		for (size_t j = 1; j < board[i].size(); ++j)
		{
			row += (board[i][j] == board[i][j - 1]);
			col += (board[j][i] == board[j - 1][i]);
		}
		if (row == 2 || col == 2)
		{
			return board[i][i];
		}
	}
	// Check diagonal muching
	int lubr = 0; // left-up to button-right
	int lbur = 0; // left-button to right-up
	for (size_t i = 1; i < n; ++i)
	{
		lubr += (board[i][i] == board[i - 1][i - 1]);
		lbur += (board[i][n - i - 1] == board[i - 1][n - i]);
	}
	if (lubr == 2 || lbur == 2)
	{
		return board[1][1];
	}
	return 0;
}

void XO3::printBoard(unsigned int printSpeed) const
{
	const std::vector<char> SYMBOLS{' ', 'X', 'O'};
	for (size_t i = 0; i < board.size(); ++i)
	{
		std::cout << "+-+-+-+" << std::endl;
		std::cout << "|";
		for (size_t j = 0; j < board[i].size(); ++j)
		{
			std::cout << SYMBOLS[board[i][j]] << "|";
			std::cout.flush();
			std::this_thread::sleep_for(std::chrono::milliseconds(printSpeed));
		}
		std::cout << std::endl;
	}
	std::cout << "+-+-+-+" << std::endl;
}