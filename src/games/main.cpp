#include "game.h"
#include "XO3.h"
#include "rps.h"
#include "minesweeper.h"

#include <iostream>
#include <cstring>
#include <memory>

int main(int argc, char **argv)
{

	if (argc < 3)
	{
		std::cerr << "Error: could define username" << std::endl;
		std::cerr << "Usage: gameName userName_1 userName_2" << std::endl;
		std::cerr << "Available games: XO3 | RPS | Minesweeper" << std::endl;
		return 0;
	}

	std::unique_ptr<Game> game;
	if (strcmp(argv[1], "XO3") == 0)
	{
		game.reset(new XO3);
	}
	else if (strcmp(argv[1], "RPS") == 0)
	{
		game.reset(new RockPaperScissors);
	}
	else if (strcmp(argv[1], "Minesweeper") == 0)
	{
		game.reset(new Minesweeper);
	}
	else
	{
		std::cerr << "The game \"" << argv[1] << "\" not found" << std::endl;
		return 1;
	}

	std::cout << "User \"" << argv[2] << "\" and user \"" << argv[3] << "\" start the game." << std::endl;
	game->play();

	int winner = game->getWinner();
	switch (winner)
	{
	case 0:
		std::cout << argv[2] << " " << argv[3] << std::endl;
		break;
	case 1:
		std::cout << argv[2] << std::endl;
		break;
	case 2:
		std::cout << argv[3] << std::endl;
		break;
	default:
		std::cerr << "Missing winner" << std::endl;
		break;
	}

	return 0;
}
