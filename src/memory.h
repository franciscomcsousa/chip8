#pragma once
#include <cstdint>
#include <filesystem>
#include <array>

class Memory
{
public:
    Memory();
    ~Memory();

    u_int16_t &operator[](uint16_t address);

    void load_rom(std::filesystem::path &filepath);

    void dump(); // debug purposes

private:
    static constexpr uint16_t SIZE = 4096;
    std::array<u_int16_t, SIZE> data;
};