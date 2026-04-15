#pragma once

#include <cstdint>
#include <array>
#include <stack>
#include "memory.h"
#include "display.h"

class Processor
{
public:
    Processor();
    ~Processor();

    u_int16_t fetch(Memory &memory);
    void decode_execute(Memory &memory, Display &display, u_int16_t instruction);

private:
    std::stack<uint16_t> stack;
    std::array<uint16_t, 16> v; // labeled V0 to VF
    uint16_t vi;
    uint16_t pc;
};