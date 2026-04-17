#include "processor.h"
#include <iostream>

Processor::Processor(Memory &memory, Display &display) : stack(), v(), vi(), pc(0x200), memory(memory), display(display)
{
    std::srand(std::time({}));
}

Processor::~Processor() = default;

void Processor::cycle()
{
    keys = display.get_input();

    // TODO - should be synchronized at 60hz, not with cpu clock
    if (delay_timer > 0)
        delay_timer--;

    if (sound_timer > 0)
        sound_timer--;

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
        break;
    }
    case 0x6: // shift left
    {
        // TODO - implement classic version as well
        // v[nibbles.x] = v[nibbles.y];
        v[0xf] = v[nibbles.x] & 0b1;
        v[nibbles.x] >>= 1;
        break;
    }
    case 0x7: // vy - vx
    {
        v[nibbles.y] >= v[nibbles.x] ? v[0xf] = 1 : v[0xf] = 0;
        v[nibbles.x] = v[nibbles.y] - v[nibbles.x];
        break;
    }
    case 0xe: // shift right
    {
        // TODO - implement classic version as well
        // v[nibbles.x] = v[nibbles.y];
        v[0xf] = v[nibbles.x] & 0b1;
        v[nibbles.x] <<= 1;
        break;
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
    // TODO - implement classic version as well
    // pc = v[0x0] + nibbles.nnn;
    pc = v[nibbles.x] + nibbles.nnn;
}
void Processor::op_0xc(Nibbles nibbles)
{
    v[nibbles.x] = std::rand() & nibbles.nn;
}
void Processor::op_0xd(Nibbles nibbles)
{
    // Display
    u_int8_t x_cord = v[nibbles.x] % Display::COLS;
    u_int8_t y_cord = v[nibbles.y] % Display::ROWS;
    v[0xf] = 0; // collision register

    for (int i = 0; i < nibbles.n && y_cord + i <= Display::ROWS; i++)
    {
        u_int8_t row_byte = memory[vi + i];
        for (int j = 0; j < 0x8 && x_cord + j <= Display::COLS; j++)
        {
            u_int8_t pixel = ((row_byte) >> (0x7 - j) & 0b1);
            if (pixel && display(x_cord + j, y_cord + i))
                v[0xf] = 1;

            display(x_cord + j, y_cord + i) ^= pixel;
        }
    }
    display.draw();
}
void Processor::op_0xe(Nibbles nibbles)
{
    if ((nibbles.nn == 0x9e && keys[nibbles.x] == 1) || (nibbles.nn == 0xa1 && keys[nibbles.x] == 0))
        pc += 2;
}
void Processor::op_0xf(Nibbles nibbles)
{
    switch (nibbles.nn)
    {
    case 0x07:
        v[nibbles.x] = delay_timer;
        break;
    case 0x15:
        delay_timer = v[nibbles.x];
        break;
    case 0x18:
        sound_timer = v[nibbles.x];
        break;
    case 0x1e:
    {
        vi + v[nibbles.x] > 0xfff ? v[0xf] = 1 : v[0xf] = 0;
        vi += v[nibbles.x];
        break;
    }
    case 0x0a:
    {
        pc -= 2;
        for (u_int8_t i = 0; i < v.size(); i++)
        {
            if (keys[i] == 1)
            {
                v[nibbles.x] = i;
                pc += 2;
                break;
            }
        }
        break;
    }
    case 0x29:
    {
        vi = Memory::FONTSTART * (v[nibbles.x] * 5);
        break;
    }
    case 0x33:
    {
        u_int16_t num = v[nibbles.x];
        for (int i = 2; i >= 0; i--)
        {
            memory[vi + i] = num % 10;
            num /= 10;
        }
        break;
    }
    case 0x55:
    {
        // TODO - implement classic form as well
        for (int i = 0; i <= 0xf; i++)
        {
            v[i] = memory[i + vi];
        }
        break;
    }
    case 0x65:
    {
        // TODO - implement classic form as well
        for (int i = 0; i <= 0xf; i++)
        {
            memory[i + vi] = v[i];
        }
        break;
    }
    default:
        break;
    }
}