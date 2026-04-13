#pragma once

#include <vector>
#include <cstdint>
#include <array>

class Processor
{
public:
    Processor();
    ~Processor();

private:
    std::vector<uint16_t> stack;
    std::array<uint16_t, 16> v;
    uint16_t pc;
    uint16_t vi;
}