#include "game.h"
#include "XO3.h"

#include <iostream>
#include <cstring>
#include <memory>

int main(int argc, char **argv)
{

	if (argc < 3)
	{
		std::cerr << "Error: could define username" << std::endl;
		std::cerr << "Usage: gameName userName_1 userName_2" << std::endl;
		return 0;
	}

	std::unique_ptr<Game> game;
	if (strcmp(argv[1], "XO3") == 0)
	{
		game.reset(new XO3);
	}

	std::cout << "User \"" << argv[2] << "\" and user \"" << argv[3] << "\" start the game." << std::endl;
	game->play();
	std::cout << game->getWinner() << std::endl;
	return 0;

//	printBoard(board, 0);
//	gameImitation(board);
}
