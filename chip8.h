//
// Created by pazuzu on 14/04/19.
//

#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H


#include <zconf.h>
#include <cstring>
#include <SFML/Audio.hpp>

class Chip8 {
    private:
            // registers V0 to VF
            u_int8_t V[16];

            // 4K RAM
            u_int8_t RAM[0x1000];

            // stack pointer
            int SP;

            // program counter
            int PC;

            // memory addr register
            u_int16_t I;

            // stack
            u_int16_t stack[16];

            // keys
            bool keys[16];

            int** display;

            // timers
            u_int8_t DT;
            u_int8_t ST;

            u_int8_t chip8font[0x50] = {
                                    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                                    0x20, 0x60, 0x20, 0x20, 0x70, // 1
                                    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                                    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                                    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                                    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                                    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                                    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                                    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                                    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                                    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                                    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                                    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                                    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                                    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                                    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
                                  };

            sf::Sound buzzer;
            sf::SoundBuffer soundBuffer;

            short getNNN(u_int16_t opcode);
            short getByte(u_int16_t opcode);
            short getX(u_int16_t opcode);
            short getY(u_int16_t opcode);
            short getN(u_int16_t opcode);

            void clearDisplay();

    public:
            Chip8()
            {
                // initialize registers and stack
                memset(V, 0, sizeof(V));
                memset(stack, 0, sizeof(stack));
                memset(RAM, 0, sizeof(RAM));

                SP = 0;
                PC = 0x200;
                I = 0;

                DT = 0;
                ST = 0;

                // initialize display
                display = new int*[32];

                for (int i = 0; i < 32; i ++)
                {
                    display[i] = new int[64];
                    for (int j = 0; j < 64; j ++)
                    {
                        display[i][j] = 0;
                    }
                }

                // put font set into RAM
                for (int i = 0; i < 0x50; i ++)
                {
                    RAM[i] = chip8font[i];
                }

                // initialize sound
                soundBuffer.loadFromFile("../sound/tone.wav");
                buzzer.setBuffer(soundBuffer);
                buzzer.setLoop(true);

                // seed randomness
                srand(time(NULL));
            }

            u_int8_t* getRAM() { return RAM; }
            int* getPC() { return &PC; }
            int* getSP() { return &SP; }
            u_int8_t* getDT() { return &DT; }
            u_int8_t* getST() { return &ST; }
            void decrementDT() { DT --; }
            void decrementST() { ST --; }
            int** getDisplay() { return display; }
            u_int8_t* getRegisterV(int index) { return &this->V[index]; }
            u_int16_t* getRegisterI() { return &this->I; }
            sf::Sound* getBuzzer() { return &buzzer; }

            void setKeyState(bool state, int key) { keys[key] = state; }

            void increasePC() { PC += 1; }
            void resetPC() { PC = 0x200; }

            void emulateCycle();

};


#endif //CHIP8_CHIP8_H
