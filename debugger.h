//
// Created by pazuzu on 14/04/19.
//

#ifndef CHIP8_DEBUGGER_H
#define CHIP8_DEBUGGER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "chip8.h"

class Debugger {
    private:
            sf::Font font;
            sf::Text text;

            Chip8 *chip8;
            sf::RenderWindow *window;

            std::string getInstruction(u_int16_t opcode, int currentPC);

            short getNNN(u_int16_t opcode);
            short getByte(u_int16_t opcode);
            short getX(u_int16_t opcode);
            short getY(u_int16_t opcode);
            short getN(u_int16_t opcode);

            bool isBranchInstruction(u_int16_t opcode);

            void drawText(char *str, float x, float y);

            sf::Color *light;
            sf::Color *dark;

            sf::RectangleShape *background;

            int instructionMarker;
            u_int16_t currentInstruction;

            bool changeRAMLocation = false;

    public:
            Debugger(sf::RenderWindow *window, Chip8 *chip8)
            {
                if (!font.loadFromFile("../fonts/5x5.ttf"))
                {
                    std::cout << "Error loading font!\n";
                    exit(1);
                }

                text.setFont(font);
                text.setCharacterSize(20);

                this->chip8 = chip8;
                this->window = window;

                light = new sf::Color(252, 202, 0);
                dark = new sf::Color(176, 91, 0);

                background = new sf::RectangleShape(sf::Vector2f(300, 250));
                background->setFillColor(*dark);

                instructionMarker = *chip8->getPC();
            }

            void drawRAM();
            void drawRegistersStatus();
};


#endif //CHIP8_DEBUGGER_H
