#include "memory.h"
#include <iostream>

Memory::Memory() : data() {}

Memory::~Memory() = default;

u_int16_t &Memory::operator[](uint16_t address)
{
    return data[address];
}

void Memory::load_rom(std::filesystem::path &filepath)
{
    // TODO
}

void Memory::dump()
{
    for (size_t i = 0; i < size(data); i++)
    {
        std::cout << std::setfill('0') << std::setw(4) << std::hex << data[i] << " ";
    }
}