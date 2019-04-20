//
// Created by pazuzu on 14/04/19.
//

#include <cstdlib>
#include <iostream>
#include <fstream>
#include "game.h"

void Game::handleChip8Input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::X) chip8->setKeyState(true, 0x0);
        if (event.key.code == sf::Keyboard::Num1) chip8->setKeyState(true, 0x1);
        if (event.key.code == sf::Keyboard::Num2) chip8->setKeyState(true, 0x2);
        if (event.key.code == sf::Keyboard::Num3) chip8->setKeyState(true, 0x3);
        if (event.key.code == sf::Keyboard::Q) chip8->setKeyState(true, 0x4);
        if (event.key.code == sf::Keyboard::W) chip8->setKeyState(true, 0x5);
        if (event.key.code == sf::Keyboard::E) chip8->setKeyState(true, 0x6);
        if (event.key.code == sf::Keyboard::A) chip8->setKeyState(true, 0x7);
        if (event.key.code == sf::Keyboard::S) chip8->setKeyState(true, 0x8);
        if (event.key.code == sf::Keyboard::D) chip8->setKeyState(true, 0x9);
        if (event.key.code == sf::Keyboard::Z) chip8->setKeyState(true, 0xA);
        if (event.key.code == sf::Keyboard::C) chip8->setKeyState(true, 0xB);
        if (event.key.code == sf::Keyboard::Num4) chip8->setKeyState(true, 0xC);
        if (event.key.code == sf::Keyboard::R) chip8->setKeyState(true, 0xD);
        if (event.key.code == sf::Keyboard::E) chip8->setKeyState(true, 0xE);
        if (event.key.code == sf::Keyboard::V) chip8->setKeyState(true, 0xF);

    }
    else if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::X) chip8->setKeyState(false, 0x0);
        if (event.key.code == sf::Keyboard::Num1) chip8->setKeyState(false, 0x1);
        if (event.key.code == sf::Keyboard::Num2) chip8->setKeyState(false, 0x2);
        if (event.key.code == sf::Keyboard::Num3) chip8->setKeyState(false, 0x3);
        if (event.key.code == sf::Keyboard::Q) chip8->setKeyState(false, 0x4);
        if (event.key.code == sf::Keyboard::W) chip8->setKeyState(false, 0x5);
        if (event.key.code == sf::Keyboard::E) chip8->setKeyState(false, 0x6);
        if (event.key.code == sf::Keyboard::A) chip8->setKeyState(false, 0x7);
        if (event.key.code == sf::Keyboard::S) chip8->setKeyState(false, 0x8);
        if (event.key.code == sf::Keyboard::D) chip8->setKeyState(false, 0x9);
        if (event.key.code == sf::Keyboard::Z) chip8->setKeyState(false, 0xA);
        if (event.key.code == sf::Keyboard::C) chip8->setKeyState(false, 0xB);
        if (event.key.code == sf::Keyboard::Num4) chip8->setKeyState(false, 0xC);
        if (event.key.code == sf::Keyboard::R) chip8->setKeyState(false, 0xD);
        if (event.key.code == sf::Keyboard::E) chip8->setKeyState(false, 0xE);
        if (event.key.code == sf::Keyboard::V) chip8->setKeyState(false, 0xF);
    }
}

void Game::loadGame()
{
    FILE *fp = fopen("../games/HIDDEN", "rb");

    if (!fp)
    {
        std::cout << "There was an error opening file\n";
        exit(0);
    }

    u_int8_t* RAM = chip8->getRAM();

    while (!feof(fp))
    {
        RAM[*chip8->getPC()] = fgetc(fp);
        chip8->increasePC();
    }

    chip8->resetPC();

    debugger = new Debugger(window, chip8);
}

void Game::run()
{
    bool debugStep = true;
    bool firstFrame = true;
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
            {
                debugStep = true;
            }

            handleChip8Input(event);
        }

        if (debugStep)
        {
            if (!firstFrame)
            {
                chip8->emulateCycle();
            }
            updateScreen();

            if (*chip8->getDT() > 0) chip8->decrementDT();

            if (*chip8->getST() > 0)
            {
                chip8->decrementST();
                if (!chip8->getBuzzer()->Stopped)
                    chip8->getBuzzer()->play();
            }
            else
            {
                if (chip8->getBuzzer()->Playing)
                    chip8->getBuzzer()->stop();
            }

            // debugStep = false;
            firstFrame = false;
        }

    }
}

void Game::updateScreen()
{
    window->clear(YELLOW_BG);

    for (int i = 0; i < 32; i ++)
    {
        for (int j = 0; j < 64; j ++)
        {
            if (chip8->getDisplay()[i][j] == 0)
            {
                bRect->setPosition(j * 10 + 20, i * 10 + 20);
                window->draw(*bRect);
            }
            else
            {
                wRect->setPosition(j * 10 + 20, i * 10 + 20);
                window->draw(*wRect);
            }
        }
    }

    debugger->drawRAM();
    debugger->drawRegistersStatus();

    window->display();
}