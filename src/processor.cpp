#include "processor.h"
#include <iostream>

Processor::Processor(Memory &memory, Display &display) : stack(),
                                                         v(),
                                                         vi(),
                                                         pc(0x200),
                                                         memory(memory),
                                                         display(display) {}

Processor::~Processor() = default;

void Processor::cycle()
{
    u_int16_t instr = Processor::fetch();
    Nibbles nibbles = Processor::decode(instr);
    Processor::execute(nibbles);
}

u_int16_t Processor::fetch()
{
    u_int16_t instruction = (memory[pc] << 8) + (memory[pc + 1]);
    pc += 2;
    return instruction;
}

Nibbles Processor::decode(u_int16_t inst)
{
    return Nibbles(
        ((inst >> 12) & 0x0f),
        ((inst >> 8) & 0x0f),
        ((inst >> 4) & 0x0f),
        ((inst) & 0x000f),
        ((inst) & 0x00ff),
        ((inst) & 0x0fff));
}

void Processor::execute(Nibbles nibbles)
{
    ((*this).*(dispatch_table[nibbles.type]))(nibbles);
}

void Processor::op_0x0(Nibbles nibbles)
{
    if (nibbles.nn == 0xe0)
        display.clear();
}
void Processor::op_0x1(Nibbles nibbles)
{
    pc = nibbles.nnn;
}
void Processor::op_0x2(Nibbles nibbles)
{
}
void Processor::op_0x3(Nibbles nibbles)
{
}
void Processor::op_0x4(Nibbles nibbles)
{
}
void Processor::op_0x5(Nibbles nibbles)
{
}
void Processor::op_0x6(Nibbles nibbles)
{
    v[nibbles.x] = nibbles.nn;
}
void Processor::op_0x7(Nibbles nibbles)
{
    v[nibbles.x] += nibbles.nn;
}
void Processor::op_0x8(Nibbles nibbles)
{
}
void Processor::op_0x9(Nibbles nibbles)
{
}
void Processor::op_0xa(Nibbles nibbles)
{
    vi = nibbles.nnn;
}
void Processor::op_0xb(Nibbles nibbles)
{
}
void Processor::op_0xc(Nibbles nibbles)
{
}
void Processor::op_0xd(Nibbles nibbles)
{
    // Display
    u_int16_t x_cord = v[nibbles.x] % Display::COLS;
    u_int16_t y_cord = v[nibbles.y] % Display::ROWS;
    v[0xf] = 0; // collision register

    for (int i = 0; i < nibbles.n && y_cord + i <= Display::ROWS; i++)
    {
        u_int16_t row_byte = memory[vi + i];
        for (int j = 0; j < 0x8 && x_cord + j <= Display::COLS; j++)
        {
            u_int16_t pixel = ((row_byte) >> (0x7 - j) & 0b1);
            if (pixel && display(x_cord + j, y_cord + i))
                v[0xf] = 1;

            display(x_cord + j, y_cord + i) ^= pixel;
        }
    }
    display.draw();
}
void Processor::op_0xe(Nibbles nibbles)
{
}
void Processor::op_0xf(Nibbles nibbles)
{
}