#include "processor.h"
#include <iostream>

Processor::Processor() : stack(), v(), vi(), pc(0x200) {}

Processor::~Processor() = default;

// TODO - make memory const
u_int16_t Processor::fetch(Memory &memory)
{
    u_int16_t instruction = (memory[pc] << 8) + (memory[pc + 1]);
    pc += 2;
    return instruction;
}

void Processor::decode_execute(Memory &memory, Display &display, u_int16_t inst)
{
    u_int16_t type = (inst >> 12) & 0x0f;
    u_int16_t x = (inst >> 8) & 0x0f;
    u_int16_t y = (inst >> 4) & 0x0f;
    u_int16_t n = inst & 0x000f;
    u_int16_t nn = inst & 0x00ff;
    u_int16_t nnn = inst & 0x0fff;

    switch (type)
    {
    case 0x0:
    {
        if (nn == 0xe0)
            display.clear();
        break;
    }
    case 0x1:
    {
        pc = nnn;
        break;
    }
    case 0x2:
        break;
    case 0x3:
        break;
    case 0x4:
        break;
    case 0x5:
        break;
    case 0x6:
    {
        v[x] = nn;
        break;
    }
    case 0x7:
    {
        v[x] += nn;
        break;
    }
    case 0x8:
        break;
    case 0x9:
        break;
    case 0xA:
        vi = nnn;
        break;
    case 0xB:
        break;
    case 0xC:
        break;
    case 0xD:
    {
        // Display
        u_int16_t x_cord = v[x] % Display::COLS;
        u_int16_t y_cord = v[y] % Display::ROWS;
        v[0xf] = 0; // collision register

        for (int i = 0; i < n && y_cord + i <= Display::ROWS; i++)
        {
            u_int16_t row_byte = memory[vi + i];
            for (int j = 0; j < 0x8 && x_cord + j <= Display::COLS; j++)
            {
                u_int16_t pixel = (row_byte) >> (0x7 - j) & 0b1;
                if (pixel && display(x_cord + j, y_cord + i))
                    v[0xf] = 1;

                display(x_cord + j, y_cord + i) ^= pixel;
            }
        }
        display.draw();
        break;
    }
    case 0xE:
        break;
    case 0xF:
        break;
    }
}