#pragma once

#include <cstdint>
#include <array>
#include <stack>

class Processor
{
public:
    Processor();
    ~Processor();

private:
    std::stack<uint16_t> stack;
    std::array<uint16_t, 16> v; // labeled V0 to VF
    uint16_t vi;
    uint16_t pc;
};