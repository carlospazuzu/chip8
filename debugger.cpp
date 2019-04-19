//
// Created by pazuzu on 14/04/19.
//

#include "debugger.h"
#include <cstdio>

short Debugger::getByte(u_int16_t opcode)
{
    return (opcode & 0x00FF);
}

short Debugger::getNNN(u_int16_t opcode)
{
    return (opcode & 0x0FFF);
}

short Debugger::getX(u_int16_t opcode)
{
    return (opcode & 0x0F00) >> 8;
}

short Debugger::getY(u_int16_t opcode)
{
    return (opcode & 0x00F0) >> 4;
}

short Debugger::getN(u_int16_t opcode)
{
    return (opcode & 0x000F);
}

std::string Debugger::getInstruction(u_int16_t opcode, int currentPC)
{
    std::string result;
    char str[30];

    switch (opcode & 0xF000)
    {
        case 0x0000:
        {
            switch (opcode & 0x0FF)
            {
                case 0x00E0:
                    sprintf(str, "%X:  \t\t  CLS", currentPC);
                    break;

                case 0x0EE:
                    sprintf(str, "%X:  \t  RET", currentPC);
                    break;
            }
        }
                    break;

        case 0x1000:
        {
            short addr = getNNN(opcode);

            sprintf(str, "%X:  \t  JP \t\t%X", currentPC, addr);

        }
                    break;

        case 0x2000:
        {
            short addr = getNNN(opcode);

            sprintf(str, "%X:  \t  CALL \t%X", currentPC, addr);

        }
                    break;

        case 0x3000:
        {
            short regX = getX(opcode);
            short kk = getByte(opcode);
            sprintf(str, "%X:  \t  SE   \t\tV%X, \t%d", currentPC, regX, kk);
        }
                    break;

        case 0x4000:
        {
            short regX = getX(opcode);
            short kk = getByte(opcode);
            sprintf(str, "%X:  \t  SNE   \tV%X, \t%d", currentPC, regX, kk);
        }
                    break;

        case 0x5000:
        {
            short regX = getX(opcode);
            short regY = getY(opcode);
            sprintf(str, "%X:  \t  SE   \tV%X, \tV%X", currentPC, regX, regY);
        }
            break;

        case 0x6000:
        {
            short regX = getX(opcode);
            short kk = getByte(opcode);

            sprintf(str, "%X:  \t  LD   \t\tV%X, \t%d", currentPC, regX, kk);

        }
                    break;

        case 0x7000:
        {
            short regX = getX(opcode);
            short kk = getByte(opcode);

            sprintf(str, "%X:  \t  ADD   \tV%X, \t%d", currentPC, regX, kk);

        }
            break;

        case 0x8000:
        {
            short regX = getX(opcode);
            short regY = getY(opcode);

            switch(opcode & 0x000F)
            {
                case 0x0000:
                {
                    sprintf(str, "%X:  \t  LD   \t\tV%X, \tV%X", currentPC, regX, regY);
                }
                            break;

                case 0x0001:
                {
                    sprintf(str, "%X:  \t  OR   \t\tV%X, \tV%X", currentPC, regX, regY);
                }
                            break;

                case 0x0002:
                {
                    sprintf(str, "%X:  \t  AND  \tV%X, \tV%X", currentPC, regX, regY);
                }
                            break;

                case 0x0003:
                {
                    sprintf(str, "%X:  \t  XOR  \tV%X, \tV%X", currentPC, regX, regY);
                }
                            break;

                case 0x0004:
                {
                    sprintf(str, "%X:  \t  ADD  \tV%X, \tV%X", currentPC, regX, regY);
                }
                            break;

                case 0x0005:
                {
                    sprintf(str, "%X:  \t  SUB \tV%X, \tV%X", currentPC, regX, regY);
                }
                            break;

                case 0x0006:
                {
                    sprintf(str, "%X:  \t  SHR  \tV%X {, \t  V%X}", currentPC, regX, regY);
                }
                            break;

                case 0x0007:
                {
                    sprintf(str, "%X:  \t  SUBN  \tV%X, \tV%X", currentPC, regX, regY);
                }
                            break;

                case 0x000E:
                {
                    sprintf(str, "%X:  \t  SHL  \tV%X {, \t  V%X}", currentPC, regX, regY);
                }
                            break;
            }
        }
                    break;

        case 0x9000:
        {
            short regX = getX(opcode);
            short regY = getY(opcode);

            sprintf(str, "%X:  \t  SNE  \tV%X, \tV%X", currentPC, regX, regY);
        }
                    break;

        case 0xA000:
        {
            short nnn = getNNN(opcode);
            sprintf(str, "%X:  \t  LD   \t\tI, \t  %X", currentPC, nnn);
        }
                    break;

        case 0xB000:
        {
            short nnn = getNNN(opcode);
            sprintf(str, "%X:  \t  JP   \t\tV0,  %X", currentPC, nnn);
        }
                    break;

        case 0xC000:
        {
            short regX = getX(opcode);
            short kk = getByte(opcode);

            sprintf(str, "%X:  \t  RND  \tV%X, \t%X", currentPC, regX, kk);
        }
                    break;

        case 0xD000:
        {
            short regX = getX(opcode);
            short regY = getY(opcode);
            short n = getN(opcode);
            sprintf(str, "%X:  \t  DRW   \tV%X, \tV%X, \t%d", currentPC, regX, regY, n);
        }
                    break;

        case 0xE000:
        {
            switch(opcode & 0x00FF)
            {
                case 0x009E:
                {
                    short regX = getX(opcode);
                    sprintf(str, "%X:  \t  SKP   \tV%X", currentPC, regX);
                }
                            break;

                case 0x00A1:
                {
                    short regX = getX(opcode);
                    sprintf(str, "%X:  \t  SKNP   \tV%X", currentPC, regX);
                }
                    break;
            }
        }
                    break;

        case 0xF000:
        {
            short regX = getX(opcode);
            switch (opcode & 0x00FF)
            {
                case 0x0007:
                {
                    sprintf(str, "%X:  \t  LD   \t\tV%X, \tDT", currentPC, regX);
                }
                            break;

                case 0x000A:
                {
                    sprintf(str, "%X:  \t  LD   \t\tV%X, \tK", currentPC, regX);
                }
                    break;

                case 0x0015:
                {
                    sprintf(str, "%X:  \t  LD   \t\tDT, \tV%X", currentPC, regX);
                }
                    break;

                case 0x0018:
                {
                    sprintf(str, "%X:  \t  LD   \t\tST, \tV%X", currentPC, regX);
                }
                    break;

                case 0x001E:
                {
                    sprintf(str, "%X:  \t  ADD   \tI, \tV%X", currentPC, regX);
                }
                    break;

                case 0x0029:
                {
                    sprintf(str, "%X:  \t  LD   \t\tF, \tV%X", currentPC, regX);
                }
                    break;

                case 0x0033:
                {
                    sprintf(str, "%X:  \t  LD   \t\tB, \tV%X", currentPC, regX);
                }
                    break;

                case 0x0055:
                {
                    sprintf(str, "%X:  \t  LD   \t\t[I], \tV%X", currentPC, regX);
                }
                    break;

                case 0x0065:
                {
                    sprintf(str, "%X:  \t  LD   \t\tV%X, \t[I]", currentPC, regX);
                }
                    break;
            }
        }
                    break;

        default:
        {
            sprintf(str, "%X:  \t  %X", currentPC, opcode);
        }
                break;
    }

    result = str;

    return result;
}

bool Debugger::isBranchInstruction(u_int16_t opcode)
{
    // TODO implement all branch instructions
    return (opcode & 0xF000) == 0x2000 || opcode == 0x00EE || (opcode & 0xF000) == 0x1000;
}

void Debugger::drawRAM()
{
    int currentPC = *chip8->getPC();

    background->setPosition(20, 20 + 320 + 24 + 4);
    window->draw(*background);
    background->setPosition(360, 20 + 320 + 24 + 4);
    window->draw(*background);

    if (changeRAMLocation)
    {
        instructionMarker = currentPC - 2;
        changeRAMLocation = false;
    }

    // check if the next instruction will be a branch instruction
    if (isBranchInstruction((chip8->getRAM()[currentPC] << 8) | chip8->getRAM()[currentPC + 1]))
        changeRAMLocation = true;

    sf::RectangleShape rect(sf::Vector2f(300, 20));
    rect.setFillColor(*light);
    rect.setPosition(20, 20 + 320 + 24 + 4 + 25 * ((currentPC - instructionMarker) / 2));
    window->draw(rect);

    int staticInstructionMarker = instructionMarker;

    for (int i = 0; i < 10; i ++)
    {
        short higherByte = chip8->getRAM()[instructionMarker];
        short lowerByte = chip8->getRAM()[instructionMarker + 1];

        currentInstruction = (higherByte << 8) | lowerByte;

        text.setString(getInstruction(currentInstruction, instructionMarker));
        text.setPosition(24, 20 + 320 + 24 + i * 25);

        if ((currentPC - staticInstructionMarker) / 2 == i)
            text.setFillColor(*dark);
        else
            text.setFillColor(*light);

        window->draw(text);

        instructionMarker += 2;
    }

    instructionMarker = staticInstructionMarker;

    if ((currentPC - staticInstructionMarker) / 2 >= 9)
        changeRAMLocation = true;

    short i = ((chip8->getRAM()[currentPC] << 8) | chip8->getRAM()[currentPC + 1]);

    if ((i & 0xF000 == 0xE000) && (i & 0x00FF == 0x00A1))
        changeRAMLocation = true;

}

void Debugger::drawText(char *str, float x, float y)
{
    text.setPosition(x, y);
    text.setString(str);
    window->draw(text);
}

void Debugger::drawRegistersStatus()
{
    char str[20];

    text.setFillColor(*light);

    // draw registers from V0 to VA
    for (int i = 0; i < 10; i ++)
    {
        sprintf(str, "V%X \t= \t %d", i, *chip8->getRegisterV(i));
        drawText(str, 364, 20 + 320 + 24 + i * 25);
    }

    // draw registers from VB to VF
    for (int i = 0; i <= 5; i ++)
    {
        sprintf(str, "V%X \t= \t %d", i + 0xA, *chip8->getRegisterV(i + 0xA));
        drawText(str, 524, 20 + 320 + 24 + i * 25);
    }

    // draw register I
    sprintf(str, "   I \t= \t %X", *chip8->getRegisterI());
    drawText(str, 524, 514);

    // draw program counter
    sprintf(str, "PC \t= \t %X", *chip8->getPC());
    drawText(str, 524, 539);

    // draw stack pointer
    sprintf(str, "SP \t= \t %d", *chip8->getSP());
    drawText(str, 524, 564);

    // draw delay timer
    sprintf(str, "DT \t= \t %d", *chip8->getDT());
    drawText(str, 524, 589);

}