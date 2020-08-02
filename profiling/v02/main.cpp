#include <iostream>
#include "Game.h"

constexpr int FILE_ARG = 1;

int main(int argc, char** argv) {
    try {
        unsigned int rounds;
        std::cout << "Ingrese la cantidad de rondas" << std::endl;
        std::cin >> rounds;

        unsigned int width;
        std::cout << "Ingrese el ancho" << std::endl;
        std::cin >> width;

        unsigned int height;
        std::cout << "Ingrese el alto" << std::endl;
        std::cin >> height;
        Game game(width, height);
        game.playRounds(rounds);
    } catch (...) {
        std::cout << "Hubo un error" << std::endl;
    }
}