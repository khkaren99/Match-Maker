/* Rock-Paper-Scissors
  The game get players username.
  The game outputs the username of the winner. 
  	If it’s a draw, it outputs usernames of both players.
*/

#include "rps.h"

#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>

void RockPaperScissors::play()
{
	srand(time(NULL));

	std::vector<std::string> SUIT{"Rock", "Paper", "Scissors"};
	size_t user1 = 0;
	size_t user2 = 0;
	
	for (size_t i = 0; i < 3; ++i)
	{
		int u1_suit = rand() % 3;
		int u2_suit = rand() % 3;
		std::cout << "Player 1: " << SUIT[u1_suit] << std::endl;
		std::cout << "Player 2: " << SUIT[u2_suit] << std::endl;
		
		if (u1_suit == u2_suit)
		{
			std::cout << "\tRound " << i+1 << ": draw" << std::endl;
			continue;
		}
		if ((u1_suit + 1) % SUIT.size() == u2_suit)
		{
			std::cout << "\tRound " << i+1 << " winner is Player 1" << std::endl;
			++user1;
		} else {
			std::cout << "\tRound " << i+1 << " winner is Player 2" << std::endl;
			++user2;
		}
	}
	if (user1 == user2)
		return;

	winner = (user1 > user2 ? 1 : 2);
}

int RockPaperScissors::getWinner() const
{
	return winner;
}