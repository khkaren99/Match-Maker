/* 
Rock-Paper-Scissors
*/

#pragma once

#include "game.h"

class RockPaperScissors : public Game
{
	int winner;
public:
	void play() override;
	int getWinner() const override;
};