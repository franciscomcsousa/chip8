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
    display.get_input();
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
    else if (nibbles.nn == 0xee)
    {
        pc = stack.top();
        stack.pop();
    }
}
void Processor::op_0x1(Nibbles nibbles)
{
    pc = nibbles.nnn;
}
void Processor::op_0x2(Nibbles nibbles)
{
    stack.push(pc);
    pc = nibbles.nnn;
}
void Processor::op_0x3(Nibbles nibbles)
{
    if (v[nibbles.x] == nibbles.nn)
        pc += 2;
}
void Processor::op_0x4(Nibbles nibbles)
{
    if (v[nibbles.x] != nibbles.nn)
        pc += 2;
}
void Processor::op_0x5(Nibbles nibbles)
{
    if (v[nibbles.x] == v[nibbles.y])
        pc += 2;
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
    // Logic and Arithmetic
    switch (nibbles.n)
    {
    case 0x0: // set
        v[nibbles.x] = v[nibbles.y];
        break;
    case 0x1: // or
        v[nibbles.x] |= v[nibbles.y];
        break;
    case 0x2: // and
        v[nibbles.x] &= v[nibbles.y];
        break;
    case 0x3: // xor
        v[nibbles.x] ^= v[nibbles.y];
        break;
    case 0x4: // add
    {
        int result = v[nibbles.x] + v[nibbles.y];
        v[nibbles.x] = result % (0xff + 1);
        result > 0xff ? v[0xf] = 1 : v[0xf] = 0;
        break;
    }
    case 0x5: // vx - vy
    {
        v[nibbles.x] >= v[nibbles.y] ? v[0xf] = 1 : v[0xf] = 0;
        v[nibbles.x] -= v[nibbles.y];
    }
    case 0x6: // shift left
    {
        // TODO - implement CHIP48 version as well
        v[nibbles.x] = v[nibbles.y];
        v[0xf] = v[nibbles.x] & 0b1;
        v[nibbles.x] >>= 1;
    }
    case 0x7: // vy - vx
    {
        v[nibbles.y] >= v[nibbles.x] ? v[0xf] = 1 : v[0xf] = 0;
        v[nibbles.x] = v[nibbles.y] - v[nibbles.x];
        ;
    }
    case 0xe: // shift right
    {
        // TODO - implement CHIP48 version as well
        v[nibbles.x] = v[nibbles.y];
        v[0xf] = v[nibbles.x] & 0b1;
        v[nibbles.x] <<= 1;
    }

    default:
        break;
    }
}
void Processor::op_0x9(Nibbles nibbles)
{
    if (v[nibbles.x] != v[nibbles.y])
        pc += 2;
}
void Processor::op_0xa(Nibbles nibbles)
{
    vi = nibbles.nnn;
}
void Processor::op_0xb(Nibbles nibbles)
{
    // TODO - implement CHIP48 version as well
    pc = v[0x0] + nibbles.nnn;
}
void Processor::op_0xc(Nibbles nibbles)
{
    // TODO
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
    // TODO
}
void Processor::op_0xf(Nibbles nibbles)
{
    // TODO
}