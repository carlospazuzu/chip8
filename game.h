//
// Created by pazuzu on 14/04/19.
//

#ifndef CHIP8_GAME_H
#define CHIP8_GAME_H


#include "chip8.h"
#include "debugger.h"

#include <SFML/Graphics.hpp>

#define LIGHT_YELLOW sf::Color(252, 202, 0)
#define DARK_YELLOW sf::Color(176, 91, 0)
#define YELLOW_BG sf::Color(59, 36, 11)

#define LIGHT_GB sf::Color(97, 125, 106)
#define DARK_GB sf::Color(23, 23, 23)
#define GB_BG sf::Color(39, 51, 43)

#define LIGHT sf::Color(36, 36, 36)
#define DARK sf::Color(240, 240, 240)
#define LIGHT_BG sf::Color(24, 24, 24)

class Game {
    private:
            Chip8 *chip8;

            sf::RenderWindow *window;
            sf::RectangleShape *wRect, *bRect;

            Debugger *debugger;

            void handleChip8Input(sf::Event event);

    public:

            Game()
            {
                chip8 = new Chip8();

                window = new sf::RenderWindow(sf::VideoMode(680, 640), "CHIP8");
                // window->setFramerateLimit(60);

                wRect = new sf::RectangleShape(sf::Vector2f(10, 10));
                bRect = new sf::RectangleShape(sf::Vector2f(10, 10));
                wRect->setFillColor(sf::Color(LIGHT));
                bRect->setFillColor(sf::Color(DARK));

            }

            void loadGame();
            void run();
            void updateScreen();


};


#endif //CHIP8_GAME_H
