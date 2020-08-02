#ifndef GAME_H
#define GAME_H
#include <vector>

class Game {
public:
	Game(unsigned int width, unsigned int height);
	~Game();
	void playRounds(unsigned int rounds);
	void toggle(unsigned int x, unsigned int y);
private:
	unsigned int width, height;
	unsigned char getSurrounding(unsigned int x, unsigned int y,
	    std::vector< std::vector<bool> > prevDots);
	void playRound();
	std::vector< std::vector<bool> > dots;
};

#endif