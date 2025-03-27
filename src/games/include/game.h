#pragma once

class Game
{
public:
	// This should imitate the game
	virtual void play() = 0;
	// The return values shoul be:
	// 							0 - draw
	//							1 - win first player
	//							2 - win second player
	virtual int getWinner() const = 0;
};