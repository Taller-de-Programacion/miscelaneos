#include "Game.h"
#include <iostream>


Game::Game(unsigned int width, unsigned int height) : width(width), height(height) {
    for (unsigned int i = 0; i < height; ++i) {
        this->dots.push_back(std::vector<bool>(width, false));
    }
}

Game::~Game() {}

void Game::playRounds(unsigned int rounds) {
    for (unsigned int i = 0; i < rounds; ++i) {
        std::cout << "Ronda: " << i << std::endl;
        playRound();
    }
}

void Game::playRound() {
    auto prevDots = this->dots;
    for (unsigned int y = 1; y < this->height - 1; ++y) {
        for (unsigned int x = 1; x < this->width - 1; ++x) {
            unsigned char surrounding = getSurrounding(x, y, prevDots);
            if (surrounding == 3) {
                this->dots[y][x] = true;
            } else if (surrounding != 2) {
                this->dots[y][x] = false;
            }
        }
    }
}

void Game::toggle(unsigned int x, unsigned int y) {
    this->dots[x][y] = !this->dots[x][y];
}

unsigned char Game::getSurrounding(unsigned int x,
  unsigned int y, std::vector< std::vector<bool> > prevDots) {
    unsigned char count = 0;
    if (prevDots[y-1][x-1]) count++;
    if (prevDots[y-1][x]) count++;
    if (prevDots[y-1][x+1]) count++;
    if (prevDots[y][x-1]) count++;
    if (prevDots[y][x+1]) count++;
    if (prevDots[y+1][x-1]) count++;
    if (prevDots[y+1][x]) count++;
    if (prevDots[y+1][x+1]) count++;
    return count;
}
