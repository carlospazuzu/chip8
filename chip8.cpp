//
// Created by pazuzu on 14/04/19.
//

#include <iostream>
#include "chip8.h"

void Chip8::clearDisplay()
{
    for (int i = 0; i < 32; i ++)
        for (int j = 0; j < 64; j ++)
            display[i][j] = 0;
}

short Chip8::getByte(u_int16_t opcode)
{
    return (opcode & 0x00FF);
}

short Chip8::getNNN(u_int16_t opcode)
{
    return (opcode & 0x0FFF);
}

short Chip8::getX(u_int16_t opcode)
{
    return (opcode & 0x0F00) >> 8;
}

short Chip8::getY(u_int16_t opcode)
{
    return (opcode & 0x00F0) >> 4;
}

short Chip8::getN(u_int16_t opcode)
{
    return (opcode & 0x000F);
}

void Chip8::emulateCycle()
{
    u_int16_t opcode = (RAM[PC] << 8) | RAM[PC + 1];

    switch (opcode & 0xF000)
    {

        case 0x0000:
        {
            switch (opcode & 0x00FF)
            {

                // 00E0 - CLS
                // clear the display
                case 0x00E0:
                {
                    clearDisplay();
                    PC += 2;
                }
                            break;
                // 00EE - RET
                // return from a subroutine
                // decrements SP and set PC to stack value SP is now poiting
                case 0x00EE:
                {
                    if (SP == 0)
                    {
                        std::cout << "Error! stack is empty!\n";
                        exit(1);
                    }

                    SP --;
                    PC = stack[SP];
                    PC += 2; // to avoid the function who was previously called be called again
                }
                    break;
            }
        }
                    break;

        // 1nnn - JP addr
        // jump to location nnn
        case 0x1000:
        {
            short nnn = getNNN(opcode);
            PC = nnn;
        }
                    break;

        // 2nnn - CALL addr
        // call subroutine at nnn
        case 0x2000:
        {
            if (SP == 16)
            {
                std::cout << "Error! Stack is full!" << "\n";
                exit(1);
            }

            short nnn = getNNN(opcode);

            stack[SP] = PC;
            SP ++;
            PC = nnn;
        }
                    break;

        // 3xkk - SE Vx, byte
        // skip next instruction if Vx = kk
        case 0x3000:
        {
            short x = getX(opcode);
            short kk = getByte(opcode);

            if (V[x] == kk)
                PC += 4;
            else
                PC += 2;
        }
                    break;

        // 3xkk - SNE Vx, byte
        // skip next instruction if Vx != kk
        case 0x4000:
        {
            short x = getX(opcode);
            short kk = getByte(opcode);

            if (V[x] != kk)
                PC += 4;
            else
                PC += 2;
        }
                    break;

        // 5xy0 - SE Vx, Vy
        // skip next instruction if Vx = Vy
        case 0x5000:
        {
            short x = getX(opcode);
            short y = getY(opcode);

            if (V[x] == V[y])
                PC += 4;
            else
                PC += 2;
        }
                    break;

        // 6xkk - LD Vx, byte
        // puts the value kk in the register Vx
        case 0x6000:
        {
            short x = getX(opcode);
            short kk = getByte(opcode);

            V[x] = kk;

            PC += 2;
        }
                    break;

        // 7xkk - ADD Vx - byte
        // adds the value kk to the value of register Vx and stores the result in Vx
        case 0x7000:
        {
            short x = getX(opcode);
            short kk = getByte(opcode);

            V[x] += kk;

            PC += 2;
        }
                    break;

        case 0x8000:
        {
            short x = getX(opcode);
            short y = getY(opcode);

            switch (opcode & 0x000F)
            {
                // 8xy0 - LD Vx, Vy
                // set Vx = Vy
                case 0x0000:
                {
                    V[x] = V[y];
                    PC += 2;
                }
                            break;

                // 8xy1 - OR Vx, Vy
                // set Vx = Vx OR Vy
                case 0x0001:
                {
                    V[x] = V[x] | V[y];
                    PC += 2;
                }
                            break;
                // 8xy2 - AND Vx, Vy
                // set Vx = Vx AND Vy
                case 0x0002:
                {
                    V[x] = V[x] & V[y];
                    PC += 2;
                }
                            break;

                // 8xy2 - XOR Vx, Vy
                // set Vx = Vx XOR Vy
                case 0x0003:
                {
                    V[x] = V[x] ^ V[y];
                    PC += 2;
                }
                            break;

                // 8xy4 - ADD Vx, Vy
                // set Vx = Vx + Vy, set VF = 1 if Vx > 255
                case 0x0004:
                {
                    V[x] = V[x] + V[y];

                    if (V[x] > 255)
                    {
                        V[x] &= 0xFF;
                        V[0xF] = 1;
                    }
                    else
                        V[0xF] = 0;

                    PC += 2;
                }
                            break;

                // 8xy5 - SUB Vx, Vy
                // set Vx = Vx - Vy, set VF = NOT borrow
                case 0x0005:
                {
                    if (V[x] > V[y])
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;

                    V[x] = V[x] - V[y];
                    PC += 2;
                }
                            break;

                // 8xy6 - SHR Vx {, Vy }
                // set Vx = Vx SHR 1
                case 0x0006:
                {
                    // if least significant bit is 1, set VF = 1
                    if (V[x] & 1 == 1)
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;

                    V[x] = V[x] >> 1;
                    PC += 2;
                }
                            break;

                // 8xy7 - SUBN Vx, Vy
                // set Vx = Vy - Vx, VF = NOT borrow
                case 0x0007:
                {
                    if (V[y] > V[x])
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;

                    V[x] = V[y] - V[x];
                    PC += 2;
                }
                            break;

                // 8xyE - SHL Vx {, Vy}
                // set Vx = Vx SHL 1
                case 0x000E:
                {
                    if (0x80 & V[x] == 1)
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;

                    V[x] = V[x] << 1;
                    PC += 2;
                }
                            break;
            }
        }
                    break;

        // 9xy0 - SNE Vx, Vy
        // skip next instruction if Vx != Vy
        case 0x9000:
        {
            short x = getX(opcode);
            short y = getY(opcode);

            if (V[x] != V[y])
                PC += 4;
            else
                PC += 2;
        }
                    break;

        // Annn - LD I, addr
        // the value of the register I is set to nnn
        case 0xA000:
        {
            short nnn = getNNN(opcode);

            I = nnn;

            PC += 2;
        }
                    break;

        // Bnnn - JP V0, addr
        // jump to location at nnn + V0
        case 0xB000:
        {
            short nnn = getNNN(opcode);
            PC = V[0] + nnn;
        }
                    break;

        // Cxkk - RND Vx, byte
        // set Vx = random byte AND kk
        case 0xC000:
        {
            short rng = rand() % 256;

            short x = getX(opcode);
            short kk = getByte(opcode);

            V[x] = rng & kk;
            PC += 2;
        }
                    break;

        // Dxyn - DRW Vx, Vy, n
        // draw to screen n-byte sprite starting at memory location I at (Vx, Vy)
        // if collision, VF = 1
        case 0xD000:
        {
            short x = getX(opcode);
            short y = getY(opcode);
            short n = getN(opcode);

            V[0xF] = 0;
            for (int line = 0; line < n; line ++)
            {
                short sprite = RAM[I + line];
                for (int column = 0; column < 8; column ++)
                {
                    short pixel = sprite & (0x80 >> column);
                    if (pixel)
                    {
                        if (display[(V[y] + line) % 32][(V[x] + column) % 64])
                            V[0xF] = 1;
                        display[(V[y] + line) % 32][(V[x] + column) % 64] ^= 1;
                    }
                }
            }

            PC += 2;
        }
                    break;

        case 0xE000:
        {
            short x = getX(opcode);

            switch (opcode & 0x00FF)
            {
                // Ex9E - SKP Vx
                // skip next instruction if key with value of Vx is pressed
                case 0x009E:
                {
                    if (keys[V[x]])
                        PC += 4;
                    else
                        PC += 2;
                }
                    break;
                // ExA1 - SKNP Vx
                // skip next instruction if key with value of Vx is not pressed
                case 0x00A1:
                {
                    if (!keys[V[x]])
                        PC += 4;
                    else
                        PC += 2;
                }
                            break;
            }
        }
                    break;

        case 0xF000:
        {
            short x = getX(opcode);

            switch (opcode & 0x00FF)
            {
                // Fx07 - LD Vx, DT
                // value of DT is placed into Vx
                case 0x0007:
                {
                    V[x] = DT;
                    PC += 2;
                }
                            break;

                // Fx0A - LD Vx, K
                // wait for a key press, store the value of the key in Vx
                case 0x000A:
                {
                    bool keepGoing = false;
                    for (int i = 0; i <= 0xF; i ++)
                    {
                        if (keys[i])
                        {
                            V[x] = i;
                            keepGoing = true;
                            break;
                        }
                    }

                    if (keepGoing) PC += 2;

                }
                            break;

                // Fx15 - LD DT, Vx
                // set delay timer to Vx value
                case 0x0015:
                {
                    DT = V[x];
                    PC += 2;
                }
                            break;

                // Fx18 - LD ST, Vx
                // set sound timer = Vx
                case 0x0018:
                {
                    ST = V[x];
                    PC += 2;
                }
                            break;

                // Fx1E - ADD I, Vx
                // add I + Vx and store results in I
                case 0x001E:
                {
                    I = I + V[x];
                    PC += 2;
                }
                            break;
                // Fx29 - LD F, Vx
                // set I = location of sprite for digit Vx
                case 0x0029:
                {
                    I = V[x] * 0x5;

                    PC += 2;
                }
                            break;

                // Fx33 - LD B, Vx
                // store BDC representation of Vx in memory locations I, I + 1 and I + 2
                case 0x0033:
                {
                    short hundreds = V[x] / 100;
                    short tens = (V[x] / 10) % 10;
                    short units = V[x] % 10;

                    RAM[I] = hundreds;
                    RAM[I + 1] = tens;
                    RAM[I + 2] = units;

                    PC += 2;
                }
                            break;

                // Fx55 - LD [I], Vx
                // store registers V0 through Vx in memory location starting at I
                case 0x0055:
                {
                    for (int i = 0; i <= x; i ++)
                    {
                        RAM[I + i] = V[i];
                    }

                    PC += 2;
                }
                            break;

                // Fx65 - LD Vx, [I]
                // puts memory content starting at location I into registers V0 through Vx
                case 0x0065:
                {
                    for (int i = 0; i <= x; i ++)
                    {
                        V[i] = RAM[I + i];
                    }

                    PC += 2;
                }
                            break;
            }
        }
                    break;
    }
}