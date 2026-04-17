#pragma once

#include <cstdint>
#include <array>
#include <stack>
#include "memory.h"
#include "display.h"

struct Nibbles
{
    u_int16_t type, x, y, n, nn, nnn;
    Nibbles(u_int16_t type, u_int16_t x, u_int16_t y, u_int16_t n, u_int16_t nn, u_int16_t nnn)
        : type(type), x(x), y(y), n(n), nn(nn), nnn(nnn) {};
};

class Processor
{
public:
    Processor(Memory &memory, Display &display);
    ~Processor();

    void cycle();

private:
    std::stack<uint16_t> stack;
    std::array<uint8_t, 16> v; // labeled V0 to VF
    uint16_t vi;
    uint16_t pc;
    uint8_t delay_timer;
    uint8_t sound_timer;
    Memory &memory;
    Display &display;
    Display::Keys keys;

    u_int16_t fetch();
    Nibbles decode(u_int16_t instruction);
    void execute(Nibbles nibbles);

    using InstructionHandler = void (Processor::*)(Nibbles nibbles);

    void op_0x0(Nibbles nibbles);
    void op_0x1(Nibbles nibbles);
    void op_0x2(Nibbles nibbles);
    void op_0x3(Nibbles nibbles);
    void op_0x4(Nibbles nibbles);
    void op_0x5(Nibbles nibbles);
    void op_0x6(Nibbles nibbles);
    void op_0x7(Nibbles nibbles);
    void op_0x8(Nibbles nibbles);
    void op_0x9(Nibbles nibbles);
    void op_0xa(Nibbles nibbles);
    void op_0xb(Nibbles nibbles);
    void op_0xc(Nibbles nibbles);
    void op_0xd(Nibbles nibbles);
    void op_0xe(Nibbles nibbles);
    void op_0xf(Nibbles nibbles);

    const std::array<InstructionHandler, 16> dispatch_table =
        {
            &Processor::op_0x0,
            &Processor::op_0x1,
            &Processor::op_0x2,
            &Processor::op_0x3,
            &Processor::op_0x4,
            &Processor::op_0x5,
            &Processor::op_0x6,
            &Processor::op_0x7,
            &Processor::op_0x8,
            &Processor::op_0x9,
            &Processor::op_0xa,
            &Processor::op_0xb,
            &Processor::op_0xc,
            &Processor::op_0xd,
            &Processor::op_0xe,
            &Processor::op_0xf,
    };
};