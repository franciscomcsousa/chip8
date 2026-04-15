#include "memory.h"
#include <iostream>
#include <fstream>

Memory::Memory() : data()
{
    for (size_t i = 0; i < fonts.size(); i++)
        data[i + 0x50] = fonts[i];
}

Memory::~Memory() = default;

u_int16_t &Memory::operator[](uint16_t address)
{
    return data[address];
}

void Memory::load_rom(std::filesystem::path &filepath)
{
    std::ifstream file(filepath, std::ios::binary);

    if (!file)
        throw std::runtime_error("Failed to open file: " + filepath.string());

    file.seekg(0, file.end);
    size_t length = file.tellg();
    file.seekg(0, file.beg);

    size_t max_rom_size = MEMORYSIZE - 0x200;
    if (length > max_rom_size)
        throw std::runtime_error("ROM file too large");

    char *buffer = new char[length];
    file.read(buffer, length);

    for (size_t i = 0; i < length; i++)
        data[i + 0x200] = (unsigned char)buffer[i];
    
    delete[] buffer;
}

void Memory::dump()
{
    for (size_t i = 0x0; i < MEMORYSIZE; i++)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << data[i] << " ";
        if (i % 32 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
}