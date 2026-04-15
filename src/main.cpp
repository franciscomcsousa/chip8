#include "display.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <thread>
#include "memory.h"
#include "processor.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n"
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int delay = atoi(argv[2]);
    std::filesystem::path rom_path = argv[3];

    if (!std::filesystem::exists(rom_path))
    {
        std::cout << "ROM file does not exist." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    Display display{atoi(argv[1])};
    display.clear();
    display.draw();

    Memory memory{};
    memory.load_rom(rom_path);
    memory.dump();

    Processor cpu{};

    while (!display.shouldQuit())
    {
        u_int16_t instr = cpu.fetch(memory);
        cpu.decode_execute(memory, display, instr);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    return 0;
}